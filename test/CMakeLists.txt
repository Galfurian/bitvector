# -----------------------------------------------------------------------------
# TEST 1
# -----------------------------------------------------------------------------
# Add the test.
add_executable(${PROJECT_NAME}_test_bitvector test_bitvector.cpp)
# Liking for the test.
target_link_libraries(${PROJECT_NAME}_test_bitvector bvlib)
# Add the test.
add_test(${PROJECT_NAME}_test_bitvector_run ${PROJECT_NAME}_test_bitvector)

# -----------------------------------------------------------------------------
# TEST 2
# -----------------------------------------------------------------------------
# Add the test.
add_executable(${PROJECT_NAME}_test_scalar_bitvector test_scalar_bitvector.cpp)
# Liking for the test.
target_link_libraries(${PROJECT_NAME}_test_scalar_bitvector bvlib)
# Add the test.
add_test(${PROJECT_NAME}_test_scalar_bitvector_run ${PROJECT_NAME}_test_scalar_bitvector)
