##
# framework_test.sh
#
# Test framework for cli testing
#
# * Always source this file before any test cli commands are given
#   source $(dirname "$0")/framework_test.sh
#
# * Perform cli command
#
# * Call function check to verify the command, in the form:
#   check 3<< 'STDOUT'
#   <unit/>
#   STDOUT
#
# * Optionally also give expected stderr of command
#   check 3<< 'STDOUT' 4<< 'STDERR'
#   <unit/>
#   STDOUT
#   STDERR
#
# * Multiple tests of cli command followed by call to function check
#   can be made

# always exit whenever a command or comparison fails
set -e

# make sure to find the srcml executable
export PATH=$PATH:bin/

# output files for standard output and error from the command
STDERR=.stderr_$(basename $0)
STDOUT=.stdout_$(basename $0)

# save stdout and stderr to our files
exec 6>&1 7>&2 1>$STDOUT 2>$STDERR

check() {

   # return stdout and stderr to standard streams
   exec 1>&6 2>&7

   # verify stdout of command (stdin to this check)
   if [ $# -eq 1 ]; then
       # compare the file whose filename was passed as a parameter to the required output
       diff $1 <(cat <&3)
   else
       # compare the captured stdout to the required output
       diff $STDOUT <(cat <&3)
   fi

   # verify stderr of command agrees with caught stderr
   if [ -e /dev/fd/4 ]; then
       # compare the captured stderr to the required stderr
       diff $STDERR <(cat <&4)
   else
       # make sure the captured stderr is blank
       [ ! -s $STDERR ]
   fi

   # return to saving stdout and stderr to our files
   exec 6>&- 7>&- 6>&1 7>&2 1>$STDOUT 2>$STDERR
}
