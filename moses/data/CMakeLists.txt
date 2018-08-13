ADD_SUBDIRECTORY(table)

ADD_LIBRARY(data SHARED
        table/table
        table/table_io
        )

TARGET_LINK_LIBRARIES(data
	comboreduct
	comboant
    ${Boost_DATE_TIME_LIBRARY}
    ${COGUTIL_LIBRARY}
	${ATOMSPACE_atombase_LIBRARY}
)


# Install library
IF (WIN32)
	INSTALL(TARGETS data DESTINATION "lib${LIB_DIR_SUFFIX}/moses")
ELSE (WIN32)
	INSTALL(TARGETS data
	        LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}"    # lib*.so files
	        ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}")   # lib*.a files
ENDIF (WIN32)