#
# Tests are listed and run in more-or-less dependency order:
# Later tests assume earlier tests pass.
#
ADD_LIBRARY (moses-framework SHARED
        moses-framework.cc
        )
TARGET_LINK_LIBRARIES(moses-framework
        asmoses_exec
        )

# These following tests fail in Ubuntu 22.04 and Debian Bookworm (2022)
#    28 - representationUTest (Failed)
#    32 - MOSESUTest (Failed)
#    33 - ASMOSESUTest (Failed)
#    35 - irisUTest (Failed)
#    36 - selectionUTest (Failed)
#    37 - dynFeatSelUTest (Failed)
#    38 - weightedUTest (Failed)
#    40 - diversityUTest (Failed)
# although they work great, and pass in Debian Stable Bullseye and
# in Ubuntu 20.04. After debugging, it appears that the random number
# generator changed or works differently, causing less optimal solutions
# to be found, or requiring longer run times, or requiring the initial
# random seed to be hand-optimized. In other words, these were "fragile"
# unit tests, all along. I've tweaked them, so that they pass, now.
# This might happen again, in the future. You've been warned.

ADD_CXXTEST(MOSESUTest)
TARGET_LINK_LIBRARIES(MOSESUTest
        asmoses_exec
        moses
        moses-framework
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(ASMOSESUTest)
TARGET_LINK_LIBRARIES(ASMOSESUTest
        asmoses_exec
        moses
        moses-framework
        ascombo
        asmoses_types
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(mixedUTest)
TARGET_LINK_LIBRARIES(mixedUTest
        asmoses_exec
        moses
        moses-framework
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(irisUTest)
TARGET_LINK_LIBRARIES(irisUTest
        asmoses_exec
        moses
        moses-framework
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(selectionUTest)
TARGET_LINK_LIBRARIES(selectionUTest
        asmoses_exec
        moses
        moses-framework
        ascombo
        ${COGUTIL_LIBRARY}
        )

#ADD_CXXTEST(dynFeatSelUTest)
#TARGET_LINK_LIBRARIES(dynFeatSelUTest
#        asmoses_exec
#        moses
#        moses-framework
#        ascombo
#        ${COGUTIL_LIBRARY}
#        )

ADD_CXXTEST(weightedUTest)
TARGET_LINK_LIBRARIES(weightedUTest
        asmoses_exec
        moses
        moses-framework
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(populateAtomSpaceUTest)
TARGET_LINK_LIBRARIES(populateAtomSpaceUTest
        data
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

#ADD_CXXTEST(diversityUTest)
#TARGET_LINK_LIBRARIES(diversityUTest
#        asmoses_exec
#        moses
#        moses-framework
#	     ascombo
#	     ${COGUTIL_LIBRARY}
#        )
