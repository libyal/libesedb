/<\/Platforms>/ {
	i \	\	<Platform\r
	i \	\	\	Name="x64"\r
	i \	\	/>\r
}

/<Configurations>/ {
	n
	h
:loop1
	n
	/<\/Configurations>/ !{
		s/OutputDirectory="\$(SolutionDir)\$(ConfigurationName)"/OutputDirectory="\$(SolutionDir)\$(ConfigurationName)\\\$(PlatformName)"/
		s/IntermediateDirectory="\$(ConfigurationName)"/IntermediateDirectory="\$(ConfigurationName)\\\$(PlatformName)"/
		H
		b loop1
	}
	x
	s/OutputDirectory="\$(SolutionDir)\$(ConfigurationName)"/OutputDirectory="\$(SolutionDir)\$(ConfigurationName)\\\$(PlatformName)"/g
	s/IntermediateDirectory="\$(ConfigurationName)"/IntermediateDirectory="\$(ConfigurationName)\\\$(PlatformName)"/g
	s/TargetMachine=1/TargetMachine=3/g
	s/Win32/x64/g
	a \	</Configurations>\r
}

