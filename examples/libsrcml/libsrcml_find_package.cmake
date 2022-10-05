# Any more recent cmake can be used
cmake_minimum_required(VERSION 3.24)

project(libsrcml_find_package)

# Produces the target LibsrcML::LibsrcML
find_package(libsrcml REQUIRED)

# add_executable(Example Example.cpp)
# target_link_libraries(Example PRIVATE LibsrcML::LibsrcML)
