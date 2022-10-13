This directory contains a set of examples of using libsrcml.

Build the examples using cmake.

Some notes:

* Any more recent CMake version can be used
* The package LibsrcML has two components, *shared* and *static*
* The default component is *shared*
* The `target_link_library(<EXECUTABLE> PRIVATE srcML::LibsrcML)` is all that
    is required for access to the include file srcml.h and the libsrcml library.
