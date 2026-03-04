"""
Comprehensive unit tests for vibe/core/menu.py
Tests the interactive menu functionality.
This code is AI-generated.
"""

import unittest
import sys
import os
from unittest.mock import patch, MagicMock, call
from io import StringIO

# Add parent directory to path to import vibe modules
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from vibe.core.menu import run_menu


class TestMenu(unittest.TestCase):
    """Tests for interactive menu"""

    def setUp(self):
        self.mock_compiler = MagicMock()
        self.mock_compiler.version_file = "/tmp/VERSION"

    @patch('builtins.input', side_effect=['13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_exit(self, mock_exists, mock_open, mock_input):
        """Test that option 13 exits the menu"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        # Should exit cleanly

    @patch('builtins.input', side_effect=['1', 'testproj', 'basic', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_init_project(self, mock_exists, mock_open, mock_input):
        """Test that option 1 initializes a project"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.init_project.assert_called_once_with('testproj', 'basic')

    @patch('builtins.input', side_effect=['2', '', 'none', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_build_project(self, mock_exists, mock_open, mock_input):
        """Test that option 2 builds a project"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.build_project.assert_called_once()

    @patch('builtins.input', side_effect=['3', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_run_project(self, mock_exists, mock_open, mock_input):
        """Test that option 3 runs a project"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        self.mock_compiler.build_project.return_value = True
        run_menu(self.mock_compiler)
        self.mock_compiler.build_project.assert_called_once()
        self.mock_compiler.run_project.assert_called_once()

    @patch('builtins.input', side_effect=['3', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_run_skips_on_build_failure(self, mock_exists, mock_open, mock_input):
        """Test that run skips execution if build fails"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        self.mock_compiler.build_project.return_value = False
        run_menu(self.mock_compiler)
        self.mock_compiler.build_project.assert_called_once()
        self.mock_compiler.run_project.assert_not_called()

    @patch('builtins.input', side_effect=['4', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_clean_project(self, mock_exists, mock_open, mock_input):
        """Test that option 4 cleans the project"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.clean_project.assert_called_once()

    @patch('builtins.input', side_effect=['5', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_show_status(self, mock_exists, mock_open, mock_input):
        """Test that option 5 shows project status"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.project_status.assert_called_once()

    @patch('builtins.input', side_effect=['6', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_list_headers(self, mock_exists, mock_open, mock_input):
        """Test that option 6 lists headers"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.list_headers.assert_called_once()

    @patch('builtins.input', side_effect=['7', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_list_templates(self, mock_exists, mock_open, mock_input):
        """Test that option 7 lists templates"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.list_templates.assert_called_once()

    @patch('builtins.input', side_effect=['8', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_install_globally(self, mock_exists, mock_open, mock_input):
        """Test that option 8 installs globally"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.install_globally.assert_called_once()

    @patch('builtins.input', side_effect=['9', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_uninstall_globally(self, mock_exists, mock_open, mock_input):
        """Test that option 9 uninstalls globally"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.uninstall_globally.assert_called_once()

    @patch('builtins.input', side_effect=['10', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_run_tests(self, mock_exists, mock_open, mock_input):
        """Test that option 10 runs tests"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.run_tests.assert_called_once()

    @patch('builtins.input', side_effect=['11', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_run_audit(self, mock_exists, mock_open, mock_input):
        """Test that option 11 runs security audit"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.run_audit.assert_called_once()

    @patch('builtins.input', side_effect=['12', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_update_compiler(self, mock_exists, mock_open, mock_input):
        """Test that option 12 updates compiler"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.update_compiler.assert_called_once()

    @patch('builtins.input', side_effect=['999', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_invalid_choice(self, mock_exists, mock_open, mock_input):
        """Test that invalid choice shows error and continues"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        # Should handle gracefully and continue to exit

    @patch('builtins.input', side_effect=['2', 'aarch64-linux-gnu', 'static', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_build_with_options(self, mock_exists, mock_open, mock_input):
        """Test build with architecture and library type"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.build_project.assert_called_once_with(arch='aarch64-linux-gnu', lib_type='static')

    @patch('builtins.input', side_effect=['1', 'myapp', '', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_init_with_default_template(self, mock_exists, mock_open, mock_input):
        """Test init with default template (empty input)"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.init_project.assert_called_once_with('myapp', 'basic')

    @patch('builtins.input', side_effect=['2', '', '', '13'])
    @patch('builtins.open')
    @patch('os.path.exists', return_value=True)
    def test_menu_build_with_defaults(self, mock_exists, mock_open, mock_input):
        """Test build with all default options"""
        mock_open.return_value.__enter__.return_value.read.return_value = "1.5.11"
        run_menu(self.mock_compiler)
        self.mock_compiler.build_project.assert_called_once_with(arch=None, lib_type='none')


class TestMenuVersionDisplay(unittest.TestCase):
    """Tests for version display in menu"""

    @patch('builtins.input', side_effect=['13'])
    @patch('os.path.exists', return_value=False)
    def test_menu_missing_version_file(self, mock_exists, mock_input):
        """Test menu handles missing version file"""
        mock_compiler = MagicMock()
        mock_compiler.version_file = "/nonexistent/VERSION"
        run_menu(mock_compiler)
        # Should display "unknown" version and continue


if __name__ == '__main__':
    unittest.main()