IF (HAVE_GUILE)
    ADD_CXXTEST(load_tableUTest)
    TARGET_LINK_LIBRARIES(load_tableUTest
            data
            ${ATOMSPACE_LIBRARIES}
            ${GUILE_LIBRARIES}
            )
ENDIF (HAVE_GUILE)
