# Building srcML

srcML is built using cmake, www.cmake.org, (version 2.8.12 or above) and currently supports builds for
macOS, Fedora, Ubuntu, CentOS, OpenSUSE, and Windows Visual Studio.

Out of source builds (builds outside the source directory) are required. In source builds are not supported.

---

## Unix

* To generate a makefile in the current directory run, for out of source builds:

    `cmake <path_to_srcml>`

* For in source builds, use:

    `cmake .`

* The following make commands are supported with their usual meaning

    `make`  
    `make clean`  
    `make test`

* Client tests are enabled by default, while libsrcml and parser tests are disabled by default.
    These tests can be enabled/disabled via the cmake command:

    `cmake -DBUILD_CLIENT_TESTS=OFF -DBUILD_LIBSRCML_TESTS=ON -DBUILD_PARSER_TESTS=ON`

* You may need to run `ldconfig` to get the link to the libsrcml shared library path up to date

#### macOX

##### Packages Required

* antlr2
* boost
* cmake

For srcML documentation generation:

* man2html
* ronn
* doxygen

Additional packages that may not needed, but are recommended (for timing etc.):

* coreutils
* gnu-sed
* gnu-time

Distribution of dmg/pkg:

* pkgbuild

##### Notes

* The srcML client requires LibArchive with a version >= 3. OS X and macOS includes a version < 3. Because of this, LibArchive is
  statically linked into the client. There are two sources:

    * Install the libarchive that is part of brew, `brew install libarchive`
    * Download and build LibArchive in a directory `libarchive` parallel to the source directory, typically ~/libarchive


#### Linux Builds

Linux builds for Ubuntu, Fedora, CentOS and OpenSUSE are supported.

To find what is needed, it is recommended to consult the dockerfiles for the particular version:

* Ubuntu: https://github.com/srcML/Docker/blob/ubuntu_latest/base/Dockerfile
* Fedora: https://github.com/srcML/Docker/blob/fedora_latest/base/Dockerfile
* CentOS: https://github.com/srcML/Docker/blob/centos_latest/base/Dockerfile
* OpenSUSE:  https://github.com/srcML/Docker/blob/opensuse_latest/base/Dockerfile

Commands to install what is needed can be adapted from these, and they are tested to work.

You will also find dockerfiles for other versions of these distributions.

## Windows Using MSVC

Building in Windows requires that you have MSVC installed. Visual Studio 2017 or newer is known to work, while older versions have not been tested. This build only supports 64-bit binaries.

## Packages

* [Python 2.7.XX](https://www.python.org/downloads/)
* [Java JRE/JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [CMake](http://www.cmake.org)
* [Visual Studio 2017 or later](https://www.visualstudio.com/downloads/)
* Zipped [Visual Studio 2017 Build Dependencies](http://www.sdml.cs.kent.edu/build/deps-06_20_18-VS2017.zip)

##### Instructions
* Install Visual Studio 2017 or newer, CMake, and Python 2.7.XX
* Locate the source code for srcML
* Extract the zipped build dependencies
    * The extracted folder will be named deps, and its structure will look like the following:
    ```
        deps/
            include/
            tools/
            x64/
    ```        
    * When copied into the srcML source code directory the result should look like the following:
    ```
        srcML/
                bindings/
                BUILD.md
                CMake/
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
    * If Python 2.7.XX is not in the windows PATH environment variable, you may need to provide CMake with the path to python.exe
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
        ex. cmake ..\srcML\ -G "Visual Studio 15 2017 Win64"
    ```
    * Execute the build.
    ```
        cmake --build . --config [build mode]
        ex. cmake --build . --config release
    ```

* Once built, locate the build folder. Within that folder there is now a directory named `bin` containing the release or debug versions of srcML executable and library along with all other dependencies.
