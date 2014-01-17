# @copyright
# 
# Copyright (C) 2013-2014  SDML (www.srcML.org)
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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


# 
# It is epxected that the windows dependency directoy will contain a folder
# named dep which will contain all of the dependencies. For windows.
# 
# The layout of the folder will be as follows.
#       bin
#       include
#       lib


# Adding SVN Configuration.
option(ENABLE_SVN_INTEGRATION "Build with SVN integration." OFF)
set_property(GLOBAL PROPERTY SVN_ENABLED ${ENABLE_SVN_INTEGRATION})

# Adding SAX to configuration.
option(LIBSRCML_SAX2_ENABLED "Build with SAX2Framework for srcML" OFF)
set_property(GLOBAL PROPERTY SAX2_ENABLED ${LIBSRCML_SAX2_ENABLED})

# Dynamic Load libraries (Unix only)
option(DYNAMIC_LOAD_ENABLED "Dynamically load some libraries such as libxslt and libexslt" ON)
set_property(GLOBAL PROPERTY DYNAMIC_ENABLED ${DYNAMIC_LOAD_ENABLED})

# Adding build option for srcml executable.
option(ENABLE_NEW_SRCML_EXEC_BUILD "Build the newer version of the srcML executable." OFF)
set_property(GLOBAL PROPERTY ENABLE_NEW_SRCML_EXEC_BUILD ${ENABLE_NEW_SRCML_EXEC_BUILD})

# Setting some windows only properties.
if(WIN32)
    # Adding suspected windows include directory for ANTRL
    include_directories("C:/antlr/277/include/antlr")
    set(WINDOWS_DEP_PATH ${PROJECT_SOURCE_DIR}/dep)
    include_directories(${WINDOWS_DEP_PATH}/include)
    link_directories(${WINDOWS_DEP_PATH}/lib)
    if(ENABLE_SVN_INTEGRATION)
        message(FATAL_ERROR "SVN integration not tested on windows.")
    endif()
    # FIXME
    set_property(GLOBAL PROPERTY LIBXSLT_LIBS "")
    set_property(GLOBAL PROPERTY LIBARCHIVE_LIBS "")
    set_property(GLOBAL PROPERTY LIBXML2_LIBS "")
    include_directories(C:/antlr/277/include)
else()
    set(WINDOWS_DEP_PATH "")
    # Locating packages.
    find_package(LibArchive REQUIRED)
    find_package(LibXml2 REQUIRED)
    find_package(LibXslt)
    find_package(Boost COMPONENTS program_options filesystem system thread regex REQUIRED)

    # add include directories
    include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})

    if(LIBXSLT_FOUND)
        include_directories(${LIBXSLT_INCLUDE_DIR})
    endif()

    if(ENABLE_SVN_INTEGRATION)
        include_directories(/usr/include/apr-1.0 /usr/include/subversion-1 /usr/local/include/subversion-1)
    endif()

    if(LIBXSLT_EXSLT_LIBRARY)
        set_property(GLOBAL PROPERTY LIBXSLT_LIBS ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY})
    else()
        set_property(GLOBAL PROPERTY LIBXSLT_LIBS "")
    endif()
    
    # Setting Properties
    set_property(GLOBAL PROPERTY LIBARCHIVE_LIBS ${LibArchive_LIBRARIES})
    set_property(GLOBAL PROPERTY LIBXML2_LIBS ${LIBXML2_LIBRARIES})
endif()
set_property(GLOBAL PROPERTY WINDOWS_DEP_PATH ${WINDOWS_DEP_PATH})

set_property(GLOBAL PROPERTY BOOST_PROGRAM_OPTIONS_LIB ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})

# Locating the antlr library.
find_library(ANTLR_LIB NAMES libantlr-pic.a libantlr.a libantlr2-0.dll antlr.lib PATHS /usr/lib /usr/local/lib C:/antlr/277/lib)
set_property(GLOBAL PROPERTY ANTLR_LIB ${ANTLR_LIB})

# Finding antlr library.
find_program(ANTLR_EXE NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin)
set_property(GLOBAL PROPERTY ANTLR_EXE ${ANTLR_EXE})

# Finding SED
find_program(SED_EXE NAMES gsed sed PATHS /opt/local/bin /usr/local /bin ${WINDOWS_DEP_PATH}/bin)
set_property(GLOBAL PROPERTY SED_EXE ${SED_EXE})

# Finding GREP
find_program(GREP_EXE grep PATHS /bin /usr/bin ${WINDOWS_DEP_PATH}/bin)
set_property(GLOBAL PROPERTY GREP_EXE ${GREP_EXE})

find_package(PythonInterp REQUIRED)

# Enforcing that the version of python being used must have a major version of 2.
# and the minor version be greater than version 6 (this means version 2.7 of python 
# version 2 or newer).
if(NOT ${PYTHON_VERSION_MAJOR} EQUAL "2")
    message(FATAL_ERROR "Version of python found is not 2.X.X")
    if(${PYTHON_VERSION_MINOR} LESS EQUAL 6)
        message(FATAL_ERROR "Version of python found is not 2.6.X")
    endif()
endif()
set_property(GLOBAL PROPERTY PYTHON_INTERP_EXE ${PYTHON_EXECUTABLE})


# Adding global configuration for the load DLL macro.
if(NOT ${DYNAMIC_LOAD_ENABLED})
    add_definitions(-DNO_DLLOAD)
endif()

# Adding compiler configuration for GCC.
# The default configuration is to compile in DEBUG mode. These flags can be directly
# overridden by setting the property of a target you wish to change them for.
if(${CMAKE_COMPILER_IS_GNUCXX})
    set(GCC_WARNINGS "-Wno-long-long -Wall -Wextra  -Wall -pedantic -Wempty-body -Wignored-qualifiers -Wsign-compare -Wtype-limits -Wuninitialized")
    # Adding global compiler definitions.
    set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O3 -DNDEBUG ${GCC_WARNINGS}")
    set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -DDEBUG --coverage -fprofile-arcs ${GCC_WARNINGS}")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # Configuring the Clang compiler
    set(CLANG_WARNINGS "-Wno-long-long -Wall -Wextra -Wshorten-64-to-32")
    set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -DNDEBUG ${CLANG_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O3 -DNDEBUG ${CLANG_WARNINGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -DDEBUG ${CLANG_WARNINGS}")
    
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    message(FATAL_ERROR "Configuration Not Implemented: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
    
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # message(STATUS "MSVC Compiler not completely configured yet")
    set(MSVC_WARNINGS "/W3 ")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /Ox")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /Ox")
    # message(STATUS "${CMAKE_CXX_FLAGS_RELEASE}")
    # message(STATUS "DEBUGGING Flags${CMAKE_CXX_FLAGS_DEBUG}")
    # Actual Debug cmd line args
    # /GS /TP /analyze- /W3 /Zc:wchar_t /I"C:/antlr/277/include/antlr" /I"C:/Users/bbart_000/Documents/GitHub/srcML/dep/include"
    # /I"C:/antlr/277/include" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/srcml"
    # /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/oldclient"
    # /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/libsrcml"
    # /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/parser"
    # /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/translator"
    # /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/SAX2Framework"
    # /Zi /Gm- /Od /Ob0 /Fd"srcml_static.dir\Debug\vc120.pdb" /fp:precise
    # /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "VERSION=\"1\"" /D "REVISION=\"1\""
    # /D "CMAKE_INTDIR=\"Debug\"" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope
    # /RTC1 /GR /Gd /Oy- /MDd /Fa"Debug/" /EHsc /nologo /Fo"srcml_static.dir\Debug\"
    # /Fp"srcml_static.dir\Debug\srcml.pch" 
    
    # Actual Release cmd line arguments
    # /GS /TP /analyze- /W3 /Zc:wchar_t /I"C:/antlr/277/include/antlr" /I"C:/Users/bbart_000/Documents/GitHub/srcML/dep/include" /I"C:/antlr/277/include" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/srcml" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/oldclient" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/libsrcml" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/parser" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/translator" /I"C:/Users/bbart_000/Documents/GitHub/srcML/src/SAX2Framework" /Gm- /O2 /Ob2 /Fd"srcml_static.dir\Release\vc120.pdb" /fp:precise /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "VERSION=\"1\"" /D "REVISION=\"1\"" /D "CMAKE_INTDIR=\"Release\"" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /GR /Gd /Oy- /MD /Fa"Release/" /EHsc /nologo /Fo"srcml_static.dir\Release\" /Fp"srcml_static.dir\Release\srcml.pch" 
    
    # Release Default Command Line Options
    # /GS /GL /analyze- /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /Fd"Release\vc120.pdb"
    # /fp:precise /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_LIB" /D "_UNICODE" /D "UNICODE"
    # /errorReport:prompt /WX- /Zc:forScope /Gd /Oy- /Oi /MD /Fa"Release\" /EHsc
    # /nologo /Fo"Release\" /Fp"Release\TestingCompilerFlags.pch" 
    
    # Debugging Command line options
    # /GS /analyze- /W3 /Zc:wchar_t /ZI /Gm /Od /Fd"Debug\vc120.pdb" /fp:precise
    # /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_LIB" /D "_UNICODE" /D "UNICODE"
    # /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /Oy- /MDd /Fa"Debug\" /EHsc 
    # /nologo /Fo"Debug\" /Fp"Debug\TestingCompilerFlags.pch" 
    # set(GCC_WARNINGS "-Wno-long-long -Wall -Wextra  -Wall -pedantic -Wempty-body -Wignored-qualifiers -Wsign-compare -Wtype-limits -Wuninitialized")
    # # Adding global compiler definitions.
    # set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O3 -DNDEBUG ${GCC_WARNINGS}")
    # set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -DDEBUG --coverage -fprofile-arcs ${GCC_WARNINGS}")
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
endif()
