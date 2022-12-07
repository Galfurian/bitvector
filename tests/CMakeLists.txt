# -----------------------------------------------------------------------------
# TEST 1
# -----------------------------------------------------------------------------
# Add the test.
add_executable(${PROJECT_NAME}_test_operators test_operators.cpp)
# Liking for the test.
target_link_libraries(${PROJECT_NAME}_test_operators bvlib)
# Add the test.
add_test(${PROJECT_NAME}_test_operators_run ${PROJECT_NAME}_test_operators)

# -----------------------------------------------------------------------------
# TEST 2
# -----------------------------------------------------------------------------
# Add the test.
add_executable(${PROJECT_NAME}_test_comparison test_comparison.cpp)
# Liking for the test.
target_link_libraries(${PROJECT_NAME}_test_comparison bvlib)
# Add the test.
add_test(${PROJECT_NAME}_test_comparison_run ${PROJECT_NAME}_test_comparison)
