Large Systems Test
==================

Executes srcML on a list of large systems, and for each system records:
* how long it takes for srcML to execute on the system when it is in a
compressed and uncompressed format
* the size of original compressed file, uncompressed directory, and output
srcML files
* the output of a  `cmp` comparison of the srcML output files when run on
compressed and uncompressed inputs

The records for time, diff, and size are stored in timestamped logs under the
build directory in `cli/large_systems_test/logs`. It can be run via
CMake/CTest, but more options are available when executing straight from bash.


### Build

The `BUILD_LARGE_SYSTEMS_TESTS` option from `CMake/config.cmake` must be set
to `ON`. After running CMake, build **and install** srcML.


### Run

After building, run via `ctest`, `make test`, or from bash with
`./large_systems.sh`. The benefit of running from bash is that arguments can
be specified to pick which system(s) to execute the test on (see the next
section). The list of all systems that are used are in a CSV file in
`cli/large_systems_test/large_systems_list.csv`. Each line in the file
contains information for a system on which srcML will execute. This
information includes: 1) where to get the tar for that system, 2) the name of
the system, and 3) the main language it's in.

The source code for each system is downloaded with curl and cached locally in
order to make executing the test faster when run consecutively.


### Output

The resulting output of the test is stored in three timestamped files: a time
log, size log, and diff log. Each log contains the command & arguments that
were executed, and the result of the command (`time`, `du -hs`, or `cmp`).


### Filtering systems

When executed through bash, tests can be filtered based on name or language by
passing strings to match against any of the 3 parameters listed in the CSV
file. The test will execute on a system if it matches any one of the given
arguments. With no arguments, the test will exceute on all systems listed in
the file. It takes ~30 minutes to run on 6 systems.

For example, to run the test only on the Linux kernel source code, use:
```
$ ./large_systems.sh linux
```

To run the test only on systems whose main language is C:
```
$ ./large_systems.sh language=C
```

More than one argument can be used to match multiple strings. The following
example will run the test on Linux, Curl, and AFNetworking:
```
$ ./large_systems.sh linux curl afnetwork
```
Notice that the argument string doesn't have to match the full name, so
`afnetwork` matches `AFNetworking`.

Filtering by project name can be combined with filtering by language. For
example, if you want to run the test on Linux as well as any system whose main
language is Objective-C, use:
```
$ ./large_systems.sh linux language=Objective-C
```


### Notes

The order of arguments doesn't matter and case is ignored.

For specifying the language, it must be in the form `language=<language>`,
without spaces before or after the `=` because it's stored in the CSV file in
that way.
