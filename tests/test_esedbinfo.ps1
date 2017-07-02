# Info tool testing script
#
# Version: 20161101

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$TestPrefix = Split-Path -path ${Pwd}.Path -parent
$TestPrefix = Split-Path -path ${TestPrefix} -leaf
$TestPrefix = ${TestPrefix}.Substring(3)
$TestSuffix = "info"

$TestToolDirectory = "..\msvscpp\Release"
$TestTool = "${TestPrefix}${TestSuffix}"
$InputDirectory = "input"
$InputGlob = "*"

If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2012\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2013\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2015\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$TestExecutable = "${TestToolDirectory}\${TestTool}.exe"

If (-Not (Test-Path -Path "${InputDirectory}"))
{
	Exit ${ExitSuccess}
}

Get-ChildItem -Path "${InputDirectory}\${InputGlob}" | Foreach-Object
{
	Invoke-Expression ${TestExecutable} $_

	If (${LastExitCode} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${LastExitCode}

