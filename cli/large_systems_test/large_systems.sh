#!/bin/bash

# use relative paths
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
mkdir -p "/${DIR}/logs"

# timestamp log files
TIMESTAMP=$(date +"%Y%m%d-%H%M%S")
TLOG="${DIR}/logs/timelog-${TIMESTAMP}.txt"
DLOG="${DIR}/logs/difflog-${TIMESTAMP}.txt"
SLOG="${DIR}/logs/sizelog-${TIMESTAMP}.txt"

echo "Time log for this session at: ${TLOG}" && echo > "${TLOG}"
echo "Diff log for this session at: ${DLOG}" && echo > "${DLOG}"
echo "Size log for this session at: ${SLOG}" && echo > "${SLOG}"

# use CSV input to find big system download locations
(cat "${DIR}/large_systems_list.csv" ; echo) | while IFS=',' read -r location name language
do
    # determine if the test should even be run on this system
    TEST_SYSTEM=false

    # if no arguments were given, always test (for all systems)
    if [[ $# -eq 0 ]] ; then
        TEST_SYSTEM=true
    else
        # check that the description (where to download from, name, or language)
        # for current system from the CVS list matches one of the args given
        lloc=$(echo "$location" | tr '[:upper:]' '[:lower:]')
        lname=$(echo "$name" | tr '[:upper:]' '[:lower:]')
        llang=$(echo "$language" | tr '[:upper:]' '[:lower:]')

        for system in "$@"; do
            lsys=$(echo "$system" | tr '[:upper:]' '[:lower:]')
            if [[ "${lloc}" == *"${lsys}"*  || "${lname}" == *"${lsys}"* || "${llang}" == "${lsys}" ]] ; then
                TEST_SYSTEM=true
            fi
        done
    fi

    # ignore empty lines at EOF of CSV, and only run systems that were specified
    if [[ $location != "" ]] && [[ "$TEST_SYSTEM" == true ]] ; then
        echo "--------------------------------------"

        ZNAME="${DIR}/logs/${name}"
        NAME="${ZNAME/.tar.*z/}"   # without the .tar.gz or .tar.xz extensions

        # get the compressed file, if it doesn't already exist
        if [ ! -f "${ZNAME}" ] ; then
            echo "Grabbing ${location} into ${ZNAME}"
            curl -L -k "${location}" -o "${ZNAME}"
        fi

        # store size of orginal compressed file
        ZSIZE_ORIGINAL="$(du -hs ${ZNAME})"
        echo "${ZSIZE_ORIGINAL}" | tee -a "${SLOG}"

        # ------------- compressed file --> srcml ------------------
        # redirect stderr to stdout to store the time, but keep the srcml output
        # for other tests.
        echo "Testing srcML on compressed ${ZNAME} ..."
        XZOUTPUT="${ZNAME}-output.xml"
        TIME="$(time ( srcml ${ZNAME} --in-order -o ${XZOUTPUT} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "srcml ${ZNAME} --in-order -o ${XZOUTPUT}" | tee -a "${TLOG}"
        echo "${TIME}" | tee -a "${TLOG}"

        # store output size
        ZSIZE_OUTPUT="$(du -hs ${XZOUTPUT})"
        echo "${ZSIZE_OUTPUT}" >> "${SLOG}"

        # ------------- uncompressed dir --> srcml ------------------
        # uncompress, if the unzipped version doesn't already exist
        if [ ! -d "${NAME}" ] ; then
            mkdir "${NAME}"
            echo "Uncompressing ${ZNAME} to ${NAME} ..."
            tar -xf "${ZNAME}" -C "${NAME}" --strip-components 1
        fi

        # store uncompressed size
        SIZE_ORIGINAL="$(du -hs ${NAME})"
        echo "${SIZE_ORIGINAL}" >> "${SLOG}"

        # redirect stderr to stdout to store time
        echo "Testing srcML on uncompressed ${NAME} ..."
        UNZOUTPUT="${NAME}-output.xml" # output from
        TIME="$(time ( srcml ${NAME} --in-order -o ${UNZOUTPUT} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "" | tee -a "${TLOG}"
        echo "srcml ${NAME} --in-order -o ${UNZOUTPUT}" | tee -a "${TLOG}"
        echo "${TIME}" | tee -a "${TLOG}"

        # store srcml on uncompressed size
        SIZE_OUTPUT="$(du -hs ${UNZOUTPUT})"
        echo "${SIZE_OUTPUT}" >> "${SLOG}"

        # cmp srcml output from the compressed and uncompressed input
        echo "Comparing srcML output from compressed and uncompressed input ..."
        DIFF="$( cmp ${XZOUTPUT} ${UNZOUTPUT} )"

        echo "cmp ${XZOUTPUT} ${UNZOUTPUT}" | tee -a "${DLOG}"
        echo "${DIFF}" | tee -a "${DLOG}"

        # ------------- srcml --> dir ------------------------
        echo "Testing from srcML archive to directory ..."
        SMLOUTPUTDIR="${NAME}-from-srcml"
        TIME="$(time ( srcml ${UNZOUTPUT} --to-dir=${SMLOUTPUTDIR} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "srcml ${UNZOUTPUT} --to-dir=${SMLOUTPUTDIR}" | tee -a "${TLOG}"
        echo "${TIME}" | tee -a "${TLOG}"

        # store output size
        SMLSIZE_OUTPUT="$(du -hs ${SMLOUTPUTDIR})"
        echo "${SMLSIZE_OUTPUT}" | tee -a "${SLOG}"

        # diff srcml output to original directory
        echo "Comparing srcML --to-dir output to original directory ..."
        DIFF="$( diff -r ${NAME} ${SMLOUTPUTDIR} )"

        echo "diff -r ${NAME} ${SMLOUTPUTDIR}" | tee -a "${DLOG}"
        echo "${DIFF}" | tee -a "${DLOG}"


        # keep the logs and the big system (test runs faster consecutively), but
        # cleanup srcml outputs
        rm -r "${NAME}"    # uncompressed directory
        rm "${XZOUTPUT}"   # srcml output from running on compressed file
        rm "${UNZOUTPUT}"  # srcml output from running on directory
        rm -r "${SMLOUTPUTDIR}" # created from srcml --to-dir

        echo "" >> "${TLOG}"
        echo "" >> "${DLOG}"
        echo "" >> "${SLOG}"
    fi
done
