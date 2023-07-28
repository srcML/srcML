 // SPDX-License-Identifier: GPL-3.0-only
/**
 * @file xpathTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libexslt/exslt.h>

#include <xpathTransformation.hpp>

#include <srcml_translator.hpp>
#include <srcml_sax2_utilities.hpp>
#include <libxml2_utilities.hpp>
#include <memory>

const char* const xpathTransformation::simple_xpath_attribute_name = "location";

#define stringOrNull(m) (m ? m : "")

/**
 * xpathTransformation
 * @param options list of srcML options
 * @param compiled_xpath Combined XPath expression
 * @param prefix an element namepace prefix
 * @param uri an element namepace uri
 * @param element the tag name
 * @param attr_prefix an attribute namepace prefix
 * @param attr_uri an attribute namepace uri
 * @param attr_name the attribute name
 * @param attr_value the attribute value
 * @param fd file descriptor in which to write
 *
 * Constructor.
 */
xpathTransformation::xpathTransformation(srcml_archive* /* oarchive */, const char* xpath,
                        const char* element_prefix, const char* element_uri, const char* element,
                        const char* attr_prefix, const char* attr_uri, const char* attr_name, const char* attr_value)
    : xpath(xpath), prefix(stringOrNull(element_prefix)), uri(stringOrNull(element_uri)), element(stringOrNull(element)), attr_prefix(stringOrNull(attr_prefix)), attr_uri(stringOrNull(attr_uri)), attr_name(stringOrNull(attr_name)), attr_value(stringOrNull(attr_value)) {

    // compile the xpath expression
    // errors will show up when it is first used
    compiled_xpath = xmlXPathCompile(BAD_CAST xpath);

    // create a namespace for the new attribute (if needed)
    if (attr_uri) {
        attr_ns = xmlNewNs(NULL, (const xmlChar *) attr_uri, (const xmlChar *) attr_prefix);
    }

    // create a namespace for the new element (if needed)
    if (element_uri) {
        element_ns = xmlNewNs(NULL, (const xmlChar*) uri.data(), (const xmlChar*) prefix.data());
    }
}

xpathTransformation::~xpathTransformation() {

    // free the compiled xpath
    xmlXPathFreeCompExpr(compiled_xpath);

    // free the namespace for any added attributes
    if (attr_ns)
        xmlFreeNs(attr_ns);

    // free the namespace for any added elements
    if (element_ns)
        xmlFreeNs(element_ns);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * form_simple_xpath
 * @param root_result_node the root node form xpath query result
 *
 * Form a simple xpath expression that marks the location of the result.
 * @returns the simple xpath to the result node as a string.
 */
void xpathTransformation::form_simple_xpath(xmlTextWriterPtr bufwriter, xmlNodePtr root_result_node) {

    if ((!root_result_node) || (root_result_node->type != XML_ELEMENT_NODE) || (!root_result_node->name) || ((const char*) root_result_node->name == "unit"sv)) {
        return;
    }

    form_simple_xpath(bufwriter, root_result_node->parent);

    xmlTextWriterWriteFormatString(bufwriter, "/%s:%s[%d]",
     ((root_result_node->ns && root_result_node->ns->prefix) ? (const char*) root_result_node->ns->prefix : "src"),
     root_result_node->name,
     child_offset(root_result_node)
     );

}

/**
 * child_offset
 * @param root_result_node the root node form xpath query result
 *
 * Find the child offset.
 * @returns the child offset number as a string.
 */
int xpathTransformation::child_offset(xmlNodePtr root_result_node) {

    std::string_view rootName = (const char*) root_result_node->name;
    int child_count = 1;
    for(xmlNodePtr sibling_node = root_result_node->prev; sibling_node; sibling_node = sibling_node->prev) {

        if (sibling_node->type != XML_ELEMENT_NODE)
            continue;

        if (rootName == (const char*) sibling_node->name &&
            ((root_result_node->ns == NULL && sibling_node->ns == NULL) || (root_result_node->ns->prefix == sibling_node->ns->prefix))) {
            ++child_count;

        }
    }

    return child_count;
}

/**
 * append_attribute_to_node
 * @param node an xmlNode to append the attribute
 * @param attr_name the attribute name
 * @param attr_value the attribute value
 *
 * Append an attribute to the given node.  Only the prefix and uri can vary.  The
 * rest are the same throughout all calls and are part of the class.
 */
void xpathTransformation::append_attribute_to_node(xmlNodePtr node, const char* /* attr_prefix */, const char* append_attr_uri) const {

    // grab current value
    const char* value = (char*) xmlGetNsProp(node, BAD_CAST attr_name.data(), BAD_CAST append_attr_uri);
    const char* newvalue = attr_value.data();

    // previous property
    std::string curvalue;
    if (value && std::string_view(value) != newvalue) {

        curvalue = value;
        curvalue += ' ';
        curvalue += attr_value;
        newvalue = curvalue.data();
    }

    xmlNewNsProp(node, attr_ns, (const xmlChar *) attr_name.data(), (const xmlChar *) newvalue);
}

xmlXPathContextPtr xpathTransformation::createContext(xmlDocPtr doc) const {

    auto context = xmlXPathNewContext(doc);

    return context;
}
#pragma GCC diagnostic push

// isStatic() XPath extension function
void isStatic(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:type/src:specifier='static' or (src:specifier='static' and (self::src:class or self::src:constructor) or (self::src:decl_stmt and src:decl/src:type/src:specifier='static'))";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isInline() XPath extension function
void isInline(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:type/src:specifier='inline' or src:specifier='inline'";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isPureVirtual() XPath extension function
void isPureVirtual(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:function or self::src:function_decl) and ((ancestor::src:unit[1]/@language='C++' and src:literal=0) or (ancestor::src:unit[1]/@language='C#' and parent::src:block/parent::src:interface or src:type/src:specifier='abstract') or (ancestor::src:unit[1]/@language='Java' and parent::src:block/parent::src:interface))";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isExplicit() XPath extension function
void isExplicit(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:specifier='explicit' or src:type/src:specifier='explicit' or src:expr/src:specifier='explicit'";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isDeleted() XPath extension function
void isDeleted(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:specifier='delete'";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isConstant() XPath extension function
void isConstant(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:decl or self::src:decl_stmt or self::src:function or self::src:function_decl) and (src:decl/src:type/src:specifier='const' or src:type/src:specifier='const' or src:specifier='const') or (ancestor::src:unit[1]/@language='Java' and src:specifier='final')";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isConst() XPath extension function
void isConst(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:decl or self::src:decl_stmt or self::src:function or self::src:function_decl) and (src:decl/src:type/src:specifier='const' or src:type/src:specifier='const' or src:specifier='const')";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isFinal() XPath extension function
void isFinal(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:decl or self::src:decl_stmt or self::src:function or self::src:function_decl) and (ancestor::src:unit[1]/@language='Java' and src:specifier='final')";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isOperatorFunction() XPath extension function
void isOperatorFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:function or self::src:function_decl) and @type='operator'";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isCopyConstructor() XPath extension function
void isCopyConstructor(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:name = src:parameter_list/src:parameter/src:decl/src:type/src:name and (src:parameter_list/src:parameter/src:decl/src:type/src:modifier!='&&' or not(src:parameter_list/src:parameter/src:decl/src:type/src:modifier)) and (not(src:parameter_list/src:parameter[position() > 1 and not(src:decl/src:init)]) or count(src:parameter_list/src:parameter)=1)";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isMoveConstructor() XPath extension function
void isMoveConstructor(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:parameter_list/src:parameter/src:decl/src:type/src:modifier='&&' and not(src:parameter_list/src:parameter[position() > 1 and not(src:decl/src:init)])";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isDefaultConstructor() XPath extension function
void isDefaultConstructor(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:constructor or self::src:constructor_decl) and not(src:parameter_list/src:parameter[not(src:decl/src:init)])";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isCopyAssignmentOperator() XPath extension function
void isCopyAssignmentOperator(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "@type='operator' and src:name/src:name='=' and (src:parameter_list/src:parameter/src:decl/src:type/src:modifier[1]!='&&' or not(src:parameter_list/src:parameter/src:decl/src:type/src:modifier)) and count(src:parameter_list/src:parameter)=1 and src:parameter_list/src:parameter/src:decl/src:type/src:name=src:type/src:name";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isMoveAssignmentOperator() XPath extension function
void isMoveAssignmentOperator(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "@type='operator' and src:name/src:name='=' and src:parameter_list/src:parameter/src:decl/src:type/src:modifier[1]='&&' and src:parameter_list/src:parameter/src:decl/src:type/src:name=src:type/src:name";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isMethod() XPath extension function
void isMethod(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(self::src:function or self::src:function_decl) and (../parent::src:block/parent::src:class or parent::src:block/parent::src:class)";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isFreeFunction() XPath extension function
void isFreeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "not(ancestor-or-self::src:class) and (self::src:function or self::src:function_decl)";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isStatement() XPath extension function
void isStatement(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "self::src:assert or self::src:block or self::src:break or self::src:case or self::src:checked or self::src:continue or self::src:default or self::src:do or self::src:empty_stmt or self::src:expr_stmt or self::src:finally or self::src:fixed or self::src:for or self::src:foreach or self::src:goto or self::src:if_stmt or self::src:label or self::src:lock or self::src:return or self::src:switch or self::src:unchecked or self::src:unsafe or self::src:using_stmt or self::src:while";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isDeclaration() XPath extension function
void isDeclaration(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "self::src:function_decl or self::src:decl_stmt or self::src:class_decl or self::src:struct_decl or self::src:typedef";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isDefault() XPath extension function
void isDefault(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:specifier='default'";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isMutable() XPath extension function
void isMutable(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "src:specifier='mutable'";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isPublic() XPath extension function
void isPublic(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(ancestor::src:unit[1]/@language='C++' and parent::src:public) or (ancestor::src:unit[1]/@language=('C#' or 'Java') and src:type/src:specifier='public')";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isPrivate() XPath extension function
void isPrivate(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(ancestor::src:unit[1]/@language='C++' and parent::src:private) or (ancestor::src:unit[1]/@language=('C#' or 'Java') and src:type/src:specifier='private')";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}
// isProtected() XPath extension function
void isProtected(xmlXPathParserContextPtr ctxt, int nargs) {
    const static xmlChar* xpathExpr = BAD_CAST "(ancestor::src:unit[1]/@language='C++' and parent::src:protected) or (ancestor::src:unit[1]/@language=('C#' or 'Java') and src:type/src:specifier='protected')";
    extensionBooleanFunctionHandler(ctxt, nargs, xpathExpr);
}

// XPath extension function handler
void extensionBooleanFunctionHandler(xmlXPathParserContextPtr ctxt, int nargs, const xmlChar* xpathExpr) {
    // check if the number of arguments is correct
    if (nargs != 1) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    // pop the argument from the stack
    xmlXPathObjectPtr obj = valuePop(ctxt);

    // check if the argument is a nodeset
    if (obj->type != XPATH_NODESET) {
        xmlXPathFreeObject(obj);
        xmlXPathSetTypeError(ctxt);
        return;
    }

    int flag = 0;

    // Loop through all the nodes in the nodeset
    for (int i = 0; i < obj->nodesetval->nodeNr; i++) {

        xmlNodePtr node = obj->nodesetval->nodeTab[i];

        // temporarily set the context node to the current node
        xmlNodePtr oldNode = ctxt->context->node;
        ctxt->context->node = node;

        // evaluate the XPath expression on the temporary context node
        xmlXPathObject* result = xmlXPathEvalExpression(xpathExpr, ctxt->context);

        // restore the context node
        ctxt->context->node = oldNode;

        // find any true results
        if (result != NULL && result->type == XPATH_BOOLEAN && result->boolval) {
            flag = 1;
            xmlXPathFreeObject(result);
            break;
        }

        xmlXPathFreeObject(result);
    }

    // Free the input object
    xmlXPathFreeObject(obj);

    // Push the result back to the stack.
    valuePush(ctxt, xmlXPathNewBoolean(flag));
}

/**
 * apply
 *
 * Apply XPath expression, writing results.
 *
 * @returns true on success false on failure.
 */
TransformationResult xpathTransformation::apply(xmlDocPtr doc, int /* position */) const {

    std::unique_ptr<xmlXPathContext> context(createContext(doc));
    if (!context) {
        fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
        return TransformationResult();
    }

    // register standard prefixes for standard namespaces
    for (const auto& ns : default_namespaces) {

        const char* registerURI = ns.uri.data();
        const char* registerPrefix = ns.prefix.data();
        if (ns.uri == SRCML_SRC_NS_URI)
            registerPrefix = "src";

        if (xmlXPathRegisterNs(context.get(), BAD_CAST registerPrefix, BAD_CAST registerURI) == -1) {
            fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", registerPrefix, registerURI);
        }
    }

    // register prefixes from the doc
    for (auto p = doc->children->nsDef; p; p = p->next) {

        xmlXPathRegisterNs(context.get(), p->prefix, p->href);
    }

    // register extension functions
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isStatic", BAD_CAST "http://www.srcML.org/srcML/src", isStatic);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isInline", BAD_CAST "http://www.srcML.org/srcML/src", isInline);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isPureVirtual", BAD_CAST "http://www.srcML.org/srcML/src", isPureVirtual);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isExplicit", BAD_CAST "http://www.srcML.org/srcML/src", isExplicit);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isDeleted", BAD_CAST "http://www.srcML.org/srcML/src", isDeleted);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isConstant", BAD_CAST "http://www.srcML.org/srcML/src", isConstant);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isConst", BAD_CAST "http://www.srcML.org/srcML/src", isConst);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isFinal", BAD_CAST "http://www.srcML.org/srcML/src", isFinal);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isOperatorFunction", BAD_CAST "http://www.srcML.org/srcML/src", isOperatorFunction);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isCopyConstructor", BAD_CAST "http://www.srcML.org/srcML/src", isCopyConstructor);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isMoveConstructor", BAD_CAST "http://www.srcML.org/srcML/src", isMoveConstructor);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isDefaultConstructor", BAD_CAST "http://www.srcML.org/srcML/src", isDefaultConstructor);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isCopyAssignmentOperator", BAD_CAST "http://www.srcML.org/srcML/src", isCopyAssignmentOperator);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isMoveAssignmentOperator", BAD_CAST "http://www.srcML.org/srcML/src", isMoveAssignmentOperator);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isMethod", BAD_CAST "http://www.srcML.org/srcML/src", isMethod);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isFreeFunction", BAD_CAST "http://www.srcML.org/srcML/src", isFreeFunction);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isStatement", BAD_CAST "http://www.srcML.org/srcML/src", isStatement);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isDeclaration", BAD_CAST "http://www.srcML.org/srcML/src", isDeclaration);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isDefault", BAD_CAST "http://www.srcML.org/srcML/src", isDefault);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isMutable", BAD_CAST "http://www.srcML.org/srcML/src", isMutable);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isPublic", BAD_CAST "http://www.srcML.org/srcML/src", isPublic);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isPrivate", BAD_CAST "http://www.srcML.org/srcML/src", isPrivate);
    xmlXPathRegisterFuncNS(context.get(), BAD_CAST "isProtected", BAD_CAST "http://www.srcML.org/srcML/src", isProtected);
    xmlXPathRegisterNs(context.get(), BAD_CAST "src", BAD_CAST "http://www.srcML.org/srcML/src");

    // evaluate the xpath
    std::unique_ptr<xmlXPathObject> result_nodes(xmlXPathCompiledEval(compiled_xpath, context.get()));
    if (!result_nodes) {
        fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
        return TransformationResult();
    }

    TransformationResult tresult;
    tresult.unitWrapped = false;

    // xpath evaluation produces a nodeset result, even if there are no results
    tresult.nodeType = result_nodes->type == XPATH_NODESET && (result_nodes->nodesetval == nullptr || result_nodes->nodesetval->nodeNr == 0)? 0 : result_nodes->type;

    // update scalar values, if the type is right
    if (result_nodes->type == XPATH_NUMBER) {
        tresult.numberValue = result_nodes->floatval;
    }
    if (result_nodes->type == XPATH_BOOLEAN) {
        tresult.boolValue = result_nodes->boolval != 0;
    }
    if (result_nodes->type == XPATH_STRING) {
        tresult.stringValue = (const char*) result_nodes->stringval;
    }

    // when result is not a nodeset, then return nullptr, and the calling code will check the other values
    if (result_nodes->type != XPATH_NODESET)
        return tresult;

    if (!element.empty()) {

        addElementXPathResults(doc, result_nodes.get());

        tresult.unitWrapped = true;
        tresult.nodeType = SRCML_RESULT_UNITS;
        tresult.nodeset.reset(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc)));

        return tresult;
    }

    // convert all the found nodes
    if (!attr_name.empty()) {
        if (result_nodes->nodesetval) {
            for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

                xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

                append_attribute_to_node(onode, attr_prefix.data(), attr_value.data());
            }
        }
        tresult.unitWrapped = true;
        tresult.nodeType = SRCML_RESULT_UNITS;
        tresult.nodeset.reset(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc)));

        return tresult;
    }

    if (!result_nodes->nodesetval)
        return tresult;

    if (!result_nodes->nodesetval->nodeNr)
        return tresult;

    if (result_nodes->nodesetval->nodeTab[0] && result_nodes->nodesetval->nodeTab[0]->name &&
        (const char*) result_nodes->nodesetval->nodeTab[0]->name == "unit"sv)
        tresult.unitWrapped = true;

    tresult.nodeset.reset(result_nodes->nodesetval);
    result_nodes->nodesetval = nullptr;

    return tresult;
}

// process the resulting nodes
void xpathTransformation::addElementXPathResults(xmlDocPtr doc, xmlXPathObjectPtr result_nodes) const {

    if (!result_nodes || !(result_nodes->type == 1) || !(result_nodes->nodesetval))
        return;

    xmlNodePtr a_node = xmlDocGetRootElement(doc);

    // add the element to all nodes
    for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

        xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

        // set up node to insert
        xmlNodePtr element_node = xmlNewNode(element_ns, (const xmlChar*) element.data());

        if (!attr_name.empty())
            append_attribute_to_node(element_node, !attr_uri.empty() ? attr_prefix.data() : prefix.data(),
                !attr_uri.empty() ? attr_uri.data() : uri.data());

        // result node is not a unit
        if (a_node != onode) {

            xmlReplaceNode(onode, element_node);
            xmlAddChild(element_node, onode);

        // result node is a unit
        } else {

            element_node->children = onode->children;
            element_node->last = onode->last;
            element_node->parent = onode;
            element_node->next = 0;
            element_node->prev = 0;
            onode->children = element_node;
            onode->last = element_node;
        }
    }
}
