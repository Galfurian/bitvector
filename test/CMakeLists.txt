# -----------------------------------------------------------------------------
# TEST 1
# -----------------------------------------------------------------------------
# Add the test.
add_executable(test_bitvector test_bitvector.cpp)
# Liking for the test.
target_link_libraries(test_bitvector bvlib)
# Add the test.
add_test(test_1 test_bitvector)

# -----------------------------------------------------------------------------
# TEST 2
# -----------------------------------------------------------------------------
# Add the test.
add_executable(test_scalar_bitvector test_scalar_bitvector.cpp)
# Liking for the test.
target_link_libraries(test_scalar_bitvector bvlib)
# Add the test.
add_test(test_2 test_scalar_bitvector)
