#SET_DIRECTORY_PROPERTIES(PROPERTIES CLEAN_NO_CUSTOM true)
OPENCOG_ADD_ATOM_TYPES(
		atom_types.script
		atom_types.h
		atom_types.definitions
		atom_types.inheritance
		reduct_types.scm
		reduct_types.pyx
)
ADD_CUSTOM_TARGET(reduct_atom_types DEPENDS atom_types.h)

# The atom_types.h file is written to the build directory
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

ADD_LIBRARY (reduct-types SHARED
		atom_types
		ReductTypes.cc
		)
ADD_GUILE_EXTENSION(SCM_CONFIG reduct-types "reduct_types_path")

TARGET_LINK_LIBRARIES(reduct-types
		${ATOMSPACE_LIBRARIES}
		)

# Without this, parallel make will race and crap up the generated files.
ADD_DEPENDENCIES(reduct-types reduct_atom_types)
INSTALL(TARGETS  reduct-types
		EXPORT ASMosesTargets
		DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")

INSTALL (FILES
		${CMAKE_CURRENT_BINARY_DIR}/atom_types.h
		DESTINATION "include/opencog/${PROJECT_NAME}/urereduct/types"
		)

ADD_GUILE_MODULE (FILES
		${CMAKE_CURRENT_BINARY_DIR}/reduct_types.scm
		MODULE_DESTINATION "${GUILE_SITE_DIR}/opencog/urereduct/types"
		DEPENDS reduct_atom_types
		)
