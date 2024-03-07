#include "xpath_generator.hpp"
#include <srcml.h>
#include <algorithm>
#include <map>
#include <memory>

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
    if(x_node->is_add_call_node() || x_node->is_match_call_node()) {
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

void change_adds_to_matches(XPathNode* node) {
    std::string node_text = node->get_text();
    if(node_text.find("qli:add-element",0) != std::string::npos) {
        node_text.replace(node_text.find("qli:add-element",0),15,"qli:match-element");
        int start_quote = node_text.find("\"",0);
        int end_quote = node_text.find("\"",start_quote+1);
        node_text.replace(node_text.find("X",end_quote),1,"0");
        //node_text.replace(node_text.find(")",0)-2,2,"");
        node->set_text(node_text);
    }
    for(auto child : node->get_children()) {
        change_adds_to_matches(child);
    }
}

void add_inner_specifier(XPathNode* node, int scope_count) {
    std::string node_text = node->get_text();
    if(node_text.find("qli:add-element",0) != std::string::npos) {
        //std::cout << node_text << " -> ";
        int start = node_text.find("qli:add-element",0);
        int start_quote = node_text.find("\"",start);
        int end_quote = node_text.find("\"",start_quote+1);
        std::string identifier = node_text.substr(start_quote+1, end_quote-start_quote-1);
        node_text.insert(end_quote,"_INNER_"+std::to_string(scope_count));
        node->set_text(node_text);
    }
    for(auto child : node->get_children()) {
        add_inner_specifier(child,scope_count);
    }
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
    else if(node_text.find("qli:match-element",0) != std::string::npos) {
        //std::cout << node_text << " -> ";
        int start = node_text.find("qli:match-element",0);
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
    else if(node_text.find("qli:regex-match",0) != std::string::npos) {
        XPathNode* id_child = node->get_children()[0];
        std::string id_text = id_child->get_text();
        int end_quote = node_text.find("\"",1);
        id_text.insert(id_text.size()-1,"_"+std::to_string(group));
        id_child->set_text(id_text);
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
    std::vector<std::string> operations;
    std::vector<XPathNode*> source_exprs;

    std::vector<std::string> tokens = split(src_query, " ");
    // int a_1 = 0;
    // for(auto a_2 : tokens) { std::cout << a_1++ << ": " << a_2 << std::endl; }
    // std::cout << std::endl; a_1 = 0;
    /*****
    Loop through each token within the provides srcQL query.

    Each operation and source code expresion is parsed out and saved to the respective
    vector.

    Position of operations is important - an operation at position N operates on the
    source code expressions which are at N and N+1
    *****/
    std::string expr_type = "";
    std::string build_expr = "";
    bool is_where_clause = false;
    int inner_id = 0;
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        // FIND - no-op, does nothing
        if (token == "FIND") { /* Do nothing */}

        // Other no-ops
        else if (token == "BY") { /* Do nothing */ }

        // srcQL operators - save whatever the current built token is as the next expr
        else if (token == "CONTAINS"   ||
                 token == "FOLLOWED"   ||
                 token == "WITHIN"     ||
                 token == "WHERE"      ||
                 token == "FROM"       ||
                 token == "UNION"      ||
                 token == "INTERSECT"  ||
                 token == "DIFFERENCE" ||
                 i == tokens.size() - 1) {
            // First, if last token, do some tweaks
            if (i == tokens.size() - 1) {
                if(build_expr == "") { build_expr = token; }
                else { build_expr += " " + token; }
                token = "END";
            }

            XPathNode* node;

            // If this is a where clause, do different steps
            if(!is_where_clause) {
                // Then determine expr type if not set
                if (expr_type == "") {
                    // XPATH
                    if (build_expr.find("/",0) == 0) { expr_type = "XPATH"; }
                    else if(build_expr.find(" ",0) == std::string::npos &&
                            build_expr.find(":",0) != std::string::npos) {
                        if (split(build_expr,":").size() <= 2) {
                            expr_type = "TAG";
                        }
                        else { expr_type = "PATTERN"; }
                    }
                    else {
                        expr_type = "PATTERN";
                    }
                }
                // Next, convert into an XPathNode
                if (expr_type == "PATTERN") {
                    node = get_xpath_from_argument(build_expr);
                }
                else if (expr_type == "XPATH") {
                    int count = 0;
                    while(build_expr.find("/",0) == 0) {
                        ++count;
                        build_expr.erase(0,1);
                    }
                    node = new XPathNode(build_expr);
                    if(count == 1) { node->set_type(NEXT); }
                }
                else if (expr_type == "TAG") {
                    node = new XPathNode(build_expr);
                }
            }
            else {
                // Need to check what kind of where clause
                // NOT([source_expr])
                if(build_expr.find("NOT",0) == 0) {
                    size_t start_paren = build_expr.find_first_of("(");
                    size_t end_paren = build_expr.find_last_of(")");
                    std::string not_content = build_expr.substr(start_paren+1,end_paren - start_paren - 1);
                    node = get_xpath_from_argument(not_content);
                    add_inner_specifier(node,inner_id);
                    ++inner_id;
                    operations.push_back("WHERE-NOT");
                }
                // MATCH([logical_variable],[regex])
                else if(build_expr.find("MATCH",0) == 0) {
                    size_t start_paren = build_expr.find_first_of("(");
                    size_t end_paren = build_expr.find_last_of(")");
                    std::string match_content = build_expr.substr(start_paren+1,end_paren - start_paren - 1);
                    std::vector<std::string> args = split(match_content,",");
                    node = new XPathNode("qli:regex-match",CALL);
                    node->add_child(new XPathNode("\""+args[0]+"\""));
                    node->add_child(new XPathNode(args[1]));
                    operations.push_back("WHERE-MATCH");
                }
                // COUNT([source_expr]) and other predicates
                else {
                    node = new XPathNode("",NO_CONN);
                    XPathNode* insert_point = node;
                    std::string other_clause = build_expr;
                    int start = 0;
                    while(other_clause.find("COUNT",start) != std::string::npos) {
                        int count_pos = other_clause.find("COUNT",start);
                        XPathNode* extra_text = new XPathNode(other_clause.substr(start,count_pos-start),NO_CONN);
                        insert_point->add_child(extra_text);
                        insert_point = extra_text;
                        int start_paren = other_clause.find("(",count_pos);
                        int i = start_paren+1;
                        int count = 1;
                        while (count != 0) {
                            if (other_clause[i] == '(') { ++count; }
                            else if (other_clause[i] == ')') { --count; }
                            i++;
                        }
                        int end_paren = i;
                        start = end_paren;
                        std::string count_expr = other_clause.substr(start_paren+1,end_paren-start_paren-2);\

                        // Determine type of expr, and convert to XPathNode
                        XPathNode* inner;
                        if (count_expr.find("/",0) == 0) { 
                            int count = 0;
                            while(count_expr.find("/",0) == 0) {
                                ++count;
                                count_expr.erase(0,1);
                            }
                            inner = new XPathNode(count_expr);
                            if(count == 1) { inner->set_type(NEXT); }
                        }
                        else if(count_expr.find(" ",0) == std::string::npos &&
                                count_expr.find(":",0) != std::string::npos) {
                            if (split(count_expr,":").size() == 2) {
                                inner = new XPathNode(count_expr);
                            }
                            else { inner = get_xpath_from_argument(count_expr); }
                        }
                        else {
                            inner = get_xpath_from_argument(count_expr);
                        }
                        if (inner->get_type() != NEXT) { inner->set_type(ANY); }
                        XPathNode* call_arg = new XPathNode(".",NO_CONN);
                        call_arg->add_child(inner);
                        XPathNode* count_call = new XPathNode("count",CALL);
                        count_call->add_child(call_arg);
                        insert_point->add_child(count_call);
                    }
                    insert_point->add_child(new XPathNode(other_clause.substr(start,other_clause.size() - start)));
                    add_inner_specifier(node,inner_id);
                    ++inner_id;
                    operations.push_back("WHERE-COUNT");
                }

                is_where_clause = false;
            }
            // Add node and operation to vectors
            if (token == "WHERE") { is_where_clause = true; }
            else if (token != "END") { operations.push_back(token); }
            source_exprs.push_back(node);
            // std::cout << (a_1++) << ": " << build_expr << "\n" << (a_1++) << ": " << token << std::endl;
            // Reset strings
            expr_type = "";
            build_expr = "";
        }

        // Expression specifiers - set the next build token to be a specific type
        else if (token == "PATTERN" ||
                 token == "XPATH"   ||
                 token == "TAG") {
            expr_type = token;
        }

        // Part of an expression - add to current build token
        else {
            if(build_expr == "") { build_expr = token; }
            else { build_expr += " " + token; }
        }
    }

    // std::cout << std::endl;
    // for(auto op : operations) { std::cout << "op:" << op << std::endl; }
    // for(auto arg : source_exprs) { std::cout << "arg:" << arg->to_string() << std::endl; }
    // std::cout << "---------------------" << std::endl;


    XPathNode* top_copy = new XPathNode(*source_exprs[0]);

    if(operations.size() != source_exprs.size()-1) { return "ASSERT ERROR: OPS and ARGS COUNT OFF"; }

    /* WHERE NOT check
     *     x WHERE not(y)
     *     x[not(self::y)]
     */
    for(size_t i = 0; i < operations.size(); ++i) {
        if(operations[i] == "WHERE-NOT") {
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            rhs->set_type(NO_CONN);
            XPathNode* self_axis = new XPathNode("self::",NO_CONN);
            self_axis->add_child_beginning(rhs);
            XPathNode* not_call = new XPathNode("not",CALL);
            not_call->add_child_beginning(self_axis);
            XPathNode* empty_pred = new XPathNode("",PREDICATE);
            empty_pred->add_child(not_call);
            lhs->add_child(empty_pred);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
    }

    /* WHERE COUNT check
     *     x WHERE not(y)
     *     x[not(self::y)]
     */
    for(size_t i = 0; i < operations.size(); ++i) {
        if(operations[i] == "WHERE-COUNT") {
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            rhs->set_type(PREDICATE);
            lhs->add_child(rhs);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
    }

    /* WITHIN Check
     *     x WITHIN y
     *     x[ancestor::y]
     */
    for(size_t i = 0; i < operations.size(); ++i) {
        if(operations[i] == "WITHIN") {
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            XPathNode* insert;
            if(rhs->get_type() == PARENTHESES) {
                auto terms = split(rhs->get_text(),"|");
                rhs->set_text(std::string("ancestor::"+split(terms[0],"//")[0]+"|ancestor::"+split(terms[1],"//")[0]));
                insert = new XPathNode("",PREDICATE);
            }
            else {
                rhs->set_type(NO_CONN);
                insert = new XPathNode("ancestor::",PREDICATE);
            }

            insert->add_child_beginning(rhs);
            lhs->add_child(insert);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
    }
    /* FOLLOWED BY Check
     *     x CONTAINS y FOLLOWED BY z
     *     x[.//y[qli:intersect(./following::z,./ancestor::x//descendant::z)]][.//z]
     */
    for(int i = operations.size()-1; i >= 0; --i) {
        if(operations[i] == "FOLLOWED") {
            XPathNode* rhs = source_exprs[i+1]; // y
            XPathNode* lhs = source_exprs[i]; // z
            XPathNode* call = new XPathNode("set:intersection",CALL);
            XPathNode* left_arg = new XPathNode("./following::",NO_CONN);
            XPathNode* rhs_copy = new XPathNode(*rhs);
            XPathNode* insert = new XPathNode(*rhs);

            // Left Argument
            if(rhs->get_type() == PARENTHESES) {
                auto terms = split(rhs->get_text(),"|");
                rhs->set_text(std::string("./following::"+split(terms[0],"//")[0]+"|./following::"+split(terms[1],"//")[0]));
                left_arg->set_text("");
            }
            else {
                rhs->set_type(NO_CONN);
            }
            left_arg->add_child(rhs);

            // Right Argument Z part
            XPathNode* right_arg;
            XPathNode* right_arg_end;
            if(rhs_copy->get_type() == PARENTHESES) {
                right_arg_end = new XPathNode("descendant::*",ANY);
                XPathNode* temp = new XPathNode("",PREDICATE);
                auto terms = split(rhs_copy->get_text(),"|");
                rhs_copy->set_text(std::string("self::"+split(terms[0],"//")[0]+"|self::"+split(terms[1],"//")[0]));
                temp->add_child(rhs_copy);
                rhs_copy = temp;
            }
            else {
                right_arg_end = new XPathNode("descendant::",ANY);
                rhs_copy->set_type(NO_CONN);
            }
            right_arg_end->add_child(rhs_copy);

            // Right Argument X part
            XPathNode* add_top = new XPathNode(*top_copy);
            if(add_top->get_type() == PARENTHESES) {
                auto terms = split(add_top->get_text(),"|");
                add_top->set_text(std::string("./ancestor::"+split(terms[0],"//")[0]+"|./ancestor::"+split(terms[1],"//")[0]));
                add_top->add_child(right_arg_end);
                right_arg = new XPathNode("");
            }
            else {
                add_top->set_type(NO_CONN);
                add_top->add_child(right_arg_end);
                right_arg = new XPathNode("./ancestor::");
            }

            right_arg->add_child(add_top);

            change_adds_to_matches(right_arg);
            call->add_child(left_arg);
            call->add_child(right_arg);

            XPathNode* pred = new XPathNode("",PREDICATE);
            pred->add_child(call);
            lhs->add_child(pred);

            operations[i] = "CONTAINS";

            change_adds_to_matches(insert);
            source_exprs[i+1] = insert;

            // operations.erase(operations.begin()+i);
            // source_exprs.erase(source_exprs.begin()+i+1);
        }
    }
    /* CONTAINS check
     *    x CONTAINS y
     *    x[.//y]
     */
    for(size_t i = 0; i < operations.size(); ++i) {
        if(operations[i] == "CONTAINS") {
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            XPathNode* insert = new XPathNode(".",PREDICATE);
            if(rhs->get_type() == PARENTHESES) {
                auto terms = split(rhs->get_text(),"|");
                rhs->set_text(std::string(".")+terms[0]+"|."+terms[1]);
                insert->set_text("");
            }
            else {
                rhs->set_type(ANY);
            }
            insert->add_child_beginning(rhs);
            lhs->add_child(insert);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
    }


    /* WHERE-MATCH check
     *     x WHERE match($,"reg")
     *     x[qli:regex-match("$","reg")]
     *
     */
    for(size_t i = 0; i < operations.size(); ++i) {
        if(operations[i] == "WHERE-MATCH") {
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            XPathNode* empty_pred = new XPathNode("",PREDICATE);
            empty_pred->add_child(rhs);
            lhs->add_child_beginning(empty_pred);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
    }

    // Number the add calls and add clears before grouping into set operations
    for(size_t i = 0; i < source_exprs.size(); ++i) {
        add_bucket_clears(source_exprs[i],i);
        number_add_calls(source_exprs[i],i);
    }

    /* FROM check
     *     FIND x FROM FIND y
     *     //y//x
     */
    for(size_t i = 0; i < operations.size(); ++i) {
        if(operations[i] == "FROM") {
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];

            if(lhs->get_type() == PARENTHESES) {
                XPathNode* pred_temp = new XPathNode("",PREDICATE);
                XPathNode* any_temp = new XPathNode("descendant::*",ANY);
                auto terms = split(lhs->get_text(),"|");
                lhs->set_text(std::string("self::"+split(terms[0],"//")[0]+"|self::"+split(terms[1],"//")[0]));
                pred_temp->add_child(lhs);
                any_temp->add_child(pred_temp);
                lhs = any_temp;
            }
            else {
                if(lhs->get_type() != NEXT) {
                    lhs->set_type(ANY);
                }
            }
            
            rhs->add_child(lhs);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i);
            --i;
        }
    }


    // UNION, INTERSECT, and DIFFERENCE check
    bool is_a_call = false;
    for(size_t i = 0; i < operations.size(); ++i) {
        /* UNION check
         *     FIND x UNION FIND y
         *     //x|//y
         */
        if(operations[i] == "UNION") {
            is_a_call = true;
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            XPathNode* uni = new XPathNode("",UNION);
            if(lhs->get_type() != CALL && lhs->get_type() != PARENTHESES) {
                lhs->set_type(ANY);
            }
            if(rhs->get_type() != CALL && rhs->get_type() != PARENTHESES) {
                rhs->set_type(ANY);
            }
            uni->add_child(rhs);
            lhs->add_child(uni);
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
        /* INTERSECT check
         *     FIND x INTERSECT FIND y
         *     qli:intersect(//x,//y)
         */
        else if (operations[i] == "INTERSECT") {
            is_a_call = true;
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            XPathNode* call = new XPathNode("set:intersection",CALL);
            if(lhs->get_type() != CALL && lhs->get_type() != PARENTHESES) {
                lhs->set_type(ANY);
            }
            if(rhs->get_type() != CALL && rhs->get_type() != PARENTHESES) {
                rhs->set_type(ANY);
            }
            call->add_child(lhs);
            call->add_child(rhs);
            source_exprs[i] = call;
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
        /* DIFFERENCE check
         *     FIND x DIFFERENCE FIND y
         *     qli:difference(//x,//y)
         */
        else if (operations[i] == "DIFFERENCE") {
            is_a_call = true;
            XPathNode* lhs = source_exprs[i];
            XPathNode* rhs = source_exprs[i+1];
            XPathNode* call = new XPathNode("set:difference",CALL);
            if(lhs->get_type() != CALL && lhs->get_type() != PARENTHESES) {
                lhs->set_type(ANY);
            }
            if(rhs->get_type() != CALL && rhs->get_type() != PARENTHESES) {
                rhs->set_type(ANY);
            }
            call->add_child(lhs);
            call->add_child(rhs);
            source_exprs[i] = call;
            operations.erase(operations.begin()+i);
            source_exprs.erase(source_exprs.begin()+i+1);
            --i;
        }
    }


    if(!is_a_call && source_exprs[0]->get_type() != PARENTHESES) { source_exprs[0]->set_type(ANY); }

    std::string xpath = source_exprs[0]->to_string();

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
                // Special check for converting expr patterns to include decls
                if(get_full_name(node) == "src:expr_stmt" && get_full_name(node->children) == "src:expr" && xmlChildElementCount(node->children) == 1 && get_full_name(node->children->children) == "src:name") {

                    if(x_node->get_parent() == nullptr) {
                        x_node->set_type(PARENTHESES);
                        x_node->set_text("//src:expr_stmt|//src:decl_stmt");
                    }
                    else { 
                        XPathNode* parentheses = new XPathNode(child_num != 0 ? "following-sibling::src:expr_stmt|following-sibling::src:decl_stmt" : "src:expr_stmt|src:decl_stmt",PARENTHESES);
                        x_node->add_child(parentheses);
                        x_node = parentheses;
                    }
                }
                else if(get_full_name(node) == "src:expr" && xmlChildElementCount(node) == 1 && get_full_name(node->children) == "src:name") {

                    if(x_node->get_parent() == nullptr) {
                        x_node->set_type(PARENTHESES);
                        x_node->set_text("//src:expr|//src:decl");
                    }
                    else { 
                        XPathNode* parentheses = new XPathNode(child_num != 0 ? "following-sibling::src:expr|following-sibling::src:decl" : "src:expr|src:decl",PARENTHESES);
                        x_node->add_child(parentheses);
                        x_node = parentheses;
                    }
                }
                else { // Otherwise, just add the tag normally
                    x_node->set_text((child_num != 0 ? "following-sibling::" : "") + get_full_name(node));
                }

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
