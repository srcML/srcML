#!/bin/bash

mkdir logs

# use CSV input to find big system download locations
(cat large_systems_list.csv ; echo) | while IFS=',' read -r location
do
    # ignore empty lines at EOF
    if [[ $location != "" ]] ; then
        echo "--------------------------------------"

        # timestamp all files
        TSZNAME="logs/$(date +"%Y%m%d-%H%M%S")-${location##*/}"
        TSNAME="${TSZNAME/.tar.*z/}"   # without the .tar.gz or .tar.xz extensions
        TLOG="${TSNAME}-timelog"
        DLOG="${TSNAME}-difflog"
        SLOG="${TSNAME}-sizelog"

        echo "Timed log for this session at: ${TLOG}"
        echo "Diff log for this session at: ${DLOG}"
        echo "Size log for this session at: ${SLOG}"

        # get the compressed file
        echo "Grabbing ${location} into ${TSZNAME}"
        curl -o "${TSZNAME}" "${location}"

        echo "Testing srcML on compressed format..."

        # redirect stderr to stdout to store the time, but keep the srcml output
        # for other tests.
        XZOUTPUT="${TSZNAME}-compressed-output.xml"
        TIME="$(time ( srcml ${TSZNAME} -o ${XZOUTPUT} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "srcml ${TSZNAME} -o ${XZOUTPUT}" > "${TLOG}"
        echo "${TIME}" >> "${TLOG}"

        # uncompress
        mkdir "${TSNAME}"
        echo "Uncompressing ${TSZNAME} to ${TSNAME}..."
        tar -xf "${TSZNAME}" -C "${TSNAME}" --strip-components 1

        # redirect stderr to stdout to store time
        echo "Testing srcML on uncompressed format..."
        UNZOUTPUT="${TSNAME}-output.xml" # output from
        TIME="$(time ( srcml ${TSNAME} -o ${UNZOUTPUT} ) 2>&1 1>/dev/null )"

        # store timed output
        echo "" >> "${TLOG}"
        echo "srcml ${TSNAME} -o ${UNZOUTPUT}" >> "${TLOG}"
        echo "${TIME}" >> "${TLOG}"

        # diff srcml output from the compressed and uncompressed input
        #echo "Diffing srcML output from compressed and uncompressed input..."
        #DIFF="$( diff ${XZOUTPUT} ${UNZOUTPUT} )"

        #echo "diff ${XZOUTPUT} ${UNZOUTPUT}" > "${DLOG}"
        #echo "${DIFF}" >> "${DLOG}"

        # cleanup everything but the logs
        rm "${TSZNAME}"    # compressed file that was downloaded using curl
        rm -rf "${TSNAME}" # directory after compressed file was uncompressed
        rm "${XZOUTPUT}"   # srcml output from running on compressed file
        rm "${UNZOUTPUT}"  # srcml output from running on directory
    fi
done
