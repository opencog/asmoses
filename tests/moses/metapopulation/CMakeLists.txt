ADD_CXXTEST(metapopulationUTest)
TARGET_LINK_LIBRARIES(metapopulationUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(atomese_metapopUTest)
TARGET_LINK_LIBRARIES(atomese_metapopUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
