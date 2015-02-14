#!/bin/bash

mkdir logs

# timestamp log files
TIMESTAMP=$(date +"%Y%m%d-%H%M%S")

TLOG="logs/timelog-${TIMESTAMP}.txt"
DLOG="logs/difflog-${TIMESTAMP}.txt"
SLOG="logs/sizelog-${TIMESTAMP}.txt"

echo "Time log for this session at: ${TLOG}" && echo > "${TLOG}"
echo "Diff log for this session at: ${DLOG}" && echo > "${DLOG}"
echo "Size log for this session at: ${SLOG}" && echo > "${SLOG}"

# use CSV input to find big system download locations
(cat large_systems_list.csv ; echo) | while IFS=',' read -r location ignore
do
    # ignore empty lines at EOF
    if [[ $location != "" ]] ; then
        echo "--------------------------------------"

        ZNAME="logs/${location##*/}"
        NAME="${ZNAME/.tar.*z/}"   # without the .tar.gz or .tar.xz extensions

        # get the compressed file, if it doesn't already exist
        if [ ! -f "${ZNAME}" ] ; then
            echo "Grabbing ${location} into ${ZNAME}"
            curl -o "${ZNAME}" "${location}"
        fi

        echo "Testing srcML on compressed ${ZNAME} ..."

        # redirect stderr to stdout to store the time, but keep the srcml output
        # for other tests.
        XZOUTPUT="${ZNAME}-output.xml"
        TIME="$(time ( srcml ${ZNAME} --in-order -o ${XZOUTPUT} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "srcml ${ZNAME} --in-order -o ${XZOUTPUT}" >> "${TLOG}"
        echo "${TIME}" >> "${TLOG}"

        # uncompress, if the unzipped version doesn't already exist
        if [ ! -d "${NAME}" ] ; then
            mkdir "${NAME}"
            echo "Uncompressing ${ZNAME} to ${NAME} ..."
            tar -xf "${ZNAME}" -C "${NAME}" --strip-components 1
        fi

        # redirect stderr to stdout to store time
        echo "Testing srcML on uncompressed ${NAME} ..."
        UNZOUTPUT="${NAME}-output.xml" # output from
        TIME="$(time ( srcml ${NAME} --in-order -o ${UNZOUTPUT} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "" >> "${TLOG}"
        echo "srcml ${NAME} --in-order -o ${UNZOUTPUT}" >> "${TLOG}"
        echo "${TIME}" >> "${TLOG}"

        # cmp srcml output from the compressed and uncompressed input
        #echo "Comparing srcML output from compressed and uncompressed input..."
        DIFF="$( cmp ${XZOUTPUT} ${UNZOUTPUT} )"

        echo "cmp ${XZOUTPUT} ${UNZOUTPUT}" >> "${DLOG}"
        echo "${DIFF}" >> "${DLOG}"

        # keep the logs and the big system (test runs faster consecutively), but
        # cleanup srcml outputs
        rm "${XZOUTPUT}"   # srcml output from running on compressed file
        rm "${UNZOUTPUT}"  # srcml output from running on directory
    fi
done
