##
# CMakeInstall.inc
#
# Install directives for cmake.

install(TARGETS src2srcml srcml2src srcml_shared srcml_static 
                RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES src/libsrcml/srcml.h DESTINATION include)
install(DIRECTORY src/libsrcml DESTINATION src FILES_MATCHING PATTERN *.hpp PATTERN *.cpp)
