# -----------------------------------------------------------------------------
# @brief  : CmakeFile for building the documentation.
# @author : Enrico Fraccaroli
# @create : 23/03/2021
# @update : 23/03/2021
# -----------------------------------------------------------------------------

# Find the doxygen tool.
find_package(Doxygen)

# If we have found doxygen, then we can add the target.
if (DOXYGEN_FOUND)
    # Set input and output files.
    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Add the actual target.
    add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
