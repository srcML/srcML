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
# * If a comparison pipe is not open, then it assumes blank
#   I.e., the following check assumes that both stdout and stderr are empty
#   check
#
# * Instead of pipe 3 being the expected contents of stdout of the command, it can be file
#   check foo.xml
#
# * Multiple tests of cli command followed by call to function check
#   can be made

#trap 'exec 1>&6 2>&7; OIFS="$IFS"; IFS=$'\n'; array=($(<commands)); IFS="$OIFS"; echo "HI"' EXIT
#trap 'exec 1>&6 2>&7; OIFS="$IFS"; IFS=$'\n'; array=($(<commands)); IFS="$OIFS"; echo "HI"; echo ${#array}; echo ${array[${#array[@]} - 1]}' EXIT

# always exit whenever a command or comparison fails
set -e
set -o history

# make sure to find the srcml executable
export PATH=$PATH:bin/

# output files for standard output and error from the command
STDERR=.stderr_$(basename $0)
STDOUT=.stdout_$(basename $0)

# save stdout and stderr to our files
exec 6>&1 1>$STDOUT
exec 7>&2 2>$STDERR

trap "echo $BASH_COMMAND >> commands " DEBUG

check() {

   # return stdout and stderr to standard streams
   exec 1>&6 2>&7

   # trace the command
   var=$(history 2 | head -1 | cut -c8-);
   echo "$var"

  # verify stdout of command (stdin to this check)
   if [ $# -eq 1 ]; then
       # compare the file whose filename was passed as a parameter to the required output
       diff $1 <(cat <&3)
   elif [ -e /dev/fd/3 ]; then
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
   exec 6>&1 1>$STDOUT
   exec 7>&2 2>$STDERR
}
