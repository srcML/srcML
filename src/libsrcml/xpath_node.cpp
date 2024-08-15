// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file xpath_node.cpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#include "xpath_node.hpp"

XPathNode::XPathNode(const XPathNode& orig) {
    std::cout << "Copying!" << std::endl;
    text = orig.text;
    type = orig.type;
    for(auto child : orig.children) {
        add_child(new XPathNode(*child));
    }
}

XPathNode::~XPathNode() {
    if(!children.empty()) {
        for(auto child : children) {
            delete child;
        }
    }
}

std::ostream& operator<<(std::ostream& out, const XPathNode& node) {
    if(node.type == PREDICATE) { out << '[' ; }
    else if(node.type == NEXT) { out << '/' ; }
    else if(node.type == ANY)  { out << "//"; }

    out << node.text;
    for(auto child : node.children) { out << *child; }

    if(node.type == PREDICATE) { out << ']' ; }

    return out;
}

std::string XPathNode::to_string(std::string_view rtn_view) {
    std::string rtn(rtn_view);
    if(type == PREDICATE) { rtn += '[' ; }
    else if(type == NEXT) { rtn += '/' ; }
    else if(type == ANY)  { rtn += "//"; }

    rtn += text;
    for(auto child : children) { rtn += child->to_string(); }

    //if(text == "") { rtn += "*[false]"; } // Invalid expression, fail

    if(type == PREDICATE) { rtn += ']' ; }

    return rtn;
}

XPathNode* XPathNode::pop_child_beginning() {
    if(children.size()) {
        XPathNode* rtn = children.front();
        children.pop_front();
        return rtn;
    }
    return nullptr;
}
