# -----------------------------------------------------------------------------
# @brief  : Examples.
# @author : Enrico Fraccaroli
# @create : 23/03/2021
# @update : 05/06/2022
# -----------------------------------------------------------------------------

# Add the example.
add_executable(${PROJECT_NAME}_example example.cpp)
# Liking for the test.
target_link_libraries(${PROJECT_NAME}_example bvlib)
