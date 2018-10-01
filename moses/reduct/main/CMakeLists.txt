ADD_EXECUTABLE (action-reductor action-reductor.cc)
TARGET_LINK_LIBRARIES(action-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)

ADD_EXECUTABLE (contin-reductor contin-reductor.cc)
TARGET_LINK_LIBRARIES(contin-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)

ADD_EXECUTABLE (full-reductor full-reductor.cc)
TARGET_LINK_LIBRARIES(full-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)

ADD_EXECUTABLE (logical-reductor logical-reductor.cc)
TARGET_LINK_LIBRARIES(logical-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)

ADD_EXECUTABLE (mixed-reductor mixed-reductor.cc)
TARGET_LINK_LIBRARIES(mixed-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)

ADD_EXECUTABLE (perception-reductor perception-reductor.cc)
TARGET_LINK_LIBRARIES(perception-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)

ADD_EXECUTABLE (interactive-reductor interactive-reductor.cc)
TARGET_LINK_LIBRARIES(interactive-reductor
		asreduct
		ascomboreduct
		ascomboant
		${COGUTIL_LIBRARY}
		)