# CLI Test Scripts

This directory contains CLI test scripts. They are written in bash, mostly using a
set of special commands, defined in framework_test.sh:

* check - main function to check results of the command, given the expected stdout and stderr as strings
* check_exit - checks the return exit value, mostly for error routines
* define - create a variable from a string of literal source code or srcML
* readfile - read the contents of a file into a variable
* createfile - creates a file from a string
* rmfile - deletes a file
* message - debugging messages
* xmlcheck - check the validity of XML in a file or a string

## Creating a test script
The example test cli_test_example.sh is provided as a starting point

* Make sure to include framework_test.sh
* Each script should be executable, chmod u+x cli_test_example.sh

## Directories

Test cases are run in the subdirectory tmp of this directory. The files created remain after the script is run, but removed before the script starts again. This is useful for debugging the commands. The entire tmp directory can be deleted with no harm.

## Policy

* Each cli test file should test one option/command
* Variations of options (e.g., short vs long) are done in the same test

## CLI naming

All of the tests run srcml. However, they are named as below to indicate purpose:

* *src2srcml* - conversion from source code to srcML, similar to previous cli tool src2srcml
* *srcml2src* - conversion from srcML to source code, or actions on srcML, similar to previous cli tool srcml2src
* *srcml* - features only available in new client, typically new options, new option names, and combinations of src2srcml and srcml2src features in new client
