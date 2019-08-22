ADD_CXXTEST(type_treeUTest)
TARGET_LINK_LIBRARIES(type_treeUTest
	ascomboant
	ascombo
	asmoses_types
	atom_types
	${COGUTIL_LIBRARY}
)
