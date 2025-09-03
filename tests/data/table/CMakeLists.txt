ADD_CXXTEST(tableUTest)
TARGET_LINK_LIBRARIES(tableUTest
	asmosesutils
	data
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)
