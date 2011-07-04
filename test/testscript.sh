#!/bin/bash

trap report INT

function report() {
    echo
    echo $errors " out of " $count " cases"
    exit
}

count=0
errors=0
for file in "$@"
do
    # save the info
    info=$($SRCML2SRC --longinfo $file)

    # number of individual units
    units=""
    if [[ $info =~ units=\"([^\"]*)\" ]]
    then
        units=${BASH_REMATCH[1]}
    fi

    # language
    language=""
    if [[ $info =~ language=\"([^\"]*)\" ]]
    then
        language=${BASH_REMATCH[1]}
    fi

    # dir
    directory=""
    if [[ $info =~ directory=\"([^\"]*)\" ]]
    then
        directory=${BASH_REMATCH[1]}
    fi

    # determine if operator option is needed
    OPERATOR=''
    if [[ $info =~ "http://www.sdml.info/srcML/operator" ]]
    then
        OPERATOR='--operator'
    fi

    # determine if literal option is needed
    LITERAL=''
    if [[ $info =~ "http://www.sdml.info/srcML/literal" ]]
    then
        LITERAL='--literal'
    fi

    for ((i = 1; i<=$units; i++ ))
    do
	echo $file $i

        save=$($SRCML2SRC --unit=$i --xml $file)
	txt=$(echo "$save" | $SRCML2SRC)

        # filename
        FILENAMEOPTION=''
        if [[ $save =~ filename=\"([^\"]*)\" ]]
        then
            FILENAMEOPTION='--filename='${BASH_REMATCH[1]}
        fi

        new=$(echo "$txt" | $SRC2SRCML $OPERATOR $LITERAL --language=$language --dir=$directory $FILENAMEOPTION)

        if [[ "$save" != "$new" ]]
        then
            echo "BUG: "$i
            echo "$save"
            echo "$new"
            let "errors += 1"
        fi

        let "count += 1"
    done
done

report
