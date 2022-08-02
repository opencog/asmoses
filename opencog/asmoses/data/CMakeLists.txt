ADD_SUBDIRECTORY (representation)
ADD_SUBDIRECTORY (table)
ADD_SUBDIRECTORY (main)

ADD_LIBRARY(data SHARED
		representation/load_table.cc

		table/table.cc
		table/table_io.cc
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
