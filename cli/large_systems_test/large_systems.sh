#!/bin/bash

# use relative paths
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
mkdir -p "/${DIR}/logs"

# timestamp log files
TIMESTAMP=$(date +"%Y-%m-%dT%H-%M-%S")
TLOG="${DIR}/logs/timelog_${TIMESTAMP}.txt"
DLOG="${DIR}/logs/difflog_${TIMESTAMP}.txt"
SLOG="${DIR}/logs/sizelog_${TIMESTAMP}.txt"

echo "Time log for this session at: ${TLOG}" && echo > "${TLOG}"
echo "Diff log for this session at: ${DLOG}" && echo > "${DLOG}"
echo "Size log for this session at: ${SLOG}" && echo > "${SLOG}"

# optionally use max-threads
if [ $# -ge 2 ] && [[ "${1}" == *"max-threads" && $2 =~ ^-?[0-9]+$ ]] ; then
    MAX_THREADS="--max-threads=${2}"
    shift 2
fi


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

        # get the compressed file, if it doesn't already exist
        ZNAME="${DIR}/logs/${name}"
        if [ ! -f "${ZNAME}" ] ; then
            echo "Grabbing ${location} into ${ZNAME}"
            curl -L -k "${location}" -o "${ZNAME}"
        fi

        # store size of orginal compressed file
        ZSIZE_ORIGINAL="$(du -hs ${ZNAME})"
        echo "${ZSIZE_ORIGINAL}" | tee -a "${SLOG}"

        # --- all.tar.gz -> all/ -> source-only/ -> source-only.tar.gz ---
        # uncompress, if the unzipped version doesn't already exist
        NAME="${ZNAME/.tar.*z/}-onlysrc" # without the .tar.gz or .tar.xz extensions
        if [ ! -d "${NAME}" ] ; then
            echo "Uncompressing ${ZNAME} to ${NAME} ..."
            mkdir "${NAME}" && tar -xf "${ZNAME}" -C "${NAME}" --strip-components 1
        fi

        # get rid of non-source code files; include files for C, C++, Objective-C, Aspect-J, and Java
        find "${NAME}" -type f -not -name "*.[cChHmM]" -not -name "*.[cChH][pP][pP]" -not -name "*.aj" -not -name ".java" -exec rm "{}" \;

        # re-compress project
        SRCZNAME="${DIR}/logs/${name/.tar.*z/}-onlysrc.tar.gz"
        tar czf "${SRCZNAME}" "${NAME}"

        # store size of source-only compressed file
        ZSIZE_SRCONLY="$(du -hs ${SRCZNAME})"
        echo "${ZSIZE_SRCONLY}" | tee -a "${SLOG}"

        # store uncompressed size
        SIZE_ORIGINAL="$(du -hs ${NAME})"
        echo "${SIZE_ORIGINAL}" | tee -a "${SLOG}"

        # ----------------- source-only/ --> srcml -----------------------
        UNZOUTPUT="${NAME}-output.xml" # output from
        echo "srcml ${MAX_THREADS} ${NAME} --in-order -o ${UNZOUTPUT}" | tee -a "${TLOG}"

        # store timed output; redirect stderr to stdout to store time
        TIME="$(time ( srcml ${MAX_THREADS} ${NAME} --in-order -o ${UNZOUTPUT} ) 2>&1 1>/dev/null )"
        echo "${TIME}" | tee -a "${TLOG}"

        # store srcml on uncompressed size
        SIZE_OUTPUT="$(du -hs ${UNZOUTPUT})"
        echo "${SIZE_OUTPUT}" | tee -a "${SLOG}"

        # ---------------- source-only.tar.gz --> srcml ----------------
        echo | tee -a "${TLOG}"
        XZOUTPUT="${SRCZNAME}-output.xml"
        echo "srcml ${MAX_THREADS} ${SRCZNAME} --in-order -o ${XZOUTPUT}" | tee -a "${TLOG}"

        # store timed output; redirect stderr to stdout to store time
        TIME="$(time ( srcml ${MAX_THREADS} ${SRCZNAME} --in-order -o ${XZOUTPUT} ) 2>&1 1>/dev/null )"
        echo "${TIME}" | tee -a "${TLOG}"

        # store output size
        ZSIZE_OUTPUT="$(du -hs ${XZOUTPUT})"
        echo "${ZSIZE_OUTPUT}" | tee -a "${SLOG}"

        # cmp srcml output from the compressed and uncompressed input
        echo "cmp ${XZOUTPUT} ${UNZOUTPUT}" | tee -a "${DLOG}"
        DIFF="$( cmp ${XZOUTPUT} ${UNZOUTPUT} )"
        echo "${DIFF}" | tee -a "${DLOG}"

        # -------------- srcml --> source-only/ ------------------------
        SMLOUTPUTDIR="${NAME}-from-srcml"
        echo "srcml ${MAX_THREADS} ${UNZOUTPUT} --to-dir=${SMLOUTPUTDIR}" | tee -a "${TLOG}"

        # store timed output
        TIME="$(time ( srcml ${MAX_THREADS} ${UNZOUTPUT} --to-dir=${SMLOUTPUTDIR} ) 2>&1 1>/dev/null )"
        echo "${TIME}" | tee -a "${TLOG}"

        # store output size
        SMLSIZE_OUTPUT="$(du -hs ${SMLOUTPUTDIR})"
        echo "${SMLSIZE_OUTPUT}" | tee -a "${SLOG}"

        # diff srcml output to original directory
        echo "diff -r ${NAME} ${SMLOUTPUTDIR}" | tee -a "${DLOG}"
        DIFF="$( diff -r ${NAME} ${SMLOUTPUTDIR} )"
        echo "${DIFF}" | tee -a "${DLOG}"


        # keep the logs and the bigsystem.tar.gz (test runs faster consecutively),
        # but cleanup srcml outputs and bigsystem/ dir
        rm "${SRCZNAME}"   # compressed file soucrce-only-tar.gz
        rm -r "${NAME}"    # uncompressed directory soucrce-only-tar/
        rm "${XZOUTPUT}"   # srcml output from running on soucrce-only-tar.gz
        rm "${UNZOUTPUT}"  # srcml output from running on soucrce-only-tar/
        rm -r "${SMLOUTPUTDIR}" # created from srcml --to-dir

        echo | tee -a "${TLOG}" "${DLOG}" "${SLOG}"
    fi
done
