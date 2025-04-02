# Need to use -fno-strict-aliasing when compiling cython code, in order
# to avoid nasty compiler warnings about aliasing.  Cython explicitly
# performs aliasing, in order to emulate python object inheritance.
# See, for example,
# https://groups.google.com/forum/#!topic/cython-users/JV1-KvIUeIg
#
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")

MESSAGE(STATUS "Setting python RPATH to ${PYTHON_DEST}")
SET(CMAKE_SHARED_LINKER_FLAGS
	"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath -Wl,${PYTHON_DEST}")

INCLUDE_DIRECTORIES(
	${Python3_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
)

SET(CYTHON_FLAGS "-f")

# Use this as a guide:
# https://github.com/opencog/atomspace/blob/master/opencog/cython/opencog
CYTHON_ADD_MODULE_PYX(pyasmoses)

# opencog.pyasmoses Python bindings
ADD_LIBRARY(moses_cython SHARED
	pyasmoses.cpp
)

# Don't reverse the order of moses_exec and moses, if reversed will give
# undefined symbol: _ZNK5boost15program_options22error_with_option_name4whatEv
# XXX Why???
TARGET_LINK_LIBRARIES(moses_cython
	asmoses_exec
	moses
)

IF (HAVE_MPI)
	TARGET_LINK_LIBRARIES (moses_cython
		# These two work for both MPICH2 and OpenMPI, for me.
		${MPI_LIBRARY}
		${MPI_EXTRA_LIBRARY}
	)
ENDIF (HAVE_MPI)

# Python is expecting the shared object to be called `pyasmoses.so`
SET_TARGET_PROPERTIES(moses_cython PROPERTIES
	PREFIX ""
	OUTPUT_NAME pyasmoses)

INSTALL (TARGETS moses_cython
	EXPORT ASMosesTargets
	DESTINATION "${PYTHON_DEST}")
