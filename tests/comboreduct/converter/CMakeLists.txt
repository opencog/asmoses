ADD_CXXTEST(combo_atomeseUTest)
TARGET_LINK_LIBRARIES(combo_atomeseUTest
    comboreduct
    comboant
    ${COGUTIL_LIBRARY}
    ${ATOMSPACE_atombase_LIBRARY}
)
