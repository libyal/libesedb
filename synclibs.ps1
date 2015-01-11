# Script that synchronizes the local library dependencies
#
# Version: 20150105

$GitUrlPrefix = "https://github.com/libyal"
$LocalLibs = @("libbfio","libcdata","libcerror","libcfile","libclocale","libcnotify","libcpath","libcsplit","libcstring","libcsystem","libcthreads","libfcache","libfdata","libfdatetime","libfguid","libfmapi","libfvalue","libfwnt","libmapidb","libuna")

foreach (${LocalLib} in ${LocalLibs})
{
	# PowerShell will raise NativeCommandError if git writes to stdout or stderr
	# therefore 2>&1 is added and the output is stored in a variable.
	$Output = Invoke-Expression -Command "git clone ${GitUrlPrefix}/${LocalLib}.git ${LocalLib}-${pid} 2>&1"

	if (Test-Path ${LocalLib}-${pid})
	{
		$LocalLibVersion = Get-Content -Path ${LocalLib}-${pid}\configure.ac | select -skip 4 -first 1 | % { $_ -Replace " \[","" } | % { $_ -Replace "\],","" }

		if (Test-Path ${LocalLib})
		{
			Remove-Item -Path ${LocalLib} -Force -Recurse
		}
		New-Item -ItemType directory -Path ${LocalLib} -Force | Out-Null

		if (Test-Path ${LocalLib})
		{
			Copy-Item -Path ${LocalLib}-${pid}\${LocalLib}\*.[ch] -Destination ${LocalLib}\
			Get-Content -Path ${LocalLib}-${pid}\${LocalLib}\${LocalLib}_definitions.h.in | % { $_ -Replace "@VERSION@",${LocalLibVersion} } > ${LocalLib}\${LocalLib}_definitions.h
		}
		Remove-Item -Path ${LocalLib}-${pid} -Force -Recurse
	}
}

