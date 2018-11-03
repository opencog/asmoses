ADD_CXXTEST(optimizationUTest)
TARGET_LINK_LIBRARIES(optimizationUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
