
ADD_LIBRARY(asmosesutil SHARED
	log_prog_name.cc
)

INSTALL (FILES
		hashing.h
		jaccard_index.h
		KLD.h
		log_prog_name.h
		lru_cache.h
		MannWhitneyU.h
		valueUtils.h
	DESTINATION
		"include/opencog/${PROJECT_NAME}/utils"
)
