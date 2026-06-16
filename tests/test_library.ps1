# Tests library functions and types.
#
# Version: 20260615

$LibraryTests = "block_descriptor block_tree block_tree_node catalog catalog_definition checksum column column_type compression data_definition data_segment database error file_header index io_handle leaf_page_descriptor long_value multi_value notify page page_header page_tree page_tree_key page_tree_value page_value record table root_page_header space_tree space_tree_value table_definition"
$LibraryTestsWithInput = "file support"
$OptionSets = "" -split " "

. .\test_functions.ps1

Function RunTest
{
	param( [string]$TestName )

	$TestBinary = "esedb_test_${TestName}"

	$TestDescription = "${TestBinary}"
	$TestExecutable = "${TestExecutablesDirectory}\${TestBinary}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		WriteTestResult ${TestDescription} ${ExitIgnore}

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression ${TestExecutable}
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

Function RunTestWithInput
{
	param( [string]$TestName, [string[]]$TestInput )

	$OptionSet = $TestInput[0]
	$Options = $TestInput[1]
	$TestFile = $TestInput[2]

	$TestBinary = "esedb_test_${TestName}"
	$TestFileName = (${TestFile} -split '\\')[-2..-1] -join '\'

	$TestDescription = "${TestBinary} with input: '${TestFileName}"
	$TestExecutable = "${TestExecutablesDirectory}\${TestBinary}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		WriteTestResult ${TestDescription} ${ExitIgnore}

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression "${TestExecutable} ${Options} ${TestFile}"
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
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
		$Result = RunTestWithInput ${TestName} ${TestInput}

		If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
		{
			Break
		}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}
