ADD_CXXTEST(optimizationUTest)
TARGET_LINK_LIBRARIES(optimizationUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
