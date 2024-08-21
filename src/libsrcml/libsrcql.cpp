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
#include <unordered_map>

static std::unordered_map<std::string, std::string> queries;

const char* srcql_convert_query_to_xpath(const char* src_query, const char* language) {

    std::string xpath;

    if(auto search = queries.find(std::string(language)+":"+src_query); search != queries.end()) {
        xpath = search->second;
    }
    else {
        XPathGenerator generator(src_query,language);
        xpath = generator.convert();
        queries.insert(std::make_pair(std::string(language)+":"+src_query,xpath));
    }

    char* returned_xpath = new char[xpath.length()+1];
    strcpy(returned_xpath,xpath.c_str());
    return returned_xpath;
}

