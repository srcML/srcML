# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for the library libsrcml

# Build options
option(BUILD_LIBSRCML_STATIC "Build a static version of libsrcml" ON)
option(LINK_LIBSRCML_STATIC "Link srcml client, tests, and examples with static version of libsrcml" OFF)

find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)
find_package(Iconv REQUIRED)

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

# With MSVC, interprocedural optimization leads to a 10x static library, with only a slight reduction in size for the dynamic library
set_target_properties(libsrcml PROPERTIES INTERPROCEDURAL_OPTIMIZATION $<IF:$<CXX_COMPILER_ID:MSVC>,OFF,ON> POSITION_INDEPENDENT_CODE ON
    OPTIMIZE_DEPENDENCIES YES)

# Use the libsrcml/srcml.h rather than the /usr/local/include/srcml.h
target_include_directories(libsrcml BEFORE PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# libsrcml dependencies
target_link_libraries(libsrcml PRIVATE LibXml2::LibXml2 Iconv::Iconv LibXslt::LibXslt LibXslt::LibExslt parser)
if(NOT MSVC)
    find_package(Threads REQUIRED)
    target_link_libraries(libsrcml PRIVATE Threads::Threads)
endif()

# target_compile_options(libsrcml PUBLIC "-fvisibility=hidden")

# precompiled header list of most commonly used
if(SRCML_BUILD_USEPCH)
    target_precompile_headers(libsrcml PRIVATE <srcmlns.hpp> <srcml_types.hpp> <srcml_sax2_utilities.hpp> <srcml_translator.hpp> <srcml_sax2_reader.hpp>)
endif()

# Building static library for srcML

# Merge all object code into one object file to build static library from
# Use this to build static library as it 1) hides filenames 2) hides symbols that are internal only
add_custom_command(
  OUTPUT libsrcml.o
  COMMAND ld -r $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:antlr> -o libsrcml.o
  DEPENDS libsrcml parser antlr
  COMMAND_EXPAND_LISTS
)

add_library(libsrcml_static STATIC libsrcml.o)
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
    # On Windows, generate a debug of the release for the PDB file, but use optimization options
    set_target_properties(libsrcml_shared PROPERTIES OUTPUT_NAME libsrcml LINK_FLAGS_RELEASE "/DEBUG /OPT:REF /OPT:ICF")
    set_target_properties(libsrcml_static PROPERTIES OUTPUT_NAME libsrcmls LINK_FLAGS_RELEASE "/LTCG:OFF")
else()
    # The output name automatically prepends "lib"
    set_target_properties(libsrcml_shared libsrcml_static PROPERTIES OUTPUT_NAME srcml)
    set_target_properties(libsrcml_static PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# Restrict exported symbols to those in the export list
# The "-Wl" passes the option to the linker
if(APPLE)
    set_target_properties(libsrcml_shared libsrcml_static PROPERTIES LINK_FLAGS "-Wl,-exported_symbols_list ${CMAKE_SOURCE_DIR}/src/libsrcml/export_list")
elseif(NOT WIN32)
    set_target_properties(libsrcml_shared libsrcml_static PROPERTIES LINK_FLAGS "-Wl,--version-script=${CMAKE_SOURCE_DIR}/src/libsrcml/libsrcml.version")
endif()

# which types of libraries
if(WIN32)
    # libsrcml shared
    install(TARGETS libsrcml_shared EXPORT libsrcml_shared_Targets RUNTIME COMPONENT SRCML ARCHIVE COMPONENT DEVLIBS)
else()
    install(TARGETS libsrcml_shared EXPORT libsrcml_shared_Targets LIBRARY COMPONENT SRCML NAMELINK_COMPONENT DEVLIBS)
endif()

include(CMakePackageConfigHelpers)

# directory to put the config files
if(WIN32)
    set(CONFIG_FILE_DESTINATION "cmake")
elseif(APPLE)
    set(CONFIG_FILE_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/srcml/")
else()
    set(CONFIG_FILE_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/srcml")
endif()

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

if(LINK_LIBSRCML_STATIC AND BUILD_LIBSRCML_STATIC)
    add_library(srcML::LibsrcML ALIAS libsrcml_static)
else()
    add_library(srcML::LibsrcML ALIAS libsrcml_shared)
endif()

# install the libsrcml include file
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/srcml.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT DEVLIBS)

# On Windows with vcpkg, install all the dlls that the vcpkg installed
# NOTE: Would be better to use $<TARGET_RUNTIME_DLLS:libsrcml_shared>, but that does not include
# indirect dependencies, e.g., bzip2 for libxml
if(WIN32 AND DEFINED VCPKG_TARGET_TRIPLET)
    file(GLOB VCPKG_SRCML_DEPENDENCIES "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
    install(FILES ${VCPKG_SRCML_DEPENDENCIES} DESTINATION bin COMPONENT SRCML)
endif()

# Run ldconfig for install (not cpack) on Ubuntu
# The component is for ignoring in cpack
if(DISTRO AND DISTRO MATCHES "Ubuntu")
    find_program(LDCONFIG ldconfig)
    if(LDCONFIG)
        install(CODE "message(STATUS \"Running: ldconfig\")" COMPONENT HIDE)
        install(CODE "execute_process(COMMAND ${LDCONFIG})" COMPONENT HIDE)
    else()
        install(CODE "message(NOTICE \"   NOTE: Unable to run ldconfig. Library libsrcml.so may not be found.\n   Suggest running ldconfig manually.\")" COMPONENT HIDE)
    endif()
endif()
