# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# @create : 23/03/2021
# @update : 23/03/2021
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(bvlib CXX)

# -----------------------------------------------------------------------------
# Set the compilation flags.
# -----------------------------------------------------------------------------
# Only meddle with non-required compilation flags,
# if we are the top level project to ensure consistency
# with parent project otherwise
get_directory_property(hasParent PARENT_DIRECTORY)
if(NOT hasParent)

    # Set the default build type to Debug.
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Debug' as none was specified.")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    endif()
    
    # Set flags that affect compilation
    if(MSVC)
        # Enable standards-conformance mode for MSVC compilers that support this flag
        # (Visual C++ 2017 and later).
        add_compile_options(/permissive-)
    else()
        add_compile_options(
            $<$<CONFIG:Debug>:-g3>
            $<$<CONFIG:Debug>:-ggdb>
            $<$<CONFIG:Debug>:-O0>
        )
    endif()

    # Set Warning flags
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall)
        add_compile_options(-Werror)
        add_compile_options(-pedantic)
        add_compile_options(-pedantic-errors)
    endif()

endif() # hasParent

# -----------------------------------------------------------------------------
# Add the C++ Library.
# -----------------------------------------------------------------------------
add_library(${PROJECT_NAME} INTERFACE)
# Inlcude header directories and set the library.
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include/)

# -----------------------------------------------------------------------------
# Add tests.
# -----------------------------------------------------------------------------
# CMake has support for adding tests to a project:
enable_testing()
# Add the subdirectory containing the tests (which imports also their target).
add_subdirectory(test)