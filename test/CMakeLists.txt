

enable_testing()

include(
	GoogleTest
)


add_executable(fncTest
	${FNC_TEST}/fncExecTest.cpp
	${FNC_TEST}/fncFunctionTest.cpp
	${FNC_TEST}/fncConversionTest.cpp
	${FNCFILE_SRCS}
)

target_link_libraries(
	fncTest
	gtest_main
)

gtest_discover_tests(fncTest)

