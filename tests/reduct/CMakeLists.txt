#ADD_CXXTEST(ActionReductUTest)
#TARGET_LINK_LIBRARIES(ActionReductUTest
#	combo
#   util
#)

ADD_CXXTEST(BooleanReductUTest)
TARGET_LINK_LIBRARIES(BooleanReductUTest
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(BooleanFullUTest)
TARGET_LINK_LIBRARIES(BooleanFullUTest
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(ContinReductUTest)
TARGET_LINK_LIBRARIES(ContinReductUTest
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(FullReductUTest)
TARGET_LINK_LIBRARIES(FullReductUTest
	asreduct
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(MixedReductUTest)
TARGET_LINK_LIBRARIES(MixedReductUTest
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
	asreduct
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(FoldReductUTest)
TARGET_LINK_LIBRARIES(FoldReductUTest
	asreduct
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)
