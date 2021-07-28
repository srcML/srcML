# After the rpm is generated, use the complete name (including distro
# and architecture) for the archive filenames
if(CPACK_GENERATOR MATCHES "RPM")

    foreach(PACKAGE ${CPACK_PACKAGE_FILES})

        # Name of the package with no path and no extension
        cmake_path(GET PACKAGE FILENAME PNAME)
        cmake_path(GET PNAME STEM LAST_ONLY PACKAGE_BASE_NAME)

        # Update the archive filenames based on the RPM filenames
        if(PACKAGE_BASE_NAME MATCHES "srcml-dev")
            set(UPPERNAME "SRCMLDEV")
        else()
            set(UPPERNAME "SRCML")
        endif()
        set(CPACK_ARCHIVE_${UPPERNAME}_FILE_NAME ${PACKAGE_BASE_NAME} PARENT_SCOPE)
    endforeach()

endif()
