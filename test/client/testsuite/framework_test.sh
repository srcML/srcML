#!/bin/bash
# framework_test.sh
#
# Test framework for CLI testing
#
# * Always source this file before any test CLI commands are given:
#   source $(dirname "$0")/framework_test.sh
#
# * Perform CLI command
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
# * If a comparison pipe is not open, then it assumes blank.
#   I.e., the following check assumes that both stdout and stderr are empty:
#   check
#
# * Instead of pipe 3 being the expected contents of stdout of the command, it can be a file, i.e.:
#   check foo.xml
#
# * Multiple tests of CLI command followed by call to function check
#   can be made.

# current revision number, replaced in expected output strings
export REVISION=1.0.0

# construct a temporary directory name based on the test name (without the .sh)
ORIG_PWD="$PWD"
TEMPDIR="./tmp/$(basename "$0" .sh)"

# print all errors if the script crashes
trap 'if [ $? -ne 0 ] && [ -f "$STDERR" ]; then echo "!!! SCRIPT CRASHED. STDERR:"; cat "$STDERR"; fi' EXIT

# remove old TEMPDIR, and create new fresh one
rm -rf "$TEMPDIR"
mkdir -p "$TEMPDIR"
cd "$TEMPDIR"

# make sure to find the srcml executable; if majority of tests are failing, this is probably the problem
export PATH=".:$PATH"
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    echo "DEBUG: Configuring for MSYS/Windows" >&2
    EOL="\r\n"
    export MSYS2_ARG_CONV_EXCL="*"
    diff='diff -Z --strip-trailing-cr '

    # PREFERRED: Use the exact path provided by CMake
    if [ -n "$SRCML_EXE" ]; then
        SRCML="$SRCML_EXE"
        echo "DEBUG: Using cmake provided srcml: '$SRCML'" >&2
    # Fallback: Check system PATH
    elif command -v srcml >/dev/null 2>&1; then
        SRCML=$(command -v srcml)
        echo "DEBUG: Found srcml in PATH at '$SRCML'" >&2
    # Last Resort: Hardcoded home
    else
        SRCML="$SRCML_HOME/srcml.exe"
        echo "DEBUG: Fallback to SRCML_HOME: '$SRCML'" >&2
    fi
else
    echo "DEBUG: Configuring for Unix/Linux" >&2
    EOL="\n"
    diff='diff --strip-trailing-cr '
    if [ -z "$SRCML" ]; then
        if [ -e "/usr/bin/srcml" ]; then
            SRCML='/usr/bin/srcml'
        elif [ -e "/usr/local/bin/srcml" ]; then
            SRCML='/usr/local/bin/srcml'
        elif command -v srcml >/dev/null 2>&1; then
            SRCML=$(command -v srcml)
        elif [ -x "$ORIG_PWD/../../.."/bin/srcml ]; then
            SRCML="$ORIG_PWD/../../.."/bin/srcml
        fi
    fi
fi

echo "DEBUG: Final SRCML command set to: '$SRCML'" >&2

# Health Check Function
check_srcml_health() {
    # Only run this check once
    if [ -n "$SRCML_HEALTH_CHECKED" ]; then
        return
    fi
    export SRCML_HEALTH_CHECKED=1

    echo "=== DEBUG: srcML Health Check ===" >&2
    echo "Executable path: $SRCML" >&2

    if [ ! -f "$SRCML" ]; then
        echo "❌ CRITICAL ERROR: srcML executable not found at '$SRCML'" >&2
        ls -l "$(dirname "$SRCML")" >&2
        exit 1
    fi

    # Check for missing DLLs on Windows (using ldd)
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        if command -v ldd > /dev/null; then
            # Filter output for "not found"
            ldd "$SRCML" | grep "not found" && echo "❌ MISSING DLL DETECTED via ldd" >&2
        fi
    fi

    # Dry Run - catches immediate crashes
    echo "Attempting to run: $SRCML --version" >&2
    "$SRCML" --version > /dev/null 2> health_check.err
    local exit_code=$?

    if [ $exit_code -ne 0 ]; then
        echo "CRITICAL ERROR: srcML crashed immediately! Exit code: $exit_code" >&2
        echo "Possible causes: Missing DLLs (libarchive, libxml2) or architecture mismatch." >&2
        echo "Stderr from health check:" >&2
        cat health_check.err >&2

        # On failure, dump full dependencies
        if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
             command -v ldd >/dev/null && ldd "$SRCML" >&2
        fi
        rm -f health_check.err
        exit 1
    else
        echo "srcML started successfully." >&2
        rm -f health_check.err
    fi
    echo "=================================" >&2
}

# Run the health check once SRCML path is set
check_srcml_health

srcml() {
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]] && [[ ! -t 0 ]]; then
        # Capture stdin to a temporary file if it's not a terminal (Windows compatibility)
        local temp_input
        temp_input=$(mktemp)
        cat > "$temp_input"
        "$SRCML" "$@" < "$temp_input"
        local status=$?
        rm -f "$temp_input"
        return $status
    fi
    "$SRCML" "$@"
}

# Function to normalize paths in XML output specifically for Windows
normalize_output() {
    if [[ "$OSTYPE" == "fakeos" ]]; then
        # Convert backslashes to forward slashes in filename attributes
        sed -i 's|filename="\(.*\)\\"|filename="\1/"|g' "$1" 2>/dev/null || true
    fi
}

# turn history on so we can output the command issued
# note that the fc command accesses the history
set -o history
HISTIGNORE="check:#"
HISTSIZE=2
HISTFILESIZE=0

# output the last entry in the history file, without numbers
firsthistoryentry() {
    fc -l -n -1
}

CAPTURE_STDOUT=true
CAPTURE_STDERR=true

# variable $1 is set to the contents of stdin
define() {
    # read stdin into variable $1
    IFS= read -r -d '' "$1" || true

    # replace any mention of REVISION with the revision number
    eval "$1=\"\${$1//REVISION/${REVISION}}\""
}

# variable $1 is set to the contents of stdin and checked for well-formed XML
defineXML() {
    define "$1"
    echo "${!1}" | xmllint --noout -
}

# file with name $1 is created from the contents of string variable $2
createfile() {
    # make directory paths as needed
    mkdir -p "$(dirname "$1")"

    # add contents to file
    echo -ne "$2" > "$1"
}

rmfile() { rm -f "$1"; }

rmdir()  { rm -rf "$1"; }

# capture stdout and stderr
capture_output() {
    [[ "$CAPTURE_STDOUT" == "true" ]] && exec 3>&1 1>"$STDOUT"
    [[ "$CAPTURE_STDERR" == "true" ]] && exec 4>&2 2>"$STDERR"
}

# uncapture stdout and stderr
uncapture_output() {
    [[ "$CAPTURE_STDOUT" == "true" ]] && exec 1>&3
    [[ "$CAPTURE_STDERR" == "true" ]] && exec 2>&4
}

message() {
    # return stdout and stderr to standard streams
    uncapture_output

    # trace the command
    echo "$1" >&2

    capture_output
    true
}

# output filenames for capturing stdout and stderr from the command
base=$(basename "$0" .sh)
STDERR=".stderr_$base"
STDOUT=".stdout_$base"

# save stdout and stderr to our files
capture_output

# checks the result of a command
#
# If stdout is not specified, it is assumed to be empty
# If stderr is not specified, it is assumed to be empty
check() {
    local exit_status=$?

    # return stdout and stderr to standard streams
    uncapture_output

    normalize_output "$STDOUT"

    # trace the command
    firsthistoryentry

    set -e

    # Generate a unique temporary file prefix for this check
    local line
    line=$(caller | cut -d' ' -f1)
    local tmp_prefix="$PWD/.test.$line"

    # check <filename> stdoutstr stderrstr
    if [[ $# -ge 3 ]]; then
        local tmp2="$tmp_prefix.2"
        echo -en "$2" > "$tmp2"
        $diff "$tmp2" "$1"

        local tmp3="$tmp_prefix.3"
        echo -en "$3" > "$tmp3"
        $diff "$tmp3" "$STDERR"

    # check <filename> stdoutstr
    # note: empty string reports as a valid file
    elif [[ $# -ge 2 && -n "$1" && -e "$1" ]]; then
        local tmp2="$tmp_prefix.2"
        echo -en "$2" > "$tmp2"
        $diff "$tmp2" "$1"
        [[ ! -s "$STDERR" ]]

    # check stdoutstr stderrstr
    elif [[ $# -ge 2 ]]; then
        local tmp1="$tmp_prefix.1"
        echo -en "$1" > "$tmp1"
        $diff "$tmp1" "$STDOUT"

        local tmp2="$tmp_prefix.2"
        echo -en "$2" > "$tmp2"
        $diff "$tmp2" "$STDERR"

    # check <filename>
    elif [[ $# -ge 1 && -n "$1" && -e "$1" ]]; then
        $diff "$1" "$STDOUT"
        [[ ! -s "$STDERR" ]]

    # check stdoutstr
    elif [[ $# -ge 1 ]]; then
        local tmp1="$tmp_prefix.1"
        echo -en "$1" > "$tmp1"
        $diff "$tmp1" "$STDOUT"
        [[ ! -s "$STDERR" ]]

    else
        # check that both stdout and stderr are empty
        [[ ! -s "$STDOUT" ]]
        [[ ! -s "$STDERR" ]]
    fi

    set +e

    if [[ $exit_status -ne 0 ]]; then
        echo "Command failed with exit status $exit_status" >&2
        if [[ -s "$STDERR" ]]; then
             echo "--- STDERR Output (failure cause) ---" >&2
             cat "$STDERR" >&2
             echo "-------------------------------------" >&2
        fi
        exit 1
    fi

    # return to capturing stdout and stderr
    capture_output
    true
}

# checks that a file matches another file and stderr is empty
check_file() {
    local exit_status=$?

    # return stdout and stderr to standard streams
    uncapture_output

    # trace the command
    firsthistoryentry

    set -e
    $diff "$2" "$1"
    [[ ! -s "$STDERR" ]]

    if [[ $exit_status -ne 0 ]]; then
        echo " Command failed with exit status $exit_status" >&2
        cat "$STDERR" >&2
        exit 1
    fi
    set +e

    # return to capturing stdout and stderr
    capture_output
    true
}

# checks the exit status of a command and optionally expected stdout/stderr strings
#   $1 expected return value
check_exit() {
    local exit_status=$?

    # return stdout and stderr to standard streams
    uncapture_output

    # trace the command
    firsthistoryentry

    # verify expected exit status
    if [[ $exit_status -ne "$1" ]]; then
        echo "error: exit was $exit_status instead of $1"
        if [[ -s "$STDERR" ]]; then
             echo "--- STDERR Output ---" >&2
             cat "$STDERR" >&2
        fi
        exit 8
    fi

    set -e

    # Generate a unique temporary file prefix for this check
    local line
    line=$(caller | cut -d' ' -f1)
    local tmp_prefix="$PWD/.test.$line"

    if [[ $# -eq 2 ]]; then
        local tmp2="$tmp_prefix.2"
        echo -en "$2" > "$tmp2"
        $diff "$tmp2" "$STDERR"
        [[ ! -s "$STDOUT" ]]
    fi

    if [[ $# -eq 3 ]]; then
        local tmp2="$tmp_prefix.2"
        echo -en "$2" > "$tmp2"
        $diff "$tmp2" "$STDOUT"

        local tmp3="$tmp_prefix.3"
        echo -en "$3" > "$tmp3"
        $diff "$tmp3" "$STDERR"
    fi

    set +e

    # return to capturing stdout and stderr
    capture_output
    true
}

# checks that stdout has the expected number of lines
#   $1 expected number of lines in stdout
check_lines() {
    # return stdout and stderr to standard streams
    uncapture_output

    # trace the command
    firsthistoryentry

    local stdcount
    stdcount=$(wc -l "$STDOUT" | cut -d'.' -f1 | sed 's/^ *//;s/ *$//')

    # verify line count
    if [[ "$stdcount" != "$1" ]]; then
        echo "error: expected $1 lines, got $stdcount"
        exit 9
    fi

    # return to capturing stdout and stderr
    capture_output
    true
}

# Check the validity of the XML
# Currently only checks for well-formed XML, not DTD validity
xmlcheck() {
    set -e
    if command -v xmllint &> /dev/null; then
        if [[ "${1:0:1}" != "<" ]]; then
            echo "${!1}" | xmllint --noout -
        else
            echo "${1}" | xmllint --noout -
        fi
    fi
    set +e
    true
}
