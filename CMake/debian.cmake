# set Debian package name
set(CPACK_DEBIAN_PACKAGE_NAME "srcML")

# set architecture
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")

# set dependencies
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libxml2 (>= 2.7.8), libxslt1.1 (>= 1.1.26), libarchive12 (>= 3.0.3")

# set package type
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

# set the package priority
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# set recommended package
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "yes")

# set package suggests
set(CPACK_DEBIAN_PACKAGE_SUGGESTS "yes")

# set extra processing
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "")