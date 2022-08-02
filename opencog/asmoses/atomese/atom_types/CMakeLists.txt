# --------------------------------------------------
OPENCOG_ADD_ATOM_TYPES(atom_types.script
		atom_types.h
		atom_types.definitions
		atom_types.inheritance
		atom_types.scm
		atom_types.pyx)
ADD_CUSTOM_TARGET(asmoses_atom_types DEPENDS atom_types.h)
ADD_LIBRARY(asmoses_types SHARED
		atom_types.h
		atom_types_init.cc)
ADD_GUILE_EXTENSION(SCM_CONFIG asmoses_types "atom_types_path")

TARGET_LINK_LIBRARIES(asmoses_types
		${ATOMSPACE_LIBRARIES}
		${COGUTIL_LIBRARY})

# Without this, parallel make will race and crap up the generated files.
ADD_DEPENDENCIES(asmoses_types asmoses_atom_types)
INSTALL(TARGETS  asmoses_types
        EXPORT ASMosesTargets
        DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")

ADD_GUILE_MODULE (FILES
        ${CMAKE_CURRENT_BINARY_DIR}/atom_types.scm
        MODULE_DESTINATION "${GUILE_SITE_DIR}/opencog/asmoses/types"
        DEPENDS asmoses_atom_types
        )

#install header files
INSTALL(FILES

		${CMAKE_CURRENT_BINARY_DIR}/atom_types.h

		DESTINATION

		"include/opencog/${PROJECT_NAME}/atomese/atom_types"
		)
