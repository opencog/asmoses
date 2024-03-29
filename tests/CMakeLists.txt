
ENABLE_TESTING()
INCLUDE(AddCxxtest)

IF (WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
	                -D_CRT_SECURE_NO_WARNINGS)
ENDIF (WIN32)

ADD_DEFINITIONS(-DPROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
                -DPROJECT_BINARY_DIR="${CMAKE_BINARY_DIR}")

# The atom_types.h file is written to the build directory
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

# Perform tests in component-dependency order, as much as possible.
# For example, since everything depends on util and config functions,
# these are tested first. Since most things depend on the atomspace,
# that is tested next, etc.
IF (CXXTEST_FOUND)
	# ADD_SUBDIRECTORY (util)

	#ADD_SUBDIRECTORY (hillclimbing)

	ADD_SUBDIRECTORY (combo)
	ADD_SUBDIRECTORY (moses)
	ADD_SUBDIRECTORY (feature-selection)
	ADD_SUBDIRECTORY (atomese)
	ADD_SUBDIRECTORY (data)
	ADD_SUBDIRECTORY (reduct)

ENDIF (CXXTEST_FOUND)

IF (HAVE_CYTHON)
	ADD_SUBDIRECTORY (cython)
ENDIF (HAVE_CYTHON)

IF (HAVE_ATOMSPACE AND HAVE_GUILE)
# Stub out. Unit test is failing
#	ADD_SUBDIRECTORY (urereduct)
ENDIF (HAVE_ATOMSPACE AND HAVE_GUILE)

