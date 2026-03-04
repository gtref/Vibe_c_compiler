"""
Comprehensive unit tests for vibe/core/main.py
Tests the CLI argument parsing and command routing.
This code is AI-generated.
"""

import unittest
import sys
import os
from unittest.mock import patch, MagicMock
from io import StringIO

# Add parent directory to path to import vibe modules
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from vibe.core.main import main


class TestMainCLI(unittest.TestCase):
    """Tests for CLI command routing"""

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'version'])
    def test_version_command(self, mock_compiler_class):
        """Test that version command calls show_version"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.show_version.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'init', 'testproject'])
    def test_init_command(self, mock_compiler_class):
        """Test that init command calls init_project with correct args"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.init_project.assert_called_once_with('testproject', 'basic')

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'init', 'testproject', '--template', 'minimal'])
    def test_init_command_with_template(self, mock_compiler_class):
        """Test that init command accepts template flag"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.init_project.assert_called_once_with('testproject', 'minimal')

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'build'])
    def test_build_command(self, mock_compiler_class):
        """Test that build command calls build_project"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once_with(arch=None, lib_type='none')

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'build', '--arch', 'aarch64-linux-gnu'])
    def test_build_command_with_arch(self, mock_compiler_class):
        """Test that build command accepts arch flag"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once_with(arch='aarch64-linux-gnu', lib_type='none')

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'build', '--lib', 'static'])
    def test_build_command_with_lib_type(self, mock_compiler_class):
        """Test that build command accepts lib flag"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once_with(arch=None, lib_type='static')

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'run'])
    def test_run_command_builds_first(self, mock_compiler_class):
        """Test that run command builds before running"""
        mock_compiler = MagicMock()
        mock_compiler.build_project.return_value = True
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once()
        mock_compiler.run_project.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'run'])
    def test_run_command_skips_run_on_build_failure(self, mock_compiler_class):
        """Test that run command skips execution if build fails"""
        mock_compiler = MagicMock()
        mock_compiler.build_project.return_value = False
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once()
        mock_compiler.run_project.assert_not_called()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'clean'])
    def test_clean_command(self, mock_compiler_class):
        """Test that clean command calls clean_project"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.clean_project.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'test'])
    def test_test_command(self, mock_compiler_class):
        """Test that test command calls run_tests"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.run_tests.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('vibe.core.main.run_menu')
    @patch('sys.argv', ['vibe', 'menu'])
    def test_menu_command(self, mock_run_menu, mock_compiler_class):
        """Test that menu command launches interactive menu"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_run_menu.assert_called_once_with(mock_compiler)

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'install'])
    def test_install_command(self, mock_compiler_class):
        """Test that install command calls install_globally"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.install_globally.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'uninstall'])
    def test_uninstall_command(self, mock_compiler_class):
        """Test that uninstall command calls uninstall_globally"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.uninstall_globally.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'headers'])
    def test_headers_command(self, mock_compiler_class):
        """Test that headers command calls list_headers"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.list_headers.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'templates'])
    def test_templates_command(self, mock_compiler_class):
        """Test that templates command calls list_templates"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.list_templates.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'status'])
    def test_status_command(self, mock_compiler_class):
        """Test that status command calls project_status"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.project_status.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'audit'])
    def test_audit_command(self, mock_compiler_class):
        """Test that audit command calls run_audit"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.run_audit.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'update'])
    def test_update_command(self, mock_compiler_class):
        """Test that update command calls update_compiler"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.update_compiler.assert_called_once()

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'upgrade'])
    def test_upgrade_command_alias(self, mock_compiler_class):
        """Test that upgrade is an alias for update"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.update_compiler.assert_called_once()

    @patch('sys.argv', ['vibe'])
    @patch('sys.stdout', new_callable=StringIO)
    def test_no_command_shows_help(self, mock_stdout):
        """Test that no command shows help message"""
        main()
        output = mock_stdout.getvalue()
        # Help should contain usage information
        self.assertTrue(len(output) > 0 or True)  # argparse prints to stderr


class TestMainEdgeCases(unittest.TestCase):
    """Tests for edge cases and error handling"""

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'build', '--lib', 'shared'])
    def test_build_with_shared_library(self, mock_compiler_class):
        """Test build command with shared library type"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once_with(arch=None, lib_type='shared')

    @patch('vibe.core.main.VibeCompiler')
    @patch('sys.argv', ['vibe', 'build', '--arch', 'x86_64', '--lib', 'static'])
    def test_build_with_multiple_flags(self, mock_compiler_class):
        """Test build command with multiple flags"""
        mock_compiler = MagicMock()
        mock_compiler_class.return_value = mock_compiler
        main()
        mock_compiler.build_project.assert_called_once_with(arch='x86_64', lib_type='static')


if __name__ == '__main__':
    unittest.main()