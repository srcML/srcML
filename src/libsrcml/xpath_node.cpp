#include "xpath_node.hpp"

XPathNode::XPathNode(const XPathNode& orig) {
    //std::cout << "Copying!" << std::endl;
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
    if(node.type != CALL) {
        for(auto child : node.children) { out << *child; }
    }
    else {
        out << node.text << "(";
        int i = 0;
        for(auto child : node.children) { 
            if(i++) { out << ","; }
            out << *child;
        }
        out << ")";
    }
    if(node.type == PREDICATE) { out << ']' ; }

    return out;
}

std::string XPathNode::to_string(std::string_view rtn_view) {
    //std::cout << "IN" << text << "|" << this << std::endl;
    std::string rtn(rtn_view);
    if(type == PREDICATE)     { rtn += '['  ; }
    else if(type == NEXT)     { rtn += '/'  ; }
    else if(type == ANY)      { rtn += "//" ; }
    else if(type == UNION)    { rtn += "|"  ; }

    if(type != UNION) { rtn += text; };
    if(type != CALL) {
        for(auto child : children) { rtn += child->to_string(); }
    }
    else {
        rtn += "(";
        int i = 0;
        for(auto child : children) {
            if(i != 0) { rtn += ","; }
            rtn += child->to_string();
            ++i;
        }
        rtn += ")";
    }

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
