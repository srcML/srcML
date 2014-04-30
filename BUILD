* # Building srcML #

	srcML is built using cmake www.cmake.org (version 2.8.12 or above) and
	supports builds for OSX, Fedora, Ubuntu, Linux Mint, and Windows
	Visual Studio (except clients src2srcml and srcml2src).

	In source (builds within the source directory) and out of source
	builds (builds outside the source directory) are supported.  However,
	the use of out of source build are usually recommended.

* ## Unix ##

	To generate a simple make file in the current directory run:

	For in source builds:  
	`cmake .`  

	For out of source builds:  
	`cmake path_to_srcml`  

	The following make commands are supported with their usual meaning
	`make`  
	`make clean`  
	`make tests`  

* ### Unix distro specifics: ###

* #### OSX ####

	* #####Packages Required#####

		antlr 2  
		boost  
		cmake  

		*# for srcML documentation generation*  
		man2html  
		docbook  
		docbook2x  

		*# additional packages that may not needed, but are recommended (for timing etc.)*  
		coreutils  
		gnu-sed  
		gnu-time  

		*# for dmg/pkg distruction*  
		PackageMaker  

	* #####Notes#####

		* OSX does not provide headers for libarchive visit
		http://www.libarchive.org and download 2.8.5 for headers that are
		close to the versions distributed on OSX.  The files needed are
		archive.h and archive_entry.h.

		* PackageMaker is part of the auxiliary tools (late July 2012 for
		Mountain Lion or February 2012 for Lion) that are not distributed by
		default and needs to be gotten from the following URL:
		https://developer.apple.com/downloads/index.action

Fedora:

Packages required:

gcc
gcc-c++
antlr
libxml2-devel
libxslt-devel
cmake
libarchive-devel
boost-devel
boost
libcurl-devel
openssl-devel
antlr-C++

# for srcML documentation generation
man2html
docbook2X

# for rpm generation
@development-tools
fedora-packager

# for mingw cross compilation for Windows
dos2unix
mingw32-gcc-c++
mingw32-antlr
mingw32-libarchive
mingw32-libxml2
mingw32-libxslt
mingw32-boost-static
mingw32-pthreads
mingw32-libgnurx
mingw32-curl
mingw32-openssl
mingw32-nsis

For Fedora and Windows Cross compiling a few addition things are
needed:

First fnmatch.h needs copied over into mingw includes.

cp /usr/include/fnmatch.h /usr/i686-w64-mingw32/sys-root/mingw/include/

To actually run the compiled srcML, the dlls provided by mingw will
need to be copied along with the source to the intended Windows
machine.

openSUSE:

Packages Required:

gcc
gcc-c++
antlr
antlr-devel
libxml2-devel
libxslt-deve
libxslt-toolsl
libarchive-devel
libcurl-devel
libopenssl-devel
docbook2X

See 64-bit Linux for installing boost.

Building srcML requires a version of cmake greater that what is
typically supported.  Visit http://www.cmake.org to download and
install the lastest version of cmake.

There is no man2html package on OpenSUSE for srcML documentation
generation. The url is:
http://www.oit.uci.edu/indiv/ehood/tar/man2html3.0.1.tar.gz

Use the following to install as ./install.me does not appear to use
the right location

perl install.me

Ubuntu (and Linux Mint):

gcc
g++
libxml2-dev
libxslt1-dev
libarchive-dev
antlr
libantlr-dev
libcurl4-openssl-dev
libssl-dev

# for srcML documentation generation
man2html
docbook2X


Currently, Ubuntu versions before 14.04 (possibly before 13) do not
provide a recent enough version of cmake.  Visit http://www.cmake.org
to download abd install the lastest version of cmake.

boost also needs to be installed separately see 64-bit Linux.


64-bit Linux:

The boost libraries are statically compiled in, 64-bit machines do not
provide adequate libraries for static compilation (require compilation
with -fPIC)

Download boost at http://www.boost.org, the commands to build boost
(written for boost 1.55.0) are:

./bootstrap.sh --without-libraries=atomic,chrono,context,coroutine,exception,graph,graph_parallel,iostreams,locale,log,math,mpi,python,random,serialization,signals,test,timer,wave
./b2 link=static cxxflags="-fPIC -static -Wl,--whole-archive"
threading=multi install

Windows Visual Studio (clients src2srcml and srcml2src not supported):

Installing for Windows Visual Studio, requires a far larger amount of
preparation.  In summary you will need:

cmake (GUI from http://www.cmake.org)

# some GNU command line utilities (from cygwin or other sources)
grep
sed

# required libraries (can be gotten from http://xmlsoft.org/sources/win32/)
iconv
libxml2
libxslt
zlib

antlr 2 (http://www.antlr2.org/download/antlr-2.7.7.msi and http://www.antlr2.org/download/antlr-2.7.7.tar.gz)
boost

For the required libraries, you will need to create a folder dep
in the top level directory of srcML project.  It needs to have the
following structure:

dep/
    include/
    bin/
    lib/

Copy the contents of the required libraries into the correct
locations.  If the includes are a directory, copy the entire directory
and not the source within the directory e.g. copy entire libxml
subdirectory.  Note: There may be some runtime problems with these
libraries and the ones installed on your Windows machine, in which
case, a Windows Visual Studio build of each may be required.

For antlr 2 you will need the installer and the source code.  The .lib
file provided with antlr 2 does not seem to work.  So, a Windows
Visual Studio build will need to be done to generate a correct copy of
the .lib.  When cmake is configuring the project, it will ask for the
correct location of antlr and the library.  Note: Create a empty
project in Visual Studio, set it to generate a static library and turn
of clr, and add all the source from (lib/antlr/cpp/src) to "Source
Files".  Then, set lib/antlr/cpp/antlr as an include directory.

boost will need to be compiled.  The zip file is large, and can take a
large amount of time on Windows.  Then, run:

./bootstrap.sh

This will create a project-config.jam file copy the following contents
replacing everything (Using boostrap.sh to configure does not seem to
work correctly on Windows):

import option ; 

using msvc ; 

libraries =  --without-atomic --without-chrono --without-context --without-coroutine --without-exception --without-graph --without-graph_parallel --without-iostreams --without-locale --without-log --without-math --without-mpi --without-python --without-random --without-serialization --without-signals --without-test --without-timer --without-wave ;

option.set keep-going : false ; 

Then, run:

./b2.exe link=static threading=multi

Currently, cmake uses an environment variable for the location of
boost.  Create/set an envionment variable BOOST_ROOT to have the path
to the boost source.
