# Building srcML

srcML is built using cmake, www.cmake.org, (version 2.8.12 or above) and currently supports builds for OS X, Fedora, Ubuntu, Linux Mint, and Windows Visual Studio.

In source (builds within the source directory) and out of source builds (builds outside the source directory) are supported.  However, the use of out of source build is recommended.

---

## Unix

* To generate a simple make file in the current directory run:

	* For in source builds:  

		`cmake .`

	* For out of source builds:

		`cmake path_to_srcml`

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

*# For srcML documentation generation:*  

  * man2html  
  * docbook  
  * docbook2x  

*# additional packages that may not needed, but are recommended (for timing etc.)*  

  *	coreutils  
  *	gnu-sed  
  *	gnu-time  

*# for dmg/pkg distribution*

  * PackageMaker  

##### Notes

* OSX does not provide headers for libarchive. Visit http://www.libarchive.org` and download 2.8.5 for headers that are close to the versions distributed on OSX.  The files needed are:
 * archive.h
 * archive_entry.h

* PackageMaker is part of the auxiliary tools (late July 2012 for Mountain Lion or February 2012 for Lion) that are not distributed by default and need to be obtained from https://developer.apple.com/downloads/index.action

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

*# srcML documentation generation*

* man2html  
* 	docbook2X  

*# rpm generation*

* @development-tools  
* fedora-packager  

*# for mingw cross compilation for Windows*

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
needed:

* First fnmatch.h needs copied over into mingw includes (something like):

`cp /usr/include/fnmatch.h /usr/i686-w64-mingw32/sys-root/mingw/include/`  

* To actually run the compiled srcML, the dlls provided by mingw will
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

*# srcML documentation generation*

* docbook2X

##### Notes

* See 64-bit Linux for installing boost.

* Building srcML requires a version of cmake greater that what is
typically supported.  Visit http://www.cmake.org to download and
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
`#include <strings.h>` and replacing EOF in line 475 to 1.  Then you can build using
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

*# for srcML documentation generation*

* man2html  
* docbook2X  

##### Notes

* Ubuntu versions before 14.04 (possibly before 13) do not
provide a recent enough version of cmake.  Visit http://www.cmake.org
to download and install the lastest version of cmake.

* boost also needs to be installed separately, see 64-bit Linux.


#### 64-bit Linux

##### Notes

* The boost libraries are statically compiled in, 64-bit machines do not provide adequate libraries for static compilation (require compilation with -fPIC).  Download boost at http://www.boost.org.  The commands to build boost (written for boost 1.55.0) are:

	`./bootstrap.sh --without-libraries=atomic,chrono,context,coroutine,exception,graph,graph_parallel,iostreams,locale,log,math,mpi,python,random,serialization,signals,test,timer,wave`
	
	
	`./b2 link=static cxxflags="-fPIC -static -Wl,--whole-archive" threading=multi install`

---

## Windows Using MSVC
 
Building under MSVC requires that you have MSVC installed. Visual studio 12.0 or newer is known to work, other versions have not been tested. 
	
## Packages

* cmake from `http://www.cmake.org`  
* Visual Studio 12 Express (Optional. If you already have Visual studio installed you don't need this) `http://www.microsoft.com/en-us/download/details.aspx?id=34673`
* Dependencies (win-dep.zip): **LINK PENDING**

##### Notes

##### How to build srcML on windows using Visual Studio
* Install visual studio 12.0 or newer
* Locate the source code for srcML
* Extract the win-dep.zip dependencies folder into srcML folder.
    * The extracted folder must be named dep (it should already be named dep).
    * The folder structure will look like the following:
        dep/
            bin/
            include/
            lib/
    * When copied into the srcML source code directory the result should look like the following:
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
* Run cmake on the project using the cmake GUI program.
    * When opening CMake select Visual Studio 12 as your target system, at this time 64-bit compilation under windows is not supported.
    * It's best practice to have CMake output into a separate directory, as this can cause problems if you decide to rebuild later on.

* Uncheck the `ENABLE_UNIT_TESTS` option as this will cause the build to fail.
* Configure and generate cmake.
    * cmake may complain about not being able to locate xsltproc, but that's only used for executing the test suite so it can be ignored.

* Once this is done locate the CMake build output directory and open the solution `srcML.sln`.
* Once the solution is loaded change the Solution Configuration from `Debug` to `Release`.
    * Currently, a debug version of srcML cannot be built under windows due to an issue with a dependency.
    
* Build solution.
* Once built locate the build folder, within the build folder there is a now a folder named `bin/` containing the srcML executable and libraries.