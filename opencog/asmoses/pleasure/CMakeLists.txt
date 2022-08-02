ADD_SUBDIRECTORY (pleasure)
ADD_SUBDIRECTORY (main)

ADD_LIBRARY (pleasure SHARED

	pleasure/generation.cc
	pleasure/generation_table.cc
	pleasure/node_list.cc
	pleasure/population.cc
	
)

TARGET_LINK_LIBRARIES (pleasure
	comboreduct
	${COGUTIL_LIBRARY}
)

