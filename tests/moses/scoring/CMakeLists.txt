ADD_CXXTEST(scoringUTest)
TARGET_LINK_LIBRARIES(scoringUTest
	moses
	ascombo
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(atomese_LBS_UTest)
TARGET_LINK_LIBRARIES(atomese_LBS_UTest
		moses
		ascombo
		${COGUTIL_LIBRARY}
		)
    
ADD_CXXTEST(discriminating_scoreUTest)
TARGET_LINK_LIBRARIES(discriminating_scoreUTest
		moses
		ascombo
		${COGUTIL_LIBRARY}
		)

ADD_CXXTEST(contin_bscoreUTest)
TARGET_LINK_LIBRARIES(contin_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(compressed_truth_table_bscoreUTest)
TARGET_LINK_LIBRARIES(compressed_truth_table_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(discretize_contin_bscoreUTest)
TARGET_LINK_LIBRARIES(discretize_contin_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(enum_bscoreUTest)
TARGET_LINK_LIBRARIES(enum_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(select_bscoreUTest)
TARGET_LINK_LIBRARIES(select_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(precision_bscoreUTest)
TARGET_LINK_LIBRARIES(precision_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)

ADD_CXXTEST(precision_conj_bscoreUTest)
TARGET_LINK_LIBRARIES(precision_conj_bscoreUTest
	moses
	ascombo
	atomese
	${COGUTIL_LIBRARY}
	)
