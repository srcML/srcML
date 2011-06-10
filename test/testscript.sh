#!/bin/bash

for file in "$@"
do
    # number of individual units
    units=$($SRCML2SRC --units $file | dos2unix)

    # language
    language=$($SRCML2SRC --language $file | dos2unix)

    # dir
    directory=$($SRCML2SRC --directory $file | dos2unix)

    #echo src2srcml --language=$language --directory=$directory --filename=\"$filename\"

    for i in $(seq $units)
    do
	echo $file $i
        $SRCML2SRC --unit=$i --xml $file -o .save
	$SRCML2SRC .save -o .save.txt

        # filename
        filename=$($SRCML2SRC --filename .save | dos2unix)

	if [[ "$filename" != "" ]]
	then
	    cat .save.txt | $SRC2SRCML --language=$language --dir=$directory --filename=$filename -o .new
	else
	    cat .save.txt | $SRC2SRCML --language=$language --dir=$directory -o .new
	fi

	diff .save .new
    done
done



#ls ../suite/*.c.xml | xargs -I^ -t bash -c "gseq \$(../bin/srcml2src --units ^) | xargs -I% -t bash -c '../bin/srcml2src --unit=% --xml ^ | tee .save | ../bin/srcml2src | ../bin/src2srcml -l C++ --dir=\"if\" | tee .new | diff - .save'" | tee linuxc.txt