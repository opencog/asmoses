ADD_EXECUTABLE (repl repl.cc)
TARGET_LINK_LIBRARIES(repl
	asmosesutils
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (tree_gen tree_gen.cc)
TARGET_LINK_LIBRARIES(tree_gen
	asmosesutils
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (tree-sim tree-sim.cc)
TARGET_LINK_LIBRARIES(tree-sim
	asmosesutils
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
)

ADD_EXECUTABLE (asgen-disj-conj gen-disj-conj.cc)
TARGET_LINK_LIBRARIES(asgen-disj-conj
	asmosesutils
	ascombo
	ascomboant
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY} 
)

ADD_EXECUTABLE (ascombo-fmt-converter combo-fmt-converter.cc)
TARGET_LINK_LIBRARIES(ascombo-fmt-converter
	asmosesutils
	ascombo
	ascomboant
	data
	${COGUTIL_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# install eval-table
INSTALL(TARGETS asgen-disj-conj ascombo-fmt-converter
        EXPORT ASMosesTargets
        RUNTIME DESTINATION bin)

INSTALL(FILES

        tree_generation.h

   DESTINATION

   "include/opencog/${PROJECT_NAME}/combo/main"
)

