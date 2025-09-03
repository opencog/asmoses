
# This test fails in Ubntu 22.04 and I don't have time to debug it,
# so I am commenting it out.
ADD_CXXTEST(representationUTest)
TARGET_LINK_LIBRARIES(representationUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(AtomeseRepresentationUTest)
TARGET_LINK_LIBRARIES(AtomeseRepresentationUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(KnobTurningUTest)
TARGET_LINK_LIBRARIES(KnobTurningUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
ADD_CXXTEST(FieldSetUTest)
TARGET_LINK_LIBRARIES(FieldSetUTest
        asmosesutils
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
