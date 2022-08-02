ADD_SUBDIRECTORY(scorers)
ADD_SUBDIRECTORY(main)
ADD_SUBDIRECTORY(man)

ADD_LIBRARY(asfeature_selection SHARED
	algo/deme_optimize.cc
	algo/incremental.cc
	algo/random.cc
	algo/simple.cc
	algo/stochastic_max_dependency.cc
	scorers/moses_optim.cc
	main/feature-selection.cc
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
