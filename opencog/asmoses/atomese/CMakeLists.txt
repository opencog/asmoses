ADD_SUBDIRECTORY(interpreter)
ADD_SUBDIRECTORY(atom_types)
ADD_SUBDIRECTORY(atomese_utils)
ADD_SUBDIRECTORY(scm)
ADD_SUBDIRECTORY(type_checker)

ADD_LIBRARY(atomese SHARED
		interpreter/Interpreter.cc
		interpreter/logical_interpreter.cc
		interpreter/condlink_interpreter.cc
		# atom_types.h
		atomese_utils/constants.cc
		type_checker/AtomeseTypeChecker.cpp
)

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
