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

trap "rm -f $pstdout $pstderr" EXIT

# make sure to find the srcml executable
export PATH=$PATH:bin/

# always exit whenever a command or comparison fails
set -e

# turn history on so we can output the command issued
set -o history

# output filenames for capturing stdout and stderr from the command
typeset STDERR=.stderr_$(basename $0)
typeset STDOUT=.stdout_$(basename $0)

# save stdout and stderr to our files
exec 6>&1 1>$STDOUT
exec 7>&2 2>$STDERR

##
# checks the result of a command
#   $1 (optional) file of expected stdout
#   $2 (optional) file of expected stderr
#   $STDOUT - filename of captured stdout, file descriptor 6
#   $STDERR - filename of captured stderr, file descriptor 7
#
check() {

    # return stdout and stderr to standard streams
    exec 1>&6
    exec 2>&7

    # trace the command
    echo $(history 2 | head -1 | cut -c8-)

    # verify expected stderr to the captured stdout
    if [ $# -ge 1 ]; then
        # compare the parameter file to the expected output
        diff $STDOUT $1

    elif [ -e /dev/fd/3 ]; then
        diff $STDOUT /dev/fd/3

    else
        # check that the captured stdout is empty
        [ ! -s $STDOUT ]
    fi

    # verify expected stderr to the captured stderr
    if [ $# -ge 2 ]; then
        # compare the captured stderr to the required stderr
        diff $STDERR $2

    elif [ -e /dev/fd/4 ]; then
        diff $STDOUT /dev/fd/4

    else
        # check that the captured stderr is empty
        [ ! -s $STDERR ]
    fi

    # return to capturing stdout and stderr
    exec 6>&1 1>$STDOUT
    exec 7>&2 2>$STDERR
}
