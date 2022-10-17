// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_read_archive_memory.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Take an archive and extract the invidual units and write to a filesystem.
*/

#include <srcml.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    // input archive contents
    std::string input = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

<unit revision="1.0.0" language="C++" filename="decl_stmt.cpp" hash="2b22284231f33eb19e66388951726a07ccbec135"><decl_stmt><decl><type><name>int</name></type> <name>n</name> <init>= <expr><literal type="number">0</literal></expr></init></decl>;</decl_stmt>
</unit>

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="1.0.0" language="C++" filename="define.cpp" hash="94bee5f917b27fab6514d5c2c46c4256f90ab9e8"><cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>MAX</name></cpp:macro> <cpp:value>10</cpp:value></cpp:define>
</unit>

<unit revision="1.0.0" language="C++" filename="expr.cpp" hash="bbfc93b885fa50211bbde022372c33ae3033f7be"><expr_stmt><expr><name>n</name> <operator>=</operator> <literal type="number">0</literal></expr>;</expr_stmt>
</unit>

</unit>
)";

    // create a new archive from the string
    srcml_archive* archive = srcml_archive_create();
    srcml_archive_read_open_memory(archive, input.data(), input.size());

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(archive))) {

        // output the current filename
        std::cout << srcml_unit_get_filename(unit) << '\n';

        // uparse into a buffer
        char* buffer = nullptr;
        size_t size = 0;
        srcml_unit_unparse_memory(unit, &buffer, &size);

        // output the source in the buffer
        std::cout << buffer;

        // free the buffer
        free((void *)buffer);

        // free the unit
        srcml_unit_free(unit);
    }

    // close the archive
    srcml_archive_close(archive);

    // free the archive
    srcml_archive_free(archive);

    return 0;
}
