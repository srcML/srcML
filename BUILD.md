# Building srcML

srcML is built using cmake, www.cmake.org, (version 2.8.12 or above) and currently supports builds for OS X, Fedora, Ubuntu, Linux Mint, and Windows Visual Studio.

Out of source builds (builds outside the source directory) are recommended, however in source builds (builds within the source directory) are supported.

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

    Tests are enabled by default, but can be disabled by editing the file `CMake/config.cmake`. Search for the section "Turn ON/OFF Tests", which contains a list of all test categories. For example, the CLI tests can be turned off by changing the line
    ```bash
    option(BUILD_CLIENT_TESTS "Build cli tests" ON)
    ```
    to
    ```bash
    option(BUILD_CLIENT_TESTS "Build cli tests" OFF)
    ```
* You may need to run `ldconfig` to get the link to the libsrcml shared library path up to date

### Unix Distribution Specifics

#### OSX

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

* PackageMaker

##### Notes

* OS X does not install header files for libarchive. You can download these from Apple's:

    * [archive.h](https://opensource.apple.com/source/libarchive/libarchive-41.70.1/libarchive/libarchive/archive.h)
    * [archive_entry.h](http://www.opensource.apple.com/source/libarchive/libarchive-30/libarchive/libarchive/archive_entry.h?txt)

* PackageMaker is currently being used. However, it is deprecated and no longer available. A conversion to pkgbuild is in progress

#### Fedora

##### Required Packages

* gcc
* gcc-c++
* antlr
* libxml2-devel
* libxslt-devel
* cmake
* libarchive-devel
* boost-devel
* boost
* libcurl-devel
* openssl-devel
* antlr-C++

srcML documentation generation

* man2html
* ronn

rpm generation

* @development-tools
* fedora-packager

for mingw cross compilation for Windows

* dos2unix
* mingw32-gcc-c++
* mingw32-antlr
* mingw32-libarchive
* mingw32-libxml2
* mingw32-libxslt
* mingw32-boost-static
* mingw32-pthreads
* mingw32-libgnurx
* mingw32-curl
* mingw32-openssl
* mingw32-nsis

##### Notes

* Fedora and Windows cross compiling require:

First fnmatch.h needs copied over into mingw includes (something like):

    cp /usr/include/fnmatch.h /usr/i686-w64-mingw32/sys-root/mingw/include/

To actually run the compiled srcML, the dlls provided by mingw will
need to be copied along with the source to the intended Windows
machine.

#### openSUSE

##### Required Packages

* gcc
* gcc-c++
* antlr
* antlr-devel
* libxml2-devel
* libxslt-devel
* libxslt-tools
* libarchive-devel
* libcurl-devel
* libopenssl-devel

srcML documentation generation:

* ronn

##### Notes

* See 64-bit Linux for installing boost.

* Building srcML requires a version of cmake greater that what is
typically supported. Visit http://www.cmake.org to download and
install the lastest version of cmake.

* There is no man2html package on OpenSUSE for srcML documentation
generation. The url is:

    `http://www.oit.uci.edu/indiv/ehood/tar/man2html3.0.1.tar.gz`

* Use the following to install as ./install.me does not appear to use
the right location

    perl install.me

* libantlr was not compiled with -fPIC for 64-bit (at least for 13.2). To do so,
change line 59 of scripts/cxx.sh.in to `cxxflags="-felide-constructors -pipe -fPIC"`.
You will also need to fix the compilation of lib/cpp/antlr/CharScanner.hpp by adding
`#include <strings.h>` and replacing EOF in line 475 to 1.Then you can build using
```./configure``` and ```make``` do not make install as you need the distributed antlr.
Instead, copy lib/cpp/src/libantlr.a to /usr/local/lib64/.

#### Ubuntu (and Linux Mint)

##### Required Packages

* gcc
* g++
* libxml2-dev
* libxslt1-dev
* libarchive-dev
* antlr
* libantlr-dev
* libcurl4-openssl-dev
* libssl-dev

For srcML documentation generation:

* man2html
* ronn

##### Notes

* Ubuntu versions before 14.04 (possibly before 13) do not
provide a recent enough version of cmake. Visit http://www.cmake.org
to download and install the lastest version of cmake.

* boost also needs to be installed separately, see 64-bit Linux.


#### 64-bit Linux

##### Notes

* The boost libraries are statically compiled in, 64-bit machines do not provide adequate libraries for static compilation (require compilation with -fPIC). Download boost at http://www.boost.org. The commands to build boost are:

    `./bootstrap.sh --without-libraries=atomic,chrono,context,coroutine,exception,graph,graph_parallel,iostreams,locale,log,math,mpi,python,random,serialization,signals,test,timer,wave`


    `./b2 link=static cxxflags="-fPIC -static -Wl,--whole-archive" threading=multi install`

---

## Windows Using MSVC

Building in Windows requires that you have MSVC installed. Visual Studio 2015 or newer is known to work, while older versions have not been tested.

## Packages

* [Python 2.7.XX](https://www.python.org/downloads/)
* [Java JRE/JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [CMake](http://www.cmake.org)
* [Visual Studio 2015 or later](https://www.visualstudio.com/downloads/)
* Zipped [Visual Studio 2017 Build Dependencies](http://www.sdml.cs.kent.edu/build/deps-VS2017.zip)
* Zipped [Visual Studio 2015 Build Dependencies](http://www.sdml.cs.kent.edu/build/deps-VS2015.zip)

##### Instructions
* Install Visual Studio 2015 or newer, CMake, and Python 2.7.XX
* Locate the source code for srcML
* Extract the zipped build dependencies
    * The extracted folder will be named deps, and its structure will look like the following:
    ```
        deps/
            include/
            tools/
            x64/
            x86/
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
                    x86/
                doc/
                ...etc...
    ```
* NOTES:
    * Building srcml should be done in a separate directory external to the source code to avoid issues
    * If Python 2.7.XX is not in the windows PATH environment variable, you may need to provide CMake with the path to python.exe
* Graphical Interface Build:
    * Open the CMake GUI program.
    * Browser for the srcML source code directory and your target build directory
    * Hit configure and select the appropriate Visual Studio version and architecture (x86 or x64) as the target system.
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
