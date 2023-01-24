# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(bvlib CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------

option(BVLIB_BUILD_EXAMPLES "Build examples" OFF)
option(BVLIB_BUILD_TESTS "Build tests" OFF)
option(BVLIB_STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(BVLIB_WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)

# -----------------------------------------------------------------------------
# COMPILATION FLAGS
# -----------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS)

    if(BVLIB_WARNINGS_AS_ERRORS)
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} /WX)
    endif(BVLIB_WARNINGS_AS_ERRORS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(BVLIB_WARNINGS_AS_ERRORS)
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} -Werror)
    endif(BVLIB_WARNINGS_AS_ERRORS)
endif()

if(BVLIB_STRICT_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} /experimental:external)
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} /external:I ${CMAKE_BINARY_DIR})
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} /external:anglebrackets)
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} /external:W0)
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} /W4)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(BVLIB_COMPILE_OPTIONS ${BVLIB_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -pedantic)
    endif()
endif(BVLIB_STRICT_WARNINGS)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------

# Add the C++ Library.
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
# Inlcude header directories and set the library.
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set compilation flags.
target_compile_options(${PROJECT_NAME} INTERFACE ${BVLIB_COMPILE_OPTIONS})

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(BVLIB_BUILD_EXAMPLES)
    # Add the example.
    add_executable(${PROJECT_NAME}_example examples/example.cpp)
    # Liking for the test.
    target_link_libraries(${PROJECT_NAME}_example PUBLIC bvlib)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_example PUBLIC ${BVLIB_COMPILE_OPTIONS})
endif()

# -----------------------------------------------------------------------------
# TESTS
# -----------------------------------------------------------------------------

if(BVLIB_BUILD_TESTS)
    # CMake has support for adding tests to a project.
    enable_testing()

    # -------------------------------------
    # TEST 1
    # -------------------------------------
    # Add the test.
    add_executable(${PROJECT_NAME}_test_operators tests/test_operators.cpp)
    # Liking for the test.
    target_link_libraries(${PROJECT_NAME}_test_operators bvlib)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_test_operators PUBLIC ${BVLIB_COMPILE_OPTIONS})
    # Add the test.
    add_test(${PROJECT_NAME}_test_operators_run ${PROJECT_NAME}_test_operators)

    # -------------------------------------
    # TEST 2
    # -------------------------------------
    # Add the test.
    add_executable(${PROJECT_NAME}_test_comparison tests/test_comparison.cpp)
    # Liking for the test.
    target_link_libraries(${PROJECT_NAME}_test_comparison bvlib)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_test_comparison PUBLIC ${BVLIB_COMPILE_OPTIONS})
    # Add the test.
    add_test(${PROJECT_NAME}_test_comparison_run ${PROJECT_NAME}_test_comparison)
endif()

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

find_package(Doxygen)
if(DOXYGEN_FOUND)
    # Doxygen theme
    include(FetchContent)

    message(STATUS "Retrieving `doxygen-awesome-css`...")
    FetchContent_Declare(
        doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG 4cd62308d825fe0396d2f66ffbab45d0e247724c # 2.0.3
    )
    FetchContent_GetProperties(doxygenawesome)
    if(NOT doxygenawesome_POPULATED)
        FetchContent_MakeAvailable(doxygenawesome)
    endif()

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )

    doxygen_add_docs(${PROJECT_NAME}_documentation
        ${PROJECT_SOURCE_DIR}/include/bvlib/bitvector.hpp
        ${PROJECT_SOURCE_DIR}/include/bvlib/math.hpp
        ${PROJECT_SOURCE_DIR}/README.md
    )
endif()
