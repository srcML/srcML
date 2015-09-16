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
 	`make tests`  

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

    * [archive.h](http://www.opensource.apple.com/source/libarchive/libarchive-30/libarchive/libarchive/archive.h?txt)
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

* The boost libraries are statically compiled in, 64-bit machines do not provide adequate libraries for static compilation (require compilation with -fPIC). Download boost at http://www.boost.org. The commands to build boost (written for boost 1.55.0) are:

	`./bootstrap.sh --without-libraries=atomic,chrono,context,coroutine,exception,graph,graph_parallel,iostreams,locale,log,math,mpi,python,random,serialization,signals,test,timer,wave`
	
	
	`./b2 link=static cxxflags="-fPIC -static -Wl,--whole-archive" threading=multi install`

---

## Windows Using MSVC
 
Building in Windows requires that you have MSVC installed. Visual Studio 12.0 Express or newer is known to work, while older versions have not been tested. 
	
## Packages

* [Python 2.7.XX](https://www.python.org/downloads/release/python-2710/)
* [CMake](http://www.cmake.org)
* [Visual Studio 12](http://www.microsoft.com/en-us/download/details.aspx?id=34673)
* Zipped [Build Dependencies](http://www.sdml.info/build/srcml_windows_build_dependencies.zip)
* Zipped [Executable Dependencies](http://www.sdml.info/build/srcml_windows_exe_dependencies.zip)

##### Instructions
* Install Visual Studio 12.0 or newer, CMake, and Python
* Locate the source code for srcML
* Extract the zipped [build dependencies](http://www.sdml.info/build/srcml_windows_build_dependencies.zip), srcml_windows_build_dependencies.zip, into the srcML folder.
    * The extracted folder will be named dep, and its structure will look like the following:
    ```
        dep/
            bin/
            include/
            lib/
    ```        
    * When copied into the srcML source code directory the result should look like the following:
    ```
        srcML/
                bindings/
                cli/
                CMake/
                dep/
                    bin/
                    include/
                    lib/
                doc/
                ...etc...
    ```
* Run CMake on the project using the CMake GUI program. Select the appropriate Visual Studio as the target system. At this tyme, 64-it compilation under Windows is not supported. Note that it's best practice to have CMake output into a separate directory, as this can cause problems if you decide to rebuild later on. Configure and generate CMake.

* Locate the CMake build output directory and open the solution `srcML.sln`. When the solution loads, change the Solution Configuration from `Debug` to `Release`. A debug version of srcML under windows is not supported.
    
* Build solution. Once built, locate the build folder. Within that folder there is now a folder named `bin/Release` containing the srcML executable and libraries. Extract the dlls from the zipped [executable dependencies](http://www.sdml.info/build/srcml_windows_exe_dependencies.zip) to `bin/Release` for dynamic linking.


##### Notes

* The boost and antlr libraries provided were built for Visual Studio 12. If you are using a different version of Visual Studio, these will need to be compiled separately and replace the libraries in the dependency folder.
