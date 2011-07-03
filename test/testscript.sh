#!/bin/bash

count=0
errors=0
for file in "$@"
do
    # number of individual units
    units=$($SRCML2SRC --units $file | dos2unix)

    # language
    language=$($SRCML2SRC --language $file | dos2unix)

    # dir
    directory=$($SRCML2SRC --directory $file | dos2unix)

    # save the info
    $SRCML2SRC --info $file | grep 'xmlns' > .info

    # determine if operator option is needed
    OPERATOR=$(grep 'http://www.sdml.info/srcML/operator' .info)
    if [[ "$OPERATOR" != "" ]]
    then
        OPERATOR='--operator'
    fi

    # determine if literal option is needed
    LITERAL=$(grep 'http://www.sdml.info/srcML/literal' .info)
    if [[ "$LITERAL" != "" ]]
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
