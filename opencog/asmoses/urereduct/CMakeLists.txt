ADD_GUILE_MODULE(FILES
		reduct.scm
		utilities.scm
		MODULE_DESTINATION "${GUILE_SITE_DIR}/opencog"
		)

ADD_SUBDIRECTORY (rules)
ADD_SUBDIRECTORY (types)
