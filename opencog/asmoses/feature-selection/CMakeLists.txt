ADD_SUBDIRECTORY(scorers)
ADD_SUBDIRECTORY(main)
ADD_SUBDIRECTORY(man)

ADD_LIBRARY(asfeature_selection SHARED
	algo/deme_optimize
	algo/incremental
	algo/random
	algo/simple
	algo/stochastic_max_dependency
	scorers/moses_optim
	main/feature-selection
)

TARGET_LINK_LIBRARIES(asfeature_selection
    INTERFACE moses
	asmoses_types
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

# Install library
IF (WIN32)
	INSTALL(TARGETS asfeature_selection
	        EXPORT ASMosesTargets
	        DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL(TARGETS asfeature_selection
	        EXPORT ASMosesTargets
	        LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"    # lib*.so files
	        ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")   # lib*.a files
ENDIF (WIN32)
