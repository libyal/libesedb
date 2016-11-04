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

$TestToolDirectory = "..\vs2010\Release"
$TestTool = "${TestPrefix}${TestSuffix}"
$InputDirectory = "input"
$InputGlob = "*"

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

