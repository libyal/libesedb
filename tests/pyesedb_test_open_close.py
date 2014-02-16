#!/usr/bin/env python
#
# Python-bindings open close testing program
#
# Copyright (c) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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
#

import sys

import pyesedb

def pyesedb_test_single_open_close_file( filename, mode ):
	esedb_file = pyesedb.file()
	esedb_file.open( filename, mode )
	esedb_file.close()

def pyesedb_test_multi_open_close_file( filename, mode ):
	esedb_file = pyesedb.file()
	esedb_file.open( filename, mode )
	esedb_file.close()
	esedb_file.open( filename, mode )
	esedb_file.close()

def pyesedb_test_single_open_close_file_object( filename, mode ):
	file_object = open( filename, mode )
	esedb_file = pyesedb.file()
	esedb_file.open_file_object( file_object, mode )
	esedb_file.close()

def pyesedb_test_single_open_close_file_object_with_dereference( filename, mode ):
	file_object = open( filename, mode )
	esedb_file = pyesedb.file()
	esedb_file.open_file_object( file_object, mode )
	del file_object
	esedb_file.close()

def pyesedb_test_multi_open_close_file_object( filename, mode ):
	file_object = open( filename, mode )
	esedb_file = pyesedb.file()
	esedb_file.open_file_object( file_object, mode )
	esedb_file.close()
	esedb_file.open_file_object( file_object, mode )
	esedb_file.close()

def main( argc, argv ):
	result = 0

	if argc < 2:
		print "Usage: pyesedb_test_open_close.py filename\n"
		return 1

	print "Testing single open close of: %s with access: read\t" %( sys.argv[ 1 ] ),

	try:
		pyesedb_test_single_open_close_file( argv[ 1 ], "r" )
		result = 0
	except:
		result = 1

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	print "Testing single open close of: None with access: read\t"

	result = 1
	try:
		pyesedb_test_single_open_close_file( None, "r" )
	except TypeError, exception:
		if exception.message == "argument 1 must be string, not None":
			result = 0
	except:
		pass

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	print "Testing single open close of: %s with access: write\t" %( sys.argv[ 1 ] ),

	result = 1
	try:
		pyesedb_test_single_open_close_file( argv[ 1 ], "w" )
	except ValueError, exception:
		if exception.message == "pyesedb_file_open: unsupported mode: w.":
			result = 0
	except:
		pass

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	print "Testing multi open close of: %s with access: read\t" %( sys.argv[ 1 ] ),

	try:
		pyesedb_test_multi_open_close_file( argv[ 1 ], "r" )
		result = 0
	except:
		result = 1

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	print "Testing single open close of file-like object of: %s with access: read\t" %( sys.argv[ 1 ] ),

	try:
		pyesedb_test_single_open_close_file_object( argv[ 1 ], "r" )
		result = 0
	except:
		result = 1

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	print "Testing single open close of file-like object with dereference of: %s with access: read\t" %( sys.argv[ 1 ] ),

	try:
		pyesedb_test_single_open_close_file_object_with_dereference( argv[ 1 ], "r" )
		result = 0
	except:
		result = 1

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	print "Testing multi open close of file-like object of: %s with access: read\t" %( sys.argv[ 1 ] ),

	try:
		pyesedb_test_multi_open_close_file_object( argv[ 1 ], "r" )
		result = 0
	except:
		result = 1

	if result != 0:
		print "(FAIL)"
		return 1
	print "(PASS)"

	return 0

if __name__ == "__main__":
	sys.exit( main( len( sys.argv ), sys.argv ) )

