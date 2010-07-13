#!/bin/bash

NAME=$1

shift

FILES=$@

zip -v $NAME.zip $FILES

gzip -cv $FILES > $NAME.gz

bzip2 -cv $FILES > $NAME.bz2

tar -cvf $NAME.tar $FILES

echo $FILES | tr " " "\n" | cpio -ov > $NAME.cpio

gzip -cv $NAME.zip > $NAME.zip.gz

gzip -cv $NAME.bz2 > $NAME.bz2.gz

gzip -cv $NAME.cpio > $NAME.cpio.gz

bzip2 -cv $NAME.zip > $NAME.zip.bz2

bzip2 -cv $NAME.gz > $NAME.gz.bz2

bzip2 -cv $NAME.tar > $NAME.tar.bz2

bzip2 -cv $NAME.cpio > $NAME.cpio.bz2

tar -cvzf $NAME.tar.gz $FILES

tar -cvzf $NAME.tgz $FILES

