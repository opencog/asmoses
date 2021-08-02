#install header files
INSTALL(FILES

        action.h
        action_symbol.h
        ann.h
        argument.h
        assumption.h
        builtin_action.h
        combo.h
        common_def.h
        convert_ann_combo.h
        definite_object.h
        enum_type.h
        indefinite_object.h
        iostream_combo.h
        message.h
        operator_base.h
        perception.h
        procedure_call.h
        procedure_repository.h
		similarity.h
        simple_nn.h
        vertex.h

	DESTINATION

	"include/opencog/${PROJECT_NAME}/combo/combo"
)
