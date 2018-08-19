ADD_CXXTEST(feature_selectionUTest)
TARGET_LINK_LIBRARIES(feature_selectionUTest
	asfeature_selection
	moses
	${COGUTIL_LIBRARY}
)
