ADD_CXXTEST(atomese_LBS_UTest)
TARGET_LINK_LIBRARIES(atomese_LBS_UTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
