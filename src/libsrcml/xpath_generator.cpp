#include "xpath_generator.hpp"
#include <srcml.h>
#include <algorithm>
#include <map>

// HELPERS
// Helper function for adding clears. Populates locations with the XPathNode* that are add-element calls

std::vector<std::string> split(std::string str, std::string delim) {
    std::vector<std::string> res;
    size_t pos = 0;
    while(str.find(delim,pos) != std::string::npos) {
        size_t end = str.find(delim,pos);
        std::string sub = str.substr(pos,end-pos);
        if(sub != "") { res.push_back(sub); }
        pos = end + delim.size();
    }
    res.push_back(str.substr(pos,str.size()-pos));
    return res;
}

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

// Takes the raw XPath string and orders all add-element calls with the correct #
void number_add_calls(XPathNode* node, int group, std::map<std::string,int>* count = nullptr) {
    bool clean = false;
    if(count == 0) {
        clean = true;
        count = new std::map<std::string,int>();
    }

    //std::cout << "#" << group << ' ' << node->to_string() << std::endl;

    std::string node_text = node->get_text();
    if(node_text.find("qli:add-element",0) != std::string::npos) {
        //std::cout << node_text << " -> ";
        int start = node_text.find("qli:add-element",0);
        int start_quote = node_text.find("\"",start);
        int end_quote = node_text.find("\"",start_quote+1);
        std::string identifier = node_text.substr(start_quote+1, end_quote-start_quote-1);
        if(count->find(identifier) == count->end()) {
            (*count)[identifier] = 0;
        }
        ++((*count)[identifier]);
        node_text.replace(end_quote+2,1,std::to_string((*count)[identifier]));
        node_text.insert(end_quote,"_"+std::to_string(group));
        node->set_text(node_text);
        //std::cout << node_text << std::endl;
    }
    for(auto child : node->get_children()) {
        number_add_calls(child,group,count);
    }

    if(clean) {
        delete count;
    }
}
std::string number_add_calls(std::string_view xpath_view) {
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

XPathNode* XPathGenerator::get_xpath_from_argument(std::string src_query) {
    // GET SRCML
    srcml_archive* holder = srcml_archive_create();
    char* s = 0;
    size_t size = -1;
    if(srcml_archive_write_open_memory(holder, &s, &size) != SRCML_STATUS_OK) {
        std::cout << "srcML Error" << std::endl; return 0;
        return NULL;
    }
    srcml_unit* converter = srcml_unit_create(holder);
    srcml_unit_set_language(converter, language.c_str());
    srcml_unit_parse_memory(converter, src_query.c_str(), src_query.size());

    std::string pat_to_srcml = srcml_unit_get_srcml_outer(converter);

    srcml_unit_free(converter);

    bool change_from_macro = pat_to_srcml.find("macro") != std::string::npos;

    if(change_from_macro) {
        srcml_unit* macro_change = srcml_unit_create(holder);
        srcml_unit_set_language(macro_change, language.c_str());
        srcml_unit_parse_memory(macro_change, (src_query+";").c_str(), (src_query+";").size());

        pat_to_srcml = srcml_unit_get_srcml_outer(macro_change);

        srcml_unit_free(macro_change);
    }

    srcml_archive_close(holder);
    srcml_archive_free(holder);
    srcml_memory_free(s);

    std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> pat_doc(xmlReadMemory(pat_to_srcml.c_str(), pat_to_srcml.size(), "internal.xml", NULL, 0), xmlFreeDoc);

    xmlNode* top = xmlDocGetRootElement(pat_doc.get());

    xmlNode* srcml_root = change_from_macro ? top->children->children : top->children;

    get_variables(srcml_root);
    XPathNode* xpath_root = new XPathNode();
    // std::cout << "\nConverting: " << src_query << std::endl;
    convert_traverse(srcml_root, xpath_root);
    // std::cout << "Structure: " << xpath_root->to_string() << std::endl;
    organize_add_calls(xpath_root);
    // std::cout << "Add Calls: " << xpath_root->to_string() << std::endl;
    // add_bucket_clears(xpath_root);
    // std::cout << "Clears: " << xpath_root->to_string() << std::endl;
    // std::cout << std::endl;
    return xpath_root;
}

std::string XPathGenerator::convert() {
    std::vector<std::string> ops;
    std::vector<XPathNode*> args;

    size_t pos = 0;
    bool last_done = false;
    bool is_xpath = false;
    std::string build_token = "";
    while(src_query.find(" ",pos) != std::string::npos || !last_done) {
        size_t end = src_query.find(" ",pos);
        std::string token;
        if(end == std::string::npos) {
            token = src_query.substr(pos,src_query.size()-pos);
            last_done = true;
        }
        else {
            token = src_query.substr(pos,end-pos);
        }
        pos = last_done ? std::string::npos : end + 1;

        //std::cout << "::" << token << "::"<<build_token << std::endl;

        if(last_done) { build_token += " " + token; }

        if(((token == "CONTAINS") |
            (token == "WITHIN") |
            (token == "FOLLOWED") |
            (token == "WHERE") |
            (token == "FROM") |
            (token == "UNION") |
            (token == "INTERSECT") |
            (token == "DIFFERENCE")) || last_done) {
            if(build_token != "") {
                XPathNode* node;
                if(is_xpath) {
                    node = new XPathNode(build_token);
                }
                else {
                    node = get_xpath_from_argument(build_token);
                }
                // std::cout << "+arg:"<<build_token<<std::endl;
                args.push_back(node);
            }
            is_xpath = false;
            build_token = "";
            if(!last_done) {  
                ops.push_back(token); 
            }
        }
        else if((token == "FIND") |
                (token == "BY") |
                (token == "PATTERN")) { /* Do nothing */ }
        else if(token == "XPATH") {
            is_xpath = true;
        }
        else {
            if(build_token == "") { build_token = token; }
            else { build_token += " " + token; }
        }
    }

    //for(auto op : ops) { std::cout << "op:" << op << std::endl; }
    //for(auto arg : args) { std::cout << "arg:" << arg->to_string() << std::endl; }

    XPathNode* top_copy = new XPathNode(*args[0]);
    //std::cout << "COPY: " << top_copy->to_string() << std::endl;

    if(ops.size() != args.size()-1) { return "ASSERT ERROR: OPS and ARGS COUNT OFF"; }

    //top->set_type(ANY);
    // WITHIN Check
    for(size_t i = 0; i < ops.size(); ++i) {
        if(ops[i] == "WITHIN") {
            XPathNode* lhs = args[i];
            XPathNode* rhs = args[i+1];
            rhs->set_type(NO_CONN);
            XPathNode* insert = new XPathNode("ancestor::",PREDICATE);
            insert->add_child_beginning(rhs);
            lhs->add_child(insert);
            ops.erase(ops.begin()+i);
            args.erase(args.begin()+i+1);
            --i;
        }
    }
    // FOLLOWED BY Check
    for(int i = ops.size()-1; i >= 0; --i) {
        if(ops[i] == "FOLLOWED") {
            XPathNode* rhs = args[i+1];
            XPathNode* lhs = args[i];
            XPathNode* call = new XPathNode("qli:intersect",CALL);
            XPathNode* left_arg = new XPathNode("./following::",NO_CONN);
            rhs->set_type(NO_CONN);
            left_arg->add_child(rhs);
            XPathNode* right_arg_end = new XPathNode("descendant::",ANY);
            XPathNode* rhs_copy = new XPathNode(*rhs);
            rhs_copy->set_type(NO_CONN);
            right_arg_end->add_child(rhs_copy);
            XPathNode* add_top = new XPathNode(*top_copy);
            std::cout << "DOUBLE COPY: " << add_top->to_string() << std::endl;
            add_top->set_type(NO_CONN);
            //add_top->add_child(new XPathNode("1",PREDICATE));
            add_top->add_child(right_arg_end);
            XPathNode* right_arg = new XPathNode("./ancestor::");
            right_arg->add_child(add_top);
            call->add_child(left_arg);
            call->add_child(right_arg);

            XPathNode* pred = new XPathNode("",PREDICATE);
            pred->add_child(call);
            lhs->add_child(pred);

            std::cout << "||" << lhs->to_string() << std::endl;

            ops[i] = "CONTAINS";
            args[i+1] = new XPathNode(*rhs);

            // ops.erase(ops.begin()+i);
            // args.erase(args.begin()+i+1);
        }
    }
    // CONTAINS and WHERE check
    for(size_t i = 0; i < ops.size(); ++i) {
        if(ops[i] == "CONTAINS") {
            XPathNode* lhs = args[i];
            XPathNode* rhs = args[i+1];
            rhs->set_type(ANY);
            XPathNode* insert = new XPathNode(".",PREDICATE);
            insert->add_child_beginning(rhs);
            lhs->add_child(insert);
            ops.erase(ops.begin()+i);
            args.erase(args.begin()+i+1);
            --i;
        }
        else if(ops[i] == "WHERE") {
            // TODO
        }
    }

    // Number the add calls and add clears before grouping into set operations
    for(size_t i = 0; i < args.size(); ++i) {
        add_bucket_clears(args[i],i);
        number_add_calls(args[i],i);
    }

    // FROM check
    for(size_t i = 0; i < ops.size(); ++i) {
        if(ops[i] == "FROM") {
            XPathNode* lhs = args[i];
            XPathNode* rhs = args[i+1];
            lhs->set_type(ANY);
            rhs->add_child(lhs);
            ops.erase(ops.begin()+i);
            args.erase(args.begin()+i);
            --i;
        }
    }

    // UNION, INTERSECT, and DIFFERENCE check
    bool flag = true;
    for(size_t i = 0; i < ops.size(); ++i) {
        if(ops[i] == "UNION") {
            flag = false;
            XPathNode* lhs = args[i];
            XPathNode* rhs = args[i+1];
            XPathNode* uni = new XPathNode("",UNION);
            lhs->set_type(ANY);
            rhs->set_type(ANY);
            uni->add_child(rhs);
            lhs->add_child(uni);
            ops.erase(ops.begin()+i);
            args.erase(args.begin()+i+1);
            --i;
        }
        else if (ops[i] == "INTERSECT") {
            flag = false;
            XPathNode* lhs = args[i];
            XPathNode* rhs = args[i+1];
            XPathNode* call = new XPathNode("qli:intersect",CALL);
            lhs->set_type(ANY);
            rhs->set_type(ANY);
            call->add_child(lhs);
            call->add_child(rhs);
            args[i] = call;
            ops.erase(ops.begin()+i);
            args.erase(args.begin()+i+1);
            --i;
        }
        else if (ops[i] == "DIFFERENCE") {
            flag = false;
            XPathNode* lhs = args[i];
            XPathNode* rhs = args[i+1];
            XPathNode* call = new XPathNode("qli:difference",CALL);
            lhs->set_type(ANY);
            rhs->set_type(ANY);
            call->add_child(lhs);
            call->add_child(rhs);
            std::cout << "lhs: " << lhs->to_string() << std::endl;
            std::cout << "rhs: " << rhs->to_string() << std::endl;
            std::cout << "call: " << call->to_string() << std::endl;
            args[i] = call;
            ops.erase(ops.begin()+i);
            args.erase(args.begin()+i+1);
            --i;
        }
    }

    if(flag) { args[0]->set_type(ANY); }

    std::string xpath = args[0]->to_string();

    return xpath;
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

// Moves all add-element calls to the end of it's sibling group
void XPathGenerator::organize_add_calls(XPathNode* x_node) {
    if(x_node->is_variable_node()) {
        x_node->add_child(x_node->pop_child_beginning());
    }
    for(auto xnode : x_node->get_children()) {
        organize_add_calls(xnode);
    }
}

// Finds the closest common ancestor to all add-element calls, and adds a clear call
// for every syntactic variable going through unification
void XPathGenerator::add_bucket_clears(XPathNode* x_node,int group = 0) {
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
        lineages[0][i+1]->add_child_beginning(new XPathNode("qli:clear(\""+locations.first+"_"+std::to_string(group)+"\")", PREDICATE));
    }
}

void XPathGenerator::convert_traverse(xmlNode* top_xml_node, XPathNode* x_node) {
    int child_num = 0;
    for(xmlNode* node = top_xml_node; node != NULL; node = node->next) {
        if(node->type == XML_ELEMENT_NODE && get_full_name(node) != "src:comment") { 
            x_node->set_type(x_node->get_parent() ? PREDICATE : ANY);

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
                if(storage.size_of_variable_bucket(variable) > 0 || prefix != "" || postfix != "") {
                    std::string node_text = "qli:add-element(.,\""+variable+"\",X";
                    if(postfix != "" || prefix != "") {
                        node_text += ","+add_quotes(prefix);
                    }
                    if(postfix != "") {
                        node_text += ","+add_quotes(postfix);
                    }
                    node_text += ")";
                    x_node->add_child(new XPathNode(node_text, PREDICATE));
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
    for(size_t i = 0; i < text.length(); ++i) {
        if(text[i] == '"') {
            return '\'' + text + '\'';
        }
    }
    return '"' + text + '"';
}

std::string XPathGenerator::extract_variable(std::string_view text_view) {
    std::string rtn = "";
    std::string text(text_view);
    for(size_t i = text.find("$")+1; i < text.length() && (isalnum(text[i]) || text[i] == '_'); ++i) { 
        rtn += text[i]; 
    }
    return rtn;
}

void XPathGenerator::get_variable_info(std::string_view text, std::string& variable, size_t& order) {
    std::string s = extract_variable(std::string(text));
    variable = s.substr(0,s.find("_"));
    order = stoi(s.substr(s.find("_")+1,s.length()-1));
}
