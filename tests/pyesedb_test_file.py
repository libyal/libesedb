#!/usr/bin/env python
#
# Python-bindings file type test script
#
# Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pyesedb


class FileTypeTests(unittest.TestCase):
  """Tests the file type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    esedb_file = pyesedb.file()

    esedb_file.signal_abort()

  def test_open(self):
    """Tests the open function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    esedb_file = pyesedb.file()

    esedb_file.open(test_source)

    with self.assertRaises(IOError):
      esedb_file.open(test_source)

    esedb_file.close()

    with self.assertRaises(TypeError):
      esedb_file.open(None)

    with self.assertRaises(ValueError):
      esedb_file.open(test_source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    esedb_file = pyesedb.file()

    with open(test_source, "rb") as file_object:

      esedb_file.open_file_object(file_object)

      with self.assertRaises(IOError):
        esedb_file.open_file_object(file_object)

      esedb_file.close()

      with self.assertRaises(TypeError):
        esedb_file.open_file_object(None)

      with self.assertRaises(ValueError):
        esedb_file.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    esedb_file = pyesedb.file()

    with self.assertRaises(IOError):
      esedb_file.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    test_source = unittest.source
    if not test_source:
      return

    esedb_file = pyesedb.file()

    # Test open and close.
    esedb_file.open(test_source)
    esedb_file.close()

    # Test open and close a second time to validate clean up on close.
    esedb_file.open(test_source)
    esedb_file.close()

    if os.path.isfile(test_source):
      with open(test_source, "rb") as file_object:

        # Test open_file_object and close.
        esedb_file.open_file_object(file_object)
        esedb_file.close()

        # Test open_file_object and close a second time to validate clean up on close.
        esedb_file.open_file_object(file_object)
        esedb_file.close()

        # Test open_file_object and close and dereferencing file_object.
        esedb_file.open_file_object(file_object)
        del file_object
        esedb_file.close()

  def test_get_type(self):
    """Tests the get_type function and type property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    esedb_file = pyesedb.file()

    esedb_file.open(test_source)

    type = esedb_file.get_type()
    self.assertIsNotNone(type)

    self.assertIsNotNone(esedb_file.type)

    esedb_file.close()

  def test_get_page_size(self):
    """Tests the get_page_size function and page_size property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    esedb_file = pyesedb.file()

    esedb_file.open(test_source)

    page_size = esedb_file.get_page_size()
    self.assertIsNotNone(page_size)

    self.assertIsNotNone(esedb_file.page_size)

    esedb_file.close()

  def test_get_number_of_tables(self):
    """Tests the get_number_of_tables function and number_of_tables property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    esedb_file = pyesedb.file()

    esedb_file.open(test_source)

    number_of_tables = esedb_file.get_number_of_tables()
    self.assertIsNotNone(number_of_tables)

    self.assertIsNotNone(esedb_file.number_of_tables)

    esedb_file.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
