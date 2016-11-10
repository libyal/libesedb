# Library API functions testing script
#
# Version: 20161110

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$TestPrefix = Split-Path -path ${Pwd}.Path -parent
$TestPrefix = Split-Path -path ${TestPrefix} -leaf
$TestPrefix = ${TestPrefix}.Substring(3)

$TestFunctions = "error notify"
$TestFunctionsWithInput = "support"

$TestToolDirectory = "..\msvscpp\Release"

Function TestAPIFunction
{
	param( [string]$TestFunction, [string[]]$Options, [string]$Profile )

	$TestDescription = "Testing API functions: ${TestFunction}"
	$TestExecutable = "${TestToolDirectory}\${TestPrefix}_test_${TestFunction}.exe"

	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} " -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host " (FAIL)"
	}
	Else
	{
		Write-Host " (PASS)"
	}
	Return ${Result}
}

If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestFunction} in ${TestFunctions} -split " ")
{
	$Result = TestAPIFunction ${TestFunction}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Foreach (${TestFunction} in ${TestFunctionsWithInput} -split " ")
{
	$Result = TestAPIFunction ${TestFunction}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${Result}

