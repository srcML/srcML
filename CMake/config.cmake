# @copyright
# 
# Copyright (C) 2013  SDML (www.srcML.org)
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
option(ENABLE_SVN_INTEGRATION "Build with SVN integration." ON)
set_property(GLOBAL PROPERTY SVN_ENABLED ${ENABLE_SVN_INTEGRATION})

# Adding SAX to configuration.
option(LIBSRCML_SAX2_ENABLED "Build with SAX2Framework for srcML" OFF)
set_property(GLOBAL PROPERTY SAX2_ENABLED ${LIBSRCML_SAX2_ENABLED})

# Locating packages.
find_package(LibArchive REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibXslt)
find_package(Boost COMPONENTS program_options REQUIRED)
set_property(GLOBAL PROPERTY BOOST_PROGRAM_OPTIONS_LIB ${Boost_LIBRARIES})
set_property(GLOBAL PROPERTY LIBARCHIVE_LIBS ${LibArchive_LIBRARIES})

include_directories(${LibArchive_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})
set_property(GLOBAL PROPERTY LIBXML2_LIBS ${LIBXML2_LIBRARIES})

if(LIBXSLT_FOUND)
    include_directories(${LIBXSLT_INCLUDE_DIR})
endif()

# Setting Properties
if(LIBXSLT_EXSLT_LIBRARY)
    set_property(GLOBAL PROPERTY LIBXSLT_LIBS ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY})
else()
    set_property(GLOBAL PROPERTY LIBXSLT_LIBS "")
endif()

# Setting some windows only properties.
if(WIN32)
    # Adding suspected windows include directory for ANTRL
    include_directories("C:/antlr/277/include/antlr")
    set(WINDOWS_DEP_PATH ${PROJECT_SOURCE_DIR}/"dep")
    include_directories(${WINDOWS_DEP_PATH}/apr-1.0 ${WINDOWS_DEP_PATH}/subversion-1)
else()
    set(WINDOWS_DEP_PATH "")
    if(ENABLE_SVN_INTEGRATION)
        include_directories(/usr/include/apr-1.0 /usr/include/subversion-1 /usr/local/include/subversion-1)
    endif()
endif()
set_property(GLOBAL PROPERTY WINDOWS_DEP_PATH ${WINDOWS_DEP_PATH})

# Locating the antlr library.
find_library(ANTLR_LIB NAMES libantlr-pic.a libantlr.a libantlr2-0.dll PATHS /usr/lib /usr/local/lib ${WINDOWS_DEP_PATH}/lib)
set_property(GLOBAL PROPERTY ANTLR_LIB ${ANTLR_LIB})

# Finding antlr library.
find_program(ANTLR_EXE NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin ${WINDOWS_DEP_PATH}/bin)
set_property(GLOBAL PROPERTY ANTLR_EXE ${ANTLR_EXE})

# Finding SED
find_program(SED_EXE NAMES gsed sed PATHS /opt/local/bin /usr/local /bin ${WINDOWS_DEP_PATH}/bin)
set_property(GLOBAL PROPERTY SED_EXE ${SED_EXE})

#Finding GREP
find_program(GREP_EXE grep PATHS /bin /usr/bin ${WINDOWS_DEP_PATH}/bin)
set_property(GLOBAL PROPERTY GREP_EXE ${GREP_EXE})

#  Figure out exactly what package curl is and where it is located.
# find_package(libcurl3)


# Locating python libraries and executables for use
# in compiling and testing.
find_package(PythonLibs REQUIRED)
# PYTHONLIBS_FOUND = have the Python libs been found
# PYTHON_LIBRARIES = path to the python library
# PYTHON_INCLUDE_PATH = path to where Python.h is found
# PYTHON_DEBUG_LIBRARIES = path to the debug library


find_package(PythonInterp REQUIRED)
# Enforcing that the version of python being used must have a major version of 2.
# and the minor version be greater than version 6 (this means version 2.7 of python 
# version 2 or newer).
if(NOT ${PYTHON_VERSION_MAJOR} EQUAL "2")
    message(FATAL_ERROR "Version of python found does not have a major version of 2.")
    if(${PYTHON_VERSION_MINOR} LESS EQUAL 6)
        message(FATAL_ERROR "Minor version of python is not greater than 6.")
    endif()
endif()
set_property(GLOBAL PROPERTY PYTHON_INTERP_EXE ${PYTHON_EXECUTABLE})


# message(STATUS "Python Executable: ${PYTHON_EXECUTABLE}")
# PYTHONINTERP_FOUND = Was the Python executable found.
# PYTHON_EXECUTABLE = Path to the Python interpreter.
# PYTHON_VERSION_STRING = Python version found e.g. 2.5.2.
# PYTHON_VERSION_MAJOR = Python major version found e.g. 2.
# PYTHON_VERSION_MINOR = Python minor version found e.g. 5.
# PYTHON_VERSION_PATCH = Python patch version found e.g. 2.

# Adding Compiler Configuration options
if(${CMAKE_COMPILER_IS_GNUCXX})
      # Adding global compiler definitions.
      set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wno-long-long -g -O0 -DDEBUG --coverage -fprofile-arcs -DNO_DLLOAD")
      set(CMAKE_CXX_FLAGS_RELEASE "-pedantic -Wall -Wno-long-long -O3 -DNDEBUG")
      
      # This allows for compilation of a re-locatable execuatable on GCC I need to be sure that I
      # can make this portable to compilers other than GCC.
      add_definitions(-fPIC)
endif()