IF (HAVE_GUILE)
    ADD_CXXTEST(reductUTest)
    TARGET_LINK_LIBRARIES(reductUTest
            atomese
            ${ATOMSPACE_LIBRARIES}
            ${GUILE_LIBRARIES}
            )
ENDIF (HAVE_GUILE)
