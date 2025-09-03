#ADD_CXXTEST(ActionReductUTest)
#TARGET_LINK_LIBRARIES(ActionReductUTest
#	combo
#   util
#)

ADD_CXXTEST(BooleanReductUTest)
TARGET_LINK_LIBRARIES(BooleanReductUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(BooleanFullUTest)
TARGET_LINK_LIBRARIES(BooleanFullUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(ContinReductUTest)
TARGET_LINK_LIBRARIES(ContinReductUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(FullReductUTest)
TARGET_LINK_LIBRARIES(FullReductUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(MixedReductUTest)
TARGET_LINK_LIBRARIES(MixedReductUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

#ADD_CXXTEST(PerceptionReductUTest)
#TARGET_LINK_LIBRARIES(PerceptionReductUTest
#	ascomboreduct
#   util
#)

ADD_CXXTEST(ANNReductUTest)
TARGET_LINK_LIBRARIES(ANNReductUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(FoldReductUTest)
TARGET_LINK_LIBRARIES(FoldReductUTest
	asmosesutils
	asreduct
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)
