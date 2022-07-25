INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_BINARY_DIR}
)

# We need to put this in a library, so that AntUTest can link to it.
ADD_LIBRARY (ant_scoring
	ant_scoring
)

ADD_EXECUTABLE (moses-ant-hillclimbing
	moses-ant-hillclimbing
	ant_scoring
)

ADD_EXECUTABLE (moses-ant-particleswarm
    moses-ant-particleswarm
	ant_scoring
)

TARGET_LINK_LIBRARIES (moses-ant-hillclimbing
	moses
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

TARGET_LINK_LIBRARIES (moses-ant-particleswarm
	moses
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)
