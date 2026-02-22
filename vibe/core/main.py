"""
This file has been updated to include comprehensive documentation and logic explanations.
The changes enhance maintainability by detailing the CLI argument parsing and command dispatching process.
THIS CODE IS AI GENERATED.
"""

import argparse
import sys
import os
from .compiler import VibeCompiler
from .menu import run_menu

def main():
    """Parses command-line arguments and executes the corresponding compiler actions."""
    parser = argparse.ArgumentParser(description="Vibe C Compiler - The easier C compiler")
    subparsers = parser.add_subparsers(dest="command", help="Commands")

    # init
    init_parser = subparsers.add_parser("init", help="Initialize a new project")
    init_parser.add_argument("name", help="Name of the project")
    init_parser.add_argument("--template", default="basic", help="Template to use (basic, minimal)")

    # build
    build_parser = subparsers.add_parser("build", help="Build the current project")
    build_parser.add_argument("--arch", help="Target architecture (e.g. x86_64, aarch64)")
    build_parser.add_argument("--lib", choices=["static", "shared", "none"], default="none", help="Build as a library")

    # run
    run_parser = subparsers.add_parser("run", help="Build and run the current project")

    # clean
    clean_parser = subparsers.add_parser("clean", help="Clean build artifacts")

    # test
    test_parser = subparsers.add_parser("test", help="Run project tests")

    # menu
    menu_parser = subparsers.add_parser("menu", help="Open the simple menu")

    # version
    version_parser = subparsers.add_parser("version", help="Show version")

    # install
    install_parser = subparsers.add_parser("install", help="Install vibe globally as 'vcc'")

    # uninstall
    uninstall_parser = subparsers.add_parser("uninstall", help="Uninstall vibe global link")

    # headers
    headers_parser = subparsers.add_parser("headers", help="List available Vibe headers")

    # templates
    templates_parser = subparsers.add_parser("templates", help="List available templates")

    # status
    status_parser = subparsers.add_parser("status", help="Show current project status")

    # audit
    audit_parser = subparsers.add_parser("audit", help="Run a security audit on the current project")

    # update
    update_parser = subparsers.add_parser("update", help="Update the compiler to the latest version from GitHub")
    upgrade_parser = subparsers.add_parser("upgrade", help="Alias for 'update'")

    args = parser.parse_args()

    compiler = VibeCompiler()

    if args.command == "init":
        compiler.init_project(args.name, args.template)
    elif args.command == "build":
        compiler.build_project(arch=args.arch, lib_type=args.lib)
    elif args.command == "run":
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
        parser.print_help()

if __name__ == "__main__":
    main()
