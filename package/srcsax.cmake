# SPDX-License-Identifier: GPL-3.0-only

include(FetchContent)
FetchContent_Declare(srcsax
    GIT_REPOSITORY https://github.com/srcML/srcSAX.git
    GIT_TAG fbcd31409ff6eee85603096ca26c55ca082d6ec0
)

# Keep srcSAX's build options and directory settings local to its build.
function(srcml_add_srcsax)
    set(INCLUDE_UNIT_TESTS OFF)
    set(INCLUDE_EXAMPLES OFF)
    set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME DEVLIBS)
    FetchContent_MakeAvailable(srcsax)
endfunction()

srcml_add_srcsax()
