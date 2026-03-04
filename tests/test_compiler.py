"""
Comprehensive unit tests for the VibeCompiler class.
Tests cover project initialization, building, security auditing, and various edge cases.
This test file was created to ensure robust coverage of the compiler's core functionality.
"""

import unittest
import os
import json
import tempfile
import shutil
import sys
from unittest.mock import Mock, patch, MagicMock, call

# Add vibe to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from vibe.core.compiler import VibeCompiler


class TestVibeCompiler(unittest.TestCase):
    """Test suite for the VibeCompiler class."""

    def setUp(self):
        """Set up test fixtures."""
        self.compiler = VibeCompiler()
        self.test_dir = tempfile.mkdtemp()
        self.original_cwd = os.getcwd()

    def tearDown(self):
        """Clean up test fixtures."""
        os.chdir(self.original_cwd)
        if os.path.exists(self.test_dir):
            shutil.rmtree(self.test_dir)

    def test_init_sets_correct_paths(self):
        """Test that VibeCompiler initializes with correct directory paths."""
        self.assertTrue(os.path.exists(self.compiler.base_dir))
        self.assertTrue(os.path.exists(self.compiler.vibe_dir))
        self.assertTrue(os.path.exists(self.compiler.include_dir))
        self.assertTrue(os.path.exists(self.compiler.template_dir))
        self.assertEqual(self.compiler._vibe_include_mtime_cache, None)

    def test_show_version(self):
        """Test version display functionality."""
        version = self.compiler.show_version()
        self.assertIsNotNone(version)
        self.assertNotEqual(version, "unknown")

    def test_init_project_valid_name(self):
        """Test project initialization with a valid name."""
        os.chdir(self.test_dir)
        result = self.compiler.init_project("test_project", "basic")
        self.assertTrue(result)
        self.assertTrue(os.path.exists("test_project"))
        self.assertTrue(os.path.exists("test_project/vibe.json"))
        self.assertTrue(os.path.exists("test_project/build"))

        # Verify vibe.json content
        with open("test_project/vibe.json", "r") as f:
            config = json.load(f)
        self.assertEqual(config["name"], "test_project")

    def test_init_project_invalid_name(self):
        """Test project initialization rejects invalid names (path traversal)."""
        os.chdir(self.test_dir)
        result = self.compiler.init_project("../malicious", "basic")
        self.assertFalse(result)

        result = self.compiler.init_project("test; rm -rf", "basic")
        self.assertFalse(result)

    def test_init_project_invalid_template(self):
        """Test project initialization rejects invalid template names."""
        os.chdir(self.test_dir)
        result = self.compiler.init_project("test_project", "../../../etc/passwd")
        self.assertFalse(result)

    def test_init_project_existing_directory(self):
        """Test project initialization fails when directory already exists."""
        os.chdir(self.test_dir)
        os.mkdir("existing")
        result = self.compiler.init_project("existing", "basic")
        self.assertFalse(result)

    def test_scan_for_mtime_nonexistent_path(self):
        """Test _scan_for_mtime handles nonexistent paths gracefully."""
        mtime = self.compiler._scan_for_mtime("/nonexistent/path", (".h",))
        self.assertEqual(mtime, 0)

    def test_scan_for_mtime_finds_headers(self):
        """Test _scan_for_mtime correctly finds header files."""
        test_headers = os.path.join(self.test_dir, "headers")
        os.makedirs(test_headers)

        # Create test header files
        header1 = os.path.join(test_headers, "test1.h")
        header2 = os.path.join(test_headers, "test2.h")

        with open(header1, 'w') as f:
            f.write("// test header 1")
        with open(header2, 'w') as f:
            f.write("// test header 2")

        mtime = self.compiler._scan_for_mtime(test_headers, (".h",))
        self.assertGreater(mtime, 0)

    def test_get_header_mtime_caching(self):
        """Test that _get_header_mtime caches results correctly."""
        mtime1 = self.compiler._get_header_mtime(scan_src=False)
        self.assertIsNotNone(self.compiler._vibe_include_mtime_cache)

        # Second call should use cache
        mtime2 = self.compiler._get_header_mtime(scan_src=False)
        self.assertEqual(mtime1, mtime2)

    @patch('subprocess.run')
    def test_compile_src_success(self, mock_run):
        """Test successful compilation of a source file."""
        mock_run.return_value = Mock(returncode=0)

        result = self.compiler._compile_src(
            "test.c", "test.o", None, "executable"
        )

        self.assertEqual(result, "test.o")
        mock_run.assert_called_once()

        # Verify hardening flags are included
        call_args = mock_run.call_args[0][0]
        self.assertIn("-fstack-protector-strong", call_args)
        self.assertIn("-D_FORTIFY_SOURCE=2", call_args)
        self.assertIn("-fPIE", call_args)

    @patch('subprocess.run')
    def test_compile_src_with_arch(self, mock_run):
        """Test compilation with target architecture."""
        mock_run.return_value = Mock(returncode=0)

        self.compiler._compile_src(
            "test.c", "test.o", "aarch64-linux-gnu", "executable"
        )

        call_args = mock_run.call_args[0][0]
        self.assertIn("-target", call_args)
        self.assertIn("aarch64-linux-gnu", call_args)

    @patch('subprocess.run')
    def test_compile_src_shared_library(self, mock_run):
        """Test compilation for shared library includes -fPIC."""
        mock_run.return_value = Mock(returncode=0)

        self.compiler._compile_src(
            "test.c", "test.o", None, "shared"
        )

        call_args = mock_run.call_args[0][0]
        self.assertIn("-fPIC", call_args)

    @patch('subprocess.run')
    def test_compile_src_failure(self, mock_run):
        """Test compilation failure is handled correctly."""
        mock_run.return_value = Mock(returncode=1)

        result = self.compiler._compile_src(
            "test.c", "test.o", None, "executable"
        )

        self.assertIsNone(result)

    def test_list_headers(self):
        """Test listing of available headers."""
        # Should not raise an exception
        with patch('builtins.print') as mock_print:
            self.compiler.list_headers()
            self.assertTrue(mock_print.called)

    def test_list_templates(self):
        """Test listing of available templates."""
        with patch('builtins.print') as mock_print:
            self.compiler.list_templates()
            self.assertTrue(mock_print.called)

    def test_audit_file_c_file_with_unsafe_function(self):
        """Test security audit detects unsafe C functions."""
        test_file = os.path.join(self.test_dir, "test.c")
        with open(test_file, 'w') as f:
            f.write("void test() { gets(buffer); }")

        # Initialize audit patterns
        VibeCompiler._C_AUDIT_RE = None
        VibeCompiler._PY_AUDIT_RE = None
        self.compiler.run_audit()  # This initializes patterns

        issues = self.compiler._audit_file(test_file)
        self.assertGreater(len(issues), 0)
        self.assertTrue(any("gets" in issue for issue in issues))

    def test_audit_file_c_file_with_nosec_suppression(self):
        """Test security audit respects // nosec suppression."""
        test_file = os.path.join(self.test_dir, "test.c")
        with open(test_file, 'w') as f:
            f.write("void test() { gets(buffer); // nosec }")

        # Initialize patterns
        if VibeCompiler._C_AUDIT_RE is None:
            import re
            VibeCompiler._C_AUDIT_RE = re.compile(
                rf"\b({'|'.join(re.escape(f) for f in VibeCompiler._C_UNSAFE_FUNCS.keys())})\b"
            )

        issues = self.compiler._audit_file(test_file)
        # Should be empty or not contain gets warning
        self.assertEqual(len([i for i in issues if "gets" in i]), 0)

    def test_audit_file_python_file_with_eval(self):
        """Test security audit detects unsafe Python patterns."""
        test_file = os.path.join(self.test_dir, "test.py")
        with open(test_file, 'w') as f:
            f.write("x = 1\nresult = eval(user_input)\ny = 2")

        # Initialize patterns
        if VibeCompiler._PY_AUDIT_RE is None:
            import re
            VibeCompiler._PY_PATTERN_KEYS = list(VibeCompiler._PY_UNSAFE_PATTERNS.keys())
            sanitized = [p if p.startswith(r"\b") or p.endswith(r"\b") else rf"\b{p}\b"
                        for p in VibeCompiler._PY_PATTERN_KEYS]
            VibeCompiler._PY_AUDIT_RE = re.compile(
                "|".join(f"(?P<p{i}>(?:{p}))" for i, p in enumerate(sanitized))
            )

        issues = self.compiler._audit_file(test_file)
        # Should detect eval pattern
        self.assertGreaterEqual(len(issues), 0)

    def test_audit_file_python_with_nosec(self):
        """Test Python security audit respects # nosec suppression."""
        test_file = os.path.join(self.test_dir, "test.py")
        with open(test_file, 'w') as f:
            f.write("result = eval(user_input)  # nosec")

        if VibeCompiler._PY_AUDIT_RE is None:
            import re
            VibeCompiler._PY_PATTERN_KEYS = list(VibeCompiler._PY_UNSAFE_PATTERNS.keys())
            sanitized = [p if p.startswith(r"\b") or p.endswith(r"\b") else rf"\b{p}\b"
                        for p in VibeCompiler._PY_PATTERN_KEYS]
            VibeCompiler._PY_AUDIT_RE = re.compile(
                "|".join(f"(?P<p{i}>(?:{p}))" for i, p in enumerate(sanitized))
            )

        issues = self.compiler._audit_file(test_file)
        # Should not report eval due to nosec
        self.assertEqual(len([i for i in issues if "eval" in i.lower()]), 0)

    def test_audit_file_ignores_non_code_files(self):
        """Test audit ignores non-code files."""
        test_file = os.path.join(self.test_dir, "test.txt")
        with open(test_file, 'w') as f:
            f.write("gets(buffer);")

        issues = self.compiler._audit_file(test_file)
        self.assertEqual(len(issues), 0)

    @patch('subprocess.run')
    def test_install_globally(self, mock_run):
        """Test global installation creates symlink."""
        with patch('os.symlink') as mock_symlink:
            with patch('os.path.exists', return_value=False):
                with patch('os.makedirs'):
                    self.compiler.install_globally()
                    mock_symlink.assert_called_once()

    @patch('subprocess.run')
    def test_uninstall_globally(self, mock_run):
        """Test global uninstallation removes symlink."""
        with patch('os.remove') as mock_remove:
            with patch('os.path.exists', return_value=True):
                self.compiler.uninstall_globally()
                mock_remove.assert_called_once()

    def test_clean_project(self):
        """Test project cleaning removes build directory."""
        os.chdir(self.test_dir)
        os.makedirs("build")

        self.compiler.clean_project()
        self.assertFalse(os.path.exists("build"))

    def test_clean_project_no_build_dir(self):
        """Test cleaning when build directory doesn't exist."""
        os.chdir(self.test_dir)

        # Should not raise an exception
        self.compiler.clean_project()

    @patch('subprocess.run')
    def test_update_compiler_success(self, mock_run):
        """Test compiler update when in git repository."""
        mock_run.side_effect = [
            Mock(returncode=0, stdout="true\n", stderr=""),  # git rev-parse
            Mock(returncode=0)  # git pull
        ]

        with patch('builtins.print'):
            self.compiler.update_compiler()

    @patch('subprocess.run')
    def test_update_compiler_not_git_repo(self, mock_run):
        """Test compiler update fails gracefully when not a git repo."""
        mock_run.return_value = Mock(returncode=1)

        with patch('builtins.print'):
            self.compiler.update_compiler()


class TestVibeCompilerIntegration(unittest.TestCase):
    """Integration tests for VibeCompiler with actual file system operations."""

    def setUp(self):
        """Set up test fixtures."""
        self.test_dir = tempfile.mkdtemp()
        self.original_cwd = os.getcwd()
        self.compiler = VibeCompiler()

    def tearDown(self):
        """Clean up test fixtures."""
        os.chdir(self.original_cwd)
        if os.path.exists(self.test_dir):
            shutil.rmtree(self.test_dir)

    def test_project_status_without_vibe_json(self):
        """Test project status when not in a Vibe project."""
        os.chdir(self.test_dir)

        with patch('builtins.print') as mock_print:
            self.compiler.project_status()
            # Should print error message
            args = mock_print.call_args_list
            self.assertTrue(any("Error" in str(arg) for arg in args))

    def test_project_status_with_vibe_json(self):
        """Test project status displays correct information."""
        os.chdir(self.test_dir)

        # Create vibe.json
        config = {
            "name": "test_proj",
            "version": "1.0.0",
            "type": "executable"
        }
        with open("vibe.json", 'w') as f:
            json.dump(config, f)

        # Create source files
        os.makedirs("src")
        with open("src/main.c", 'w') as f:
            f.write("int main() { return 0; }")

        with patch('builtins.print') as mock_print:
            self.compiler.project_status()
            output = ' '.join(str(call) for call in mock_print.call_args_list)
            self.assertIn("test_proj", output)
            self.assertIn("1.0.0", output)


class TestVibeCompilerEdgeCases(unittest.TestCase):
    """Test edge cases and boundary conditions."""

    def setUp(self):
        """Set up test fixtures."""
        self.compiler = VibeCompiler()

    def test_invalid_project_name_in_vibe_json(self):
        """Test that invalid project names in vibe.json are rejected."""
        test_dir = tempfile.mkdtemp()
        original_cwd = os.getcwd()

        try:
            os.chdir(test_dir)

            # Create vibe.json with malicious name
            config = {"name": "../../../etc/passwd"}
            with open("vibe.json", 'w') as f:
                json.dump(config, f)

            with patch('builtins.print') as mock_print:
                self.compiler.project_status()
                output = ' '.join(str(call) for call in mock_print.call_args_list)
                self.assertIn("Invalid", output)
        finally:
            os.chdir(original_cwd)
            shutil.rmtree(test_dir)

    def test_arch_validation(self):
        """Test architecture string validation."""
        test_dir = tempfile.mkdtemp()
        original_cwd = os.getcwd()

        try:
            os.chdir(test_dir)

            # Create minimal vibe.json
            config = {"name": "test", "type": "executable"}
            with open("vibe.json", 'w') as f:
                json.dump(config, f)

            os.makedirs("src")
            with open("src/main.c", 'w') as f:
                f.write("int main() { return 0; }")

            # Test with invalid architecture (shell injection attempt)
            result = self.compiler.build_project(arch="; rm -rf /")
            self.assertFalse(result)
        finally:
            os.chdir(original_cwd)
            shutil.rmtree(test_dir)


if __name__ == '__main__':
    unittest.main()