"""
This module serves as the entry point for the Vibe C Compiler CLI, routing commands to the appropriate VibeCompiler methods.
In version 1.5.9, it supports the latest security audits, parallel builds, and high-performance SWAR-based hashing.
This code is AI-generated.
"""

import argparse
import sys
import os
from .compiler import VibeCompiler
from .menu import run_menu

def main():
    # Internal Logic: Define the CLI structure using argparse with support for various subcommands and flags.
    # The argparse module automatically handles help generation and argument validation for the commands.
    parser = argparse.ArgumentParser(description="Vibe C Compiler - The easier C compiler")
    subparsers = parser.add_subparsers(dest="command", help="Commands")

    # init: Initializes a new project from a template.
    init_parser = subparsers.add_parser("init", help="Initialize a new project")
    init_parser.add_argument("name", help="Name of the project")
    init_parser.add_argument("--template", default="basic", help="Template to use (basic, minimal)")

    # build: Compiles the project with optional architecture and library type flags.
    build_parser = subparsers.add_parser("build", help="Build the current project")
    build_parser.add_argument("--arch", help="Target architecture (e.g. x86_64, aarch64)")
    build_parser.add_argument("--lib", choices=["static", "shared", "none"], default="none", help="Build as a library")

    # run: Builds and then executes the project.
    run_parser = subparsers.add_parser("run", help="Build and run the current project")

    # clean: Removes all build artifacts.
    clean_parser = subparsers.add_parser("clean", help="Clean build artifacts")

    # test: Runs all unit tests found in the project's tests directory.
    test_parser = subparsers.add_parser("test", help="Run project tests")

    # menu: Launches the interactive Terminal User Interface (TUI).
    menu_parser = subparsers.add_parser("menu", help="Open the simple menu")

    # version: Displays the current version of the Vibe C Compiler.
    version_parser = subparsers.add_parser("version", help="Show version")

    # install/uninstall: Manages the global 'vcc' symlink in the user's local bin directory.
    subparsers.add_parser("install", help="Install vibe globally as 'vcc'")
    subparsers.add_parser("uninstall", help="Uninstall vibe global link")

    # headers/templates/status: Provides information about the environment and project.
    subparsers.add_parser("headers", help="List available Vibe headers")
    subparsers.add_parser("templates", help="List available templates")
    subparsers.add_parser("status", help="Show current project status")

    # audit: Performs a security scan of the project codebase.
    subparsers.add_parser("audit", help="Run a security audit on the current project")

    # update/upgrade: Pulls the latest version of the compiler from the remote repository.
    subparsers.add_parser("update", help="Update the compiler to the latest version from GitHub")
    subparsers.add_parser("upgrade", help="Alias for 'update'")

    args = parser.parse_args()
    compiler = VibeCompiler()

    # Internal Logic: Map the parsed command to the corresponding method in the VibeCompiler class instance.
    if args.command == "init":
        compiler.init_project(args.name, args.template)
    elif args.command == "build":
        compiler.build_project(arch=args.arch, lib_type=args.lib)
    elif args.command == "run":
        # Internal Logic: Run command first ensures the project is successfully built before attempting execution.
        if compiler.build_project():
            compiler.run_project()
    elif args.command == "clean":
        compiler.clean_project()
    elif args.command == "menu":
        run_menu(compiler)
    elif args.command == "version":
        compiler.show_version()
    elif args.command == "install":
        compiler.install_globally()
    elif args.command == "uninstall":
        compiler.uninstall_globally()
    elif args.command == "headers":
        compiler.list_headers()
    elif args.command == "templates":
        compiler.list_templates()
    elif args.command == "status":
        compiler.project_status()
    elif args.command == "test":
        compiler.run_tests()
    elif args.command == "audit":
        compiler.run_audit()
    elif args.command == "update" or args.command == "upgrade":
        compiler.update_compiler()
    else:
        # Internal Logic: Default to printing the help message if no command or an invalid command is provided.
        parser.print_help()

if __name__ == "__main__":
    main()
