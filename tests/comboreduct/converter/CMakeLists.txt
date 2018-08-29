ADD_CXXTEST(combo_atomeseUTest)
TARGET_LINK_LIBRARIES(combo_atomeseUTest
    ascomboreduct
    ascomboant
    ${COGUTIL_LIBRARY}
    ${ATOMSPACE_atombase_LIBRARY}
)
