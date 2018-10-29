ADD_CXXTEST(logical_bscore_UTest)
TARGET_LINK_LIBRARIES(logical_bscore_UTest
        moses
        ascomboreduct
        ${COGUTIL_LIBRARY}
        )
