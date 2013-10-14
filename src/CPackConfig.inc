##
# CPackConfig.inc
#
# CMake directives and config for CPack.

# package version
set(CPACK_PACKAGE_VERSION "")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")


# needs to be last so not overwritten
include(CPack)