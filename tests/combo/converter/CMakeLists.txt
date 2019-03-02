ADD_CXXTEST(combo_atomeseUTest)
TARGET_LINK_LIBRARIES(combo_atomeseUTest
    ascombo
    ascomboant
    ${ATOMSPACE_LIBRARIES}
    ${COGUTIL_LIBRARY}
)
