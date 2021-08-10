ADD_SUBDIRECTORY (representation)
ADD_SUBDIRECTORY (table)
ADD_SUBDIRECTORY (main)

ADD_LIBRARY(data SHARED
		representation/load_table

		table/table
		table/table_io
		)

TARGET_LINK_LIBRARIES (data
		ascombo
		${COGUTIL_LIBRARY}
		${ATOMSPACE_LIBRARIES}
		)

#install library
IF (WIN32)
	INSTALL(TARGETS data DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL(TARGETS data
			EXPORT ASMosesTargets
			LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"   # lib*.so files
			ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")  # lib*.a files
ENDIF (WIN32)
