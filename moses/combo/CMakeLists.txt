ADD_SUBDIRECTORY(combo)
ADD_SUBDIRECTORY(interpreter)
ADD_SUBDIRECTORY(type_checker)
ADD_SUBDIRECTORY(ant_combo_vocabulary)
ADD_SUBDIRECTORY(main)
ADD_SUBDIRECTORY(man)
ADD_SUBDIRECTORY(crutil)
ADD_SUBDIRECTORY(converter)

ADD_LIBRARY(ascombo SHARED
	crutil/exception

	combo/action
	combo/action_symbol
	combo/argument
	combo/assumption
	combo/builtin_action
	combo/convert_ann_combo
	combo/definite_object
	combo/descriptions
	combo/enum_type
	combo/indefinite_object
	combo/iostream_combo
	combo/message
	combo/perception
	combo/procedure_call
	combo/procedure_repository
	combo/similarity
	combo/simple_nn
	combo/vertex

	interpreter/interpreter
    interpreter/eval

	type_checker/type_tree_def
	type_checker/type_tree

	converter/combo_atomese
)

TARGET_LINK_LIBRARIES (ascombo
	atomese
	${COGUTIL_LIBRARY}
	${Boost_DATE_TIME_LIBRARY}
	${Boost_THREAD_LIBRARY}
	${Boost_REGEX_LIBRARY}
	${ATOMSPACE_atombase_LIBRARY}
)

ADD_LIBRARY(ascomboant SHARED
	ant_combo_vocabulary/ant_builtin_action
	ant_combo_vocabulary/ant_perception
	ant_combo_vocabulary/ant_action_symbol
	ant_combo_vocabulary/ant_indefinite_object
	ant_combo_vocabulary/ant_combo_vocabulary
)

TARGET_LINK_LIBRARIES (ascomboant
	ascombo
	${COGUTIL_LIBRARY}
)


#install library
IF (WIN32)
	INSTALL(TARGETS ascombo ascomboant
	        EXPORT ASMosesTargets
	        DESTINATION "lib${LIB_DIR_SUFFIX}/moses")
ELSE (WIN32)
	INSTALL(TARGETS ascombo ascomboant
	        EXPORT ASMosesTargets
           LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}"   # lib*.so files
           ARCHIVE DESTINATION "lib${LIB_DIR_SUFFIX}")  # lib*.a files
ENDIF (WIN32)
