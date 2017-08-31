# Info tool testing script
#
# Version: 20170827

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$InputGlob = "*"

$TestToolDirectory = "..\msvscpp\Release"

If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\msvscpp\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2012\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2012\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2013\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2013\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2015\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2015\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2017\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2017\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$TestExecutable = "${TestToolDirectory}\esedbinfo.exe"

If (-Not (Test-Path -Path "input"))
{
	Exit ${ExitSuccess}
}

Get-ChildItem -Path "input\${InputGlob}" | Foreach-Object
{
	Invoke-Expression ${TestExecutable} $_

	If (${LastExitCode} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${LastExitCode}

