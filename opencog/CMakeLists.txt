# The atom_types.h file is written to the build directory)
INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_BINARY_DIR}
)

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
