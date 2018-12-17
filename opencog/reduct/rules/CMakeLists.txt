#install header files
INSTALL(FILES

		action_rules.h
		branch_rules.h
		contin_rules.h
		general_rules.h
		logical_rules.h
		meta_rules.h
		mixed_rules.h
		perception_rules.h
		fold_rules.h

		DESTINATION

		"include/opencog/${PROJECT_NAME}/reduct/rules"
		)
