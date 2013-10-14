install(TARGETS src2srcml srcml2src srcml_shared srcml_static 
                RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES libsrcml/srcml.h DESTINATION include)
install(DIRECTORY libsrcml DESTINATION src FILES_MATCHING REGEX [ch]pp$)
