ADD_SUBDIRECTORY(interpreter)
ADD_SUBDIRECTORY(atom_types)
ADD_SUBDIRECTORY(atomese_utils)
ADD_SUBDIRECTORY(scm)

ADD_LIBRARY(atomese SHARED
		interpreter/Interpreter
		interpreter/logical_interpreter
		interpreter/condlink_interpreter
		atom_types
		atomese_utils/constants
		type_checker/AtomeseTypeChecker)

TARGET_LINK_LIBRARIES(atomese
	${ATOMSPACE_LIBRARIES}
	${COGUTIL_LIBRARY}
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
