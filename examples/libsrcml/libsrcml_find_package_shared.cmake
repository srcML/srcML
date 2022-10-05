# Any more recent cmake can be used
cmake_minimum_required(VERSION 3.24)

project(libsrcml_find_package_shared)

# Produces the target LibsrcML::LibsrcML specifying as a shared library
find_package(libsrcml REQUIRED shared)

# add_executable(Example Example.cpp)
# target_link_libraries(Example PRIVATE LibsrcML::LibsrcML)
