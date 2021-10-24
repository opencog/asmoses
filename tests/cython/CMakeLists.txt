IF (HAVE_NOSETESTS)
	ADD_TEST(CythonMoses ${NOSETESTS_EXECUTABLE} -vs
		${CMAKE_SOURCE_DIR}/tests/cython/moses/)

	SET_TESTS_PROPERTIES(CythonMoses
		PROPERTIES
		ENVIRONMENT "PYTHONDONTWRITEBYTECODE=1"

		# Not needed, if we run the test from the source dir, above.
		# ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/opencog/cython"
	)
ENDIF (HAVE_NOSETESTS)
