// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file libsrcql.cpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#include "srcql.hpp"

#include "xpath_generator.hpp"
#include "unification_table.hpp"

#include <srcml.h>

#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

#include <set>
#include <algorithm>
#include <string>
#include <cstring>
#include <memory>

const char* srcql_convert_query_to_xpath(const char* src_query, const char* language) {
    //////////////////////////////////////////////
    // ONLY SUPPORTS srcPat CURRENTLY
    //////////////////////////////////////////////

    srcml_archive* holder = srcml_archive_create();
    char* s = 0;
    size_t size = -1;
    if(srcml_archive_write_open_memory(holder, &s, &size) != SRCML_STATUS_OK) {
        std::cout << "srcML Error" << std::endl; return 0;
        return NULL;
    }
    srcml_unit* converter = srcml_unit_create(holder);
    srcml_unit_set_language(converter, language);
    srcml_unit_parse_memory(converter, src_query, strlen(src_query));

    std::string pat_to_srcml = srcml_unit_get_srcml_outer(converter);

    srcml_unit_free(converter);

    bool change_from_macro = pat_to_srcml.find("macro") != std::string::npos;

    if(change_from_macro) {
        srcml_unit* macro_change = srcml_unit_create(holder);
        srcml_unit_set_language(macro_change, language);
        srcml_unit_parse_memory(macro_change, (std::string(src_query)+";").c_str(), (std::string(src_query)+";").size());

        pat_to_srcml = srcml_unit_get_srcml_outer(macro_change);

        srcml_unit_free(macro_change);
    }

    srcml_archive_close(holder);
    srcml_archive_free(holder);
    srcml_memory_free(s);

    std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> pat_doc(xmlReadMemory(pat_to_srcml.c_str(), pat_to_srcml.size(), "internal.xml", NULL, 0), xmlFreeDoc);

    xmlNode* top = xmlDocGetRootElement(pat_doc.get());

    XPathGenerator generator(change_from_macro ? top->children->children : top->children);

    std::string xpath = generator.convert();

    char* returned_xpath = new char[xpath.length()+1];
    strcpy(returned_xpath,xpath.c_str());


    return returned_xpath;
}
