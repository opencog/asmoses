#
# Tests are listed and run in more-or-less dependency order:
# Later tests assume earlier tests pass.
#
ADD_CXXTEST(MOSESUTest)
TARGET_LINK_LIBRARIES(MOSESUTest
        asmoses_exec
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(mixedUTest)
TARGET_LINK_LIBRARIES(mixedUTest
        asmoses_exec
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(irisUTest)
TARGET_LINK_LIBRARIES(irisUTest
        asmoses_exec
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(selectionUTest)
TARGET_LINK_LIBRARIES(selectionUTest
        asmoses_exec
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(dynFeatSelUTest)
TARGET_LINK_LIBRARIES(dynFeatSelUTest
        asmoses_exec
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(weightedUTest)
TARGET_LINK_LIBRARIES(weightedUTest
        asmoses_exec
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

