# The atom_types.h file is written to the build directory)
INCLUDE_DIRECTORIES(
		..
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_BINARY_DIR}
)
DECLARE_GUILE_CONFIG_TARGET(SCM_CONFIG "opencog asmoses-config" "ASMOSES_TEST")

ADD_SUBDIRECTORY (combo)
ADD_SUBDIRECTORY (moses)
ADD_SUBDIRECTORY (feature-selection)
ADD_SUBDIRECTORY (atomese)
ADD_SUBDIRECTORY (data)
ADD_SUBDIRECTORY (reduct)
ADD_SUBDIRECTORY (utils)

# Currently, the pleasure code does not build, as it uses various
# obsolete combo types and routines and etc.  It needs to be ported
# over to the modern combo interfaces.
#
#ADD_SUBDIRECTORY (pleasure)

IF (HAVE_CYTHON)
	ADD_SUBDIRECTORY (cython)
ENDIF (HAVE_CYTHON)

IF (HAVE_ATOMSPACE AND HAVE_GUILE )
	ADD_SUBDIRECTORY (urereduct)
ENDIF (HAVE_ATOMSPACE AND HAVE_GUILE )


WRITE_GUILE_CONFIG(${GUILE_BIN_DIR}/opencog/asmoses-config.scm SCM_CONFIG TRUE)
WRITE_GUILE_CONFIG(${GUILE_BIN_DIR}/opencog/asmoses-config-installable.scm SCM_CONFIG FALSE)
INSTALL(FILES ${GUILE_BIN_DIR}/opencog/asmoses-config-installable.scm DESTINATION ${GUILE_SITE_DIR}/opencog RENAME asmoses-config.scm)

