ADD_SUBDIRECTORY (reduct)
ADD_SUBDIRECTORY (rules)
ADD_SUBDIRECTORY (main)

ADD_LIBRARY(asreduct SHARED

	reduct/action_reduction.cc
	reduct/clean_reduction.cc
	reduct/contin_reduction.cc
	reduct/flat_normal_form.cc
	reduct/full_reduction.cc
	reduct/logical_reduction.cc
	reduct/ann_reduction.cc
	reduct/fold_reduction.cc
	reduct/mixed_reduction.cc
	reduct/perception_reduction.cc

	reduct/reduct.cc

	rules/action_rules.cc
	rules/branch_rules.cc
	rules/contin_rules.cc
	rules/general_rules.cc
	rules/logical_rules.cc
	rules/fold_rules.cc
	rules/meta_rules.cc
	rules/mixed_rules.cc
	rules/perception_rules.cc
)

TARGET_LINK_LIBRARIES (asreduct
	data
	${COGUTIL_LIBRARY}
	${Boost_DATE_TIME_LIBRARY}
	${Boost_THREAD_LIBRARY}
	${Boost_REGEX_LIBRARY}
)

#install library
IF (WIN32)
	INSTALL(TARGETS asreduct
			EXPORT ASMosesTargets
			DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL(TARGETS asreduct
			EXPORT ASMosesTargets
			LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"   # lib*.so files
			ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")  # lib*.a files
ENDIF (WIN32)
