import os
import subprocess
import shutil
import json
import re
import concurrent.futures

class VibeCompiler:
    def __init__(self):
        # __file__ is vibe/core/compiler.py
        # dirname(dirname(dirname(__file__))) is the root directory
        self.base_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        self.vibe_dir = os.path.join(self.base_dir, "vibe")
        self.include_dir = os.path.join(self.vibe_dir, "include")
        self.template_dir = os.path.join(self.vibe_dir, "templates")
        self.version_file = os.path.join(self.base_dir, "VERSION")

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

    def _compile_file(self, src, include_dir, arch=None, extra_flags=None):
        """Compile a single C file to an object file with incremental check."""
        rel_path = os.path.relpath(src, "src")
        # Use splitext to handle extensions correctly and avoid replacing .c in middle of path
        base_name, _ = os.path.splitext(rel_path)
        obj = os.path.join("build", "obj", base_name + ".o")
        os.makedirs(os.path.dirname(obj), exist_ok=True)

        # Incremental build: skip if object file is newer than source
        if os.path.exists(obj) and os.path.getmtime(src) <= os.path.getmtime(obj):
            return obj

        print(f"  Compiling {src}...")
        cmd = ["clang", "-I" + include_dir, "-c", src, "-o", obj]
        if arch:
            cmd += ["-target", arch]
        if extra_flags:
            cmd += extra_flags

        res = subprocess.run(cmd, capture_output=True, text=True)
        if res.returncode != 0:
            print(f"Error compiling {src}:\n{res.stderr}")
            return None
        return obj

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

        proj_type = lib_type if lib_type and lib_type != "none" else config.get("type", "executable")

        if not os.path.exists("build"):
            os.makedirs("build")

        # Find all .c files in src
        src_files = []
        for root, _, files in os.walk("src"):
            for file in files:
                if file.endswith(".c"):
                    src_files.append(os.path.join(root, file))

        if not src_files:
            print("Error: No source files found in src/")
            return False

        # Prepare for parallel compilation
        print(f"Building {proj_name} ({proj_type})...")
        extra_flags = ["-fPIC"] if proj_type == "shared" else []

        # Ensure build/obj directory exists
        os.makedirs(os.path.join("build", "obj"), exist_ok=True)

        with concurrent.futures.ThreadPoolExecutor() as executor:
            # Map source files to compilation tasks
            futures = [executor.submit(self._compile_file, src, self.include_dir, arch, extra_flags) for src in src_files]
            obj_files = [f.result() for f in futures]

        if None in obj_files:
            print("Build failed due to compilation errors.")
            return False

        # Define output artifact name
        if proj_type == "static":
            output_name = f"build/lib{proj_name}.a"
        elif proj_type == "shared":
            output_name = f"build/lib{proj_name}.so"
        else:
            output_name = f"build/{proj_name}"

        # Linking stage
        if proj_type == "static":
            print(f"Creating static library {output_name}...")
            res = subprocess.run(["ar", "rcs", output_name] + obj_files)
        elif proj_type == "shared":
            print(f"Linking shared library {output_name}...")
            link_cmd = ["clang", "-shared", "-o", output_name] + obj_files
            if arch:
                link_cmd += ["-target", arch]
            res = subprocess.run(link_cmd)
        else:
            print(f"Linking executable {output_name}...")
            link_cmd = ["clang", "-o", output_name] + obj_files
            if arch:
                link_cmd += ["-target", arch]
            res = subprocess.run(link_cmd)

        if res.returncode != 0:
            print("Linking failed.")
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

        print("\n=== Vibe Project Status ===")
        print(f"Name:    {config.get('name', 'N/A')}")
        print(f"Version: {config.get('version', 'N/A')}")
        print(f"Type:    {config.get('type', 'executable')}")

        src_count = 0
        if os.path.exists("src"):
            for root, dirs, files in os.walk("src"):
                for file in files:
                    if file.endswith(".c"):
                        src_count += 1
        print(f"Sources: {src_count} .c files")

        if os.path.exists("build"):
            build_files = [f for f in os.listdir("build") if os.path.isfile(os.path.join("build", f))]
            print(f"Build:   {len(build_files)} artifacts in build/")
        else:
            print("Build:   No build directory found.")

    def _internal_c_audit(self, src_dir):
        print(f"\n--- Internal C Audit: {src_dir} ---")
        unsafe_funcs = {
            "gets": "Extremely unsafe, use fgets instead.",
            "strcpy": "Unsafe, use strncpy or strlcpy instead.",
            "strcat": "Unsafe, use strncat or strlcat instead.",
            "sprintf": "Unsafe, use snprintf instead.",
            "scanf": "Can be unsafe, use with field widths or use fgets/sscanf.",
        }

        issues_found = 0
        for root, dirs, files in os.walk(src_dir):
            for file in files:
                if file.endswith((".c", ".h")):
                    path = os.path.join(root, file)
                    try:
                        with open(path, "r", errors="ignore") as f:
                            for i, line in enumerate(f, 1):
                                for func, desc in unsafe_funcs.items():
                                    if f"{func}(" in line:
                                        print(f"  [!] {path}:{i} - Found potential unsafe function '{func}': {desc}")
                                        issues_found += 1
                    except Exception as e:
                        print(f"  [?] Could not read {path}: {e}")

        if issues_found == 0:
            print("  No obvious unsafe C functions found.")
        else:
            print(f"  Found {issues_found} potential issues.")

    def _internal_python_audit(self, py_dir):
        print(f"\n--- Internal Python Audit: {py_dir} ---")
        unsafe_patterns = {
            "eval(": "Unsafe, allows execution of arbitrary code.", # nosec
            "exec(": "Unsafe, allows execution of arbitrary code.", # nosec
            "shell=True": "Potential shell injection vulnerability.", # nosec
            "tempfile.mktemp": "Insecure, use tempfile.mkstemp instead.", # nosec
        }

        issues_found = 0
        for root, dirs, files in os.walk(py_dir):
            for file in files:
                if file.endswith(".py"):
                    path = os.path.join(root, file)
                    try:
                        with open(path, "r", errors="ignore") as f:
                            for i, line in enumerate(f, 1):
                                if "# nosec" in line:
                                    continue
                                for pattern, desc in unsafe_patterns.items():
                                    if pattern in line:
                                        print(f"  [!] {path}:{i} - Found unsafe pattern '{pattern}': {desc}")
                                        issues_found += 1
                    except Exception as e:
                        print(f"  [?] Could not read {path}: {e}")

        if issues_found == 0:
            print("  No obvious unsafe Python patterns found.")
        else:
            print(f"  Found {issues_found} potential issues.")

    def run_audit(self):
        print("\n=== Vibe Security Audit ===")

        # Run internal audits first (no dependencies)
        self._internal_python_audit(self.vibe_dir)
        if os.path.exists("src"):
            self._internal_c_audit("src")
        else:
            print("\nNote: No src/ directory found for C audit.")

        # Check for optional external tools
        print("\n--- Checking for advanced audit tools ---")

        # Check for bandit (Python security)
        try:
            import bandit # Still keep this local as it is an optional external dependency
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
            res = subprocess.run(["git", "rev-parse", "--is-inside-work-tree"], capture_output=True, text=True)
            if res.returncode != 0:
                print("Error: Not a git repository. Cannot update automatically.")
                return

            print("Fetching latest version from GitHub...")
            res = subprocess.run(["git", "pull", "origin", "main"])
            if res.returncode == 0:
                print("Successfully updated Vibe C Compiler.")
                # After update, version might have changed
                self.show_version()
            else:
                print("Failed to update. Please check your internet connection or run 'git pull' manually.")
        except Exception as e:
            print(f"An error occurred during update: {e}")
