# Building srcML

The srcML build uses cmake, www.cmake.org (version 3.28 or above), and supports
builds for Ubuntu, Fedora, OpenSUSE, macOS, and Windows. The builds are "out-of-source builds" (builds outside the source directory) with no support for in-source builds.

For proper configuration, use the cmake preset for your platform: `ci-ubuntu`, `ci-rpm`, `ci-macos`, and `ci-msvc`, as described below.

* [Linux](#linux)
* [macOS](#macos)
* [Windows](#windows)
* [Workflow Presets](#workflow-presets)
* [Internal Dependencies](#internal-dependencies)
* [Options](#options)
	* [Using Make](#using-cmake)
	* [Custom build directory](#custom-build-directory)
	* [Non-preset build](#non-preset-build)

## Linux

srcML supports Linux builds for Ubuntu, Fedora, and OpenSUSE.

### Dependencies

To find what installed packages are needed, consult these Dockerfiles for
each distribution:

* [Ubuntu](./docker/ubuntu/Dockerfile)
* [Fedora](./docker/fedora/Dockerfile)
* [OpenSUSE](./docker/opensuse/Dockerfile)

### Build

To build on Ubuntu, from the source directory use the preset:

```console
cmake --preset ci-ubuntu
```

To build on Fedora or OpenSUSE, from the source directory use the preset:

```console
cmake --preset ci-rpm
```

This creates a sibling build directory, `../srcML-build`.

The presets' default generator is [ninja](https://ninja-build.org). To use `make`, see [Options/Using Make]().

The following targets are supported with their usual meaning.

```console
ninja
ninja clean
ninja install
```

The workflow preset allows you to build, package, install the package, and test the client, the parser, and the libsrcml library of the installed package. On Ubuntu:

```console
cmake --workflow --preset ci-ubuntu
```

and on Fedora and OpenSUSE:

```console
cmake --workflow --preset ci-rpm
```

## macOS

External package dependencies are handled via [brew](https://brew.sh). Install brew first.

### Dependencies

It is assumed that g++ is installed via XCode or the XCode command line tools when you install brew.

The other packages required may be installed via brew:

```console
brew install cmake
```

To generate srcML documentation:

```console
brew install man2html doxygen
```

Additional packages that may not needed but are recommended (for timing, etc.):

```console
brew install coreutils gnu-sed gnu-time
```

On recent macOS versions (Catalina and later), the system-installed libarchive library is sufficient. The cmake configuration downloads the matching include files (`archive.h` and `archive-entry.h`) for the system-installed library. However, for previous versions of macOS, libarchive.a 3.3.* must be statically linked.

### Build

To build on macOS, from the source directory use the preset:

```console
cmake --preset ci-ubuntu
```

This creates a sibling build directory, `../srcML-build`.

The workflow preset allows you to build, package, install the package, and test the client, the parser, and the libsrcml library of the installed package. On macOS:

```console
cmake --workflow --preset ci-macos
```

## Windows

Windows is configured for a build on MSVC. Visual Studio 2017 or later is required.

### Dependencies

Necessary installed tools include:

* [Java JRE/JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [CMake](http://www.cmake.org)
* [Visual Studio 2017 or later](https://www.visualstudio.com/downloads/)

Package dependencies are handled via [vcpkg](vcpkg.io); see the [vcpkg installation directions](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd).

### Build

Generate the build files. Using the preset sets up the use of `vcpkg`:

```console
cmake --preset ci-msvc
```

By default, the created build directory is in the subdirectory `build` of the source directory.

Second, build. Doing so via cmake means you do not have to know the build program name or location:

```console
cmake --build . --config release
```

The directory `bin` in the build folder contains the srcML executable.

The workflow preset allows you to build, package, install the package, and test the client, the parser, and the libsrcml library of the installed package. On Windows:

```console
cmake --workflow --preset ci-msvc
```

## Workflow Presets

The previous discussions included how to run the workflow presets. This is a complete workflow that in one command:

* Configures CMake
* Builds
* Create packages
* Install packages
* Tests the client
* Tests libsrcml
* Tests the client

All tests include a log file. For more information on these files, see the package and test directories.

One limitation of a workflow preset is that, unlike a configure preset, it does not allow you to specify (using the `-B` option) a build directory.

## Internal Dependencies

srcML has a set of internal dependencies. These are handled automatically and do not require any external package installation:

* CLI11 - Used in the client. Include file downloaded during configuration.
* ANTLR - To generate the parser. Source code downloaded during configuration.

## Options

### Using Make

To use `make` instead of `ninja`, explicitly specify the generator when using the preset:

```console
cmake --preset ci-ubuntu -G "Unix Makefiles"
```

Replace `ninja` with `make` in the instructions above.

### Custom build directory

CMake presets must specify the build directory and are meant to be issued from the source directory. The defaults for the above platforms are in a sibling directory, `../srcML-build`, or a subdirectory, `build`.

To use a custom build directory, create the build directory. Then, use the option `-B` to specify the build directory. For example, on Ubuntu with a source directory of `~/srcML` and a custom build directory `~/Build`:

```console
cmake ~/srcML --preset ci-ubuntu -B ~/Build
```

### Non-Preset Build

The cmake files are self-sufficient, and you can build without any presets. However, not using the presets may result in compiler warnings, non-optimized libraries, and improperly configured installation packages.

On Linux and macOS in your created build directory, include the path to your source directory:

```console
cmake /path/to/srcML
```

Now you can build using `make`.

The Windows build requires vcpkg:

```console
export VCPKG_ROOT=/path/to/vcpkg
cmake /path/to/srcML ~/srcML -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

In your build directory, perform the build via cmake so you don't have to specify the build tool:

```console
cmake --build . --config release
```



