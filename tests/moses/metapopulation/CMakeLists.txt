ADD_CXXTEST(metapopulationUTest)
TARGET_LINK_LIBRARIES(metapopulationUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(atomese_metapopUTest)
TARGET_LINK_LIBRARIES(atomese_metapopUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
