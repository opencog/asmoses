ADD_CXXTEST(vertexUTest)
TARGET_LINK_LIBRARIES(vertexUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(messageUTest)
TARGET_LINK_LIBRARIES(messageUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(listUTest)
TARGET_LINK_LIBRARIES(listUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(similarityUTest)
TARGET_LINK_LIBRARIES(similarityUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(annUTest)
TARGET_LINK_LIBRARIES(annUTest
	ascombo
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(procedure_callUTest)
TARGET_LINK_LIBRARIES(procedure_callUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(procedure_repositoryUTest)
TARGET_LINK_LIBRARIES(procedure_repositoryUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(interpreterUTest)
TARGET_LINK_LIBRARIES(interpreterUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(iostream_comboUTest)
TARGET_LINK_LIBRARIES(iostream_comboUTest
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)
