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
    if(node.type == PREDICATE)        { out << '['  ; }
    else if(node.type == PARENTHESES) { out << '('  ; }
    else if(node.type == NEXT)        { out << '/'  ; }
    else if(node.type == ANY)         { out << "//" ; }
    else if(node.type == UNION)       { out << "|"  ; }

    if(node.type != UNION) { out << node.text; };
    if(node.type == PARENTHESES) { out << ')'; }
    if(node.type != CALL) {
        for(auto child : node.children) { out << child->to_string(); }
    }
    else {
        out << "(";
        int i = 0;
        bool union_child = false;
        for(auto child : node.children) {
            if(child->get_type() == UNION) {
                union_child = true;
                break;
            }
            if(i != 0) { out << ","; }
            out << child->to_string();
            ++i;
        }
        out << ")";
        if(union_child) {
            out << node.children[i]->to_string();
        }
    }

    if(node.type == PREDICATE)        { out << ']' ; }

    return out;
}

std::string XPathNode::to_string(std::string_view rtn_view) {
    //std::cout << "IN" << text << "|" << this << std::endl;
    std::string rtn(rtn_view);
    if(type == PREDICATE)        { rtn += '['  ; }
    else if(type == PARENTHESES) { rtn += '('  ; }
    else if(type == NEXT)        { rtn += '/'  ; }
    else if(type == ANY)         { rtn += "//" ; }
    else if(type == UNION)       { rtn += "|"  ; }

    if(type != UNION) { rtn += text; };
    if(type == PARENTHESES) { rtn += ')'; }
    if(type != CALL) {
        for(auto child : children) { rtn += child->to_string(); }
    }
    else {
        rtn += "(";
        int i = 0;
        bool union_child = false;
        for(auto child : children) {
            if(child->get_type() == UNION) {
                union_child = true;
                break;
            }
            if(i != 0) { rtn += ","; }
            rtn += child->to_string();
            ++i;
        }
        rtn += ")";
        if(union_child) {
            rtn += children[i]->to_string();
        }
    }

    if(type == PREDICATE)        { rtn += ']' ; }

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
