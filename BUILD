Building srcML

srcML is built using cmake www.cmake.org (version 2.8.12 or above) and supports builds for OSX, Fedora, Ubuntu, Linux Mint, and Windows Visual Studio.

In source (builds within the source directory) and out of source builds (builds outside the source directory) are supported.
However, the use of out of source build are usually recommended.




Unix:

To generate a simple make file in the current directory run:

For in source builds:
cmake .

For out of source builds:
cmake path_to_srcml

The following make commands are supported with their usual meanint
make
make clean
make tests

Unix distro specifics:

OSX:

Packages Required:

antlr 2 
boost
cmake

# for srcML documentation
man2html
docbook
docbook2x

# additional packages that may not needed, but are recommended (for timing etc.)
coreutils
gnu-sed
gnu-time

# for dmg/pkg distruction
PackageMaker

OSX does not provide headers for libarchive visit http://www.libarchive.org and 
download 2.8.5 for headers that are close to the versions distributed on OSX.
The files needed are archive.h and archive_entry.h.

PackageMaker is part of the auxiliary tools (late July 2012 for Mountain Lion or February 2012 for Lion)
that are not distributed by default and needs to be gotten from the following URL:
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

# for srcML documentation
man2html
docbook2X

# for rpm generation
@development-tools
fedora-packager

# for mingw cross compilation
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

For Fedora and Windows Cross compiling a few addition things are needed:

First fnmatch.h needs copied over into mingw includes.

cp /usr/include/fnmatch.h /usr/i686-w64-mingw32/sys-root/mingw/include/

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

# for srcML documentation
man2html
docbook2X


Currently, Ubuntu versions before 14.04 (possibly before 13) do not provide a recent enough version of cmake. 
Visit http://www.cmake.org. To download install the lastest version of cmake.

boost also needs to be installed separately see 64-bit Linux and Ubuntu.


64-bit Linux and Ubuntu:

The boost libraries are statically compiled in, 64-bit machines do not provide adequate libraries for static compilation (require compilation with -fPIC)

Download boost at http://www.boost.org, the commands to build boost (written for boost 1.55.0) are:

./bootstrap.sh --without-libraries=atomic,chrono,context,coroutine,exception,graph,graph_parallel,iostreams,locale,log,math,mpi,python,random,serialization,signals,test,timer,wave
./b2 link=static cxxflags="-fPIC -static -Wl,--whole-archive" threading=multi install

Windows Visual Studio:
