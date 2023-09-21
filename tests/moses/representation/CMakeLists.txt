
# This test fails in Ubntu 22.04 and I don't have time to debug it,
# so I am commenting it out.
ADD_CXXTEST(representationUTest)
TARGET_LINK_LIBRARIES(representationUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(AtomeseRepresentationUTest)
TARGET_LINK_LIBRARIES(AtomeseRepresentationUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )

ADD_CXXTEST(KnobTurningUTest)
TARGET_LINK_LIBRARIES(KnobTurningUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
ADD_CXXTEST(FieldSetUTest)
TARGET_LINK_LIBRARIES(FieldSetUTest
        moses
        ascombo
        ${COGUTIL_LIBRARY}
        )
