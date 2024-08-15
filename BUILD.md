# Building srcML

srcML is built using cmake, www.cmake.org (version 3.28 or above), and currently supports builds for
macOS, Fedora, Ubuntu, OpenSUSE, and Windows Visual Studio.

Out-of-source builds (builds outside the source directory) are required, and in-source builds are not supported.

For full configuration, a set of presets for each platform, including ci-ubuntu, ci-rpm, macOS, and ci-msvc, should be used.

## Unix

To generate a makefile in your build directory:

```console
cmake <path_to_srcml> --preset <platform_preset>
```

For Linux, assuming a sibling build directory:

```console
cmake ../srcML --preset ci-linux
```

The following targets are supported with their usual meaning.

```console
make
make clean
make test
make install
```

Client tests are enabled by default, while libsrcml and parser tests are turned off by default. These tests can be turned on/off via the cmake command.

```console
cmake . -DBUILD_CLIENT_TESTS=OFF -DBUILD_LIBSRCML_TESTS=ON -DBUILD_PARSER_TESTS=ON
```

### macOS

The main packages required may be installed via brew:

```console
brew install cmake
```

Libarchive greater than 3.0.0 is required. For macOS previous to Catalina (19.*.*), libarchive.a 3.3.* must be statically included. Use brew to install a more recent version:

```console
brew install libarchive
```

To generate srcML documentation:

```console
brew install man2html doxygen
```

Additional packages that may not needed but are recommended (for timing, etc.):

```
brew install coreutils gnu-sed gnu-time
```

#### Linux

Linux builds for Ubuntu, Fedora, and OpenSUSE are supported.

To find what is needed, it is recommended to consult these dockerfiles for the particular distribution:

* [Ubuntu](./docker/ubuntu/Dockerfile)
* [Fedora](./docker/fedora/Dockerfile)
* [OpenSUSE](./docker/opensuse/Dockerfile)

Commands to install what is needed can be adapted from these.

## Windows Using MSVC

Necessary tools include:

* [Java JRE/JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [CMake](http://www.cmake.org)
* [Visual Studio 2017 or later](https://www.visualstudio.com/downloads/)

Building in Windows requires MSVC. Dependencies are handled via [vcpkg](vcpkg.io), see the [vcpkg installation directions](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd). As with the other platforms, srcml should use an out-of-source build.

First, generate the build files in your target build directory. Using the preset sets up the use of vcpkg:

```console
cmake [path to srcML source directory] --preset ci-msvc
```

Second, build. Doing so via cmake means you do not have to know the build program name or location:

```console
cmake --build . --config release
```

The directory `bin` in the build folder contains the srcML executable along with all other dependencies.
