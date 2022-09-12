# Building srcML

srcML is built using cmake, www.cmake.org, (version 3.24 or above) and currently supports builds for
macOS, Fedora, Ubuntu, CentOS, OpenSUSE, and Windows Visual Studio.

Out of source builds (builds outside the source directory) are required. In source builds are not supported.

## Unix

To generate a makefile in your build directory:

    cmake <path_to_srcml>

 The following make commands are supported with their usual meaning

    make  
    make clean  
    make test  
    make install

 Client tests are enabled by default, while libsrcml and parser tests are disabled by default.
 These tests can be enabled/disabled via the cmake command, e.g. with a parallel source directory,

    cmake -DBUILD_CLIENT_TESTS=OFF -DBUILD_LIBSRCML_TESTS=ON -DBUILD_PARSER_TESTS=ON ../srcML

 You may need to run `ldconfig` to get the link to the libsrcml shared library path up to date

### macOS

The main packages required may be installed via brew:

    brew install antlr2 boost cmake

Libarchive greater than 3.0.0 is required. For macOS previous to Catalina (19.*.*), libarchive.a 3.3.* must be 
statically included. Use brew to install a more recent version:

    `brew install libarchive`

To generate srcML documentation:

    brew install man2html doxygen

Additional packages that may not needed, but are recommended (for timing etc.):

    brew install coreutils gnu-sed gnu-time

#### Linux

Linux builds for Ubuntu, Fedora, CentOS and OpenSUSE are supported.

To find what is needed, it is recommended to consult these dockerfiles for the particular distribution:

* [Ubuntu](https://github.com/srcML/Docker/blob/ubuntu_latest/base/Dockerfile)
* [Fedora](https://github.com/srcML/Docker/blob/fedora_latest/base/Dockerfile)
* [CentOS](https://github.com/srcML/Docker/blob/centos_latest/base/Dockerfile)
* [OpenSUSE](https://github.com/srcML/Docker/blob/opensuse_latest/base/Dockerfile)

Commands to install what is needed can be adapted from these, and they are tested to work. You will also find dockerfiles for older versions of these distributions.

## Windows Using MSVC

Building in Windows requires that you have MSVC installed. Visual Studio 2017 or newer is known to work, while older versions have not been tested. This build only supports 64-bit binaries.

## Packages

* [Java JRE/JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [CMake](http://www.cmake.org)
* [Visual Studio 2017 or later](https://www.visualstudio.com/downloads/)
* Zipped [Visual Studio 2017 Build Dependencies](http://www.sdml.cs.kent.edu/build/VS2017_Dependencies-06_20_18.zip)

##### Instructions
* Install Visual Studio 2017 or newer, CMake
* Locate the source code for srcML
* Extract the zipped build dependencies
    * The extracted folder will be named deps, and its structure will look like the following:

        ```
            deps/
                debug/
                include/
                release/
                tools/
        ```
    * When copied into the srcML source code directory the result should look like the following:

        ```
            srcML/  
                    BUILD.md
                    package/
                    CMakeLists.txt
                    COPYING.txt
                    CTestConfig.cmake
                    deps/
                        include/
                        tools/
                        x64/
                    doc/
                    ...etc...
        ```
* NOTES:
    * Building srcml should be done in a separate directory external to the source code to avoid issues
* Graphical Interface Build:
    * Open the CMake GUI program.
    * Browser for the srcML source code directory and your target build directory
    * Hit configure and select the appropriate Visual Studio version and x64 architecture as the target system.
    * Click Generate
    * Open srcML.sln with Visual Studio located in your the target build directory
    * Right click Project "ALL_BUILD" and choose "build"
* Command Line Buid:
    * Generate the build files in your target build directory.

        ```
            cmake [path to srcML source directory] -G [target visual studio version and architecture]   
        ```
        For example:

        ```
            cmake ..\srcML\ -G "Visual Studio 15 2017 Win64"
        ```
    * Execute the build.

        ```
            cmake --build . --config [build mode]
        ```
        For example:

        ```
            cmake --build . --config release
    ```

* Once built, locate the build folder. Within that folder there is now a directory named `bin` containing the release or debug versions of srcML executable and library along with all other dependencies.
