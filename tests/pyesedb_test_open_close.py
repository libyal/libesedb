#!/usr/bin/env python
#
# Python-bindings open close testing program
#
# Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.

from __future__ import print_function
import argparse
import sys

import pyesedb


def get_filename_string(filename):
  """Retrieves a human readable string representation of the filename."""
  if not filename:
    return "None"
  return filename


def get_mode_string(mode):
  """Retrieves a human readable string representation of the access mode."""
  if mode == "r":
    mode_string = "read"
  elif mode == "w":
    mode_string = "write"
  else:
    mode_string = "unknown ({0:s})".format(mode)
  return mode_string


def pyesedb_test_single_open_close_file(filename, mode):
  """Tests a single open and close."""
  description = (
      "Testing single open close of: {0:s} with access: {1:s}"
      "\t").format(get_filename_string(filename), get_mode_string(mode))
  print(description, end="")

  error_string = None
  result = True

  try:
    esedb_file = pyesedb.file()

    esedb_file.open(filename, mode)
    esedb_file.close()

  except TypeError as exception:
    expected_message = (
        "{0:s}: unsupported string object type.").format(
            "pyesedb_file_open")

    if not filename and str(exception) == expected_message:
      pass

    else:
      error_string = str(exception)
      result = False

  except ValueError as exception:
    expected_message = (
        "{0:s}: unsupported mode: w.").format(
            "pyesedb_file_open")

    if mode != "w" or str(exception) != expected_message:
      error_string = str(exception)
      result = False

  except Exception as exception:
    error_string = str(exception)
    result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyesedb_test_multi_open_close_file(filename, mode):
  """Tests multiple open and close."""
  description = (
      "Testing multi open close of: {0:s} with access: {1:s}"
      "\t").format(get_filename_string(filename), get_mode_string(mode))
  print(description, end="")

  error_string = None
  result = True

  try:
    esedb_file = pyesedb.file()

    esedb_file.open(filename, mode)
    esedb_file.close()
    esedb_file.open(filename, mode)
    esedb_file.close()

  except Exception as exception:
    error_string = str(exception)
    result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyesedb_test_single_open_close_file_object(filename, mode):
  """Tests a single file-like object open and close."""
  description = (
      "Testing single open close of file-like object of: {0:s} with access: "
      "{1:s}\t").format(get_filename_string(filename), get_mode_string(mode))
  print(description, end="")

  error_string = None
  result = True

  try:
    file_object = open(filename, "rb")
    esedb_file = pyesedb.file()

    esedb_file.open_file_object(file_object, mode)
    esedb_file.close()

  except Exception as exception:
    error_string = str(exception)
    result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyesedb_test_single_open_close_file_object_with_dereference(
    filename, mode):
  """Tests single file-like object open and close with dereference."""
  description = (
      "Testing single open close of file-like object with dereference of: "
      "{0:s} with access: {1:s}\t").format(
          get_filename_string(filename), get_mode_string(mode))
  print(description, end="")

  error_string = None
  result = True

  try:
    file_object = open(filename, "rb")
    esedb_file = pyesedb.file()

    esedb_file.open_file_object(file_object, mode)
    del file_object
    esedb_file.close()

  except Exception as exception:
    error_string = str(exception)
    result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyesedb_test_multi_open_close_file_object(filename, mode):
  """Tests multiple file-like object open and close."""
  description = (
      "Testing multi open close of file-like object of: {0:s} "
      "with access: {1:s}\t").format(
          get_filename_string(filename), get_mode_string(mode))
  print(description, end="")

  error_string = None
  result = True

  try:
    file_object = open(filename, "rb")
    esedb_file = pyesedb.file()

    esedb_file.open_file_object(file_object, mode)
    esedb_file.close()
    esedb_file.open_file_object(file_object, mode)
    esedb_file.close()

  except Exception as exception:
    error_string = str(exception)
    result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def main():
  args_parser = argparse.ArgumentParser(
      description="Tests open and close.")

  args_parser.add_argument(
      "source", nargs="?", action="store", metavar="FILENAME",
      default=None, help="The source filename.")

  options = args_parser.parse_args()

  if not options.source:
    print("Source value is missing.")
    print("")
    args_parser.print_help()
    print("")
    return False

  if not pyesedb_test_single_open_close_file(options.source, "r"):
    return False

  if not pyesedb_test_single_open_close_file(None, "r"):
    return False

  if not pyesedb_test_single_open_close_file(options.source, "w"):
    return False

  if not pyesedb_test_multi_open_close_file(options.source, "r"):
    return False

  if not pyesedb_test_single_open_close_file_object(options.source, "r"):
    return False

  if not pyesedb_test_single_open_close_file_object_with_dereference(
      options.source, "r"):
    return False

  if not pyesedb_test_multi_open_close_file_object(
      options.source, "r"):
    return False

  return True


if __name__ == "__main__":
  if not main():
    sys.exit(1)
  else:
    sys.exit(0)
