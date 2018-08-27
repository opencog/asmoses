ADD_EXECUTABLE (action-reductor action-reductor.cc)
TARGET_LINK_LIBRARIES(action-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (contin-reductor contin-reductor.cc)
TARGET_LINK_LIBRARIES(contin-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (full-reductor full-reductor.cc)
TARGET_LINK_LIBRARIES(full-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (logical-reductor logical-reductor.cc)
TARGET_LINK_LIBRARIES(logical-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (mixed-reductor mixed-reductor.cc)
TARGET_LINK_LIBRARIES(mixed-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (perception-reductor perception-reductor.cc)
TARGET_LINK_LIBRARIES(perception-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (interactive-reductor interactive-reductor.cc)
TARGET_LINK_LIBRARIES(interactive-reductor
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (repl repl.cc)
TARGET_LINK_LIBRARIES(repl
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (tree_gen tree_gen.cc)
TARGET_LINK_LIBRARIES(tree_gen
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (tree-sim tree-sim.cc)
TARGET_LINK_LIBRARIES(tree-sim
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (aseval-table eval-table.cc)
TARGET_LINK_LIBRARIES(aseval-table
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

ADD_EXECUTABLE (asgen-table gen-table.cc)
TARGET_LINK_LIBRARIES(asgen-table
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

ADD_EXECUTABLE (asgen-disj-conj gen-disj-conj.cc)
TARGET_LINK_LIBRARIES(asgen-disj-conj
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

ADD_EXECUTABLE (ascombo-fmt-converter combo-fmt-converter.cc)
TARGET_LINK_LIBRARIES(ascombo-fmt-converter
	ascomboreduct
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# install eval-table
INSTALL(TARGETS aseval-table asgen-table asgen-disj-conj ascombo-fmt-converter
        EXPORT ASMosesTargets
        RUNTIME DESTINATION bin)

INSTALL(FILES

   tree_generation.h

   DESTINATION

   "include/${PROJECT_NAME}/ascomboreduct/main"
)

