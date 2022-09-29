# Building srcML

[cmake]: https://cmake.org

srcML is built using [cmake] version 3.24 or above and currently supports
builds for macOS, Fedora, Ubuntu, CentOS, OpenSUSE, and Windows MSVC.

Only out-of-source builds (i.e., shadow builds) are supported. All cmake and build commands are assumed to be entered in the build directory.

While `make` is shown in the following commands, we use and recommend `ninja` as it more easily takes advantage of parallelism.

## macOS & Linux Commands

To generate a Makefile:

```console
cmake <path_to_srcml>
```

The following build commands are supported with their usual meaning:

```console
make
make clean
make test
make install
```

Note that the build does not strip the generated executable and libraries by default. To do so when installed:

```console
make install/strip
```

Starting with macOS Big Sur, the build creates a universal build with both architectures, x86 and Apple Silicon
(m1, m2) by default, unless the variable `CMAKE_OSX_ARCHITECTURES` is set on the cmake command.

To explicitly build on macOS only for x86:

```console
cmake . -DCMAKE_OSX_ARCHITECTURES="x86_64"
```

To explicitly build on macOS only for arm64:

```console
cmake . -DCMAKE_OSX_ARCHITECTURES="arm64"
```

## macOS Dependencies
[Homebrew]: https://brew.sh

The main packages required may be installed via [Homebrew]:

```console
brew install cmake boost ninja
```

Note that `ninja` is optional.

See the `vcpkg` instructions below if you alternatively want to use `vcpkg` to install and manage any dependencies.

To optionally generate srcML documentation:

```console
brew install man2html doxygen
```

Additional packages that are not required but are recommended (for timing, etc.):

```console
brew install coreutils gnu-sed gnu-time
```

* Libarchive greater than 3.0.0 is required. For macOS older than macOS Catalina (19.*.*), libarchive.a 3.3.* must be statically included. Use `brew install libarchive` to install a more recent version

## Linux Dependencies

[Ubuntu]: https://github.com/srcML/Docker/blob/ubuntu_latest/base/Dockerfile
[Fedora]: https://github.com/srcML/Docker/blob/fedora_latest/base/Dockerfile
[CentOS]: https://github.com/srcML/Docker/blob/centos_latest/base/Dockerfile
[OpenSUSE]: https://github.com/srcML/Docker/blob/opensuse_latest/base/Dockerfile

Linux builds for multiple distributions are supported. To find what is needed, it is recommended to consult these dockerfiles for the particular distribution: [Ubuntu], [Fedora], [CentOS], [OpenSUSE].

Commands to install dependencies can be adapted from these, and they are tested to work. You will also find dockerfiles for older versions of these distributions.

In most cases, Linux distributions may not have a recent version of cmake. It is highly recommended to [download a more recent version](https://cmake.org/download/), or if on Ubuntu, use the [Kitware APT Repository](https://apt.kitware.com).

Note that you can alternatively use `vcpkg` to install and manage dependencies on Linux.

## Windows Using MSVC and vcpkg

Building in Windows can use MSVC. Visual Studio 2017 or newer for 64-bit binaries.

Building in Windows handles dependencies using `vcpkg`. See the instructions below.

## vcpkg

[vcpkg Get Started]: https://vcpkg.io/en/getting-started.html
[vcpkg]: https://vcpkg.io/en/index.html

Building with [vcpkg] is a requirement to build on Windows but can also be used on other platforms. The manifest file *vcpk.json* in the main source directory lists the required dependencies. To build with vcpkg:

* Clone a copy of the vcpkg repository

```console
git clone https://github.com/Microsoft/vcpkg.git
```

* Initialize vcpkg inside the vcpkg clone:

```console
./vcpkg/bootstrap-vcpkg.sh
```

* Run cmake indicating vcpkg in the build directory:

```console
cmake ../srcML -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

The path assumes that vcpkg is a sibling directory of the build directory. If not, adjust the
path.

This may take a while the first time as it has to download and build the dependencies.

* Run the build:

```console
cmake --build . --config Release
```

You can find more detail about the vcpkg instructions at [vcpkg Get Started]. Note that using vcpkg with cmake automatically installs the needed dependencies.

## Build Hints

[unity build]: https://cmake.org/cmake/help/latest/prop_tgt/UNITY_BUILD.html

* With a complete build, a [unity build] significantly speeds up the process, e.g., 25% reduction. Specify `-DCMAKE_UNITY_BUILD=ON` in your cmake command.
  To turn on in an existing build:

```console
cmake . -DCMAKE_UNITY_BUILD=ON
```

## Testing

There are multiple types of tests for srcML:

* Client Tests - test the command line client
* libsrcml Tests - test the libsrcml API
* Parser Tests - test the srcML parser

### Client Tests

The client unit tests are shell scripts that run the command-line client and check the output and exit value. They are always available (i.e., no setup is needed) and are run via `ctest` in the build directory:

```console
ctest
```

The client tests can be run in parallel using the option `-j`, which speeds up the testing as certain tests fetch remote URLs

### libsrcml Tests

The libsrcml tests are standard unit tests that call the libsrcml API and check the results. They are not enabled by default. To enable them:

```console
cmake . -DBUILD_LIBSRCML_TESTS=ON
```

Then build accordingly.

The tests are run via `ctest` and can be run as part of the client tests. To run just the libsrcml tests:

```console
ctest -R ^test_
```

### Parser Tests

The parser tests are srcml archives, where each individual unit is a unit test. They are disabled by default. To enable the parser tests:

```console
cmake . -DBUILD_PARSER_TESTS=ON
```

Once enabled, they must be generated via a target:

```console
make gen_parser_tests
```

To run the parser tests:

```console
make run_parser_tests
```
