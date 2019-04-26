
#!/bin/bash

# GET MAKE TO RUN A RELEASE BUILD
#cmake -DCMAKE_BUILD_TYPE=Release ../srcML

while getopts ":s:b:o:v:" opt; do
  case $opt in
    s)
      source_root=${OPTARG%/}
      ;;
    b)
      build_root=${OPTARG%/}
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

echo "CLIENT"
mkdir pkg_client
cp $build_root/bin/srcml pkg_client

echo "LIB"
mkdir pkg_lib
cp -R $build_root/bin/*.dylib pkg_lib

echo "INCLUDE"
mkdir pkg_include
cp $source_root/src/libsrcml/srcml.h pkg_include

echo "RESOURCES"
mkdir pkg_resources
cp $source_root/Package/welcome.txt pkg_resources/WELCOME.txt
cp $source_root/COPYING.txt pkg_resources/LICENSE.txt
cp $source_root/README.md pkg_resources/README.txt
cp $source_root/Package/background.png pkg_resources/BACKGROUND.png

# COPY THE MAN PAGE 
echo "MANPAGES"
mkdir pkg_manpage
cp $source_root/doc/manpage/srcml.1 pkg_manpage/srcml.1

# STRIP BIN/LIB
echo "STRIP EXE AND LIB"
strip -u -r pkg_client/srcml
strip -x pkg_lib/libsrcml.*

echo "CREATE PACKAGES"
pkgbuild --root pkg_client --identifier com.srcml.client --version $version_num --install-location /usr/local/bin client.pkg
pkgbuild --root pkg_lib --identifier com.srcml.library --version $version_num --install-location /usr/local/lib lib.pkg
pkgbuild --root pkg_include --identifier com.srcml.include --version $version_num --install-location /usr/local/include include.pkg
pkgbuild --root pkg_manpage --identifier com.srcml.client.manpage --version $version_num --install-location /usr/local/share/man/man1/ client_manpage.pkg

echo "CREATE DISTRIBUTION FILE"
productbuild --synthesize --package lib.pkg --package client.pkg --package include.pkg --package client_manpage.pkg distribution.plist

echo "EDIT DISTRIBUTION FILE"
sed -i '' '3i\                          
    <title>srcML</title>\
    <welcome file="WELCOME.txt"/>\
    <license file="LICENSE.txt"/>\
    <readme file="README.txt"/>\
    <background file="BACKGROUND.png"/>
    ' distribution.plist

echo "CREATE INSTALLER"
productbuild --distribution distribution.plist --resources pkg_resources --package-path client.pkg --package-path lib.pkg --package-path client_manpage.pkg srcml.pkg

echo "CLEAN UP"
rm -rf pkg_client
rm -rf pkg_lib
rm -rf pkg_include
rm -rf pkg_resources
rm -rf pkg_manpage
rm distribution.plist
rm client.pkg
rm lib.pkg
rm include.pkg
rm client_manpage.pkg
