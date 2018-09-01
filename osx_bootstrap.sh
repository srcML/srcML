curl http://www.sdml.cs.kent.edu/build/libarchive-3.3.2.tar.gz --output ../libarchive-332.tar.gz
(cd .. ; tar -vxf libarchive-332.tar.gz ; rm libarchive-332.tar.gz; mv libarchive-3.3.2 libarchive)
(cd ../libarchive ; ./configure --without-lz4 --disable-bsdtar --disable-bsdcpio --without-xml2 ; make)
