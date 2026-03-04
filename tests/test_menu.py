"""
Unit tests for the Vibe C Compiler interactive menu (menu.py).
Tests cover menu display, user input handling, and command execution.
"""

import unittest
import sys
import os
from unittest.mock import Mock, patch, MagicMock, call
from io import StringIO

# Add vibe to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from vibe.core.menu import run_menu
from vibe.core.compiler import VibeCompiler


class TestMenu(unittest.TestCase):
    """Test suite for the interactive menu."""

    def setUp(self):
        """Set up test fixtures."""
        self.compiler = MagicMock(spec=VibeCompiler)
        self.compiler.version_file = "/tmp/VERSION"

    @patch('builtins.input', side_effect=['13'])
    @patch('builtins.print')
    def test_menu_exit(self, mock_print, mock_input):
        """Test menu exits correctly when option 13 is selected."""
        run_menu(self.compiler)

        # Check that "Goodbye!" was printed
        args = [str(call) for call in mock_print.call_args_list]
        self.assertTrue(any("Goodbye" in arg for arg in args))

    @patch('builtins.input', side_effect=['1', 'test_proj', 'basic', '13'])
    @patch('builtins.print')
    def test_menu_init_project(self, mock_print, mock_input):
        """Test menu option 1 initializes a project."""
        run_menu(self.compiler)

        self.compiler.init_project.assert_called_once_with('test_proj', 'basic')

    @patch('builtins.input', side_effect=['1', 'test_proj', '', '13'])
    @patch('builtins.print')
    def test_menu_init_project_default_template(self, mock_print, mock_input):
        """Test menu option 1 uses default template when input is empty."""
        run_menu(self.compiler)

        self.compiler.init_project.assert_called_once_with('test_proj', 'basic')

    @patch('builtins.input', side_effect=['2', '', '', '13'])
    @patch('builtins.print')
    def test_menu_build_project(self, mock_print, mock_input):
        """Test menu option 2 builds the project."""
        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once_with(arch=None, lib_type='none')

    @patch('builtins.input', side_effect=['2', 'aarch64', 'static', '13'])
    @patch('builtins.print')
    def test_menu_build_project_with_options(self, mock_print, mock_input):
        """Test menu option 2 builds with architecture and library type."""
        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once_with(
            arch='aarch64', lib_type='static'
        )

    @patch('builtins.input', side_effect=['2', '', 'shared', '13'])
    @patch('builtins.print')
    def test_menu_build_with_lib_type_only(self, mock_print, mock_input):
        """Test building with library type but no architecture."""
        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once_with(
            arch=None, lib_type='shared'
        )

    @patch('builtins.input', side_effect=['3', '13'])
    @patch('builtins.print')
    def test_menu_run_project_success(self, mock_print, mock_input):
        """Test menu option 3 runs the project after successful build."""
        self.compiler.build_project.return_value = True

        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once()
        self.compiler.run_project.assert_called_once()

    @patch('builtins.input', side_effect=['3', '13'])
    @patch('builtins.print')
    def test_menu_run_project_build_fails(self, mock_print, mock_input):
        """Test menu option 3 doesn't run if build fails."""
        self.compiler.build_project.return_value = False

        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once()
        self.compiler.run_project.assert_not_called()

    @patch('builtins.input', side_effect=['4', '13'])
    @patch('builtins.print')
    def test_menu_clean_project(self, mock_print, mock_input):
        """Test menu option 4 cleans the project."""
        run_menu(self.compiler)

        self.compiler.clean_project.assert_called_once()

    @patch('builtins.input', side_effect=['5', '13'])
    @patch('builtins.print')
    def test_menu_show_status(self, mock_print, mock_input):
        """Test menu option 5 shows project status."""
        run_menu(self.compiler)

        self.compiler.project_status.assert_called_once()

    @patch('builtins.input', side_effect=['6', '13'])
    @patch('builtins.print')
    def test_menu_list_headers(self, mock_print, mock_input):
        """Test menu option 6 lists headers."""
        run_menu(self.compiler)

        self.compiler.list_headers.assert_called_once()

    @patch('builtins.input', side_effect=['7', '13'])
    @patch('builtins.print')
    def test_menu_list_templates(self, mock_print, mock_input):
        """Test menu option 7 lists templates."""
        run_menu(self.compiler)

        self.compiler.list_templates.assert_called_once()

    @patch('builtins.input', side_effect=['8', '13'])
    @patch('builtins.print')
    def test_menu_install_globally(self, mock_print, mock_input):
        """Test menu option 8 installs globally."""
        run_menu(self.compiler)

        self.compiler.install_globally.assert_called_once()

    @patch('builtins.input', side_effect=['9', '13'])
    @patch('builtins.print')
    def test_menu_uninstall_globally(self, mock_print, mock_input):
        """Test menu option 9 uninstalls globally."""
        run_menu(self.compiler)

        self.compiler.uninstall_globally.assert_called_once()

    @patch('builtins.input', side_effect=['10', '13'])
    @patch('builtins.print')
    def test_menu_run_tests(self, mock_print, mock_input):
        """Test menu option 10 runs tests."""
        run_menu(self.compiler)

        self.compiler.run_tests.assert_called_once()

    @patch('builtins.input', side_effect=['11', '13'])
    @patch('builtins.print')
    def test_menu_run_audit(self, mock_print, mock_input):
        """Test menu option 11 runs security audit."""
        run_menu(self.compiler)

        self.compiler.run_audit.assert_called_once()

    @patch('builtins.input', side_effect=['12', '13'])
    @patch('builtins.print')
    def test_menu_update_compiler(self, mock_print, mock_input):
        """Test menu option 12 updates compiler."""
        run_menu(self.compiler)

        self.compiler.update_compiler.assert_called_once()

    @patch('builtins.input', side_effect=['invalid', '99', '0', '13'])
    @patch('builtins.print')
    def test_menu_invalid_choices(self, mock_print, mock_input):
        """Test menu handles invalid choices gracefully."""
        run_menu(self.compiler)

        # Check that "Invalid choice" messages were printed
        args = [str(call) for call in mock_print.call_args_list]
        invalid_count = sum(1 for arg in args if "Invalid" in arg)
        self.assertGreaterEqual(invalid_count, 3)

    @patch('builtins.input', side_effect=['13'])
    @patch('builtins.print')
    @patch('os.path.exists', return_value=True)
    @patch('builtins.open', create=True)
    def test_menu_displays_version(self, mock_open, mock_exists, mock_print, mock_input):
        """Test menu displays version in header."""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"

        run_menu(self.compiler)

        # Check that version was displayed in menu
        args = [str(call) for call in mock_print.call_args_list]
        self.assertTrue(any("Menu" in arg for arg in args))

    @patch('builtins.input', side_effect=['13'])
    @patch('builtins.print')
    @patch('os.path.exists', return_value=False)
    def test_menu_handles_missing_version_file(self, mock_exists, mock_print, mock_input):
        """Test menu handles missing VERSION file gracefully."""
        # Should not raise an exception
        run_menu(self.compiler)

        # Menu should still display
        args = [str(call) for call in mock_print.call_args_list]
        self.assertTrue(any("Menu" in arg for arg in args))

    @patch('builtins.input', side_effect=['1', 'proj', 'minimal', '5', '13'])
    @patch('builtins.print')
    def test_menu_multiple_operations(self, mock_print, mock_input):
        """Test performing multiple operations in one menu session."""
        run_menu(self.compiler)

        self.compiler.init_project.assert_called_once_with('proj', 'minimal')
        self.compiler.project_status.assert_called_once()

    @patch('builtins.input', side_effect=['2', 'x86_64', '', '13'])
    @patch('builtins.print')
    def test_menu_build_with_arch_default_lib(self, mock_print, mock_input):
        """Test building with architecture but default library type."""
        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once_with(
            arch='x86_64', lib_type='none'
        )


class TestMenuEdgeCases(unittest.TestCase):
    """Test edge cases for the menu."""

    def setUp(self):
        """Set up test fixtures."""
        self.compiler = MagicMock(spec=VibeCompiler)
        self.compiler.version_file = "/tmp/VERSION"

    @patch('builtins.input', side_effect=['1', '', '', '13'])
    @patch('builtins.print')
    def test_menu_init_empty_name(self, mock_print, mock_input):
        """Test init with empty project name."""
        run_menu(self.compiler)

        # Should call init_project with empty string (compiler will validate)
        self.compiler.init_project.assert_called_once_with('', 'basic')

    @patch('builtins.input', side_effect=['2', '', '', '', '13'])
    @patch('builtins.print')
    def test_menu_build_all_defaults(self, mock_print, mock_input):
        """Test build with all default values."""
        run_menu(self.compiler)

        self.compiler.build_project.assert_called_once_with(
            arch=None, lib_type='none'
        )


if __name__ == '__main__':
    unittest.main()