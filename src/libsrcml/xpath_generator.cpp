// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file xpath_generator.cpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#include "xpath_generator.hpp"
#include <algorithm>
#include <map>

XPathGenerator::XPathGenerator(xmlNode* _root) {
    srcml_root = _root;
    storage = UnificationTable();
    xpath_root = new XPathNode();
}

std::string XPathGenerator::convert() {
    get_variables(srcml_root);
    convert_traverse(srcml_root, xpath_root);
    organize_add_calls(xpath_root);
    add_bucket_clears(xpath_root);
    std::string base_xpath = xpath_root->to_string();
    return number_add_calls(base_xpath);
}

void XPathGenerator::get_variables(xmlNode* top_xml_node) {
    for(xmlNode* node = top_xml_node; node != NULL; node = node->next) {
        if(node->type == XML_ELEMENT_NODE && is_variable_node(node)) {
            std::string text = get_text(node);
            std::string variable = extract_variable(text);
            storage.add_to_variable_bucket(variable);
            size_t order = storage.size_of_variable_bucket(variable)+1;
            storage.add_to_number_bucket(variable,order);

            std::string full_variable_text = "$" + variable + "_" + std::to_string(order);

            std::string prefix = text.substr(0,text.find("$"+variable));
            std::string postfix = text.substr(prefix.length() + ("$"+variable).length(), text.length() - prefix.length() + ("$"+variable).length());

            xmlNodeSetContent(node,(xmlChar*)((prefix+full_variable_text+postfix).c_str()));
        }

        if(node->children) { get_variables(node->children); }
    }
}

void XPathGenerator::convert_traverse(xmlNode* top_xml_node, XPathNode* x_node) {
    int child_num = 0;
    for(xmlNode* node = top_xml_node; node != NULL; node = node->next) {
        if(node->type == XML_ELEMENT_NODE && get_full_name(node) != "src:comment") { 
            x_node->set_type(x_node->get_parent() ? PREDICATE : ANY);

            // If top, add clear
            if(!x_node->get_parent() && storage.will_unification_occur()) {
                x_node->add_child(new XPathNode("qli:clear()", PREDICATE));
            }

            // If not a variable node
            if(!is_variable_node(node)) {
                x_node->set_text((child_num != 0 ? "following-sibling::" : "") + get_full_name(node));

                // TODO Attribute Selectors

                // Add Text Selectors
                if(has_only_text_child(node) && is_primitive_element(node)) {
                    x_node->add_child(new XPathNode("text()="+add_quotes(get_text(node->children)), PREDICATE));
                }
            }
            else {
                // Add * for variable
                x_node->set_text(child_num != 0 ? "following-sibling::*" : "*");

                // Get Variable Identifier and Order #, and update storage
                std::string variable;
                size_t order;

                std::string text = get_text(node);
                get_variable_info(text,variable,order);

                std::string full_variable_text = variable + "_" + std::to_string(order);
                std::string prefix = text.substr(0,text.find("$"+full_variable_text));
                std::string postfix = text.substr(prefix.length()+full_variable_text.length()+1,full_variable_text.length() - (prefix.length()+full_variable_text.length()));
                // Add check function
                if(storage.size_of_variable_bucket(variable) > 1 || prefix != "" || postfix != "") {
                    std::string node_text = "qli:add-element(.,\""+variable+"\",X";
                    if(postfix != "" || prefix != "") {
                        node_text += ","+add_quotes(prefix);
                    }
                    if(postfix != "") {
                        node_text += ","+add_quotes(postfix);
                    }
                    node_text += ")";
                    x_node->add_child(new XPathNode(node_text, PREDICATE));
                    //x_node->get_parent()->add_child_beginning(new XPathNode("qli:clear(\""+variable+"\",X)", PREDICATE));
                }

                // Add element type checker
                x_node->add_child(new XPathNode("qli:is-valid-element(.)", PREDICATE));

            }

            // Call on Children
            if(node->children && !has_only_text_child(node)) {
                // Only create a new node if first child is valid
                XPathNode* new_x_node = new XPathNode();
                x_node->add_child(new_x_node);
                convert_traverse(node->children,new_x_node);
            }

        }

        // Create new XPath node as child of current. 
        // ONLY DO IF NEXT CHILD IS A VALID ELEMENT,
        // AND CURRENT XPATHNODE IS USED
        if(node->next && node->next->type == XML_ELEMENT_NODE && x_node->get_text() != "") {
            XPathNode* next_node = new XPathNode();
            x_node->add_child(next_node);
            x_node = next_node;
            ++child_num;
        }
    }
}

// Moves all add-element calls to the end of it's sibling group
void XPathGenerator::organize_add_calls(XPathNode* x_node) {
    if(x_node->is_variable_node()) {
        x_node->add_child(x_node->pop_child_beginning());
    }
    for(auto xnode : x_node->get_children()) {
        organize_add_calls(xnode);
    }
}

// Helper function for adding clears. Populates locations with the XPathNode* that are add-element calls
void find_adds(XPathNode* x_node, std::map<std::string, std::vector<XPathNode*>>* locations) {
    if(x_node->is_add_call_node()) {
        std::string text = x_node->get_text();

        int start_quote = text.find("\"",0);
        int end_quote = text.find("\"",start_quote+1);
        std::string identifier = text.substr(start_quote+1, end_quote-start_quote-1);

        if(locations->find(identifier) == locations->end()) {
            (*locations)[identifier] = std::vector<XPathNode*>();
        }
        (*locations)[identifier].push_back(x_node);
    }
    for(auto child : x_node->get_children()) {
        find_adds(child, locations);
    }
}

bool x_node_in_all(XPathNode* x_node, std::vector<std::vector<XPathNode*>> lineages) {
    for(auto lineage : lineages) {
        if(std::find(lineage.begin(),lineage.end(),x_node) == lineage.end()) {
            return false;
        }
    }
    return true;
}

// Finds the closest common ancestor to all add-element calls, and adds a clear call
// for every syntactic variable going through unification
void XPathGenerator::add_bucket_clears(XPathNode* x_node) {
    std::map<std::string, std::vector<XPathNode*>> node_locations;
    find_adds(x_node, &node_locations);
    for(auto locations : node_locations) {
        if(locations.second.size() == 1) { continue; }
        std::vector<std::vector<XPathNode*>> lineages;
        for(auto node : locations.second) {
            std::vector<XPathNode*> lineage;
            XPathNode* loop = node;
            while(loop->get_parent()) {
                lineage.push_back(loop->get_parent());
                loop = loop->get_parent();
            }
            lineages.push_back(lineage);
        }
        int i;
        for(i = lineages[0].size() - 1; i >= 0 ; --i) {
            if(!x_node_in_all(*(lineages[0].begin()+i), lineages)) {
                break;
            }
        }
        lineages[0][i+1]->add_child_beginning(new XPathNode("qli:clear(\""+locations.first+"\")", PREDICATE));
    }
}

// Takes the raw XPath string and orders all add-element calls with the correct #
std::string XPathGenerator::number_add_calls(std::string_view xpath_view) {
    std::string xpath(xpath_view);
    std::map<std::string,int> count;
    int start = 0;
    while(xpath.find("qli:add-element",start) != std::string::npos) {
        start = xpath.find("qli:add-element",start);
        int start_quote = xpath.find("\"",start);
        int end_quote = xpath.find("\"",start_quote+1);
        std::string identifier = xpath.substr(start_quote+1, end_quote-start_quote-1);
        if(count.find(identifier) == count.end()) {
            count[identifier] = 0;
        }
        ++(count[identifier]);
        xpath.replace(end_quote+2,1,std::to_string(count[identifier]));
        start = end_quote;
    }
    return xpath;
}

// XML Node Funcs

std::string XPathGenerator::get_full_name(xmlNode* node) {
    return get_ns_prefix(node) + ":" + get_name(node);
}

std::string XPathGenerator::get_name(xmlNode* node) {
    return (char*)(node->name);
}

std::string XPathGenerator::get_ns_prefix(xmlNode* node) {
    return node->ns ? (char*)(node->ns->prefix) : "src";
}

std::string XPathGenerator::get_text(xmlNode* node) {
    xmlChar* a = xmlNodeGetContent(node);
    std::string rtn = (char*)(a);
    xmlFree(a);
    return rtn;
}

bool XPathGenerator::has_only_text_child(xmlNode* node) {
    return node->children != NULL &&
           node->children->type == XML_TEXT_NODE &&
           node->children->next == NULL;
}

bool XPathGenerator::is_variable_node(xmlNode* node) {
    return has_only_text_child(node) && get_text(node).find("$") != std::string::npos;
}

bool XPathGenerator::is_primitive_element(xmlNode* node) {
    std::string element = get_full_name(node);
    return 
        element == "src:comment" ||
        element == "src:escape" ||
        element == "src:literal" ||
        element == "src:modifier" ||
        element == "src:specifier" ||
        element == "src:name" ||
        element == "src:operator" ||
        element == "cpp:directive" ||
        element == "cpp:file" ||
        element == "cpp:literal" ||
        element == "cpp:number" ||
        element == "cpp:value"
        ;
}

std::string XPathGenerator::add_quotes(std::string_view text_view) {
    std::string text(text_view);
    for (const auto c : text) {
        if (c == '"')
            return '\'' + text + '\'';
    }
    return '"' + text + '"';
}

std::string XPathGenerator::extract_variable(std::string_view text_view) {
    std::string rtn = "";
    std::string text(text_view);
    for (size_t i = text.find("$")+1; i < text.length() && (isalnum(text[i]) || text[i] == '_'); ++i) {
        rtn += text[i]; 
    }
    return rtn;
}

void XPathGenerator::get_variable_info(std::string_view text, std::string& variable, size_t& order) {
    std::string s = extract_variable(std::string(text));
    variable = s.substr(0,s.find("_"));
    order = stoi(s.substr(s.find("_")+1,s.length()-1));
}
