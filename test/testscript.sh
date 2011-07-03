#!/bin/bash

count=0
errors=0
for file in "$@"
do
    # number of individual units
    units=$($SRCML2SRC --units $file | dos2unix)

    # save the info
    $SRCML2SRC --info $file > .info

    # language
    language=$(grep 'language' .info | grep -o "C\|C++\|C++0x\|Java" | dos2unix)

    # dir
    directory=$(grep 'directory' .info | cut -c12- | tr -d '"' | dos2unix)

    # determine if operator option is needed
    OPERATOR=''
    if [[ "$(grep 'xmlns' .info | grep 'http://www.sdml.info/srcML/operator')" != "" ]]
    then
        OPERATOR='--operator'
    fi

    # determine if literal option is needed
    LITERAL=''
    if [[ "$(grep 'xmlns' .info | grep 'http://www.sdml.info/srcML/literal')" != "" ]]
    then
        LITERAL='--literal'
    fi

    for i in $(seq $units)
    do
	echo $file $i
        $SRCML2SRC --unit=$i --xml $file -o .save
	$SRCML2SRC .save -o .save.txt

        # filename
        filename=$($SRCML2SRC --filename .save | dos2unix)

        FILENAMEOPTION=''
	if [[ "$filename" != "" ]]
	then
            FILENAMEOPTION='--filename='$filename
	fi
        cat .save.txt | $SRC2SRCML $OPERATOR $LITERAL --language=$language --dir=$directory $FILENAMEOPTION -o .new

	diff .save .new
        if [[ "$?" != "0" ]]
        then
            let "errors += 1"
        fi

        let "count += 1"
    done
done
echo
echo $errors " out of " $count " cases"
