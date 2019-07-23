LINK_LIBRARIES(
	-Wl,--no-as-needed
	ascombo
	asmoses_types
	${ATOMSPACE_LIBRARIES}
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(vertexUTest)
TARGET_LINK_LIBRARIES(vertexUTest
	ascomboant
)

ADD_CXXTEST(messageUTest)
TARGET_LINK_LIBRARIES(messageUTest
	ascomboant
)

ADD_CXXTEST(listUTest)
TARGET_LINK_LIBRARIES(listUTest
	ascomboant
)

ADD_CXXTEST(similarityUTest)
TARGET_LINK_LIBRARIES(similarityUTest
	ascomboant
)

ADD_CXXTEST(annUTest)

ADD_CXXTEST(procedure_callUTest)
TARGET_LINK_LIBRARIES(procedure_callUTest
	ascomboant
)

ADD_CXXTEST(procedure_repositoryUTest)
TARGET_LINK_LIBRARIES(procedure_repositoryUTest
	ascomboant
)

ADD_CXXTEST(interpreterUTest)
TARGET_LINK_LIBRARIES(interpreterUTest
	ascomboant
)

ADD_CXXTEST(iostream_comboUTest)
TARGET_LINK_LIBRARIES(iostream_comboUTest
	ascomboant
)
