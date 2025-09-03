
ADD_LIBRARY(asmosesutils SHARED
	log_prog_name.cc
)

INSTALL(TARGETS asmosesutils
	EXPORT ASMosesTargets
	DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")

INSTALL (FILES
		based_variant.h
		comprehension.h
		functional.h
		hashing.h
		iostreamContainer.h
		jaccard_index.h
		KLD.h
		log_prog_name.h
		lru_cache.h
		MannWhitneyU.h
		ranking.h
		selection.h
		valueUtils.h
	DESTINATION
		"include/opencog/${PROJECT_NAME}/utils"
)
