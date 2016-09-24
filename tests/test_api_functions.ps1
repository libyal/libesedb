# Library API functions testing script
#
# Version: 20160912

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$TestPrefix = Split-Path -path ${Pwd}.Path -parent
$TestPrefix = Split-Path -path ${TestPrefix} -leaf
$TestPrefix = ${TestPrefix}.Substring(3)

$TestFunctions = "get_version error" -split " "

$TestToolDirectory = "..\vs2010\Release"

Function TestAPIFunction
{
	param( [string]$TestFunction, [string[]]$Options, [string]$Profile )

	$TestExecutable = "${TestToolDirectory}\${TestPrefix}_test_${TestFunction}.exe"

	Invoke-Expression ${TestExecutable}
}

Foreach (${TestFunction} in ${TestFunctions})
{
	TestAPIFunction ${TestFunction}

	if (${LastExitCode} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${LastExitCode}

