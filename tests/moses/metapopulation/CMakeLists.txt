ADD_CXXTEST(metapopulationUTest)
TARGET_LINK_LIBRARIES(metapopulationUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
