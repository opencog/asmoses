ADD_CXXTEST(InterpreterUTest)
TARGET_LINK_LIBRARIES(InterpreterUTest
	-Wl,--no-as-needed
	atomese
	${ATOMSPACE_LIBRARIES}
	${COGUTIL_LIBRARY}
)
