##
# CPackConfig.inc
#
# CMake directives and config for CPack.

# Generator settings
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
set(CPACK_GENERATOR "ZIP")
else()
set(CPACK_GENERATOR "TGZ;TBZ2")
endif()

# set package name
set(CPACK_PACKAGE_FILE_NAME "srcML-${CMAKE_SYSTEM_NAME}")

# package version
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

# strip executables
set(CPACK_STRIP_FILES ON)

# needs to be last so not overwritten
include(CPack)
