/**
 * @file cppCallbackAdapter.hpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * srcSAX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcSAX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * cppCallbackAdapter
 *
 * Wrapper around C srcSAX api to provide for C++ interface.
 * Simply, forward the callbacks to the provided srcSAXHandler.
 */
class cppCallbackAdapter {

private:

    /** the srcSAXHandler to forward the callbacks */
    srcSAXHandler * handler;

public:

    /**
     * cppCallbackAdapter
     * @param handler a srcSAXHandler whose callbacks will be called.
     *
     * Constructor.  Initialize the handler
     */
    cppCallbackAdapter(srcSAXHandler * handler) : handler(handler) {}

    /**
     * factory
     *
     * Factory method to generate the srcsax_handler containin this classes
     * callbacks needed to perform to use the C++ wrapper API.
     *
     * @returns the generated srcsax_handler with the correct callbacks for C API.
     */
    static srcsax_handler factory() {

        srcsax_handler handler;

        handler.start_document = start_document;
        handler.end_document = end_document;
        handler.start_root = start_root;
        handler.start_unit = start_unit;
        handler.start_element = start_element;
        handler.end_root = end_root;
        handler.end_unit = end_unit;
        handler.end_element = end_element;
        handler.characters_root = characters_root;
        handler.characters_unit = characters_unit;
        handler.meta_tag = meta_tag;
        handler.comment = comment;
        handler.cdata_block = cdata_block;
        handler.processing_instruction = processing_instruction;

        return handler;

    }

    /**
     * srcml_element_stack_push
     * @param prefix the element to push prefix
     * @param localname the elements name
     *
     * Push the element to the stack.
     */
    void srcml_element_stack_push(const char * prefix, const char * localname) {

        std::string srcml_element_string = "";
        if(prefix) {

            srcml_element_string += prefix;
            srcml_element_string += ':';

        }

        srcml_element_string += localname;

        handler->get_stack().push_back(srcml_element_string);


    }

    /**
     * srcml_element_stack_pop
     *
     * Pop an element from the stack.  Testing
     * may try to pop with 0 items so simply return.
     */
    void srcml_element_stack_pop() {

        if(handler->get_stack().size() == 0) return;

        handler->get_stack().pop_back();

    }

    /**
     * start_document
     * @param context a srcSAX context
     *
     * Callback. Forwards C API start_document to C++ API srcSAXHandler startDocument.
     */
    static void start_document(struct srcsax_context * context) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->set_encoding(context->encoding);

        cpp_adapter->handler->startDocument();

    }

    /**
     * end_document
     * @param context a srcSAX context
     *
     * Callback. Forwards C API end_document to C++ API srcSAXHandler endDocument.
     */
    static void end_document(struct srcsax_context * context) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->get_stack().clear();

        cpp_adapter->handler->endDocument();


    }

    /**
     * start_root
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Callback. Forwards C API start_root to C++ API srcSAXHandler startRoot.
     */
    static void start_root(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->set_is_archive(context->is_archive);

        if(context->is_archive)
            cpp_adapter->srcml_element_stack_push((const char *)prefix, (const char *)localname);

        cpp_adapter->handler->startRoot(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);

    }

    /**
     * start_unit
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Signature srcSAX handler function for start of an unit.
     * Callback. Forwards C API start_unit to C++ API srcSAXHandler startUnit.
     */
    static void start_unit(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->srcml_element_stack_push((const char *)prefix, (const char *)localname);

        cpp_adapter->handler->startUnit(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);


    }
#if 0
    /**
     * start_function
     * @param context a srcSAX context
     * @param name the function's name
     * @param return_type the function return type
     * @param parameter_list a list of the function parameters in struct containing (declaration.type/declaration.name)
     * @param is_decl indicates if the call is a function declaration (true) or definition (false)
     *
     * Callback. Forwards C API start_function to C++ API srcSAXHandler startFunction.
     */ 
     static void start_function(struct srcsax_context * context, const char * name, const char * return_type, const struct declaration * parameter_list, _Bool is_decl) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;


     }
#endif

    /**
     * start_element
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Signature for srcSAX handler function for start of an element.
     * Callback. Forwards C API start_element to C++ API srcSAXHandler startElement.
     */
    static void start_element(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->srcml_element_stack_push((const char *)prefix, (const char *)localname);

        cpp_adapter->handler->startElement(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);


    }

    /**
     * end_root
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * Callback. Forwards C API end_root to C++ API srcSAXHandler endRoot.
     */
    static void end_root(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->srcml_element_stack_pop();

        cpp_adapter->handler->endRoot(localname, prefix, URI);

    }

    /**
     * end_unit
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * Callback. Forwards C API end_unit to C++ API srcSAXHandler endUnit.
     */
    static void end_unit(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->srcml_element_stack_pop();

        cpp_adapter->handler->endUnit(localname, prefix, URI);

    }
#if 0
    /**
     * end_function
     * @param context a srcSAX context
     *
     * Callback. Forwards C API end_function to C++ API srcSAXHandler endFunction.
     */
    static void end_function(struct srcsax_context * context) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;


    }
#endif
    /**
     * end_element
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * Callback. Forwards C API end_element to C++ API srcSAXHandler endElement.
     */
    static void end_element(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->srcml_element_stack_pop();

        cpp_adapter->handler->endElement(localname, prefix, URI);

    }

    /**
     * characters_root
     * @param context a srcSAX context
     * @param ch the characers
     * @param len number of characters
     *
     * Callback. Forwards C API characters_root to C++ API srcSAXHandler charactersRoot.
     */
    static void characters_root(struct srcsax_context * context, const char * ch, int len) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->charactersRoot(ch, len);


    }

    /**
     * characters_unit
     * @param context a srcSAX context
     * @param ch the characers
     * @param len number of characters
     *
     * Callback. Forwards C API characters_unit to C++ API srcSAXHandler charactersUnit.
     */
    static void characters_unit(struct srcsax_context * context, const char * ch, int len) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->charactersUnit(ch, len);

    }

    /**
     * meta_tag
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Callback. Forwards C API meta_tag to C++ API srcSAXHandler metaTag.
     */
    static void meta_tag(struct srcsax_context * context, const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->srcml_element_stack_push((const char *)prefix, (const char *)localname);

        cpp_adapter->handler->metaTag(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);

        cpp_adapter->srcml_element_stack_pop();

    }

    /**
     * comment
     * @param context a srcSAX context
     * @param value the comment content
     *
     * Callback. Forwards C API comment to C++ API srcSAXHandler comment.
     */
    static void comment(struct srcsax_context * context, const char * value) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->comment(value);

    }

    /**
     * cdata_block
     * @param context a srcSAX context
     * @param value the pcdata content
     * @param len the block length
     *
     * Callback. Forwards C API cdata_block to C++ API srcSAXHandler cdata_block.
     */
    static void cdata_block(struct srcsax_context * context, const char * value, int len) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->cdataBlock(value, len);

    }

    /**
     * processing_instruction
     * @param context a srcSAX context
     * @param target the processing instruction target.
     * @param data the processing instruction data.
     *
     * Callback. Forwards C API processing_instruction to C++ API srcSAXHandler processingInstruction.
     */
    static void processing_instruction(struct srcsax_context * context, const char * target, const char * data) {

        cppCallbackAdapter * cpp_adapter = (cppCallbackAdapter *)context->data;

        cpp_adapter->handler->processingInstruction(target, data);

    }

};
