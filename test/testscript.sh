#!/bin/bash

for file in "$@"
do
    # number of individual units
    units=$($SRCML2SRC --units $file)

    # language
    language=$($SRCML2SRC --language $file)

    # dir
    directory=$($SRCML2SRC --directory $file)

    #echo src2srcml --language=$language --directory=$directory

    for i in $(gseq $units)
    do
        $SRCML2SRC --unit=$i --xml $file | tee .save | $SRCML2SRC | $SRC2SRCML --language=$language --dir=$directory > .new

    done
done



#ls ../suite/*.c.xml | xargs -I^ -t bash -c "gseq \$(../bin/srcml2src --units ^) | xargs -I% -t bash -c '../bin/srcml2src --unit=% --xml ^ | tee .save | ../bin/srcml2src | ../bin/src2srcml -l C++ --dir=\"if\" | tee .new | diff - .save'" | tee linuxc.txt