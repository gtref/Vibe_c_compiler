"""
This module implements the core Vibe C Compiler logic, handling project initialization, parallel compilation, and security auditing.
In version 1.5.11, the compiler features enhanced performance for directory traversal and hardened security patterns for the internal audit tool.
This code is AI-generated.
"""

import os
import subprocess
import shutil
import re
import json
import bisect
from concurrent.futures import ThreadPoolExecutor # BOLT: Parallel compilation

class VibeCompiler:
    # BOLT: Pre-defined unsafe patterns for security audits moved to class level for performance and accessibility.
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
        "strncpy": "Can be tricky to use safely as it may not null-terminate the destination. Use vibe_str_copy instead.",
        "snprintf": "Potential format string vulnerability if third argument is not a literal. Use vibe_snprintf instead.",
        "vsnprintf": "Potential format string vulnerability if third argument is not a literal. Use vibe_vsnprintf instead.",
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

    # BOLT: Pre-compiled regexes for security audits (lazy-loaded to minimize startup time).
    _C_AUDIT_RE = None
    _PY_AUDIT_RE = None
    _PY_PATTERN_KEYS = None # BOLT: Cached pattern keys for Python audit to maintain order.

    def __init__(self):
        # Internal Logic: Initialize base directory paths and modification time caches for the compiler environment.
        # __file__ is vibe/core/compiler.py
        # dirname(dirname(dirname(__file__))) is the root directory
        self.base_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        self.vibe_dir = os.path.join(self.base_dir, "vibe")
        self.include_dir = os.path.join(self.vibe_dir, "include")
        self.template_dir = os.path.join(self.vibe_dir, "templates")
        self.version_file = os.path.join(self.base_dir, "VERSION")
        self._vibe_include_mtime_cache = None # BOLT: Cache for compiler headers to speed up incremental builds.

    def _get_header_mtime(self, scan_src=True):
        """BOLT: Get the latest modification time among all headers using efficient non-recursive scanning."""
        # Internal Logic: Check cache for global vibe headers if they haven't been scanned yet to avoid redundant I/O.
        if self._vibe_include_mtime_cache is None:
            self._vibe_include_mtime_cache = self._scan_for_mtime(self.include_dir, (".h",))

        header_mtime = self._vibe_include_mtime_cache

        # Internal Logic: Optionally scan the project's own source directory for local headers.
        if scan_src and os.path.exists("src"):
            header_mtime = max(header_mtime, self._scan_for_mtime("src", (".h",)))

        return header_mtime

    def _scan_for_mtime(self, path, extensions):
        """BOLT: Non-recursive helper to scan for latest mtime using a stack and os.scandir for performance."""
        # Internal Logic: Implement a stack-based traversal to avoid recursion depth limits and use os.scandir for faster metadata retrieval.
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
                            # BOLT: entry.stat().st_mtime is often cached by os.scandir, reducing syscalls.
                            max_mtime = max(max_mtime, entry.stat().st_mtime)
                    elif entry.is_dir(follow_symlinks=False):
                        # Sentinel: Explicitly disable symlink following to prevent infinite loops or path traversal.
                        stack.append(entry.path)
            except OSError:
                pass
        return max_mtime

    def _compile_src(self, src, obj, arch, proj_type):
        """BOLT: Helper to compile a single source file to an object file with hardening flags."""
        # Internal Logic: Execute Clang with a predefined set of security hardening flags (Stack Protector, Fortify Source, etc.).
        print(f"Compiling {src}...")
        # Sentinel: Added security hardening flags to mitigate memory corruption exploits in produced binaries.
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
        """Internal Logic: Read the VERSION file from the base directory and display it to the user."""
        try:
            with open(self.version_file, "r") as f:
                version = f.read().strip()
                print(f"Vibe C Compiler v{version}")
                return version
        except Exception as e:
            print(f"Error reading version: {e}")
            return "unknown"

    def init_project(self, name, template="basic"):
        """Internal Logic: Create a new project directory by copying a template and updating its configuration."""
        # Internal Logic: Validate project name to prevent directory traversal or shell injection attacks.
        if not re.match(r"^[a-zA-Z0-9_-]+$", name):
            print("Error: Invalid project name. Use only alphanumeric characters, underscores, and hyphens.")
            return False

        # Internal Logic: Sanitize template name to prevent path traversal within the template directory.
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

        # Internal Logic: Perform a recursive copy of the template directory to the new project location.
        shutil.copytree(template_path, name)

        # Internal Logic: Update vibe.json with the new project name using proper JSON parsing to avoid injection.
        config_path = os.path.join(name, "vibe.json")
        try:
            with open(config_path, "r") as f:
                config = json.load(f)
        except json.JSONDecodeError:
            # Fallback to string replacement if JSON is invalid due to placeholders (legacy template support).
            with open(config_path, "r") as f:
                content = f.read()
            content = content.replace("{{name}}", name)
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
        """BOLT: Perform an incremental build of the project using parallel compilation and linking."""
        # Internal Logic: Verify project structure and load configuration from vibe.json.
        if not os.path.exists("vibe.json"):
            print("Error: Not a vibe project (vibe.json not found).")
            return False

        with open("vibe.json", "r") as f:
            config = json.load(f)

        proj_name = config.get("name", "app")
        # Sentinel: Sanitize project name to prevent path traversal during build artifact generation.
        if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
            print("Error: Invalid project name in vibe.json.")
            return False

        proj_type = config.get("type", "executable")
        if lib_type and lib_type != "none":
            proj_type = lib_type

        # BOLT: Ensure object directory exists before starting parallel compilation.
        obj_root = os.path.join("build", "obj")
        os.makedirs(obj_root, exist_ok=True)

        # BOLT: Collect all source files and determine the latest header modification time in a single pass.
        src_files = []
        header_mtime = self._get_header_mtime(scan_src=False)

        def _collect_src(path, rel_root=""):
            nonlocal header_mtime
            try:
                for entry in os.scandir(path):
                    if entry.is_file():
                        if entry.name.endswith(".c"):
                            rel_path = os.path.join(rel_root, entry.name)
                            obj_path = os.path.join(obj_root, os.path.splitext(rel_path)[0] + ".o")
                            src_files.append((entry.path, obj_path, entry.stat().st_mtime))
                        elif entry.name.endswith(".h"):
                            header_mtime = max(header_mtime, entry.stat().st_mtime)
                    elif entry.is_dir(follow_symlinks=False):
                        _collect_src(entry.path, os.path.join(rel_root, entry.name))
            except OSError as e:
                print(f"Warning: Could not scan source directory '{path}': {e}")

        if os.path.exists("src"):
            _collect_src("src")

        if not src_files:
            print("Error: No source files found in src/")
            return False

        # Internal Logic: Determine the final output binary or library name based on the project type.
        if proj_type == "static":
            output_name = f"build/lib{proj_name}.a"
        elif proj_type == "shared":
            output_name = f"build/lib{proj_name}.so"
        else:
            output_name = f"build/{proj_name}"

        # Sentinel: Validate architecture string to prevent command line argument injection.
        if arch and not re.match(r"^[a-zA-Z0-9._-]+$", arch):
            print(f"Error: Invalid architecture name '{arch}'.")
            return False

        # BOLT: Cache object file modification times to speed up the incremental check.
        obj_mtimes = {}
        def _collect_obj_mtimes(path):
            try:
                for entry in os.scandir(path):
                    if entry.is_file() and entry.name.endswith(".o"):
                        obj_mtimes[entry.path] = entry.stat().st_mtime
                    elif entry.is_dir(follow_symlinks=False):
                        _collect_obj_mtimes(entry.path)
            except OSError:
                pass
        _collect_obj_mtimes(obj_root)

        # BOLT: Identify which source files actually need recompilation based on mtime logic.
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

        link_needed = not os.path.exists(output_name)

        # BOLT: Execute compilation tasks in parallel using a ThreadPoolExecutor for efficiency.
        if to_compile:
            obj_dirs = {os.path.dirname(obj) for _, obj in to_compile}
            for d in obj_dirs:
                os.makedirs(d, exist_ok=True)

            with ThreadPoolExecutor() as executor:
                results = list(executor.map(lambda x: self._compile_src(x[0], x[1], arch, proj_type), to_compile))
                if None in results:
                    print("Build failed: Some files failed to compile.")
                    return False
            link_needed = True

        if not obj_files:
            print("Build failed: No object files to link.")
            return False

        # BOLT: Determine if linking is required by comparing object file mtimes with the existing target.
        if not link_needed:
            target_mtime = os.path.getmtime(output_name)
            if any(obj_mtimes.get(obj, 0) > target_mtime for obj in obj_files):
                link_needed = True

        # Internal Logic: Link the compiled object files into the final library or executable with hardening flags.
        if link_needed:
            if proj_type == "static":
                print(f"Creating static library {output_name}...")
                res = subprocess.run(["ar", "rcs", output_name] + obj_files)
            else:
                print(f"Linking project...")
                link_cmd = ["clang"]
                # Sentinel: Apply RELRO and BIND_NOW flags to protect the binary's Global Offset Table.
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
        """Internal Logic: Execute the compiled project binary if it exists and is sanitized."""
        if not os.path.exists("vibe.json"):
            print("Error: vibe.json not found.")
            return

        with open("vibe.json", "r") as f:
            config = json.load(f)

        proj_name = config.get("name", "app")
        # Sentinel: Sanitize project name to prevent execution of unintended paths.
        if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
            print("Error: Invalid project name in vibe.json.")
            return

        output_name = os.path.join("build", proj_name)
        if not os.path.exists(output_name):
            print(f"Error: Executable {output_name} not found. Build it first.")
            return

        # Internal Logic: Launch the executable using its absolute path to avoid ambiguity.
        print(f"Running {output_name}...")
        abs_output_path = os.path.abspath(output_name)
        subprocess.run([abs_output_path])

    def clean_project(self):
        """Internal Logic: Remove the entire build directory to reset the project state."""
        if os.path.exists("build"):
            shutil.rmtree("build")
            print("Cleaned build directory.")
        else:
            print("Nothing to clean.")

    def run_tests(self):
        """BOLT: Discover, compile, and execute project unit tests in parallel."""
        if not os.path.exists("tests"):
            print("No tests/ directory found.")
            return

        # BOLT: Efficiently collect all C test files using a non-recursive scan.
        test_files = []
        def _collect_tests(path):
            try:
                for entry in os.scandir(path):
                    if entry.is_file() and entry.name.endswith(".c"):
                        test_files.append((entry.path, entry.stat().st_mtime))
                    elif entry.is_dir(follow_symlinks=False):
                        _collect_tests(entry.path)
            except OSError as e:
                print(f"Warning: Could not scan test directory '{path}': {e}")
        _collect_tests("tests")

        if not test_files:
            print("No test files (.c) found in tests/.")
            return

        if not os.path.exists("build/tests"):
            os.makedirs("build/tests", exist_ok=True)

        # Internal Logic: Determine linking requirements for tests (e.g., linking against the project's static or shared library).
        link_args = []
        lib_mtime = 0
        if os.path.exists("vibe.json"):
            try:
                with open("vibe.json", "r") as f:
                    config = json.load(f)
                proj_name = config.get("name", "app")
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

        header_mtime = self._get_header_mtime(scan_src=True)

        def _compile_test(test_file, output_bin):
            """Internal Logic: Compile a single test file with required include paths and hardening."""
            test_name = os.path.splitext(os.path.basename(test_file))[0]
            print(f"Compiling {test_file}...")
            # Sentinel: Use consistent security hardening for test binaries.
            cmd = ["clang", "-I" + self.include_dir, "-Isrc", test_file,
                   "-fstack-protector-strong", "-D_FORTIFY_SOURCE=2",
                   "-Wformat", "-Wformat-security", "-Werror=format-security",
                   "-fPIE", "-pie", "-Wl,-z,relro,-z,now"] + link_args + ["-o", output_bin]
            res = subprocess.run(cmd, capture_output=True)
            return {
                "file": test_file, "name": test_name, "bin": output_bin,
                "success": res.returncode == 0, "error": res.stderr.decode() if res.returncode != 0 else ""
            }

        # BOLT: Perform an incremental check for test binaries to skip redundant compilation.
        test_bin_mtimes = {}
        test_bin_dir = "build/tests"
        if os.path.exists(test_bin_dir):
            try:
                for entry in os.scandir(test_bin_dir):
                    if entry.is_file():
                        test_bin_mtimes[entry.path] = entry.stat().st_mtime
            except OSError:
                pass

        to_compile = []
        compilation_results = []
        for test_file, test_mtime in test_files:
            test_name = os.path.splitext(os.path.basename(test_file))[0]
            output_bin = os.path.join(test_bin_dir, test_name)
            needs_compile = True
            bin_mtime = test_bin_mtimes.get(output_bin)
            if bin_mtime is not None:
                if bin_mtime > test_mtime and bin_mtime > header_mtime and bin_mtime > lib_mtime:
                    needs_compile = False
            if needs_compile:
                to_compile.append(test_file)
            else:
                compilation_results.append({"file": test_file, "name": test_name, "bin": output_bin, "success": True, "error": ""})

        # BOLT: Compile and then run tests concurrently using ThreadPoolExecutor.
        if to_compile:
            print(f"Compiling {len(to_compile)} tests in parallel...")
            compile_args = [(tf, os.path.join(test_bin_dir, os.path.splitext(os.path.basename(tf))[0])) for tf in to_compile]
            with ThreadPoolExecutor() as executor:
                compilation_results.extend(list(executor.map(lambda x: _compile_test(x[0], x[1]), compile_args)))

        print(f"Running {len(compilation_results)} tests in parallel...")
        # Sentinel: Sanitize LD_LIBRARY_PATH to prevent unintended library loading during tests.
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
            abs_bin = os.path.abspath(result["bin"])
            res = subprocess.run([abs_bin], env=test_env, capture_output=True, text=True)
            if res.returncode == 0:
                return True, f"  [+] {result['name']} passed."
            else:
                return False, f"  [-] {result['name']} failed.\n{res.stdout}\n{res.stderr}"

        with ThreadPoolExecutor() as executor:
            execution_results = list(executor.map(_run_single_test, compilation_results))

        passed = sum(1 for success, _ in execution_results if success)
        print("\n".join(output for _, output in execution_results))
        print(f"\n=== Test Results ===\nPassed: {passed}\nFailed: {len(execution_results) - passed}\nTotal:  {len(test_files)}")

    def install_globally(self):
        """Internal Logic: Create a symlink for the vibe compiler in the user's local bin directory."""
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
        except Exception as e:
            print(f"Error creating symlink: {e}")

    def uninstall_globally(self):
        """Internal Logic: Remove the global 'vcc' symlink if it exists."""
        target_link = os.path.expanduser("~/.local/bin/vcc")
        if os.path.exists(target_link):
            try:
                os.remove(target_link)
                print(f"Successfully uninstalled 'vcc' from {target_link}")
            except Exception as e:
                print(f"Error removing symlink: {e}")
        else:
            print(f"'vcc' is not installed.")

    def list_headers(self):
        """Internal Logic: List all available custom headers in the vibe/include directory."""
        if os.path.exists(self.include_dir):
            headers = sorted([f for f in os.listdir(self.include_dir) if f.endswith(".h")])
            print("\nAvailable Vibe Headers:")
            for header in headers: print(f"  - {header}")
        else:
            print("Error: Include directory not found.")

    def list_templates(self):
        """Internal Logic: List all project templates available in the vibe/templates directory."""
        if os.path.exists(self.template_dir):
            templates = sorted([d for d in os.listdir(self.template_dir) if os.path.isdir(os.path.join(self.template_dir, d))])
            print("\nAvailable Vibe Templates:")
            for template in templates: print(f"  - {template}")
        else:
            print("Error: Template directory not found.")

    def project_status(self):
        """Internal Logic: Display the current metadata and structure of the Vibe project."""
        if not os.path.exists("vibe.json"):
            print("Error: Not in a Vibe project directory.")
            return

        with open("vibe.json", "r") as f:
            config = json.load(f)

        proj_name = config.get("name", "app")
        if not re.match(r"^[a-zA-Z0-9_-]+$", proj_name):
            print("Error: Invalid project name in vibe.json.")
            return

        print(f"\n=== Vibe Project Status ===\nName:    {proj_name}\nVersion: {config.get('version', 'N/A')}\nType:    {config.get('type', 'executable')}")

        src_count = 0
        if os.path.exists("src"):
            stack = ["src"]
            while stack:
                curr = stack.pop()
                try:
                    for entry in os.scandir(curr):
                        if entry.is_file() and entry.name.endswith(".c"): src_count += 1
                        elif entry.is_dir(follow_symlinks=False): stack.append(entry.path)
                except OSError: pass
        print(f"Sources: {src_count} .c files")

    def _audit_file(self, path):
        """BOLT: Perform a high-performance security audit on a single file using pre-compiled regexes."""
        issues = []
        ext = os.path.splitext(path)[1]
        if ext not in (".c", ".h", ".py"): return []

        try:
            with open(path, "r", errors="ignore") as f:
                content = f.read()

            # Internal Logic: Use named capture groups and bisect-based line numbering for rapid issue identification.
            if ext == ".py":
                matches = list(VibeCompiler._PY_AUDIT_RE.finditer(content))
                if not matches: return []
                line_offsets = [0] + [m.end() for m in re.finditer('\n', content)]
                for match in matches:
                    idx_in_lines = bisect.bisect_right(line_offsets, match.start()) - 1
                    line_start = line_offsets[idx_in_lines]
                    line_end = line_offsets[idx_in_lines + 1] if idx_in_lines + 1 < len(line_offsets) else len(content)
                    if "# nosec" in content[line_start:line_end]: continue
                    group_name = match.lastgroup
                    if group_name and group_name.startswith('p'):
                        idx = int(group_name[1:])
                        desc = VibeCompiler._PY_UNSAFE_PATTERNS[VibeCompiler._PY_PATTERN_KEYS[idx]]
                        issues.append(f"  [!] {path}:{idx_in_lines + 1} - Found unsafe pattern: {desc}")
            else:
                matches = list(VibeCompiler._C_AUDIT_RE.finditer(content))
                if not matches: return []
                line_offsets = [0] + [m.end() for m in re.finditer('\n', content)]
                for match in matches:
                    func = match.group(1)
                    idx_in_lines = bisect.bisect_right(line_offsets, match.start()) - 1
                    line_start = line_offsets[idx_in_lines]
                    line_end = line_offsets[idx_in_lines + 1] if idx_in_lines + 1 < len(line_offsets) else len(content)
                    # Sentinel: Support for // nosec and /* nosec */ suppression to reduce security fatigue.
                    if re.search(r'(//\s*nosec|/\*\s*nosec\s*\*/)', content[line_start:line_end]): continue
                    issues.append(f"  [!] {path}:{idx_in_lines + 1} - Found potential unsafe function '{func}': {VibeCompiler._C_UNSAFE_FUNCS[func]}")
        except Exception as e:
            issues.append(f"  [?] Could not read {path}: {e}")
        return issues

    def run_audit(self):
        """BOLT: Run a comprehensive project-wide security audit in parallel."""
        print("\n=== Vibe Security Audit ===")
        # BOLT: Pre-compile and combine all patterns into a single regex for O(1) matching per line.
        if VibeCompiler._C_AUDIT_RE is None:
            VibeCompiler._C_AUDIT_RE = re.compile(rf"\b({'|'.join(re.escape(f) for f in VibeCompiler._C_UNSAFE_FUNCS.keys())})\b")
        if VibeCompiler._PY_AUDIT_RE is None:
            VibeCompiler._PY_PATTERN_KEYS = list(VibeCompiler._PY_UNSAFE_PATTERNS.keys())
            sanitized = [p if p.startswith(r"\b") or p.endswith(r"\b") else rf"\b{p}\b" for p in VibeCompiler._PY_PATTERN_KEYS]
            VibeCompiler._PY_AUDIT_RE = re.compile("|".join(f"(?P<p{i}>(?:{p}))" for i, p in enumerate(sanitized)))

        # BOLT: Identify unique directories to audit while avoiding redundant scans of subdirectories.
        roots = [os.path.abspath(self.vibe_dir)] + [os.path.abspath(d) for d in ["src", "tests"] if os.path.exists(d)]
        unique_roots = []
        for r in sorted(roots, key=len):
            if not any(r.startswith(u + os.sep) for u in unique_roots): unique_roots.append(r)

        files_to_audit = []
        for root in unique_roots:
            print(f"Scanning {root}...")
            stack = [root]
            while stack:
                curr = stack.pop()
                try:
                    for entry in os.scandir(curr):
                        if entry.is_file() and entry.name.endswith((".c", ".h", ".py")): files_to_audit.append(entry.path)
                        elif entry.is_dir(follow_symlinks=False): stack.append(entry.path)
                except OSError: pass

        # BOLT: Distribute audit tasks across threads and summarize findings.
        if files_to_audit:
            issues_found = 0
            with ThreadPoolExecutor() as executor:
                for file_issues in executor.map(self._audit_file, files_to_audit):
                    for issue in file_issues:
                        print(issue)
                        issues_found += 1
            print(f"  Found {issues_found} potential issues.")
        else:
            print("  No relevant files found.")

        # Internal Logic: Run optional external security tools if they are available in the environment.
        print("\n--- Checking for advanced audit tools ---")
        try:
            import bandit
            subprocess.run(["bandit", "-r", self.vibe_dir])
        except ImportError: pass
        if os.path.exists("src"):
            try:
                subprocess.run(["cppcheck", "--version"], capture_output=True, check=True)
                subprocess.run(["cppcheck", "--enable=warning,style,performance,portability", "src"])
            except (FileNotFoundError, subprocess.CalledProcessError): pass

    def update_compiler(self):
        """Internal Logic: Pull the latest source code for the Vibe C Compiler from its remote Git repository."""
        print("Checking for updates...")
        try:
            # Sentinel: Verify Git repository status before attempting a pull to prevent environment corruption.
            res = subprocess.run(["git", "rev-parse", "--is-inside-work-tree"], capture_output=True, text=True, cwd=self.base_dir)
            if res.returncode == 0:
                print("Fetching latest version from GitHub...")
                if subprocess.run(["git", "pull", "origin", "main"], cwd=self.base_dir).returncode == 0:
                    print("Successfully updated Vibe C Compiler.")
                    self.show_version()
                else: print("Failed to update.")
            else: print("Error: Not a git repository.")
        except Exception as e: print(f"An error occurred: {e}")
