cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

# set includes and link locations
find_package(LibXml2)
if(LIBXML2_FOUND)
        include_directories(${LIBXML2_INCLUDE_DIR})
endif()

include_directories(../../src ../../src/libsrcml)
link_directories(../../bin)

# set compile and linker flags
add_definitions(-g)
add_definitions(--coverage)
add_definitions(-O0)
set(CMAKE_EXE_LINKER_FLAGS -fprofile-arcs)

add_executable(test_Language test_Language.cpp)
target_link_libraries(test_Language srcml)
add_executable(test_Options test_Options.cpp)
target_link_libraries(test_Options srcml)
add_executable(test_State test_State.cpp)
target_link_libraries(test_State srcml)
add_executable(test_UTF8CharBuffer test_UTF8CharBuffer.cpp)
target_link_libraries(test_UTF8CharBuffer srcml ${LIBXML2_LIBRARIES})
add_executable(test_srcMLState test_srcMLState.cpp)
target_link_libraries(test_srcMLState srcml)
add_executable(test_srcMLStateStack test_srcMLStateStack.cpp)
target_link_libraries(test_srcMLStateStack srcml)
add_executable(test_srcMLTranslatorCore test_srcMLTranslatorCore.cpp)
target_link_libraries(test_srcMLTranslatorCore srcml ${LIBXML2_LIBRARIES})

# ctest
include(CTest)
enable_testing()
set(CTEST_DROP_SITE_CDASH FALSE)
set(MEMORYCHECK_COMMAND /usr/local/bin/valgrind)
set(COVERAGE_COMMAND /usr/bin/gcov)

add_test(NAME test_Language COMMAND test_Language)
add_test(NAME test_Options COMMAND test_Options)
add_test(NAME test_State COMMAND test_State)
add_test(NAME test_UTF8CharBuffer COMMAND test_UTF8CharBuffer)
add_test(NAME test_srcMLState COMMAND test_srcMLState)
add_test(NAME test_srcMLStateStack COMMAND test_srcMLStateStack)
add_test(NAME test_srcMLTranslatorCore COMMAND test_srcMLTranslatorCore)
