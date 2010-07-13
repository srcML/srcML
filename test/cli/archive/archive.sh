#!/bin/bash

NAME=$1

shift

FILES=$@

gzip -cv $FILES > $NAME.gz

bzip2 -cv $FILES > $NAME.bz2

gzip -cv $FILES.bz2 > $NAME.bz2.gz

bzip2 -cv $FILES.gz > $NAME.gz.bz2

tar -cvf $NAME.tar $FILES

tar -cvzf $NAME.tgz $FILES

tar -cvzf $NAME.tar.gz $FILES

bzip2 -cv $FILES.tar > $NAME.tar.bz2

echo $FILES | tr " " "\n" | cpio -ov > $NAME.cpio

gzip -cv $FILES.cpio > $NAME.cpio.gz

bzip2 -cv $FILES.cpio > $NAME.cpio.bz2