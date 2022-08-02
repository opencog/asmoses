ADD_SUBDIRECTORY(combo)
ADD_SUBDIRECTORY(interpreter)
ADD_SUBDIRECTORY(type_checker)
ADD_SUBDIRECTORY(ant_combo_vocabulary)
ADD_SUBDIRECTORY(main)
ADD_SUBDIRECTORY(man)
ADD_SUBDIRECTORY(crutil)
ADD_SUBDIRECTORY(converter)

ADD_LIBRARY(ascombo SHARED
	crutil/exception.cc

	combo/action.cc
	combo/action_symbol.cc
	combo/argument.h
	combo/assumption.cc
	combo/builtin_action.cc
	combo/convert_ann_combo.h
	combo/definite_object.cc
	combo/descriptions.cc
	combo/enum_type.cc
	combo/indefinite_object.cc
	combo/iostream_combo.cc
	combo/message.cc
	combo/perception.cc
	combo/procedure_call.cc
	combo/procedure_repository.cc
	combo/similarity.cc
	combo/simple_nn.cc
	combo/vertex.cc

	interpreter/interpreter.cc
	interpreter/eval.cc

	type_checker/type_tree_def.cc
	type_checker/type_tree.cc

	converter/combo_atomese.cc
)

TARGET_LINK_LIBRARIES (ascombo
	atomese
	asmoses_types
	${ATOMSPACE_LIBRARIES}
	${COGUTIL_LIBRARY}
	${Boost_DATE_TIME_LIBRARY}
	${Boost_THREAD_LIBRARY}
	${Boost_REGEX_LIBRARY}
)

ADD_LIBRARY(ascomboant SHARED
	ant_combo_vocabulary/ant_builtin_action.cc
	ant_combo_vocabulary/ant_perception.cc
	ant_combo_vocabulary/ant_action_symbol.cc
	ant_combo_vocabulary/ant_indefinite_object.cc
	ant_combo_vocabulary/ant_combo_vocabulary.cc
)

TARGET_LINK_LIBRARIES (ascomboant
	ascombo
	${COGUTIL_LIBRARY}
)


#install library
IF (WIN32)
	INSTALL(TARGETS ascombo ascomboant asmoses_types
	        EXPORT ASMosesTargets
	        DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
ELSE (WIN32)
	INSTALL(TARGETS ascombo ascomboant
	        EXPORT ASMosesTargets
           LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"   # lib*.so files
           ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")  # lib*.a files
ENDIF (WIN32)
