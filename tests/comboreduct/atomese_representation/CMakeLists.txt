IF (HAVE_GUILE)
    ADD_CXXTEST(atomese_reprUTest)
    TARGET_LINK_LIBRARIES(atomese_reprUTest
            comboreduct
            ${ATOMSPACE_LIBRARIES}
            ${GUILE_LIBRARIES}
            )
ENDIF (HAVE_GUILE)
