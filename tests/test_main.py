"""
Unit tests for the Vibe C Compiler CLI entry point (main.py).
Tests cover argument parsing, command routing, and error handling.
"""

import unittest
import sys
import os
from unittest.mock import Mock, patch, MagicMock
from io import StringIO

# Add vibe to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from vibe.core.main import main


class TestMainCLI(unittest.TestCase):
    """Test suite for the CLI entry point."""

    def setUp(self):
        """Set up test fixtures."""
        self.original_argv = sys.argv.copy()

    def tearDown(self):
        """Restore original sys.argv."""
        sys.argv = self.original_argv

    @patch('vibe.core.main.VibeCompiler')
    def test_init_command(self, mock_compiler_class):
        """Test 'init' command calls init_project with correct arguments."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'init', 'my_project']
        main()

        mock_compiler.init_project.assert_called_once_with('my_project', 'basic')

    @patch('vibe.core.main.VibeCompiler')
    def test_init_command_with_template(self, mock_compiler_class):
        """Test 'init' command with custom template."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'init', 'my_project', '--template', 'minimal']
        main()

        mock_compiler.init_project.assert_called_once_with('my_project', 'minimal')

    @patch('vibe.core.main.VibeCompiler')
    def test_build_command(self, mock_compiler_class):
        """Test 'build' command calls build_project."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'build']
        main()

        mock_compiler.build_project.assert_called_once_with(arch=None, lib_type='none')

    @patch('vibe.core.main.VibeCompiler')
    def test_build_command_with_arch(self, mock_compiler_class):
        """Test 'build' command with architecture flag."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'build', '--arch', 'aarch64-linux-gnu']
        main()

        mock_compiler.build_project.assert_called_once_with(
            arch='aarch64-linux-gnu', lib_type='none'
        )

    @patch('vibe.core.main.VibeCompiler')
    def test_build_command_with_lib_type(self, mock_compiler_class):
        """Test 'build' command with library type."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'build', '--lib', 'static']
        main()

        mock_compiler.build_project.assert_called_once_with(arch=None, lib_type='static')

    @patch('vibe.core.main.VibeCompiler')
    def test_run_command_success(self, mock_compiler_class):
        """Test 'run' command builds and then runs project."""
        mock_compiler = MagicMock()
        mock_compiler.build_project.return_value = True
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'run']
        main()

        mock_compiler.build_project.assert_called_once()
        mock_compiler.run_project.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_run_command_build_fails(self, mock_compiler_class):
        """Test 'run' command doesn't run project if build fails."""
        mock_compiler = MagicMock()
        mock_compiler.build_project.return_value = False
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'run']
        main()

        mock_compiler.build_project.assert_called_once()
        mock_compiler.run_project.assert_not_called()

    @patch('vibe.core.main.VibeCompiler')
    def test_clean_command(self, mock_compiler_class):
        """Test 'clean' command calls clean_project."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'clean']
        main()

        mock_compiler.clean_project.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_test_command(self, mock_compiler_class):
        """Test 'test' command calls run_tests."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'test']
        main()

        mock_compiler.run_tests.assert_called_once()

    @patch('vibe.core.main.run_menu')
    @patch('vibe.core.main.VibeCompiler')
    def test_menu_command(self, mock_compiler_class, mock_run_menu):
        """Test 'menu' command launches interactive menu."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'menu']
        main()

        mock_run_menu.assert_called_once_with(mock_compiler)

    @patch('vibe.core.main.VibeCompiler')
    def test_version_command(self, mock_compiler_class):
        """Test 'version' command displays version."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'version']
        main()

        mock_compiler.show_version.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_install_command(self, mock_compiler_class):
        """Test 'install' command calls install_globally."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'install']
        main()

        mock_compiler.install_globally.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_uninstall_command(self, mock_compiler_class):
        """Test 'uninstall' command calls uninstall_globally."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'uninstall']
        main()

        mock_compiler.uninstall_globally.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_headers_command(self, mock_compiler_class):
        """Test 'headers' command lists available headers."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'headers']
        main()

        mock_compiler.list_headers.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_templates_command(self, mock_compiler_class):
        """Test 'templates' command lists available templates."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'templates']
        main()

        mock_compiler.list_templates.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_status_command(self, mock_compiler_class):
        """Test 'status' command shows project status."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'status']
        main()

        mock_compiler.project_status.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_audit_command(self, mock_compiler_class):
        """Test 'audit' command runs security audit."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'audit']
        main()

        mock_compiler.run_audit.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_update_command(self, mock_compiler_class):
        """Test 'update' command updates compiler."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'update']
        main()

        mock_compiler.update_compiler.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_upgrade_command(self, mock_compiler_class):
        """Test 'upgrade' command (alias for update)."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'upgrade']
        main()

        mock_compiler.update_compiler.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    def test_no_command_prints_help(self, mock_compiler_class):
        """Test that running with no command prints help."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc']

        # Without a command, it should just print help (not all argparse versions exit)
        main()

        # Verify no compiler methods were called
        self.assertEqual(mock_compiler.method_calls, [])

    @patch('vibe.core.main.VibeCompiler')
    def test_invalid_lib_type_rejected(self, mock_compiler_class):
        """Test that invalid library types are rejected by argparse."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'build', '--lib', 'invalid']

        with self.assertRaises(SystemExit):
            main()


class TestMainEdgeCases(unittest.TestCase):
    """Test edge cases for the CLI."""

    def setUp(self):
        """Set up test fixtures."""
        self.original_argv = sys.argv.copy()

    def tearDown(self):
        """Restore original sys.argv."""
        sys.argv = self.original_argv

    @patch('vibe.core.main.VibeCompiler')
    def test_combined_build_flags(self, mock_compiler_class):
        """Test build with both arch and lib type."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        sys.argv = ['vcc', 'build', '--arch', 'x86_64', '--lib', 'shared']
        main()

        mock_compiler.build_project.assert_called_once_with(
            arch='x86_64', lib_type='shared'
        )

    @patch('vibe.core.main.VibeCompiler')
    def test_all_lib_type_options(self, mock_compiler_class):
        """Test all valid library type options."""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler

        for lib_type in ['static', 'shared', 'none']:
            mock_compiler.reset_mock()
            sys.argv = ['vcc', 'build', '--lib', lib_type]
            main()
            mock_compiler.build_project.assert_called_once_with(
                arch=None, lib_type=lib_type
            )


if __name__ == '__main__':
    unittest.main()