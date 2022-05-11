# requires boost::math; don't build if not found.
IF(Boost_MATH_C99_FOUND)
    ADD_CXXTEST(NeighborSamplingUTest)
    TARGET_LINK_LIBRARIES(NeighborSamplingUTest
            moses
            ascombo
            ${COGUTIL_LIBRARY}
            )
ENDIF(Boost_MATH_C99_FOUND)

# AntUTest won't compile, because there is no libant_scoring.so to link
# against.
# ADD_CXXTEST(AntUTest)
# TARGET_LINK_LIBRARIES(AntUTest
#         ant_scoring
#         asmoses_exec
#         moses
#         ascombo
#         ${COGUTIL_LIBRARY}
#         )

ADD_CXXTEST(iostream_scored_combo_treeUTest)
TARGET_LINK_LIBRARIES(iostream_scored_combo_treeUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(atomese_complexityUTest)
TARGET_LINK_LIBRARIES(atomese_complexityUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
        
