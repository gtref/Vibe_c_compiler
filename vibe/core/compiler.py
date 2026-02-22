import os
import subprocess
import shutil
import re
import json
import bisect
from concurrent.futures import ThreadPoolExecutor # BOLT: Parallel compilation

class VibeCompiler:
    # BOLT: Pre-defined unsafe patterns for security audits moved to class level
    _C_UNSAFE_FUNCS = {
        "gets": "Extremely unsafe, use fgets instead.",
        "strcpy": "Unsafe, use strncpy or strlcpy instead.",
        "strcat": "Unsafe, use strncat or strlcat instead.",
        "sprintf": "Unsafe, use snprintf instead.",
        "vsprintf": "Unsafe, use vsnprintf instead.",
        "scanf": "Can be unsafe, use with field widths or use fgets/sscanf.",
        "system": "Unsafe, can lead to command injection.",
        "popen": "Unsafe, can lead to command injection.",
        "execl": "Potential for command injection if arguments are not controlled.",
        "execv": "Potential for command injection if arguments are not controlled.",
        "execle": "Potential for command injection if arguments are not controlled.",
        "execve": "Potential for command injection if arguments are not controlled.",
        "execlp": "Potential for command injection if arguments are not controlled.",
        "execvp": "Potential for command injection if arguments are not controlled.",
        "printf": "Potential format string vulnerability if first argument is not a literal.",
        "fprintf": "Potential format string vulnerability if first argument is not a literal.",
        "vprintf": "Potential format string vulnerability if first argument is not a literal.",
        "tmpnam": "Insecure, use mkstemp instead.",
        "tempnam": "Insecure, use mkstemp instead.",
        "mktemp": "Insecure, use mkstemp instead.",
        "realpath": "Can be unsafe if not checking return value or using a fixed-size buffer.",
        "strtok": "Not thread-safe, use strtok_r instead.",
        "vfork": "Unsafe, use fork or posix_spawn instead.",
        "strncat": "Can be tricky to use safely, ensure size argument is correct.",
        "strncpy": "Can be tricky to use safely as it may not null-terminate the destination.",
        "snprintf": "Potential format string vulnerability if third argument is not a literal.",
        "vsnprintf": "Potential format string vulnerability if third argument is not a literal.",
        "syslog": "Potential format string vulnerability if second argument is not a literal.",
        "setuid": "Privilege management functions require careful error handling.",
        "setgid": "Privilege management functions require careful error handling.",
        "setreuid": "Privilege management functions require careful error handling.",
        "setregid": "Privilege management functions require careful error handling.",
    }

    _PY_UNSAFE_PATTERNS = {
        r"eval": "Unsafe, allows execution of arbitrary code.", # nosec
        r"exec": "Unsafe, allows execution of arbitrary code.", # nosec
        r"shell\s*=\s*True": "Potential shell injection vulnerability.", # nosec
        r"os\.system": "Unsafe, can lead to command injection.", # nosec
        r"os\.popen": "Unsafe, can lead to command injection.", # nosec
        r"os\.spawn": "Potential for command injection if arguments are not controlled.", # nosec
        r"pickle\.load": "Insecure deserialization can lead to arbitrary code execution.", # nosec
        r"yaml\.load": "Insecure deserialization can lead to arbitrary code execution if not using SafeLoader.", # nosec
        r"pickle\.loads": "Insecure deserialization can lead to arbitrary code execution.", # nosec
        r"marshal\.load": "Insecure deserialization of marshal data.", # nosec
        r"marshal\.loads": "Insecure deserialization of marshal data.", # nosec
        r"tempfile\.mktemp": "Insecure, use tempfile.mkstemp instead.", # nosec
    }

    # BOLT: Pre-compiled regexes for security audits (lazy-loaded)
    _C_AUDIT_RE = None
    _PY_AUDIT_RE = None
    _PY_PATTERN_KEYS = None # BOLT: Cached pattern keys for Python audit

    def __init__(self):
        # __file__ is vibe/core/compiler.py
        # dirname(dirname(dirname(__file__))) is the root directory
        self.base_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        self.vibe_dir = os.path.join(self.base_dir, "vibe")
        self.include_dir = os.path.join(self.vibe_dir, "include")
        self.template_dir = os.path.join(self.vibe_dir, "templates")
        self.version_file = os.path.join(self.base_dir, "VERSION")
        self._vibe_include_mtime_cache = None # BOLT: Cache for compiler headers

    def _get_header_mtime(self, scan_src=True):
        """BOLT: Get the latest modification time among all headers using efficient scanning."""
        # Check cache for global vibe headers if they haven't been scanned yet
        if self._vibe_include_mtime_cache is None:
            self._vibe_include_mtime_cache = self._scan_for_mtime(self.include_dir, (".h",))

        header_mtime = self._vibe_include_mtime_cache

        if scan_src and os.path.exists("src"):
            header_mtime = max(header_mtime, self._scan_for_mtime("src", (".h",)))

        return header_mtime

    def _scan_for_mtime(self, path, extensions):
        """BOLT: Non-recursive helper to scan for latest mtime using a stack and os.scandir for performance."""
        max_mtime = 0
        if not os.path.exists(path):
            return 0

        stack = [path]
        while stack:
            curr_path = stack.pop()
            try:
                for entry in os.scandir(curr_path):
                    if entry.is_file():
                        if entry.name.endswith(extensions):
                            # BOLT: entry.stat().st_mtime is often cached by os.scandir
                            max_mtime = max(max_mtime, entry.stat().st_mtime)
                    elif entry.is_dir():
                        stack.append(entry.path)
            except OSError:
                pass
        return max_mtime

    def _compile_src(self, src, obj, arch, proj_type):
        """BOLT: Helper to compile a single source file to an object file."""
        # BOLT: Object path is now pre-calculated and directories pre-created
        print(f"Compiling {src}...")
        # Sentinel: Added security hardening flags
        cmd = ["clang", "-I" + self.include_dir, "-c", src, "-o", obj,
               "-fstack-protector-strong", "-D_FORTIFY_SOURCE=2",
               "-Wformat", "-Wformat-security", "-Werror=format-security"]
        if arch:
            cmd += ["-target", arch]
        if proj_type == "shared":
            cmd += ["-fPIC"]
        else:
            cmd += ["-fPIE"]

        res = subprocess.run(cmd)
        return obj if res.returncode == 0 else None

    def show_version(self):
        try:
            with open(self.version_file, "r") as f:
                version = f.read().strip()
                print(f"Vibe C Compiler v{version}")
                return version
        except Exception as e:
            print(f"Error reading version: {e}")
            return "unknown"

    def init_project(self, name, template="basic"):
        # Improved sanitization: only allow alphanumeric, underscores, and hyphens
        if not re.match(r"^[a-zA-Z0-9_-]+$", name):
            print("Error: Invalid project name. Use only alphanumeric characters, underscores, and hyphens.")
            return False

        # Sanitize template name to prevent path traversal
        if not re.match(r"^[a-zA-Z0-9_-]+$", template):
            print(f"Error: Invalid template name '{template}'. Use only alphanumeric characters, underscores, and hyphens.")
            return False

        if os.path.exists(name):
            print(f"Error: Directory '{name}' already exists.")
            return False

        template_path = os.path.join(self.template_dir, template)
        if not os.path.exists(template_path):
            print(f"Error: Template '{template}' not found. Using 'basic' instead.")
            template_path = os.path.join(self.template_dir, "basic")

        shutil.copytree(template_path, name)

        # Update vibe.json with project name using proper JSON handling
        config_path = os.path.join(name, "vibe.json")
        try:
            with open(config_path, "r") as f:
                config = json.load(f)

            # If the template used {{name}}, it might not be valid JSON if it's not quoted
            # But usually templates should have valid JSON with a placeholder.
            # If it's literally {{name}} without quotes, json.load will fail.
            # Let's check the templates.
        except json.JSONDecodeError:
            # Fallback to string replacement if JSON is invalid due to placeholders
            with open(config_path, "r") as f:
                content = f.read()
            content = content.replace("{{name}}", name)
            # Try to validate after replacement
            try:
                config = json.loads(content)
            except json.JSONDecodeError:
                print("Error: Failed to generate valid vibe.json")
                return False

        config["name"] = name
        with open(config_path, "w") as f:
            json.dump(config, f, indent=2)

        os.makedirs(os.path.join(name, "build"), exist_ok=True)

        print(f"Project '{name}' initialized successfully.")
        return True

    def build_project(self, arch=None, lib_type=None):
        if not os.path.exists("vibe.json"):
            print("Error: Not a vibe project (vibe.json not found).")
            return False

        with open("vibe.json", "r") as f:
            config = json.load(f)

        proj_name = config.get("name", "app")
        # Sanitize proj_name from config to prevent path traversal/command injection
        if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
            print("Error: Invalid project name in vibe.json.")
            return False

        proj_type = config.get("type", "executable")

        # Override project type if lib_type is specified
        if lib_type and lib_type != "none":
            proj_type = lib_type

        # BOLT: Centralized object directory
        obj_root = os.path.join("build", "obj")
        os.makedirs(obj_root, exist_ok=True)

        # BOLT: Efficient single-pass scanning of src/ for .c files and headers using os.scandir
        src_files = []
        header_mtime = self._get_header_mtime(scan_src=False)

        if os.path.exists("src"):
            # BOLT: Iterative stack-based scan to avoid recursion overhead
            stack = [("src", "")]
            while stack:
                path, rel_root = stack.pop()
                try:
                    for entry in os.scandir(path):
                        if entry.is_file():
                            if entry.name.endswith(".c"):
                                # BOLT: Pre-calculate paths during initial scan
                                rel_path = os.path.join(rel_root, entry.name)
                                obj_path = os.path.join(obj_root, os.path.splitext(rel_path)[0] + ".o")
                                src_files.append((entry.path, obj_path, entry.stat().st_mtime))
                            elif entry.name.endswith(".h"):
                                header_mtime = max(header_mtime, entry.stat().st_mtime)
                        elif entry.is_dir():
                            stack.append((entry.path, os.path.join(rel_root, entry.name)))
                except OSError as e:
                    print(f"Warning: Could not scan source directory '{path}': {e}")

        if not src_files:
            print("Error: No source files found in src/")
            return False

        if proj_type == "static":
            output_name = f"build/lib{proj_name}.a"
        elif proj_type == "shared":
            output_name = f"build/lib{proj_name}.so"
        else:
            output_name = f"build/{proj_name}"

        if arch and not re.match(r"^[a-zA-Z0-9._-]+$", arch):
            print(f"Error: Invalid architecture name '{arch}'.")
            return False

        # BOLT: Pre-collect object file mtimes using a single scandir pass to minimize stat calls
        obj_mtimes = {}
        if os.path.exists(obj_root):
            # BOLT: Iterative stack-based scan for object files
            stack = [obj_root]
            while stack:
                curr_path = stack.pop()
                try:
                    for entry in os.scandir(curr_path):
                        if entry.is_file() and entry.name.endswith(".o"):
                            obj_mtimes[entry.path] = entry.stat().st_mtime
                        elif entry.is_dir():
                            stack.append(entry.path)
                except OSError:
                    pass

        # BOLT: Pre-filter files that actually need compilation
        to_compile = []
        obj_files = []
        for src_path, obj_path, src_mtime in src_files:
            obj_files.append(obj_path)

            needs_compile = True
            obj_mtime = obj_mtimes.get(obj_path)
            if obj_mtime is not None:
                if obj_mtime > src_mtime and obj_mtime > header_mtime:
                    needs_compile = False

            if needs_compile:
                to_compile.append((src_path, obj_path))

        # BOLT: Target-level incremental check
        link_needed = not os.path.exists(output_name)

        # BOLT: Only use ThreadPoolExecutor if compilation is needed
        if to_compile:
            # BOLT: Bulk pre-create directories once to avoid redundant syscalls in threads
            obj_dirs = {os.path.dirname(obj) for _, obj in to_compile}
            for d in obj_dirs:
                os.makedirs(d, exist_ok=True)

            with ThreadPoolExecutor() as executor:
                results = list(executor.map(lambda x: self._compile_src(x[0], x[1], arch, proj_type), to_compile))
                if None in results:
                    print("Build failed: Some files failed to compile.")
                    return False
            # BOLT: If we compiled anything, we definitely need to link
            link_needed = True

        if not obj_files:
            print("Build failed: No object files to link.")
            return False

        # BOLT: If no compilation happened, check if any object file is newer than target
        if not link_needed:
            target_mtime = os.path.getmtime(output_name)
            # BOLT: Use pre-collected mtimes to avoid redundant stat calls
            if any(obj_mtimes.get(obj, 0) > target_mtime for obj in obj_files):
                link_needed = True

        if link_needed:
            if proj_type == "static":
                print(f"Creating static library {output_name}...")
                res = subprocess.run(["ar", "rcs", output_name] + obj_files)
            else:
                print(f"Linking project...")
                link_cmd = ["clang"]
                # Sentinel: Added security hardening flags for linking
                link_cmd += ["-fstack-protector-strong", "-Wl,-z,relro,-z,now"]
                if arch: link_cmd += ["-target", arch]
                if proj_type == "shared":
                    link_cmd += ["-shared", "-fPIC"]
                else:
                    link_cmd += ["-fPIE", "-pie"]
                link_cmd += obj_files + ["-o", output_name]
                res = subprocess.run(link_cmd)

            if res.returncode != 0:
                print("Build failed.")
                return False

        print(f"Build successful: {output_name}")
        return True
    def run_project(self):
        if not os.path.exists("vibe.json"):
            print("Error: vibe.json not found.")
            return

        with open("vibe.json", "r") as f:
            config = json.load(f)

        proj_name = config.get("name", "app")
        # Sanitize proj_name from config
        if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
            print("Error: Invalid project name in vibe.json.")
            return

        output_name = os.path.join("build", proj_name)

        if not os.path.exists(output_name):
            print(f"Error: Executable {output_name} not found. Build it first.")
            return

        print(f"Running {output_name}...")
        # Use absolute path for safety and to avoid confusion
        abs_output_path = os.path.abspath(output_name)
        subprocess.run([abs_output_path])

    def clean_project(self):
        if os.path.exists("build"):
            shutil.rmtree("build")
            print("Cleaned build directory.")
        else:
            print("Nothing to clean.")

    def run_tests(self):
        if not os.path.exists("tests"):
            print("No tests/ directory found.")
            return

        # BOLT: Efficiently collect test files and their mtimes using os.scandir (iterative)
        test_files = []
        test_bin_dir = "build/tests"
        if os.path.exists("tests"):
            stack = ["tests"]
            while stack:
                curr_path = stack.pop()
                try:
                    for entry in os.scandir(curr_path):
                        if entry.is_file() and entry.name.endswith(".c"):
                            # BOLT: Pre-calculate test name and binary path to avoid redundant work
                            test_name = os.path.splitext(entry.name)[0]
                            output_bin = os.path.join(test_bin_dir, test_name)
                            test_files.append((entry.path, entry.stat().st_mtime, test_name, output_bin))
                        elif entry.is_dir():
                            stack.append(entry.path)
                except OSError as e:
                    print(f"Warning: Could not scan test directory '{curr_path}': {e}")

        if not test_files:
            print("No test files (.c) found in tests/.")
            return

        if not os.path.exists("build/tests"):
            os.makedirs("build/tests", exist_ok=True)

        # Check if we should link with the project library
        link_args = []
        lib_mtime = 0
        if os.path.exists("vibe.json"):
            try:
                with open("vibe.json", "r") as f:
                    config = json.load(f)
                proj_name = config.get("name", "app")
                # Sanitize proj_name from config to prevent path traversal/argument injection
                if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
                    print("Error: Invalid project name in vibe.json.")
                    return

                proj_type = config.get("type", "executable")

                if proj_type == "static":
                    lib_path = f"build/lib{proj_name}.a"
                elif proj_type == "shared":
                    lib_path = f"build/lib{proj_name}.so"
                else:
                    lib_path = None

                if lib_path and os.path.exists(lib_path):
                    lib_mtime = os.path.getmtime(lib_path)
                    if proj_type == "static":
                        link_args = [lib_path]
                    else:
                        link_args = ["-Lbuild", f"-l{proj_name}"]
            except Exception:
                pass

        # BOLT: Calculate header_mtime for tests to enable incremental compilation
        header_mtime = self._get_header_mtime(scan_src=True)

        def _compile_test(test_file, output_bin, test_name):
            print(f"Compiling {test_file}...")
            # Sentinel: Added security hardening flags for tests
            cmd = ["clang", "-I" + self.include_dir, "-Isrc", test_file,
                   "-fstack-protector-strong", "-D_FORTIFY_SOURCE=2",
                   "-Wformat", "-Wformat-security", "-Werror=format-security",
                   "-fPIE", "-pie", "-Wl,-z,relro,-z,now"] + link_args + ["-o", output_bin]
            res = subprocess.run(cmd, capture_output=True)
            return {
                "file": test_file,
                "name": test_name,
                "bin": output_bin,
                "success": res.returncode == 0,
                "error": res.stderr.decode() if res.returncode != 0 else ""
            }

        # BOLT: Pre-collect test binary mtimes to avoid redundant stat calls
        test_bin_mtimes = {}
        if os.path.exists(test_bin_dir):
            try:
                for entry in os.scandir(test_bin_dir):
                    if entry.is_file():
                        test_bin_mtimes[entry.path] = entry.stat().st_mtime
            except OSError:
                pass

        # BOLT: Pre-filter tests that actually need compilation to avoid thread overhead
        to_compile = []
        compilation_results = []

        for test_file, test_mtime, test_name, output_bin in test_files:
            needs_compile = True
            bin_mtime = test_bin_mtimes.get(output_bin)
            if bin_mtime is not None:
                if bin_mtime > test_mtime and \
                   bin_mtime > header_mtime and \
                   bin_mtime > lib_mtime:
                    needs_compile = False

            if needs_compile:
                to_compile.append((test_file, output_bin, test_name))
            else:
                compilation_results.append({
                    "file": test_file,
                    "name": test_name,
                    "bin": output_bin,
                    "success": True,
                    "error": ""
                })

        if to_compile:
            print(f"Compiling {len(to_compile)} tests in parallel...")
            with ThreadPoolExecutor() as executor:
                compilation_results.extend(list(executor.map(lambda x: _compile_test(x[0], x[1], x[2]), to_compile)))

        # BOLT: Run tests in parallel
        print(f"Running {len(compilation_results)} tests in parallel...")

        # BOLT: Pre-calculate test environment once to avoid redundant copies/lookups
        test_env = os.environ.copy()
        ld_path = os.path.abspath("build")
        ld_parts = [ld_path]
        existing_ld_path = test_env.get("LD_LIBRARY_PATH")
        if existing_ld_path:
            ld_parts.extend([p for p in existing_ld_path.split(":") if p])
        test_env["LD_LIBRARY_PATH"] = ":".join(ld_parts)

        def _run_single_test(result):
            if not result["success"]:
                return False, f"\n[!] Failed to compile {result['file']}:\n{result['error']}"

            # BOLT: Use pre-calculated environment and absolute path
            abs_bin = os.path.abspath(result["bin"])
            res = subprocess.run([abs_bin], env=test_env, capture_output=True, text=True)
            if res.returncode == 0:
                return True, f"  [+] {result['name']} passed."
            else:
                return False, f"  [-] {result['name']} failed.\n{res.stdout}\n{res.stderr}"

        with ThreadPoolExecutor() as executor:
            execution_results = list(executor.map(_run_single_test, compilation_results))

        passed = sum(1 for success, _ in execution_results if success)
        failed = len(execution_results) - passed
        for _, output in execution_results:
            print(output)

        print("\n=== Test Results ===")
        print(f"Passed: {passed}")
        print(f"Failed: {failed}")
        print(f"Total:  {len(test_files)}")

    def install_globally(self):
        source_script = os.path.join(self.base_dir, "vibe_c_compiler")
        target_dir = os.path.expanduser("~/.local/bin")
        target_link = os.path.join(target_dir, "vcc")

        if not os.path.exists(target_dir):
            try:
                os.makedirs(target_dir)
            except Exception as e:
                print(f"Error creating {target_dir}: {e}")
                return

        if os.path.exists(target_link):
            os.remove(target_link)

        try:
            os.symlink(source_script, target_link)
            print(f"Successfully installed 'vcc' to {target_link}")
            print(f"Make sure {target_dir} is in your PATH.")
            print("You can now run 'vcc update' (or 'vcc upgrade') from anywhere to update the compiler.")
        except Exception as e:
            print(f"Error creating symlink: {e}")

    def uninstall_globally(self):
        target_dir = os.path.expanduser("~/.local/bin")
        target_link = os.path.join(target_dir, "vcc")

        if os.path.exists(target_link):
            try:
                os.remove(target_link)
                print(f"Successfully uninstalled 'vcc' from {target_link}")
            except Exception as e:
                print(f"Error removing symlink: {e}")
        else:
            print(f"'vcc' is not installed in {target_dir}")

    def list_headers(self):
        if not os.path.exists(self.include_dir):
            print("Error: Include directory not found.")
            return

        headers = [f for f in os.listdir(self.include_dir) if f.endswith(".h")]
        headers.sort()

        if not headers:
            print("No Vibe headers found.")
        else:
            print("\nAvailable Vibe Headers:")
            for header in headers:
                print(f"  - {header}")

    def list_templates(self):
        if not os.path.exists(self.template_dir):
            print("Error: Template directory not found.")
            return

        templates = [d for d in os.listdir(self.template_dir) if os.path.isdir(os.path.join(self.template_dir, d))]
        templates.sort()

        if not templates:
            print("No Vibe templates found.")
        else:
            print("\nAvailable Vibe Templates:")
            for template in templates:
                print(f"  - {template}")

    def project_status(self):
        if not os.path.exists("vibe.json"):
            print("Error: Not in a Vibe project directory (vibe.json not found).")
            return

        try:
            with open("vibe.json", "r") as f:
                config = json.load(f)
        except Exception as e:
            print(f"Error reading vibe.json: {e}")
            return

        proj_name = config.get("name", "app")
        # Sanitize proj_name from config
        if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
            print("Error: Invalid project name in vibe.json.")
            return

        print("\n=== Vibe Project Status ===")
        print(f"Name:    {proj_name}")
        print(f"Version: {config.get('version', 'N/A')}")
        print(f"Type:    {config.get('type', 'executable')}")

        src_count = 0
        def _count_src(path):
            count = 0
            try:
                for entry in os.scandir(path):
                    if entry.is_file() and entry.name.endswith(".c"):
                        count += 1
                    elif entry.is_dir():
                        count += _count_src(entry.path)
            except OSError:
                pass
            return count

        if os.path.exists("src"):
            src_count = _count_src("src")
        print(f"Sources: {src_count} .c files")

        if os.path.exists("build"):
            build_files = [f for f in os.listdir("build") if os.path.isfile(os.path.join("build", f))]
            print(f"Build:   {len(build_files)} artifacts in build/")
        else:
            print("Build:   No build directory found.")

    def _audit_file(self, path):
        """BOLT: Unified auditor for both C and Python files with lazy line offset calculation."""
        issues = []
        ext = os.path.splitext(path)[1]
        if ext not in (".c", ".h", ".py"):
            return []

        try:
            with open(path, "r", errors="ignore") as f:
                content = f.read()

            if ext == ".py":
                matches = list(VibeCompiler._PY_AUDIT_RE.finditer(content))
                if not matches:
                    return []

                # BOLT: Lazy line offset calculation ONLY if matches found
                line_offsets = [0] + [m.end() for m in re.finditer('\n', content)]
                for match in matches:
                    idx_in_lines = bisect.bisect_right(line_offsets, match.start()) - 1
                    line_start = line_offsets[idx_in_lines]
                    line_end = line_offsets[idx_in_lines + 1] if idx_in_lines + 1 < len(line_offsets) else len(content)

                    if "# nosec" in content[line_start:line_end]:
                        continue

                    group_name = match.lastgroup
                    if group_name and group_name.startswith('p'):
                        idx = int(group_name[1:])
                        desc = VibeCompiler._PY_UNSAFE_PATTERNS[VibeCompiler._PY_PATTERN_KEYS[idx]]
                        issues.append(f"  [!] {path}:{idx_in_lines + 1} - Found unsafe pattern: {desc}")
            else: # .c or .h
                matches = list(VibeCompiler._C_AUDIT_RE.finditer(content))
                if not matches:
                    return []

                # BOLT: Lazy line offset calculation ONLY if matches found
                line_offsets = [0] + [m.end() for m in re.finditer('\n', content)]
                for match in matches:
                    func = match.group(1)
                    idx_in_lines = bisect.bisect_right(line_offsets, match.start()) - 1
                    line_start = line_offsets[idx_in_lines]
                    line_end = line_offsets[idx_in_lines + 1] if idx_in_lines + 1 < len(line_offsets) else len(content)

                    # Sentinel: Added support for // nosec and /* nosec */ suppression in C files
                    line_content = content[line_start:line_end]
                    if re.search(r'(//\s*nosec|/\*\s*nosec\s*\*/)', line_content):
                        continue

                    issues.append(f"  [!] {path}:{idx_in_lines + 1} - Found potential unsafe function '{func}': {VibeCompiler._C_UNSAFE_FUNCS[func]}")
        except Exception as e:
            issues.append(f"  [?] Could not read {path}: {e}")
        return issues

    def run_audit(self):
        print("\n=== Vibe Security Audit ===")

        # BOLT: Pre-initialize regexes before starting threads for safety
        if VibeCompiler._C_AUDIT_RE is None:
            VibeCompiler._C_AUDIT_RE = re.compile(rf"\b({'|'.join(re.escape(f) for f in VibeCompiler._C_UNSAFE_FUNCS.keys())})\b")
        if VibeCompiler._PY_AUDIT_RE is None:
            VibeCompiler._PY_PATTERN_KEYS = list(VibeCompiler._PY_UNSAFE_PATTERNS.keys())
            sanitized = [p if p.startswith(r"\b") or p.endswith(r"\b") else rf"\b{p}\b" for p in VibeCompiler._PY_PATTERN_KEYS]
            VibeCompiler._PY_AUDIT_RE = re.compile("|".join(f"(?P<p{i}>(?:{p}))" for i, p in enumerate(sanitized)))

        # BOLT: Optimized single-pass audits over unique root directories
        # Use abspath for consistent deduplication
        roots = [os.path.abspath(self.vibe_dir)]
        for d in ["src", "tests"]:
            if os.path.exists(d):
                roots.append(os.path.abspath(d))

        # BOLT: Deduplicate and filter out subdirectories (e.g. self.include_dir is inside self.vibe_dir)
        unique_roots = []
        for r in sorted(roots, key=len):
            if not any(r.startswith(u + os.sep) for u in unique_roots):
                unique_roots.append(r)

        # BOLT: Collect all files from all unique roots first
        files_to_audit = []
        for root in unique_roots:
            print(f"Scanning {root}...")
            stack = [root]
            while stack:
                curr = stack.pop()
                try:
                    for entry in os.scandir(curr):
                        if entry.is_file():
                            if entry.name.endswith((".c", ".h", ".py")):
                                files_to_audit.append(entry.path)
                        elif entry.is_dir():
                            stack.append(entry.path)
                except OSError as e:
                    print(f"Warning: Could not scan directory '{curr}': {e}")

        if not files_to_audit:
            print("  No relevant files found for audit.")
        else:
            issues_found = 0
            # BOLT: Single pool for all files across all roots for maximum efficiency
            with ThreadPoolExecutor() as executor:
                for file_issues in executor.map(self._audit_file, files_to_audit):
                    for issue in file_issues:
                        print(issue)
                        issues_found += 1
            print(f"  Found {issues_found} potential issues.")

        if not any(os.path.exists(d) for d in ["src", "tests"]):
            print("\nNote: No src/ or tests/ directory found in the current project.")

        # Check for optional external tools
        print("\n--- Checking for advanced audit tools ---")

        # Check for bandit (Python security)
        try:
            import bandit
            print("\n[Optional] Running Bandit for deeper Python analysis...")
            res = subprocess.run(["bandit", "-r", self.vibe_dir])
            if res.returncode == 0:
                print("Bandit: No major issues found.")
            else:
                print("Bandit: Some issues were found.")
        except ImportError:
            pass # Silent if not installed

        # Check for cppcheck (C security)
        if os.path.exists("src"):
            try:
                # We check if it exists by running version
                subprocess.run(["cppcheck", "--version"], capture_output=True, check=True)
                print("\n[Optional] Running Cppcheck for deeper C analysis...")
                res = subprocess.run(["cppcheck", "--enable=warning,style,performance,portability", "src"])
                if res.returncode == 0:
                    print("Cppcheck: Completed.")
            except (FileNotFoundError, subprocess.CalledProcessError):
                pass # Silent if not installed

        print("\nAudit complete. Always follow best security practices!")

    def update_compiler(self):
        print("Checking for updates...")
        try:
            # Check if we are in a git repository
            res = subprocess.run(["git", "rev-parse", "--is-inside-work-tree"], capture_output=True, text=True, cwd=self.base_dir)
            if res.returncode != 0:
                print("Error: Not a git repository. Cannot update automatically.")
                return

            print("Fetching latest version from GitHub...")
            res = subprocess.run(["git", "pull", "origin", "main"], cwd=self.base_dir)
            if res.returncode == 0:
                print("Successfully updated Vibe C Compiler.")
                # After update, version might have changed
                self.show_version()
            else:
                print("Failed to update. Please check your internet connection or run 'git pull' manually.")
        except Exception as e:
            print(f"An error occurred during update: {e}")
