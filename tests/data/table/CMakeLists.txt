ADD_CXXTEST(tableUTest)
TARGET_LINK_LIBRARIES(tableUTest
	data
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)
