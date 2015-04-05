# Script to generate the necessary files for a msvscpp build
#
# Version: 20150105

$Library = Get-Content -Path configure.ac | select -skip 3 -first 1 | % { $_ -Replace " \[","" } | % { $_ -Replace "\],","" }
$Version = Get-Content -Path configure.ac | select -skip 4 -first 1 | % { $_ -Replace " \[","" } | % { $_ -Replace "\],","" }

Get-Content -Path include\${Library}.h.in > include\${Library}.h
Get-Content -Path include\${Library}\definitions.h.in | % { $_ -Replace "@VERSION@",${Version} } > include\${Library}\definitions.h
Get-Content -Path include\${Library}\features.h.in | % { $_ -Replace "@[A-Z0-9_]*@","0" } > include\${Library}\features.h
Get-Content -Path include\${Library}\types.h.in | % { $_ -Replace "@[A-Z0-9_]*@","0" } > include\${Library}\types.h
Get-Content -Path common\types.h.in | % { $_ -Replace "@PACKAGE@",${Library} } > common\types.h
Get-Content -Path ${Library}\${Library}_definitions.h.in | % { $_ -Replace "@VERSION@",${Version} } > ${Library}\${Library}_definitions.h
Get-Content -Path ${Library}\${Library}.rc.in | % { $_ -Replace "@VERSION@",${Version} } > ${Library}\${Library}.rc

