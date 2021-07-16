if(CPACK_GENERATOR STREQUAL "DEB")

    foreach(PACKAGE ${CPACK_PACKAGE_FILES})

        execute_process(COMMAND ar -x ${PACKAGE} control.tar.gz)
        execute_process(COMMAND tar xzf control.tar.gz)
        execute_process(COMMAND rm control.tar.gz)
        execute_process(COMMAND tar czf control.tar.gz ./md5sums ./control ./shlibs ./triggers)
        execute_process(COMMAND ar r ${PACKAGE} control.tar.gz)

        message(STATUS "Post build, remove postinst and postrm from package ${PACKAGE}")
    endforeach()

endif()
