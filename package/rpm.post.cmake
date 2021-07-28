# After the rpm is generated, use the complete name (including distro
# and architecture) for the archive filenames
if(CPACK_GENERATOR MATCHES "RPM")

    foreach(PACKAGE ${CPACK_PACKAGE_FILES})

        # The package name is the prefix of the base name
        cmake_path(GET PACKAGE FILENAME BASENAME)
        string(REGEX MATCH "^[a-z]+([-][a-z]+)?" PACKAGE_NAME ${BASENAME})

        # Using the PACKAGE_NAME spec file, extract the platform prefix
        execute_process(COMMAND rpm --specfile dist/_CPack_Packages/Linux/RPM/SPECS/${PACKAGE_NAME}.spec
                         OUTPUT_VARIABLE PACKAGE_BASE_NAME
                         OUTPUT_STRIP_TRAILING_WHITESPACE)

        # Update the archive filenames based on the RPM filenames
        if(PACKAGE_NAME MATCHES "srcml-dev")
            set(UPPERNAME "SRCMLDEV")
        else()
            set(UPPERNAME "SRCML")
        endif()
        set(CPACK_ARCHIVE_${UPPERNAME}_FILE_NAME ${PACKAGE_BASE_NAME} PARENT_SCOPE)
    endforeach()

endif()
