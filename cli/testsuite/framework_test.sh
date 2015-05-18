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
#
# * Put in a trap for cleanup in test file:
#   trap { cleanup; } EXIT

# generated files, list is kept to cleanup
genfiles=""

# current revision number, replaced in expected output strings
export REVISION=0.9.5

# restores environment, deletes files created with createfile command.
# or registerd with registerfile command
cleanup() {
    # remove createfile files, and registerfile files
    rm -f $genfiles

    genfiles=""
}

# registers a file so that it will be cleaned up
# does not create it
registerfile() {

    # append to our list of files
    genfiles="$genfiles  "${1}
}

trap "{ cleanup; }" EXIT

# make sure to find the srcml executable
export PATH=.:$PATH

echo "$SRC2SRCML"

if [ -z "$SRC2SRCML" ]; then
    SRC2SRCML='../../bin/srcml'
fi

if [ -z "$SRCML2SRC" ]; then
    SRCML2SRC='../../bin/srcml'
fi

if [ -z "$SRCML"]; then
    SRCML='../../bin/srcml'
fi

function src2srcml () {
    $SRC2SRCML "$@"
}

function srcml2src () {
    $SRCML2SRC "$@"
}

function srcml () {
    $SRCML "$@"
}

# always exit when a command exits with a non-zero status
set -e

# turn history on so we can output the command issued
set -o history
export HISTIGNORE=check:\#
HISTSIZE=2

CAPTURE_STDOUT=true
CAPTURE_STDERR=true

# variable $1 is set to the contents of stdin
define() {
    IFS= read -r -d '' ${1} || true;

    # temporarily store the contents of the variable named in $1
    CONTENT=${!1}

    # replace any mention of REVISION with the revision number,
    # and put the revised contents of the variable named in $1
    eval $1=\${CONTENT//REVISION/${REVISION}}
}

# variable $1 is set to the contents of file $2
readfile() { ${1}="$(cat $2)"; }

# file with name $1 is created from the contents of string variable $2
# created files are recorded so that cleanup can occur
createfile() {
    # make directory paths as needed
    if [ ! -d $(dirname $1) ]; then
        mkdir -p $(dirname $1)
    fi

    # add contents to file
    echo -ne "${2}" > ${1};

    # register to cleanup
    registerfile ${1}
}

rmfile() { rm -f ${1}; }

message() {
    # return stdout and stderr to standard streams
    [ "$CAPTURE_STDOUT" = true ] && exec 1>&5
    [ "$CAPTURE_STDERR" = true ] && exec 2>&6

    # trace the command
    echo "$1" >&2

    [ "$CAPTURE_STDOUT" = true ] && exec 5>&1 1>$STDOUT
    [ "$CAPTURE_STDERR" = true ] && exec 6>&2 2>$STDERR

    true
}

# output filenames for capturing stdout and stderr from the command
typeset STDERR=.stderr_$(basename $0 .sh)
typeset STDOUT=.stdout_$(basename $0 .sh)

# save stdout and stderr to our files
[ "$CAPTURE_STDOUT" = true ] && exec 5>&1 1>$STDOUT
[ "$CAPTURE_STDERR" = true ] && exec 6>&2 2>$STDERR

##
# checks the result of a command
#   $1 (optional) file of expected stdout
#   $2 (optional) file of expected stderr
#   $STDOUT - filename of captured stdout
#   $STDERR - filename of captured stderr
#
# If stdout is not specified, it is assumed to be empty
# If stderr is not specified, it is assumed to be empty
check() {

    # return stdout and stderr to standard streams
    [ "$CAPTURE_STDOUT" = true ] && exec 1>&5
    [ "$CAPTURE_STDERR" = true ] && exec 2>&6

    # trace the command
    echo $(history | head -n 1 | cut -c 8-)

    # verify expected stderr to the captured stdout
    if [ $# -ge 1 ]; then

        # register to cleanup
        registerfile ${1}

        # compare the parameter file to the expected output
        diff $1 <(perl -0 -pe 's/\n\n$/\n/m' /dev/fd/3)

    elif [ -e /dev/fd/3 ]; then
        # redirection using immediate here document (<<) adds newline
        diff $STDOUT <(perl -0 -pe 's/\n\n$/\n/m;' -pe "s/REVISION/${REVISION}/;" /dev/fd/3)

    else
        # check that the captured stdout is empty
        [ ! -s $STDOUT ]
    fi

    # verify expected stderr to the captured stderr
    if [ $# -ge 2 ]; then
        # compare the captured stderr to the required stderr
        diff $2 <(perl -0 -pe 's/\n\n$/\n/m' /dev/fd/4)

    elif [ -e /dev/fd/4 ]; then
        diff $STDERR <(perl -0 -pe 's/\n\n$/\n/m' /dev/fd/4)

    else
        # check that the captured stderr is empty
        [ ! -s $STDERR ]
    fi

    # # return to capturing stdout and stderr
    [ "$CAPTURE_STDOUT" = true ] && exec 5>&1 1>$STDOUT
    [ "$CAPTURE_STDERR" = true ] && exec 6>&2 2>$STDERR

    true
}
##
# checks the result of a command to verify that it is empty
#   $1 (optional) file of expected stdout
#   $2 (optional) file of expected stderr
#   $STDOUT - filename of captured stdout
#   $STDERR - filename of captured stderr
#
check_null() {

    # return stdout and stderr to standard streams
    [ "$CAPTURE_STDOUT" = true ] && exec 1>&5
    [ "$CAPTURE_STDERR" = true ] && exec 2>&6

    # trace the command
    echo $(history | head -n 1 | cut -c 8-)

    # verify expected stderr to the captured stdout

    # check that the captured stdout is empty
    [ ! -s $STDOUT ]

    # check that the captured stderr is empty
    [ ! -s $STDERR ]

    # # return to capturing stdout and stderr
    [ "$CAPTURE_STDOUT" = true ] && exec 5>&1 1>$STDOUT
    [ "$CAPTURE_STDERR" = true ] && exec 6>&2 2>$STDERR

    true
}

##
# checks the exit status of a command
#   $1 expected return value
#
# NOTE: Requires the following in test file header:
#   set +e
check_exit() {

    local exit_status=$?

    # return stdout and stderr to standard streams
    [ "$CAPTURE_STDOUT" = true ] && exec 1>&5
    [ "$CAPTURE_STDERR" = true ] && exec 2>&6

    # trace the command
    echo $(history | head -n 1 | cut -c 8-)

    # verify expected stderr to the captured stdout
    if [ $exit_status -ne $1 ]; then
        exit 8
    fi

    # # return to capturing stdout and stderr
    [ "$CAPTURE_STDOUT" = true ] && exec 5>&1 1>$STDOUT
    [ "$CAPTURE_STDERR" = true ] && exec 6>&2 2>$STDERR

    true
}
