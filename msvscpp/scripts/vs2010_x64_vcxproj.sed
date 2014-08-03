/<ProjectConfiguration/ {
	h
:loop1
	n
	/<\/ProjectConfiguration>/ !{
		H
		b loop1
	}
	p
	x
	s/Win32/x64/g
	a \ \ \ \ </ProjectConfiguration>\r
}

/<PropertyGroup.*Label="Configuration"/ {
	h
:loop2
	n
	/<\/PropertyGroup>/ !{
		H
		b loop2
	}
	p
	x
	s/Win32/x64/g
	a \ \ \ \ <PlatformToolset>Windows7.1SDK</PlatformToolset>\r
	a \ \ </PropertyGroup>\r
}

/<ImportGroup.*Label="PropertySheets">/ {
	h
:loop3
	n
	/<\/ImportGroup>/ !{
		H
		b loop3
	}
	p
	x
	s/Win32/x64/g
	a \ \ </ImportGroup>\r
}

/<OutDir / {
	p
	s/Win32/x64/g
}

/<IntDir / {
	p
	s/Win32/x64/g
}

/<LinkIncremental / {
	p
	s/Win32/x64/g
}

/<ItemDefinitionGroup/ {
	h
:loop4
	n
	/<\/ItemDefinitionGroup>/ !{
		/<\/Link>/ {
			i \ \ \ \ \ \ <TargetMachine>MachineX86</TargetMachine>\r
		}
		H
		b loop4
	}
	p
	x
	s/Win32/x64/g
	s/MachineX86/MachineX64/g
	a \ \ </ItemDefinitionGroup>\r
}

