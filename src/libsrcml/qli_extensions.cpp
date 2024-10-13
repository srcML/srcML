// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file qli_extensions.cpp
 *
 * @copyright Copyright (C) 2023-2024 srcML, LLC. (www.srcML.org)
 */

#include "qli_extensions.hpp"
#include "unification_table.hpp"

#include <string>
#include <string_view>
#include <set>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <memory>

// std::string_view literals, e.g., "abc"sv
using namespace ::std::literals::string_view_literals;

//thread_local UnificationTable table;

// std::unique_ptr deleter functions for libxml2
// usage: std::unique<xmlFree> p(xmlReadMemory);
// Call p.get() for original pointer
// Will deallocate automatically at end of std::unique_ptr lifetime
namespace std {
    template<>
    struct default_delete<xmlChar> {
        void operator()(xmlChar* s) { xmlFree(s); }
    };
#ifndef LIBXML2_DEFAULT_DELETE
    template<>
    struct default_delete<xmlNodeSet> {
        void operator()(xmlNodeSet* nodes) { xmlXPathFreeNodeSet(nodes); }
    };
#endif
}

namespace {
    // remove leading and trailing whitespace
    std::string_view trim_whitespace(std::string_view str) {

        const auto first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos)
            return "";

        const auto last = str.find_last_not_of(" \t\n\r");

        return str.substr(first, (last - first + 1));
    }

    // node text with normalized whitespace
    void get_node_text(const xmlNode* top_node, std::string& text, bool top) {

        // process list of nodes
        for (const xmlNode* node = top_node; node != NULL && (!top || node == top_node); node = node->next) {
            if (node->type == XML_TEXT_NODE) {
                xmlChar* t = xmlNodeGetContent(node);
                std::string_view st((const char*) t);

                // add normalized whitespace to the text
                if (st.find_first_not_of(" \t\n\r") != st.npos) {
                    text += trim_whitespace(st);
                    text += ' ';
                }
                xmlFree(t);
            }

            // process children depth first
            if (node->children) {
                get_node_text(node->children, text, false);
            }
        }
    }

    // node text with normalized whitespace
    std::string get_node_text(const xmlNode* top_node) {

        // use single string to avoid copying
        std::string s;
        get_node_text(top_node, s, true);
        return s;
    }

    int get_tree_depth(xmlNode* node) {
        xmlNode* loop = node;
        int height = 0;
        while(loop != NULL) {
            const std::string_view nodeName((char*) loop->name);
            if(nodeName == "unit"sv) {
                break;
            }
            ++height;
            loop = loop->parent;
        }
        if(loop == NULL) { return -1; }
        return height;
    }
}

void add_element(xmlXPathParserContext* ctxt, int nargs) {

    if (nargs < 3 || nargs > 5) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }

    // postfix
    std::string_view postfix;
    std::unique_ptr<xmlChar> ctxtPostfix;
    if (nargs == 5) {
        ctxtPostfix.reset(xmlXPathPopString(ctxt));
        postfix = (const char*)(ctxtPostfix.get());
    }

    // prefix
    std::string_view prefix;
    std::unique_ptr<xmlChar> ctxtPrefix;
    if (nargs >= 4) {
        ctxtPrefix.reset(xmlXPathPopString(ctxt));
        prefix = (const char*)(ctxtPrefix.get());
    }

    // order ?
    size_t number = (size_t) xmlXPathPopNumber(ctxt);

    // bucket name
    std::unique_ptr<xmlChar> ctxtBucket(xmlXPathPopString(ctxt));
    const std::string_view bucket = (char*)(ctxtBucket.get());

    // token
    std::unique_ptr<xmlNodeSet> node_set(xmlXPathPopNodeSet(ctxt));
    if(node_set.get() == NULL && xmlXPathCheckError(ctxt) == false) {
        node_set = std::unique_ptr<xmlNodeSet>(xmlXPathNodeSetCreate(NULL));
    }

    UnificationTable* table = (UnificationTable*)(ctxt->context->userData);

    bool isValid = false;
    for (int i = 0; i < node_set.get()->nodeNr; ++i) {

        const xmlNode* node = node_set.get()->nodeTab[i];

        // check for invalid elements
        const std::string_view nodeURI((char *) node->ns->href);
        const std::string_view nodeName((char*) node->name);
        const bool invalidElement = ("operator"sv == nodeName ||
                                    "comment"sv == nodeName ||
                                    "modifier"sv == nodeName ||
                                    "specifier"sv == nodeName) && "http://www.srcML.org/srcML/src"sv == nodeURI;
        if (invalidElement) {
            xmlXPathReturnBoolean(ctxt, false);
            return;
        }

        const std::string token(get_node_text(node));
        const auto node_ptr = reinterpret_cast<std::uintptr_t>(node);

        // @NOTE Add comment
        table->add_to_variable_bucket(bucket);
        if (table->size_of_variable_bucket(bucket) < number) {
            table->add_to_number_bucket(bucket, number);
        }

        // handle token via std::string_view for efficent trimming
        std::string_view tokenView(token);
        tokenView = trim_whitespace(tokenView);

        // remove prefix
        if (tokenView.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }
        tokenView.remove_prefix(prefix.length());

        // remove postfix
        if (tokenView.compare(tokenView.size() - postfix.size(), postfix.size(), postfix) != 0) {
            continue;
        }
        tokenView.remove_suffix(postfix.length());

        thread_local std::unordered_set<std::string> tokens;

        const auto itpair = tokens.insert(token.size() == tokenView.size() ? std::move(token) : std::string(tokenView));


        // if the variable matches the token, add to the tokens
        const bool valid = table->does_element_match_variable(bucket, number, *(itpair.first), node_ptr) && table->check_regex(std::string(bucket), std::string(tokenView));
        // Check that the name matches all of the regex rules
        if (valid) {
            table->add_to_token_list(bucket, number, *(itpair.first), node_ptr);
        }

        isValid = isValid || valid;
    }

    // return if token matches
    xmlXPathReturnBoolean(ctxt, isValid);
}

void match_element(xmlXPathParserContext* ctxt, int nargs) {
    if (nargs < 2 || nargs > 4) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }

    // postfix
    std::string_view postfix;
    std::unique_ptr<xmlChar> ctxtPostfix;
    if (nargs == 4) {
        ctxtPostfix.reset(xmlXPathPopString(ctxt));
        postfix = (const char*)(ctxtPostfix.get());
    }

    // prefix
    std::string_view prefix;
    std::unique_ptr<xmlChar> ctxtPrefix;
    if (nargs >= 3) {
        ctxtPrefix.reset(xmlXPathPopString(ctxt));
        prefix = (const char*)(ctxtPrefix.get());
    }

    // order ?
    //size_t number = (size_t) xmlXPathPopNumber(ctxt);

    // bucket name
    std::unique_ptr<xmlChar> ctxtBucket(xmlXPathPopString(ctxt));
    const std::string_view bucket = (char*)(ctxtBucket.get());

    // token
    std::unique_ptr<xmlNodeSet> node_set(xmlXPathPopNodeSet(ctxt));
    if(node_set.get() == NULL && xmlXPathCheckError(ctxt) == false) {
        node_set = std::unique_ptr<xmlNodeSet>(xmlXPathNodeSetCreate(NULL));
    }

    UnificationTable* table = (UnificationTable*)(ctxt->context->userData);

    bool isValid = false;
    for (int i = 0; i < node_set.get()->nodeNr; ++i) {

        const xmlNode* node = node_set.get()->nodeTab[i];

        // check for invalid elements
        const std::string_view nodeURI((char *) node->ns->href);
        const std::string_view nodeName((char*) node->name);
        const bool invalidElement = ("operator"sv == nodeName ||
                                    "comment"sv == nodeName ||
                                    "modifier"sv == nodeName ||
                                    "specifier"sv == nodeName) && "http://www.srcML.org/srcML/src"sv == nodeURI;
        if (invalidElement) {
            xmlXPathReturnBoolean(ctxt, false);
            return;
        }

        const std::string token(get_node_text(node));
        const auto node_ptr = reinterpret_cast<std::uintptr_t>(node);

        // @NOTE Add comment

        // handle token via std::string_view for efficent trimming
        std::string_view tokenView(token);
        tokenView = trim_whitespace(tokenView);

        // remove prefix
        if (tokenView.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }
        tokenView.remove_prefix(prefix.length());

        // remove postfix
        if (tokenView.compare(tokenView.size() - postfix.size(), postfix.size(), postfix) != 0) {
            continue;
        }
        tokenView.remove_suffix(postfix.length());

        thread_local std::unordered_set<std::string> tokens;

        const auto itpair = tokens.insert(token.size() == tokenView.size() ? std::move(token) : std::string(tokenView));

        const bool valid = table->is_element_in_bucket(bucket, table->size_of_variable_bucket(bucket), *(itpair.first), node_ptr);

        isValid = isValid || valid;
    }

    // return if token matches
    xmlXPathReturnBoolean(ctxt, isValid);
}

void clear_elements(xmlXPathParserContext* ctxt, int nargs) {

    if (nargs > 1) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }

    UnificationTable* table = (UnificationTable*)(ctxt->context->userData);

    if (nargs == 0) {

        // clear all buckets
        table->empty_buckets();

    } else if (nargs == 1) {

        // clear this bucket
        xmlChar* var = xmlXPathPopString(ctxt);
        table->empty_bucket((const char*) var);
        xmlFree(var);
    }

    // always true
    xmlXPathReturnBoolean(ctxt, true);
}

void is_valid_element(xmlXPathParserContext* ctxt, int nargs) {

    if (nargs != 1) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }

    std::unique_ptr<xmlNodeSet> node_set(xmlXPathPopNodeSet(ctxt));
    if(node_set.get() == NULL && xmlXPathCheckError(ctxt) == false) {
        node_set = std::unique_ptr<xmlNodeSet>(xmlXPathNodeSetCreate(NULL));
    }

    // @TODO Might there be more than one node?
    assert(node_set.get()->nodeNr == 1);
    const xmlNode* node = node_set.get()->nodeTab[0];

    // check for invalid elements
    const std::string_view nodeURI((char *) node->ns->href);
    const std::string_view nodeName((char*) node->name);
    const bool invalidElement = "http://www.srcML.org/srcML/src"sv == nodeURI && (
                                "operator"sv == nodeName ||
                                "comment"sv == nodeName ||
                                "modifier"sv == nodeName ||
                                "specifier"sv == nodeName);


    // return whether element is valid
    xmlXPathReturnBoolean(ctxt, !invalidElement);
}

void compare_tree_heights(xmlXPathParserContext* ctxt, int nargs) {
    if(nargs != 3) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }

    int compare = xmlXPathPopNumber(ctxt);

    std::cout << "----------------------------" << std::endl;

    // RHS - the comparison set
    std::unique_ptr<xmlNodeSet> search_set(xmlXPathPopNodeSet(ctxt));
    if(search_set.get() == NULL && xmlXPathCheckError(ctxt) == false) {
        search_set = std::unique_ptr<xmlNodeSet>(xmlXPathNodeSetCreate(NULL));
    }



    // LHS - the target node
    std::unique_ptr<xmlNodeSet> target_set(xmlXPathPopNodeSet(ctxt));
    if(target_set.get() == NULL && xmlXPathCheckError(ctxt) == false) {
        target_set = std::unique_ptr<xmlNodeSet>(xmlXPathNodeSetCreate(NULL));
    }
    xmlNode* target_node = target_set.get()->nodeTab[0];

    int target_height = get_tree_depth(target_node);
    std::cout << "Target: " << target_node << " | " << target_height << "," << get_node_text(target_node) << std::endl;

    // Loop through comparison set - return if correct comparison is present.
    std::cout << "#" << search_set.get()->nodeNr << std::endl;
    for(int i = 0; i < search_set.get()->nodeNr; ++i) {
        xmlNode* node = search_set.get()->nodeTab[i];
        if(node == target_node) { std::cout << "\tSkipping " << target_node << std::endl; continue; }

        int height = get_tree_depth(node);
        std::cout << "Node " << i << ": " << node <<  " | " << height << "," << get_node_text(node) << std::endl;

        // < and > are reversed - higher number means lower in tree
        if(compare == 0 && target_height == height) { xmlXPathReturnBoolean(ctxt, true); return; }
        else if(compare == -1 && target_height > height) { xmlXPathReturnBoolean(ctxt, true); return; }
        else if(compare == 1 && target_height < height) { xmlXPathReturnBoolean(ctxt, true); return; }
    }
    xmlXPathReturnBoolean(ctxt, false);
}

void regex_match(xmlXPathParserContext* ctxt, int nargs) {
    if(nargs != 2) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }
    xmlChar* r = xmlXPathPopString(ctxt);
    std::string regex = (const char*)(r);
    xmlChar* id = xmlXPathPopString(ctxt);
    std::string identifier = (const char*)(id);
    identifier = identifier.substr(1,identifier.size()-1);

    UnificationTable* table = (UnificationTable*)(ctxt->context->userData);
    table->add_regex_rule(identifier,regex);

    xmlXPathReturnBoolean(ctxt, true);
}

void debug_print(xmlXPathParserContext* ctxt, int nargs) {
    if(nargs != 1) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }


    xmlNodeSet* set = xmlXPathPopNodeSet(ctxt);

    if(set == NULL && xmlXPathCheckError(ctxt) == false) {
        set = xmlXPathNodeSetCreate(NULL);
    }

    for (int i = 0; i < set->nodeNr; ++i) {
        xmlNode* node = set->nodeTab[i];
        const std::string token(get_node_text(node));
        std::cerr << "\t" << i << ": " << token << " | " << node << std::endl;
    }

    xmlXPathReturnBoolean(ctxt, true);
}
