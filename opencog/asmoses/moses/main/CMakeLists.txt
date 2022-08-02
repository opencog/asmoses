
# We need to put this in a library, so that MOSESUTest can link to it.
ADD_LIBRARY (asmoses_exec SHARED
    demo-problems.cc
    problem.cc
    problem-params.cc
    moses_exec.cc
    moses_exec_def.cc
    table-problems.cc
)

ADD_EXECUTABLE (asmoses-exec moses-exec.cc)
TARGET_LINK_LIBRARIES (asmoses-exec
	asmoses_exec
	moses
	ascomboant
)
SET_TARGET_PROPERTIES(asmoses-exec PROPERTIES OUTPUT_NAME "asmoses")

ADD_EXECUTABLE (aseval-diversity eval-diversity.cc)
TARGET_LINK_LIBRARIES (aseval-diversity
	moses
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

ADD_EXECUTABLE (aseval-candidate eval-candidate.cc)
TARGET_LINK_LIBRARIES (aseval-candidate
	moses
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

ADD_EXECUTABLE (aseval-candidate-likelihood eval-candidate-likelihood.cc)
TARGET_LINK_LIBRARIES (aseval-candidate-likelihood
	moses
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# install asmoses-exec
INSTALL(TARGETS asmoses-exec aseval-diversity aseval-candidate aseval-candidate-likelihood
        EXPORT ASMosesTargets
        RUNTIME DESTINATION bin)

# Install library
IF (WIN32)
	INSTALL(TARGETS moses
	        EXPORT ASMosesTargets
	        DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL(TARGETS asmoses_exec
	        EXPORT ASMosesTargets
	        LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"    # lib*.so files
	        ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")   # lib*.a files
ENDIF (WIN32)

#install header files
INSTALL(FILES
        moses_exec.h
        moses_exec_def.h
        populate_atomspace.h

	DESTINATION

	"include/opencog/${PROJECT_NAME}/moses/main"
)
