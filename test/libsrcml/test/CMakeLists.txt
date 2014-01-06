cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

# set include and link locations
find_package(LibXml2)
if(LIBXML2_FOUND)
        include_directories(${LIBXML2_INCLUDE_DIR})
endif()

macro(add_unit_test TEST_NAME FILE_NAME)
    add_executable(${TEST_NAME} ${FILE_NAME})
    target_link_libraries(${TEST_NAME} srcml ${ARGN})
    add_test(${TEST_NAME} ${TEST_NAME})
endmacro(add_unit_test)

include_directories(../../../src/parser ../../../src/translator 
../../../src/libsrcml ../../../src/SAX2Framework ../../../src/oldclient)
link_directories(../../../bin)

# set compile and linker flags
add_definitions(-g)
add_definitions(--coverage)
add_definitions(-O0)
set(CMAKE_EXE_LINKER_FLAGS -fprofile-arcs)

# ctest
include(CTest)
enable_testing()
set(CTEST_DROP_SITE_CDASH FALSE)
set(MEMORYCHECK_COMMAND /usr/local/bin/valgrind)
set(COVERAGE_COMMAND /usr/bin/gcov)


# Build and add tests
add_unit_test(test_srcMLReader test_srcMLReader.cpp ${LIBXML2_LIBRARIES})
#add_unit_test(test_srcMLSAX2Reader test_srcMLSAX2Reader.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcml_append_transform test_srcml_append_transform.cpp)
add_unit_test(test_srcml_apply_transforms test_srcml_apply_transforms.cpp)
add_unit_test(test_srcml_archive_check_extension test_srcml_archive_check_extension.cpp)
add_unit_test(test_srcml_archive_get test_srcml_archive_get.cpp)
add_unit_test(test_srcml_archive_set test_srcml_archive_set.cpp)
add_unit_test(test_srcml_clear_transforms test_srcml_clear_transforms.cpp)
add_unit_test(test_srcml_clone_archive test_srcml_clone_archive.cpp)
add_unit_test(test_srcml_create_archive test_srcml_create_archive.cpp)
add_unit_test(test_srcml_create_unit test_srcml_create_unit.cpp)
add_unit_test(test_srcml_extract_text test_srcml_extract_text.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcml_global test_srcml_global.cpp)
add_unit_test(test_srcml_global_access test_srcml_global_access.cpp)
add_unit_test(test_srcml_list test_srcml_list.cpp)
add_unit_test(test_srcml_parse_unit test_srcml_parse_unit.cpp)
add_unit_test(test_srcml_read_open test_srcml_read_open.cpp)
add_unit_test(test_srcml_read_unit test_srcml_read_unit.cpp)
add_unit_test(test_srcml_relaxng test_srcml_relaxng.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcml_sax2_utilities test_srcml_sax2_utilities.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcml_unit_get test_srcml_unit_get.cpp)
add_unit_test(test_srcml_unit_set test_srcml_unit_set.cpp)
add_unit_test(test_srcml_unparse_unit test_srcml_unparse_unit.cpp)
add_unit_test(test_srcml_write_open test_srcml_write_open.cpp)
add_unit_test(test_srcml_write_unit test_srcml_write_unit.cpp)
add_unit_test(test_srcml_xpath test_srcml_xpath.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcml_xslt test_srcml_xslt.cpp ${LIBXML2_LIBRARIES})


