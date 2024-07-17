.PHONY: test-controls
test-controls:
	./build/tests/vnigma-tests

list-tests:
	./build/tests/vnigma-tests --gtest_list_tests


help-tests:
	./build/tests/vnigma-tests --help
