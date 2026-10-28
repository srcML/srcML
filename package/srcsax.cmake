# SPDX-License-Identifier: GPL-3.0-only

include(FetchContent)
FetchContent_Declare(srcsax
    GIT_REPOSITORY https://github.com/srcML/srcSAX.git
    GIT_TAG 9d1bf2fee324e2ae50395bfcad56cca92b0edbd1
)

# Keep srcSAX's build options and directory settings local to its build.
function(srcml_add_srcsax)
    set(INCLUDE_UNIT_TESTS OFF)
    set(INCLUDE_EXAMPLES OFF)
    set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME DEVLIBS)
    FetchContent_MakeAvailable(srcsax)
endfunction()

srcml_add_srcsax()
