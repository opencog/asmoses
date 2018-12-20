ADD_SUBDIRECTORY(interpreter)

ADD_LIBRARY(atomese SHARED
		interpreter/Interpreter
)

TARGET_LINK_LIBRARIES (atomese
	${COGUTIL_LIBRARY}
	${ATOMSPACE_LIBRARIES}
)

#install library
IF (WIN32)
	INSTALL(TARGETS atomese DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL(TARGETS atomese
			EXPORT ASMosesTargets
			LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"   # lib*.so files
			ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")  # lib*.a files
ENDIF (WIN32)
