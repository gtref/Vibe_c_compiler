"""
This module provides an interactive terminal-based menu (TUI) for the Vibe C Compiler.
In version 1.5.11, it provides easy access to the hardened build system, parallel test runner, and security auditing tools.
This code is AI-generated.
"""

import os
import sys

def run_menu(compiler):
    """
    Launches the interactive menu loop.
    Internal Logic: Continuously prompts the user for input and executes the corresponding VibeCompiler action based on the selection.
    """
    # Internal Logic: Attempt to read the version information to display in the menu header.
    version = "unknown"
    if os.path.exists(compiler.version_file):
        with open(compiler.version_file, "r") as f:
            version = f.read().strip()

    while True:
        # Internal Logic: Output the menu options to the standard output.
        print(f"\n=== Vibe C Compiler Menu (v{version}) ===")
        print("1. Initialize New Project")
        print("2. Build Project")
        print("3. Run Project")
        print("4. Clean Project")
        print("5. Show Project Status")
        print("6. List Vibe Headers")
        print("7. List Vibe Templates")
        print("8. Install Globally (vcc)")
        print("9. Uninstall Globally (vcc)")
        print("10. Run Tests")
        print("11. Run Security Audit")
        print("12. Update/Upgrade Compiler")
        print("13. Exit")

        # Internal Logic: Capture user input and sanitize it implicitly by matching against known options.
        choice = input("\nSelect an option (1-13): ")

        # Internal Logic: Dispatch the user's choice to the appropriate compiler method.
        if choice == "1":
            name = input("Enter project name: ")
            template = input("Enter template (basic/minimal, default=basic): ") or "basic"
            compiler.init_project(name, template)
        elif choice == "2":
            arch = input("Enter target architecture (leave blank for default): ")
            lib = input("Enter library type (none/static/shared, default=none): ") or "none"
            compiler.build_project(arch=arch if arch else None, lib_type=lib)
        elif choice == "3":
            # Internal Logic: Run command builds the project automatically if needed.
            if compiler.build_project():
                compiler.run_project()
        elif choice == "4":
            compiler.clean_project()
        elif choice == "5":
            compiler.project_status()
        elif choice == "6":
            compiler.list_headers()
        elif choice == "7":
            compiler.list_templates()
        elif choice == "8":
            compiler.install_globally()
        elif choice == "9":
            compiler.uninstall_globally()
        elif choice == "10":
            compiler.run_tests()
        elif choice == "11":
            compiler.run_audit()
        elif choice == "12":
            compiler.update_compiler()
        elif choice == "13":
            print("Goodbye!")
            break
        else:
            print("Invalid choice, please try again.")
