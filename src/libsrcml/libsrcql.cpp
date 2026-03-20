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
#include <mutex>
#include <shared_mutex>

static std::unordered_map<std::string, std::string> queries;
static std::shared_mutex queries_mutex;

// The current version of srcQL as a number
int srcql_version_number() {
    return SRCQL_VERSION_NUMBER;
}

// The current version of srcQL as a string
const char* srcql_version_string() {
    return SRCQL_VERSION_STRING;
}

const char* srcql_convert_query_to_xpath(const char* src_query, const char* language) {
    std::string key = std::string(language)+":"+src_query;
    std::string xpath;

    // Shared lock for read
    {
        std::shared_lock lock(queries_mutex);
        if (auto search = queries.find(key); search != queries.end()) {
            xpath = search->second;
        }
    }

    if(xpath.empty()) {
        // Unqiue lock for write
        std::unique_lock lock(queries_mutex);

        if (auto search = queries.find(key); search != queries.end()) {
            xpath = search->second;
        }
        else {
            XPathGenerator generator(src_query,language);
            xpath = generator.convert();
            queries.insert(std::make_pair(key,xpath));
        }
    }

    char* returned_xpath = new char[xpath.length()+1];
    strcpy(returned_xpath,xpath.c_str());
    return returned_xpath;
}

