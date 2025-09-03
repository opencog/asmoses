INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/opencog/asmoses/utils
)

LINK_DIRECTORIES(
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/opencog/asmoses/utils
)

LINK_LIBRARIES(
	asmosesutils
	${COGUTIL_LIBRARY}
)

ADD_CXXTEST(lazy_selectorUTest)
ADD_CXXTEST(lru_cacheUTest)
ADD_CXXTEST(iostreamContainerUTest)
ADD_CXXTEST(KLDUTest)
ADD_CXXTEST(comprehensionUTest)
ADD_CXXTEST(rankingUTest)
