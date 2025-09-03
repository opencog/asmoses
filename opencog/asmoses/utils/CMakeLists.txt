
ADD_SUBDIRECTORY(boost_ext)

ADD_LIBRARY(asmosesutils SHARED
	lazy_normal_selector.cc
	log_prog_name.cc
	tree.cc
)

INSTALL(TARGETS asmosesutils
	EXPORT ASMosesTargets
	DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")

INSTALL (FILES
		based_variant.h
		comprehension.h
		digraph.h
		functional.h
		hashing.h
		iostreamContainer.h
		jaccard_index.h
		KLD.h
		lazy_normal_selector.h
		log_prog_name.h
		lru_cache.h
		MannWhitneyU.h
		ranking.h
		selection.h
		tree.h
		valueUtils.h
	DESTINATION
		"include/opencog/${PROJECT_NAME}/utils"
)
