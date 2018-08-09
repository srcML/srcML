
#!/bin/bash

# GET MAKE TO RUN A RELEASE BUILD
#cmake -DCMAKE_BUILD_TYPE=Release ../srcML

outputdir="."

while getopts ":s:b:o:v:" opt; do
  case $opt in
    s)
      source_root=${OPTARG%/}
      ;;
    b)
      build_root=${OPTARG%/}
      ;;
    o)
      outputdir=${OPTARG%/}
      ;;
    v)
      version_num=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

if [ -z "$source_root" ] || [ -z "$build_root" ] || [ -z "$version_num" ]
then
	echo "Requires source root directory (-s), build root directory (-b), and version number (-v)"
	exit 1
fi

mkdir $outputdir/pkg_client
cp $build_root/bin/srcml $outputdir/pkg_client

mkdir $outputdir/pkg_lib
cp $build_root/bin/*.dylib $outputdir/pkg_lib

mkdir $outputdir/pkg_include
cp $source_root/src/libsrcml/srcml.h $outputdir/pkg_include

mkdir $outputdir/pkg_resources
cp $source_root/CMake/welcome.txt $outputdir/pkg_resources/WELCOME.txt
cp $source_root/COPYING.txt $outputdir/pkg_resources/LICENSE.txt
cp $source_root/README.md $outputdir/pkg_resources/README.txt
cp $source_root/CMake/background.png $outputdir/pkg_resources/BACKGROUND.png

# STRIP BIN/LIB
strip -u -r $outputdir/pkg_client/srcml
strip -x $outputdir/pkg_lib/libsrcml.dylib

pkgbuild --root ./pkg_client --identifier com.srcml.client --version $version_num --install-location /usr/local/bin client.pkg
pkgbuild --root ./pkg_lib --identifier com.srcml.library --version $version_num --install-location /usr/local/lib lib.pkg
pkgbuild --root ./pkg_include --identifier com.srcml.include --version $version_num --install-location /usr/local/include include.pkg

productbuild --synthesize --package lib.pkg --package client.pkg --package include.pkg distribution.plist

sed -i '' '3i\                          
    <title>srcML</title>\
    <welcome file="WELCOME.txt"/>\
    <license file="LICENSE.txt"/>\
    <readme file="README.txt"/>\
    <background file="BACKGROUND.png"/>
    ' distribution.plist

productbuild --distribution distribution.plist --resources ./pkg_resources --package-path client.pkg --package-path lib.pkg srcml.pkg

rm -rf pkg_client
rm -rf pkg_lib
rm -rf pkg_include
rm -rf pkg_resources
rm distribution.plist
rm client.pkg
rm lib.pkg
rm include.pkg


