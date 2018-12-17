ADD_SUBDIRECTORY (reduct)
ADD_SUBDIRECTORY (rules)
ADD_SUBDIRECTORY (main)

ADD_LIBRARY(asreduct SHARED

	reduct/action_reduction
	reduct/clean_reduction
	reduct/contin_reduction
	reduct/flat_normal_form
	reduct/full_reduction
	reduct/logical_reduction
	reduct/ann_reduction
	reduct/fold_reduction
	reduct/mixed_reduction
	reduct/perception_reduction

	reduct/reduct

	rules/action_rules
	rules/branch_rules
	rules/contin_rules
	rules/general_rules
	rules/logical_rules
	rules/fold_rules
	rules/meta_rules
	rules/mixed_rules
	rules/perception_rules
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
