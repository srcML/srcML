Large Systems Test
==================

Executes srcML on a list of large systems, and for each system records:
* how long it takes for srcML to execute on the system when it is in a
compressed and uncompressed format
* the size of original compressed file, uncompressed directory, and output
srcML files
* the output of a `cmp` comparison of the srcML output files when run on
compressed and uncompressed inputs

The records for time, diff, and size are stored in logs under the
build directory in `cli/large_systems_test/logs`. It can be run via `ctest`.


### Build

The `BUILD_LARGE_SYSTEMS_TESTS` option from `CMake/config.cmake` must be set
to `ON`. After running CMake, build **and install** srcML.


### Run

After building, run via `ctest` or `make test`. The list of all systems
that are used are in a CSV file in
`cli/large_systems_test/large_systems_list.csv`. Each line in the file
contains information for a system on which srcML will execute. This
information includes: 1) where to get the tar for that system, 2) the name of
the system, and 3) the main language it's in.

The source code for each system is downloaded with curl and cached locally in
order to make executing the test faster when run consecutively.


### Output

The resulting output of the test is stored in the log/ directory in three files: a time
log, size log, and diff log. Each log contains the command & arguments that
were executed, and the result of the command (`time`, `du -hs`, or `cmp`).
Executing the test multiple times will only append to these files. The timestamp
of when the test was executed is saved in the file before any output is
generated.

A local cache is kept in the cache/ directory of the original tar files for
each system.

A temporary directory, output/, keeps srcML output from the test. All contents
of this directory are removed after each test.


### Filtering systems

Only one or some systems can be tested by using the ctest command to filter
tests based on name. For example, to only run the linux tests, use:
```
$ ctest -R linux
```


### Max threads

The `max-threads` option can optionally be specified via
`cli/large_systems_test/CMakeLists.txt`. The `MAX_THREADS` variable matches
the command that srcML receives. By default, it is set to 8. It can be commented
out, or the number of threads changed, then re-run with CTest.

To see the changes as they were made in CMakeLists.txt, *re-run `make install`
and `ctest`*. CMake itself doesn't need to be reconfigured.
