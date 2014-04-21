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
# * Call function check to verify the command, e.g.,
#   check 3<<- 'STDOUT'
#     <unit/>
#     STDOUT
#
# * Optionally also specify expected stderr of command, e.g.,
#   check 3<<- 'STDOUT' 4<<- 'STDERR'
#     <unit/>
#     STDOUT
#     STDERR
#
# * If a comparison pipe is not open, then it assumes blank
#   I.e., the following check assumes that both stdout and stderr are empty
#   check
#
# * Instead of pipe 3 being the expected contents of stdout of the command, it can be file, i.e.
#   check foo.xml
#
# * Multiple tests of cli command followed by call to function check
#   can be made

# make sure to find the srcml executable
export PATH=$PATH:bin/

# always exit whenever a command or comparison fails
set -e

# turn history on so we can output the command issued
set -o history
HISTIGNORE=check
HISTSIZE=2

# output filenames for capturing stdout and stderr from the command
typeset STDERR=.stderr_$(basename $0)
typeset STDOUT=.stdout_$(basename $0)

# save stdout and stderr to our files
exec 5>&1 1>$STDOUT
exec 6>&2 2>$STDERR

define() { IFS= read -r -d '' ${1} || true; }

##
# checks the result of a command
#   $1 (optional) file of expected stdout
#   $2 (optional) file of expected stderr
#   $STDOUT - filename of captured stdout
#   $STDERR - filename of captured stderr
#
check() {

    # return stdout and stderr to standard streams
    exec 1>&5
    exec 2>&6

    # trace the command
    echo $(history | head -n 1 | cut -c 8-)

    # verify expected stderr to the captured stdout
    if [ $# -ge 1 ]; then
        # compare the parameter file to the expected output
        diff $STDOUT $1

    elif [ -e /dev/fd/3 ]; then
        # redirection using immediate here document (<<) adds newline
        diff $STDOUT <(perl -0 -pe 's/\n\n$/\n/m' /dev/fd/3)

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
    exec 5>&1 1>$STDOUT
    exec 6>&2 2>$STDERR
}
