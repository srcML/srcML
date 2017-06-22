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

# need -z option on GNU sed, not available on BSD sed
if [ -e /usr/local/bin/gsed ]
then
   SED="gsed"
else
   SED="sed"
fi

# close our stderr file descriptors
exec 4>&-

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
    SRC2SRCML='../../../bin/srcml'
fi

if [ -z "$SRCML2SRC" ]; then
    SRCML2SRC='../../../bin/srcml'
fi

if [ -z "$SRCML"]; then
    SRCML='../../../bin/srcml'
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
# note that the fc command accesses the history
set -o history
export HISTIGNORE=check:\#
HISTSIZE=2

# output the first entry in the history file, without numbers
firsthistoryentry() {
    fc -l -n -1
}

CAPTURE_STDOUT=true
CAPTURE_STDERR=true

# variable $1 is set to the contents of stdin
define() {

    # read stdin into variable $1
    IFS= read -r -d '' $1 || true
 
    # replace any mention of REVISION with the revision number,
    eval $1=\${$1//REVISION/${REVISION}}
}

# variable $1 is set to the contents of file $2
readfile() { ${1}="$(< $2)"; }

# file with name $1 is created from the contents of string variable $2
# created files are recorded so that cleanup can occur
createfile() {
    # make directory paths as needed
    mkdir -p $(dirname $1)

    # add contents to file
    echo -ne "${2}" > ${1}

    # register to cleanup
    registerfile ${1}
}

rmfile() { rm -f ${1}; }

# capture stdout and stderr
capture_output() {
    [ "$CAPTURE_STDOUT" = true ] && exec 5>&1 1>$STDOUT
    [ "$CAPTURE_STDERR" = true ] && exec 6>&2 2>$STDERR
}

# uncapture stdout and stderr
uncapture_output() {
    [ "$CAPTURE_STDOUT" = true ] && exec 1>&5
    [ "$CAPTURE_STDERR" = true ] && exec 2>&6
}

message() {
    # return stdout and stderr to standard streams
    uncapture_output

    # trace the command
    echo "$1" >&2

    capture_output

    true
}

compare_file_expected() {

    diff $1 <($SED -z -e 's/\n\n$/\n/m' $2)
}

# output filenames for capturing stdout and stderr from the command
base=$(basename $0 .sh)
typeset STDERR=.stderr_$base
typeset STDOUT=.stdout_$base

# save stdout and stderr to our files
capture_output

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
    uncapture_output

    # trace the command
    firsthistoryentry

    # verify expected stdout to the captured stdout
    if [ $# -ge 1 ]; then

        # register to cleanup
        registerfile ${1}

        # compare the parameter file to the expected output
        compare_file_expected $1 /dev/fd/3

    elif [ -e /dev/fd/3 ]; then
        # redirection using immediate here document (<<) adds newline
        diff $STDOUT <($SED -z -e 's/\n\n$/\n/m;' -e "s/REVISION/${REVISION}/;" /dev/fd/3)

    else
        # check that the captured stdout is empty
        [ ! -s $STDOUT ]
    fi

    # verify expected stderr to the captured stderr
    if [ $# -ge 2 ]; then
        # compare the captured stderr to the required stderr
        compare_file_expected $2 /dev/fd/4

    elif [ -e /dev/fd/4 ]; then
        compare_file_expected $STDERR /dev/fd/4

    else
        # check that the captured stderr is empty
        [ ! -s $STDERR ]
    fi

    # close our stderr file descriptors
    exec 4>&-

    # return to capturing stdout and stderr
    capture_output

    true
}

##
# checks the result of a command
#
# If stdout is not specified, it is assumed to be empty
# If stderr is not specified, it is assumed to be empty
checkv2() {

    # return stdout and stderr to standard streams
    uncapture_output

    # trace the command
    firsthistoryentry

    # check <filename> stdoutstr stderrstr
    if [ $# -ge 3 ]; then

        registerfile ${1}
        diff $1 <(echo -n "$2")
        diff $STDERR <(echo -n "$3")

    # check <filename> stdoutstr
    # note: empty string reports as a valid file
    elif [ $# -ge 2 ] && [ "$1" != "" ] && [ -e "$1" ]; then

        registerfile ${1}
        diff $1 <(echo -n "$2")
        [ ! -s $STDERR ]

    # check stdoutstr stderrstr
    elif [ $# -ge 2 ]; then

        diff $STDOUT <(echo -n "$1")
        diff $STDERR <(echo -n "$2")

    # check <filename>
    elif [ $# -ge 1 ]; then
        diff $STDOUT <(echo -n "$1" | sed -e "s/REVISION/${REVISION}/;")
    else
        # check that the captured stdout is empty
        [ ! -s $STDOUT ]
    fi

    # return to capturing stdout and stderr
    capture_output

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
    uncapture_output

    # trace the command
    firsthistoryentry

    # verify expected stderr to the captured stdout

    # check that the captured stdout is empty
    [ ! -s $STDOUT ]

    # check that the captured stderr is empty
    [ ! -s $STDERR ]

    # # return to capturing stdout and stderr
    capture_output

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
    uncapture_output

    # trace the command
    firsthistoryentry

    # verify expected stderr to the captured stdout
    if [ $exit_status -ne $1 ]; then
        exit 8
    fi

    # return to capturing stdout and stderr
    capture_output

    true
}

# Check the validity of the xml
# Currently only checks for well-formed xml, not DTD validity
xmlcheck() {

    if [ "${1:0:1}" != "<" ]; then
        xmllint --noout ${1}
    else
        echo "${1}" | xmllint --noout /dev/stdin
    fi;

    true
}
