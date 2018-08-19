
# We need to put this in a library, so that MOSESUTest can link to it.
ADD_LIBRARY (asmoses_exec SHARED
    demo-problems
    problem
    problem-params
    moses_exec
    moses_exec_def
    table-problems
)

ADD_EXECUTABLE (asmoses-exec moses-exec)
TARGET_LINK_LIBRARIES (asmoses-exec
	moses_exec
	moses
	ascomboant
)
SET_TARGET_PROPERTIES(asmoses-exec PROPERTIES OUTPUT_NAME "asmoses")

ADD_EXECUTABLE (eval-diversity eval-diversity)
TARGET_LINK_LIBRARIES (eval-diversity
	moses
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

ADD_EXECUTABLE (eval-candidate eval-candidate)
TARGET_LINK_LIBRARIES (eval-candidate
	moses
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

ADD_EXECUTABLE (eval-candidate-likelihood eval-candidate-likelihood)
TARGET_LINK_LIBRARIES (eval-candidate-likelihood
	moses
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# install asmoses-exec
INSTALL(TARGETS asmoses-exec eval-diversity eval-candidate eval-candidate-likelihood
        EXPORT MosesTargets
        RUNTIME DESTINATION bin)

# Install library
IF (WIN32)
	INSTALL(TARGETS moses
	        EXPORT MosesTargets
	        DESTINATION "lib${LIB_DIR_SUFFIX}/moses")
ELSE (WIN32)
	INSTALL(TARGETS asmoses_exec
	        EXPORT MosesTargets
	        LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}"    # lib*.so files
	        ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}")   # lib*.a files
ENDIF (WIN32)

#install header files
INSTALL(FILES
	moses_exec.h
	moses_exec_def.h

	DESTINATION

	"include/${PROJECT_NAME}/moses/main"
)
