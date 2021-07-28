

ADD_EXECUTABLE (fs-main fs-main.cc)
TARGET_LINK_LIBRARIES(fs-main
	asfeature_selection
	moses
	${COGUTIL_LIBRARY}
)
SET_TARGET_PROPERTIES(fs-main PROPERTIES OUTPUT_NAME "asfeature-selection")

ADD_EXECUTABLE (aseval-features eval-features.cc)
TARGET_LINK_LIBRARIES(aseval-features
	data
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

#install header files
INSTALL(FILES

        feature-selection.h

	DESTINATION

	"include/opencog/${PROJECT_NAME}/feature-selection/main"
)

# install feature-selection
INSTALL(TARGETS fs-main aseval-features 
        EXPORT ASMosesTargets
        RUNTIME DESTINATION bin)
