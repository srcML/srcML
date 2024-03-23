#include <deque>
#include <string>
#include <string_view>
#include <iostream>

#ifndef SRCQL_XPATH_NODE_HPP
#define SRCQL_XPATH_NODE_HPP

enum NodeConnectionType {
    NO_CONN = 0,   // TEXT
    PREDICATE,     // [TEXT]
    PARENTHESES,   // (TEXT)
    NEXT,          // /TEXT
    ANY,           // //TEXT
    CALL,          // TEXT()
    UNION,         // |
};

class XPathNode {
public:
    XPathNode() : text(""), type(NO_CONN) {};
    XPathNode(std::string_view _text) : text(_text), type(NO_CONN) {}
    XPathNode(std::string_view _text, NodeConnectionType _type) : text(_text), type(_type) {}
    XPathNode(const XPathNode&);
   ~XPathNode();

   friend std::ostream& operator<<(std::ostream&, const XPathNode&);
   std::string to_string(std::string_view rtn = "");

   void set_text(std::string_view _text) { text = _text; }
   std::string get_text() { return text; }
   void set_type(NodeConnectionType _type) { type = _type; }
   NodeConnectionType get_type() { return type; }
   std::deque<XPathNode*> get_children() { return children; }

   bool is_variable_node()         { return text.find("*") != std::string::npos && text.find("text()") == std::string::npos; }
   bool is_add_call_node()         { return text.find("qli:add-element") != std::string::npos; }
   bool is_match_call_node()       { return text.find("qli:match-element") != std::string::npos; }
   bool is_regex_match_call_node() { return text.find("qli:regex-match") != std::string::npos; }

   void set_parent(XPathNode* p) { parent = p; }
   XPathNode* get_parent() { return parent; }
   XPathNode* get_first_child() { return children.front(); }

   void add_child(XPathNode* c) { c->set_parent(this); children.push_back(c); }
   void add_child_beginning(XPathNode* c) { c->set_parent(this); children.push_front(c); }
   XPathNode* pop_child_beginning();

private:
    std::string text;
    NodeConnectionType type;
    std::deque<XPathNode*> children;
    XPathNode* parent = nullptr;
};

#endif
