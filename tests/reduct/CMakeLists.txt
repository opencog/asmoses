#ADD_CXXTEST(ActionReductUTest)
#TARGET_LINK_LIBRARIES(ActionReductUTest
#	comboreduct
#   util
#)

ADD_CXXTEST(BooleanReductUTest)
TARGET_LINK_LIBRARIES(BooleanReductUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(BooleanFullUTest)
TARGET_LINK_LIBRARIES(BooleanFullUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(ContinReductUTest)
TARGET_LINK_LIBRARIES(ContinReductUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(FullReductUTest)
TARGET_LINK_LIBRARIES(FullReductUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(MixedReductUTest)
TARGET_LINK_LIBRARIES(MixedReductUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

#ADD_CXXTEST(PerceptionReductUTest)
#TARGET_LINK_LIBRARIES(PerceptionReductUTest
#	ascomboreduct
#   util
#)

ADD_CXXTEST(ANNReductUTest)
TARGET_LINK_LIBRARIES(ANNReductUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(FoldReductUTest)
TARGET_LINK_LIBRARIES(FoldReductUTest
	asreduct
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)
