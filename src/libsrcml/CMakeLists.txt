##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
# 
# CMake files for the library libsrcml

find_package(LibXml2 REQUIRED)
find_package(LibXslt REQUIRED)
find_package(Iconv REQUIRED)

# libsrcml library
file(GLOB LIBSRCML_SOURCE *.h *.hpp *.cpp)
add_library(libsrcml OBJECT ${LIBSRCML_SOURCE})
target_include_directories(libsrcml
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        .
)

# With MSVC, interprocedural optimization leads to a 10x static library, with only a slight reduction in size for the dynamic library
set_target_properties(libsrcml PROPERTIES INTERPROCEDURAL_OPTIMIZATION $<IF:$<CXX_COMPILER_ID:MSVC>,OFF,ON> POSITION_INDEPENDENT_CODE ON)

# Use the libsrcml/srcml.h rather than the /usr/local/include/srcml.h
target_include_directories(libsrcml BEFORE PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# libsrcml dependencies
target_link_libraries(libsrcml PRIVATE LibXml2::LibXml2 Iconv::Iconv LibXslt::LibXslt LibXslt::LibExslt TinySHA1 parser)
if(NOT MSVC)
    find_package(Threads REQUIRED)
    target_link_libraries(libsrcml PRIVATE Threads::Threads)
endif()

# target_compile_options(libsrcml PUBLIC "-fvisibility=hidden")

# precompiled header list of most commonly used
if(SRCML_BUILD_USEPCH)
    target_precompile_headers(libsrcml PRIVATE <srcmlns.hpp> <srcml_types.hpp> <srcml_sax2_utilities.hpp> <srcml_translator.hpp> <srcml_sax2_reader.hpp>)
endif()

# Building static and dynamic libraries for srcML.
add_library(libsrcml_static STATIC $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:antlr>)
set_target_properties(libsrcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin POSITION_INDEPENDENT_CODE ON)
target_include_directories(libsrcml_static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(libsrcml_static PRIVATE LibXml2::LibXml2 Iconv::Iconv LibXslt::LibXslt LibXslt::LibExslt)

add_library(libsrcml_shared SHARED)
set_target_properties(libsrcml_shared PROPERTIES
    POSITION_INDEPENDENT_CODE ON

    # All of these are not needed on Unix, but are needed on Windows
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin

    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
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

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

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

install(EXPORT libsrcml_shared_Targets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libsrcml"
        NAMESPACE libsrcml::
        FILE libsrcml-shared-targets.cmake
        COMPONENT DEVLIBS)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/libsrcmlConfig.cmake.in ${CMAKE_BINARY_DIR}/libsrcmlConfig.cmake
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libsrcml"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/libsrcmlConfigVersion.cmake
    VERSION "1.0.0"
    COMPATIBILITY "SameMajorVersion")

install(FILES ${CMAKE_BINARY_DIR}/libsrcmlConfig.cmake ${CMAKE_BINARY_DIR}/libsrcmlConfigVersion.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libsrcml"
        COMPONENT DEVLIBS
)

# static library part of development
install(TARGETS libsrcml_static EXPORT libsrcml_static_Targets COMPONENT DEVLIBS)

install(EXPORT libsrcml_static_Targets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libsrcml"
        NAMESPACE libsrcml::
        FILE libsrcml-static-targets.cmake
        COMPONENT DEVLIBS)

if(LINK_LIBSRCML_STATIC AND BUILD_LIBSRCML_STATIC)
    add_library(LibsrcML::LibsrcML ALIAS libsrcml_static)
else()
    add_library(LibsrcML::LibsrcML ALIAS libsrcml_shared)
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
