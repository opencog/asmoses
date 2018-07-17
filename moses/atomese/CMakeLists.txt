ADD_SUBDIRECTORY(representation)

ADD_LIBRARY(atomese SHARED
		representation/load_table
)

TARGET_LINK_LIBRARIES (atomese
	comboreduct
	${COGUTIL_LIBRARY}
	${ATOMSPACE_LIBRARIES}
)

#install library
IF (WIN32)
	INSTALL(TARGETS atomese DESTINATION "lib${LIB_DIR_SUFFIX}/moses")
ELSE (WIN32)
	INSTALL(TARGETS atomese
			LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}"   # lib*.so files
			ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}")  # lib*.a files
ENDIF (WIN32)
