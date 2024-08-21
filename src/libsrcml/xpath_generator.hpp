#include <string>
#include <string_view>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "unification_table.hpp"
#include "xpath_node.hpp"

#ifndef SRCQL_XPATH_GENERATOR_HPP
#define SRCQL_XPATH_GENERATOR_HPP

class XPathGenerator {
public:
    XPathGenerator(std::string_view query, std::string_view lang) : src_query(query), language(lang) { storage = UnificationTable(); };
    std::string convert();


private:
    XPathNode* get_xpath_from_argument(std::string query);
    void get_variables(xmlNode* top_xml_node);
    void convert_traverse(xmlNode*, XPathNode*);
    void organize_add_calls(XPathNode*);
    void add_bucket_clears(XPathNode*,int);

    // XML Node Funcs
    std::string get_full_name(xmlNode*);
    std::string get_name(xmlNode*);
    std::string get_ns_prefix(xmlNode*);
    std::string get_text(xmlNode*);
    bool has_only_text_child(xmlNode*);
    bool is_variable_node(xmlNode*);
    bool is_primitive_element(xmlNode*);

    // Other Helpers
    std::string add_quotes(std::string_view);
    std::string extract_variable(std::string_view);
    void get_variable_info(std::string_view, std::string&, size_t&);

    // xmlNode* srcml_root;
    // XPathNode* xpath_root;
    std::string src_query;
    std::string language;
    UnificationTable storage;
};


#endif
