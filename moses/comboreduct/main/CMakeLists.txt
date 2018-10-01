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

