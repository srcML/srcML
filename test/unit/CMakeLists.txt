cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

# set includes and link locations
find_package(LibXml2)
if(LIBXML2_FOUND)
        include_directories(${LIBXML2_INCLUDE_DIR})
endif()

macro(add_unit_test TEST_NAME FILE_NAME)
    add_executable(${TEST_NAME} ${FILE_NAME})
    target_link_libraries(${TEST_NAME} srcml ${ARGN})
    add_test(${TEST_NAME} ${TEST_NAME})
endmacro(add_unit_test)

include_directories(../../src ../../src/libsrcml)
link_directories(../../bin)

# set compile and linker flags
add_definitions(-g)
add_definitions(--coverage)
add_definitions(-O0)
set(CMAKE_EXE_LINKER_FLAGS -fprofile-arcs)

# ctest
#include(CTest)
enable_testing()
set(CTEST_DROP_SITE_CDASH FALSE)
set(MEMORYCHECK_COMMAND /usr/local/bin/valgrind)
set(COVERAGE_COMMAND /usr/bin/gcov)


add_unit_test(test_Language test_Language.cpp)
add_unit_test(test_Options test_Options.cpp)
add_unit_test(test_State test_State.cpp)
add_unit_test(test_UTF8CharBuffer test_UTF8CharBuffer.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcMLState test_srcMLState.cpp)
add_unit_test(test_srcMLStateStack test_srcMLStateStack.cpp)
add_unit_test(test_srcMLTranslatorCore test_srcMLTranslatorCore.cpp ${LIBXML2_LIBRARIES})

