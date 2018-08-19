ADD_CXXTEST(vertexUTest)
TARGET_LINK_LIBRARIES(vertexUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(messageUTest)
TARGET_LINK_LIBRARIES(messageUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(listUTest)
TARGET_LINK_LIBRARIES(listUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(similarityUTest)
TARGET_LINK_LIBRARIES(similarityUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(annUTest)
TARGET_LINK_LIBRARIES(annUTest
	ascomboreduct
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(procedure_callUTest)
TARGET_LINK_LIBRARIES(procedure_callUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(procedure_repositoryUTest)
TARGET_LINK_LIBRARIES(procedure_repositoryUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(interpreterUTest)
TARGET_LINK_LIBRARIES(interpreterUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(iostream_comboUTest)
TARGET_LINK_LIBRARIES(iostream_comboUTest
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)
