
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

echo "CLIENT"
mkdir $outputdir/pkg_client
cp $build_root/bin/srcml $outputdir/pkg_client

echo "LIB"
mkdir $outputdir/pkg_lib
cp $build_root/bin/*.dylib $outputdir/pkg_lib

echo "INCLUDE"
mkdir $outputdir/pkg_include
cp $source_root/src/libsrcml/srcml.h $outputdir/pkg_include

echo "RESOURCES"
mkdir $outputdir/pkg_resources
cp $source_root/CMake/welcome.txt $outputdir/pkg_resources/WELCOME.txt
cp $source_root/COPYING.txt $outputdir/pkg_resources/LICENSE.txt
cp $source_root/README.md $outputdir/pkg_resources/README.txt
cp $source_root/CMake/background.png $outputdir/pkg_resources/BACKGROUND.png

# BUILD THE MAN PAGES AND COPY THE FILES
echo "MANPAGES"
mkdir $outputdir/pkg_manpage
(cd $source_root/doc/manpage; touch srcml.md; ./replace_variables.sh srcml_input.md srcml.md; ronn srcml.md --manual=srcml)
cp $source_root/doc/manpage/srcml $outputdir/pkg_manpage/srcml.1
(cd $source_root/doc/manpage; rm srcml srcml.html srcml.md srcml.md-e)

# STRIP BIN/LIB
echo "STRIP EXE AND LIB"
strip -u -r $outputdir/pkg_client/srcml
strip -x $outputdir/pkg_lib/libsrcml.dylib

echo "CREATE PACKAGES"
pkgbuild --root $outputdir/pkg_client --identifier com.srcml.client --version $version_num --install-location /usr/local/bin $outputdir/client.pkg
pkgbuild --root $outputdir/pkg_lib --identifier com.srcml.library --version $version_num --install-location /usr/local/lib $outputdir/lib.pkg
pkgbuild --root $outputdir/pkg_include --identifier com.srcml.include --version $version_num --install-location /usr/local/include $outputdir/include.pkg
pkgbuild --root $outputdir/pkg_manpage --identifier com.srcml.client.manpage --version $version_num --install-location /usr/local/share/man/man1/ $outputdir/client_manpage.pkg

echo "CREATE DISTRIBUTION FILE"
productbuild --synthesize --package $outputdir/lib.pkg --package $outputdir/client.pkg --package $outputdir/include.pkg --package $outputdir/client_manpage.pkg $outputdir/distribution.plist

echo "EDIT DISTRIBUTION FILE"
sed -i '' '3i\                          
    <title>srcML</title>\
    <welcome file="WELCOME.txt"/>\
    <license file="LICENSE.txt"/>\
    <readme file="README.txt"/>\
    <background file="BACKGROUND.png"/>
    ' $outputdir/distribution.plist

echo "CREATE INSTALLER"
productbuild --distribution $outputdir/distribution.plist --resources $outputdir/pkg_resources --package-path $outputdir/client.pkg --package-path $outputdir/lib.pkg --package-path $outputdir/client_manpage.pkg $outputdir/srcml.pkg

echo "CLEAN UP"
rm -rf $outputdir/pkg_client
rm -rf $outputdir/pkg_lib
rm -rf $outputdir/pkg_include
rm -rf $outputdir/pkg_resources
rm -rf $outputdir/pkg_manpage
rm $outputdir/distribution.plist
rm $outputdir/client.pkg
rm $outputdir/lib.pkg
rm $outputdir/include.pkg
rm $outputdir/client_manpage.pkg
