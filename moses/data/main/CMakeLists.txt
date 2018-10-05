ADD_EXECUTABLE (asgen-table gen-table.cc)
TARGET_LINK_LIBRARIES(asgen-table
		data
		ascombo
		ascomboant
		${COGUTIL_LIBRARY}
		${Boost_PROGRAM_OPTIONS_LIBRARY}
		)

ADD_EXECUTABLE (aseval-table eval-table.cc)
TARGET_LINK_LIBRARIES(aseval-table
		data
		ascombo
		ascomboant
		${COGUTIL_LIBRARY}
		${Boost_PROGRAM_OPTIONS_LIBRARY}
		)

# install eval-table
INSTALL(TARGETS aseval-table asgen-table
		EXPORT ASMosesTargets
		RUNTIME DESTINATION bin)
