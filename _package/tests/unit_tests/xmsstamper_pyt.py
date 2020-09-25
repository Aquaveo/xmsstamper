"""Test xms.stamper.cpp."""
import unittest

import xms.stamper


class TestStamper(unittest.TestCase):
    """Tests for the main module."""
    def test_package_docstring(self):
        """Test package has docstring."""
        doc_string = xms.stamper.__doc__
        self.assertIsNotNone(doc_string)
        self.assertGreater(len(doc_string), 0)
