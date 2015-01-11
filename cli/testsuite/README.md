# CLI Test Scripts

This directory contains CLI test scripts. They are written in bash, mostly using a
set of special commands, defined in framework_test.sh:

* check
* check_exit
* createfile
* cleanup

## Creating a test script
The example test 00_cli_test_example.sh is provided as a starting point

* Prefix with a number value for ordering purposes
* Include framework_test.sh
* Put in an exit trap for cleanup

## Policy
* Each cli test file should test one option/command
* Variations of options (e.g., short vs long) are done in the same test

## CLI naming

* *src2srcml* - conversion from source code to srcML, similar to previous cli tool src2srcml
* *srcml2src* - conversion from srcML to source code, or actions on srcML, similar to previous cli tool srcml2src
* *srcml* - features only available in new client, typically new options, new option names, and combinations of src2srcml and srcml2src features in new client