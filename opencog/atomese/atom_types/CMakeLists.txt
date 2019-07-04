# --------------------------------------------------
OPENCOG_ADD_ATOM_TYPES(atom_types.script
	atom_types.h
	atom_types.definitions
	atom_types.inheritance
	atom_types.scm
	atom_types.pyx)
ADD_CUSTOM_TARGET(asmoses_atom_types DEPENDS atom_types.h)

# The atom_types.h file is written to the build directory
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

ADD_LIBRARY(asmoses_types SHARED
	atom_types
	atom_types_init
	)

TARGET_LINK_LIBRARIES(asmoses_types
	${ATOMSPACE_LIBRARIES}
	)

# Without this, parallel make will race and crap up the generated files.
ADD_DEPENDENCIES(asmoses_types asmoses_atom_types)

IF (WIN32)
	INSTALL (TARGETS asmoses_types
		DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL (TARGETS asmoses_types
		LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ENDIF (WIN32)

INSTALL(FILES

	${CMAKE_CURRENT_BINARY_DIR}/atom_types.h

	ImpulseLink.h

	DESTINATION "include/opencog/${PROJECT_NAME}/atomese/atom_types"
	)
