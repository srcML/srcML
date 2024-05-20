# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
#
# CMake files for the library libsrcml

find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)
find_package(Iconv REQUIRED)
find_package(Threads REQUIRED)

# libsrcml library
file(GLOB LIBSRCML_SOURCE *.cpp)
add_library(libsrcml OBJECT ${LIBSRCML_SOURCE})
target_include_directories(libsrcml
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        .
)

# Use the libsrcml/srcml.h rather than the /usr/local/include/srcml.h
target_include_directories(libsrcml BEFORE PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# libsrcml dependencies
target_link_libraries(libsrcml PRIVATE LibXml2::LibXml2 Iconv::Iconv LibXslt::LibXslt LibXslt::LibExslt Threads::Threads parser)

# On macOS, what architecturesBuilding static library for srcML
if(APPLE)
    message(STATUS "CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
endif()

# Merge all object code into one object file to build static library from
# Use this to build static library as it 1) hides filenames 2) hides symbols that are internal only
set(LIBSRCML_OBJECTS $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:antlr>)
if(NOT MSVC)
    set(LIBSRCML_OBJECT_FILENAME libsrcml${CMAKE_CXX_OUTPUT_EXTENSION})

    if(NOT "${CMAKE_OSX_ARCHITECTURES}" STREQUAL "")
        # Generate a single-object library for both architectures, then combine them into one
        add_custom_command(
          OUTPUT ${LIBSRCML_OBJECT_FILENAME}
          COMMAND ld -r -arch arm64 ${LIBSRCML_OBJECTS} -o libsrcml.arm64.o
          COMMAND ld -r -arch x86_64 ${LIBSRCML_OBJECTS} -o libsrcml.x86_64.o
          COMMAND lipo -create libsrcml.arm64.o libsrcml.x86_64.o -output ${LIBSRCML_OBJECT_FILENAME}
          DEPENDS libsrcml parser antlr
          COMMAND_EXPAND_LISTS
        )
    else()
        add_custom_command(
          OUTPUT ${LIBSRCML_OBJECT_FILENAME}
          COMMAND ld -r ${LIBSRCML_OBJECTS} -o ${LIBSRCML_OBJECT_FILENAME}
          DEPENDS libsrcml parser antlr
          COMMAND_EXPAND_LISTS
        )
    endif()

    set(LIBSRCML_OBJECTS ${LIBSRCML_OBJECT_FILENAME})
endif()
add_library(libsrcml_static STATIC ${LIBSRCML_OBJECTS})
set_target_properties(libsrcml_static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    POSITION_INDEPENDENT_CODE ON
    LINKER_LANGUAGE CXX
    CXX_VISIBILITY_PRESET hidden
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
target_include_directories(libsrcml_static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(libsrcml_static PRIVATE LibXml2::LibXml2 Iconv::Iconv LibXslt::LibXslt LibXslt::LibExslt)

# Building shared library for srcML
add_library(libsrcml_shared SHARED)
set_target_properties(libsrcml_shared PROPERTIES
    POSITION_INDEPENDENT_CODE ON

    # All of these are not needed on Unix, but are needed on Windows
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin

    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"

    CXX_VISIBILITY_PRESET hidden
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
target_link_libraries(libsrcml_shared PRIVATE libsrcml parser antlr)
target_include_directories(libsrcml_shared PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

if(WIN32)
    set(SRCML_LIBSRCML_SHARED_OUTPUT_NAME "libsrcml")
    set(SRCML_LIBSRCML_STATIC_OUTPUT_NAME "libsrcml_static")
else()
    set(SRCML_LIBSRCML_SHARED_OUTPUT_NAME "srcml")
    set(SRCML_LIBSRCML_STATIC_OUTPUT_NAME "srcml")
endif()
set_target_properties(libsrcml_shared PROPERTIES OUTPUT_NAME "${SRCML_LIBSRCML_SHARED_OUTPUT_NAME}")
set_target_properties(libsrcml_static PROPERTIES OUTPUT_NAME "${SRCML_LIBSRCML_STATIC_OUTPUT_NAME}")

if(DEFINED SRCML_LIBSRCML_COMPILE_FLAGS)
    target_compile_options(libsrcml PRIVATE ${SRCML_LIBSRCML_COMPILE_FLAGS})
endif()

if(DEFINED SRCML_LIBSRCML_SHARED_LINK_FLAGS_RELEASE)
    set_target_properties(libsrcml_shared PROPERTIES LINK_FLAGS_RELEASE "${SRCML_LIBSRCML_SHARED_LINK_FLAGS_RELEASE}")
endif()

if(DEFINED SRCML_LIBSRCML_STATIC_LINK_FLAGS_RELEASE)
    set_target_properties(libsrcml_static PROPERTIES LINK_FLAGS_RELEASE "${SRCML_LIBSRCML_STATIC_LINK_FLAGS_RELEASE}")
endif()

# Apply preset linker flags for libsrcml
set_target_properties(libsrcml_shared libsrcml_static PROPERTIES LINK_FLAGS "${SRCML_LIBSRCML_LINK_FLAGS}")

# PreCompiled Headers configuration of libsrcml
if (SRCML_LIBSRCML_PCH)
    target_precompile_headers(libsrcml_shared PRIVATE "${SRCML_LIBSRCML_PCH}")
    target_precompile_headers(libsrcml_static PRIVATE "${SRCML_LIBSRCML_PCH}")
endif()

# which types of libraries
if(WIN32)
    # libsrcml shared
    install(TARGETS libsrcml_shared EXPORT libsrcml_shared_Targets RUNTIME COMPONENT SRCML ARCHIVE COMPONENT DEVLIBS)
    install(FILES $<TARGET_PDB_FILE:libsrcml_shared> DESTINATION bin OPTIONAL COMPONENT DEVLIBS)
else()
    install(TARGETS libsrcml_shared EXPORT libsrcml_shared_Targets LIBRARY COMPONENT SRCML NAMELINK_COMPONENT DEVLIBS)
endif()

include(CMakePackageConfigHelpers)

# directory to put the config files
set(CONFIG_FILE_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/srcml")

install(EXPORT libsrcml_shared_Targets
        DESTINATION "${CONFIG_FILE_DESTINATION}"
        NAMESPACE srcML::
        FILE srcML-shared-targets.cmake
        COMPONENT DEVLIBS)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/srcMLConfig.cmake.in ${CMAKE_BINARY_DIR}/srcMLConfig.cmake
    INSTALL_DESTINATION "${CONFIG_FILE_DESTINATION}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/srcMLConfigVersion.cmake
    VERSION "1.0.0"
    COMPATIBILITY "SameMajorVersion")

install(FILES ${CMAKE_BINARY_DIR}/srcMLConfig.cmake ${CMAKE_BINARY_DIR}/srcMLConfigVersion.cmake
        DESTINATION "${CONFIG_FILE_DESTINATION}"
        COMPONENT DEVLIBS
)

# static library part of development
install(TARGETS libsrcml_static EXPORT libsrcml_static_Targets COMPONENT DEVLIBS)

install(EXPORT libsrcml_static_Targets
        DESTINATION "${CONFIG_FILE_DESTINATION}"
        NAMESPACE srcML::
        FILE srcML-static-targets.cmake
        COMPONENT DEVLIBS)

if(BUILD_SHARED_LIBS)
    set(SRCML_LIBSRCML_LINK_DEFAULT "libsrcml_shared")
else()
    set(SRCML_LIBSRCML_LINK_DEFAULT "libsrcml_static")
endif()
add_library(srcML::LibsrcML ALIAS "${SRCML_LIBSRCML_LINK_DEFAULT}")

# install the libsrcml include file
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/srcml.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT DEVLIBS)

# On Windows with vcpkg, install all the dlls that the vcpkg installed
# Handles this for client also
if(WIN32 AND DEFINED VCPKG_TARGET_TRIPLET)
    file(GLOB VCPKG_SRCML_DEPENDENCIES "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
    install(FILES ${VCPKG_SRCML_DEPENDENCIES} DESTINATION bin COMPONENT SRCML)
endif()

# Run ldconfig for install (not cpack) on Ubuntu
# The component is for ignoring in cpack
if(DISTRO AND DISTRO MATCHES "Ubuntu")
    find_program(LDCONFIG ldconfig)
    if(LDCONFIG)
        install(CODE "message(STATUS \"Running: ldconfig\")" COMPONENT LOCAL)
        install(CODE "execute_process(COMMAND ${LDCONFIG})" COMPONENT LOCAL)
    else()
        install(CODE "message(NOTICE \"   NOTE: Unable to run ldconfig. Library libsrcml.so may not be found.\n   Suggest running ldconfig manually.\")" COMPONENT LOCAL)
    endif()
endif()
