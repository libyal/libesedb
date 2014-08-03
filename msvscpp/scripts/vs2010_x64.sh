#!/bin/bash
# Script to add x64 platform to msvscpp 2010 files
#
# Version: 20130113

EXIT_FAILURE=1;
EXIT_SUCCESS=0;

if [ ! -x "/usr/bin/find" ] || [ ! -x "/usr/bin/sed" ];
then
	echo "Unable to find: /usr/bin/find and /usr/bin/sed.";

	exit ${EXIT_FAILURE};
fi

if [ ! -e "msvscpp/scripts/vs2010_x64_sln.sed" ] || [ ! -e "msvscpp/scripts/vs2010_x64_vcxproj.sed" ];
then
	echo "Unable to find the sed scripts.";
	echo "Are you running the script from the source directory?";

	exit ${EXIT_FAILURE};
fi

/usr/bin/find msvscpp/ -name \*.sln -exec /usr/bin/sed -b -f msvscpp/scripts/vs2010_x64_sln.sed -i {} \;
/usr/bin/find msvscpp/ -name \*.vcxproj -exec /usr/bin/sed -b -f msvscpp/scripts/vs2010_x64_vcxproj.sed -i {} \;

exit ${EXIT_SUCCESS};

