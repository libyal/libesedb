# Tests library functions and types.

$LibraryTests = "block_descriptor block_tree block_tree_node catalog catalog_definition checksum column column_type compression data_definition data_segment database error file_header index io_handle leaf_page_descriptor long_value multi_value notify page page_header page_tree page_tree_key page_tree_value page_value record table root_page_header space_tree space_tree_value table_definition"
$LibraryTestsWithInput = "file support"
$OptionSets = "" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitSuccess}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$ResultRun = RunTestBinary ${TestExecutablesDirectory} "esedb_test_${TestName}"

	If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
	{
		$Result = ${ResultRun}
	}
}

$TestInputs = GenerateTestInputs "libesedb" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$ResultRun = RunTestBinaryWithInput ${TestExecutablesDirectory} "esedb_test_${TestName}" ${TestInput}

		If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
		{
			$Result = ${ResultRun}
		}
	}
}

Exit ${Result}
