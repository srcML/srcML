#!/bin/bash

NAME=$1

shift

FILES=$@

gzip -cv $FILES > $NAME.gz

bzip2 -cv $FILES > $NAME.bz2

tar -cvf $NAME.tar $FILES

echo $FILES | tr " " "\n" | cpio -ov > $NAME.cpio

gzip -cv $FILES.bz2 > $NAME.bz2.gz

gzip -cv $FILES.cpio > $NAME.cpio.gz

bzip2 -cv $FILES.gz > $NAME.gz.bz2

bzip2 -cv $FILES.tar > $NAME.tar.bz2

bzip2 -cv $FILES.cpio > $NAME.cpio.bz2

tar -cvzf $NAME.tar.gz $FILES

tar -cvzf $NAME.tgz $FILES

echo $FILES | sed "s/ /.gz\n/" | cpio -ov > $NAME.gz.cpio

echo $FILES | sed "s/ /.bz2\n/" | cpio -ov > $NAME.bz2.cpio