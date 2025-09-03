
ADD_LIBRARY(asmosesutil SHARED
	log_prog_name.cc
)

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
