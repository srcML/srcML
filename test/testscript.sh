#!/bin/bash

for file in "$@"
do
    # number of individual units
    units=$($SRCML2SRC --units $file | dos2unix)

    # language
    language=$($SRCML2SRC --language $file | dos2unix)

    # dir
    directory=$($SRCML2SRC --directory $file | dos2unix)

    # determine if operator option is needed
    OPERATOR=$($SRCML2SRC --info $file | grep 'xmlns' | grep 'http://www.sdml.info/srcML/operator')
    if [[ "$OPERATOR" != "" ]]
    then
        OPERATOR='--operator'
    fi

    # determine if literal option is needed
    LITERAL=$($SRCML2SRC --info $file | grep 'xmlns' | grep 'http://www.sdml.info/srcML/literal')
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

	if [[ "$filename" != "" ]]
	then
            FILENAMEOPTION='--filename='$filename
	else
            FILENAMEOPTION=''
	fi
        cat .save.txt | $SRC2SRCML $OPERATOR $LITERAL --language=$language --dir=$directory $FILENAMEOPTION -o .new

	diff .save .new
    done
done



#ls ../suite/*.c.xml | xargs -I^ -t bash -c "gseq \$(../bin/srcml2src --units ^) | xargs -I% -t bash -c '../bin/srcml2src --unit=% --xml ^ | tee .save | ../bin/srcml2src | ../bin/src2srcml -l C++ --dir=\"if\" | tee .new | diff - .save'" | tee linuxc.txt