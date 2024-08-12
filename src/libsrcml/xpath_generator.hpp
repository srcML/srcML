// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file xpath_generator.hpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

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
    XPathGenerator(xmlNode* _root);
    ~XPathGenerator() { delete xpath_root; }
    std::string convert();


private:
    void get_variables(xmlNode*);
    void convert_traverse(xmlNode*, XPathNode*);
    void organize_add_calls(XPathNode*);
    void add_bucket_clears(XPathNode*);
    std::string number_add_calls(std::string_view);

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

    xmlNode* srcml_root;
    XPathNode* xpath_root;
    UnificationTable storage;
};


#endif
