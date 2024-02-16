// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file qli_extensions.cpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
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

thread_local UnificationTable table;

// std::unique_ptr deleter functions for libxml2
// usage: std::unique<xmlFree> p(xmlReadMemory);
// Call p.get() for original pointer
// Will deallocate automatically at end of std::unique_ptr lifetime
namespace std {
    template<>
    struct default_delete<xmlChar> {
        void operator()(xmlChar* s) { xmlFree(s); }
    };
    template<>
    struct default_delete<xmlNodeSet> {
        void operator()(xmlNodeSet* nodes) { xmlXPathFreeNodeSet(nodes); }
    };
};

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
};

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
        table.add_to_variable_bucket(bucket);
        if (table.size_of_variable_bucket(bucket) < number) {
            table.add_to_number_bucket(bucket, number);
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
        // std::cout << ": Add " << token << "|" << node_ptr << " to " << bucket << "_" << number << std::endl;
        const bool valid = table.does_element_match_variable(bucket, number, *(itpair.first), node_ptr) && table.check_regex(std::string(bucket), std::string(tokenView));
        // Check that the name matches all of the regex rules
        if (valid) {
            table.add_to_token_list(bucket, number, *(itpair.first), node_ptr);
        }

        isValid = isValid || valid;
    }
    // std::cout << "Worked? " << (isValid ? "true" : "false") << std::endl;
    // std::cout << std::endl << table << std::endl << "-----------------------------" << std::endl;

    // return if token matches
    xmlXPathReturnBoolean(ctxt, isValid);
}

void match_element(xmlXPathParserContext* ctxt, int nargs) {
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

        // std::cout << ": Find " << token << "|" << node_ptr << " in " << bucket << "_" << table.size_of_variable_bucket(bucket) << " : id-" << number << std::endl;
        const bool valid = table.is_element_in_bucket(bucket, table.size_of_variable_bucket(bucket), *(itpair.first), node_ptr);

        isValid = isValid || valid;
    }
    // std::cout << "Worked? " << (isValid ? "true" : "false") << std::endl;
    //std::cout << std::endl << table << std::endl << "-----------------------------" << std::endl;

    // return if token matches
    xmlXPathReturnBoolean(ctxt, isValid);
}

void clear_elements(xmlXPathParserContext* ctxt, int nargs) {

    if (nargs > 1) {
        std::cerr << "Arg arity error" << std::endl;
        return;
    }

    if (nargs == 0) {

        // clear all buckets
        table.empty_buckets();

    } else if (nargs == 1) {

        // clear this bucket
        xmlChar* var = xmlXPathPopString(ctxt);
        table.empty_bucket((const char*) var);
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
    xmlXPathReturnBoolean(ctxt, true);
    return;

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

void regex_match(xmlXPathParserContext* ctxt, int nargs) {
    if(nargs != 2) {
        std::cout << "Arg arity error" << std::endl;
        return;
    }
    xmlChar* r = xmlXPathPopString(ctxt);
    std::string regex = (const char*)(r);
    xmlChar* id = xmlXPathPopString(ctxt);
    std::string identifier = (const char*)(id);
    identifier = identifier.substr(1,identifier.size()-1);

    table.add_regex_rule(identifier,regex);

    xmlXPathReturnBoolean(ctxt, true);
}

void debug_print(xmlXPathParserContext* ctxt, int nargs) {
    if(nargs != 1) {
        std::cout << "Arg arity error" << std::endl;
        return;
    }

    std::cerr << "Debug Print:" << std::endl;

    xmlNodeSet* set = xmlXPathPopNodeSet(ctxt);

    std::cerr << "\tGot set: " << set << std::endl; // Prints 0

    if(set == NULL && xmlXPathCheckError(ctxt) == false) {
        std::cerr << "\tNULL set returned, fixing" << std::endl;
        set = xmlXPathNodeSetCreate(NULL);
    }

    std::cerr << "\t# of Nodes: " << set->nodeNr << std::endl; // Errors here

    for (int i = 0; i < set->nodeNr; ++i) {
        xmlNode* node = set->nodeTab[i];
        const std::string token(get_node_text(node));
        std::cerr << "\t" << i << ": " << token << " | " << node << std::endl;
    }

    std::cerr << "Debug finished" << std::endl;

    xmlXPathReturnBoolean(ctxt, true);
}
