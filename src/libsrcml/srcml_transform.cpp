// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_transform.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#include <srcml_options.hpp>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#ifdef _MSC_VER
#include <io.h>
#endif

#include <libxml/parser.h>
#include <libxml/xmlIO.h>

#include <libxml/parserInternals.h>
#include <libxml/tree.h>

#include <xsltTransformation.hpp>
#include <xpathTransformation.hpp>
#include <relaxngTransformation.hpp>

#include <libxml2_utilities.hpp>

#include <unit_utilities.hpp>

#include <algorithm>
#include <cstring>
#include <vector>
#include <optional>

/**
 * Transformation result. Passed to srcml_unit_apply_transforms() to collect results of transformation
 */
struct srcml_transform_result {
    /** Transformation result type */
    int type;
    /** Array of srcml units for type SRCML_RESULT_UNITS */
    std::vector<srcml_unit*> units;
    /** Result for type SRCML_RESULT_BOOLEAN */
    std::optional<int> boolValue;
    /** Result for type SRCML_RESULT_NUMBER */
    std::optional<double> numberValue;
    /** Result for type SRCML_RESULT_STRING */
    std::optional<std::string> stringValue;
};

/**
 * srcml_append_transform_xpath_internal( * @param archive a srcml archive
 * @param xpath_string an XPath expression
 * @param prefix the element prefix
 * @param namespace_uri the element namespace
 * @param element the element name
 *
 * Append the XPath expression to the list
 * of transformation/queries.  As of yet no way to specify context.
 * Instead of outputting the results each in a separte unit tag.  Output the complete
 * archive marking the xpath results with a user provided element.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
static int srcml_append_transform_xpath_internal (struct srcml_archive* archive, const char* xpath_string,
                                                    const char* prefix, const char* namespace_uri,
                                                    const char* element,
                                                    const char* attr_prefix, const char* attr_namespace_uri,
                                                    const char* attr_name, const char* attr_value) {
    if (archive == NULL || xpath_string == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->transformations.push_back(std::unique_ptr<Transformation>(new xpathTransformation(archive, xpath_string, prefix, namespace_uri, element,
            attr_prefix, attr_namespace_uri, attr_name, attr_value)));

    return SRCML_STATUS_OK;
}

/**
 * srcml_append_transform_xpath
 * @param archive a srcml archive
 * @param xpath_string an XPath expression
 *
 * Append the XPath expression to the list
 * of transformation/queries.  As of yet no way to specify context
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xpath(srcml_archive* archive, const char* xpath_string) {

    if (archive == nullptr || xpath_string == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_xpath_internal(archive, xpath_string, 0, 0, 0, 0, 0, 0, 0);
}

/**
 * srcml_append_transform_xpath_attribute
 * @param archive a srcml archive
 * @param xpath_string an XPath expression
 * @param prefix the attribute prefix
 * @param namespace_uri the attribute namespace
 * @param attr_name the attribute name
 * @param attr_value the attribute value
 *
 * Append the XPath expression to the list
 * of transformation/queries.  As of yet no way to specify context.
 * Instead of outputting the results each in a separate unit tag.  Output the complete
 * archive marking the xpath results with a user provided attribute.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xpath_attribute(struct srcml_archive* archive, const char* xpath_string,
                                            const char* prefix, const char* namespace_uri,
                                            const char* attr_name, const char* attr_value) {

    if (archive == nullptr || xpath_string == nullptr ||
        prefix == nullptr || namespace_uri == nullptr ||
        attr_name == nullptr || attr_value == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // attribute for a previous Xpath where the attribute is blank is appended on
    if (!archive->transformations.empty()) {
        auto p = dynamic_cast<xpathTransformation*>(archive->transformations.back().get());
        if (p && p->xpath == xpath_string && p->attr_prefix.empty() && p->attr_uri.empty() && p->attr_name.empty() && p->attr_value.empty()) {

            p->attr_prefix = prefix;
            p->attr_uri = namespace_uri;
            p->attr_name = attr_name;
            p->attr_value = attr_value;

            return SRCML_STATUS_OK;
        }
    }

    return srcml_append_transform_xpath_internal(archive, xpath_string, 0, 0, 0, prefix, namespace_uri, attr_name, attr_value);
}

/**
 * srcml_append_transform_xpath_element
 * @param archive a srcml archive
 * @param xpath_string an XPath expression
 * @param prefix the element prefix
 * @param namespace_uri the element namespace
 * @param element the element name
 *
 * Append the XPath expression to the list
 * of transformation/queries.  As of yet no way to specify context.
 * Instead of outputting the results each in a separte unit tag.  Output the complete
 * archive marking the xpath results with a user provided element.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xpath_element(struct srcml_archive* archive, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element) {
    if (archive == nullptr || xpath_string == nullptr ||
        prefix == nullptr || namespace_uri == nullptr ||
        element == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_xpath_internal(archive, xpath_string, prefix, namespace_uri, element, 0, 0, 0, 0);
}

/**
 * srcml_append_transform_xslt_internal
 * @param archive a srcml_archive
 * @param doc XSLT xml document
 *
 * Append the XSLT program filename path to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
static int srcml_append_transform_xslt_internal(srcml_archive* archive, std::unique_ptr<xmlDoc> doc) {

    if (archive == NULL || doc == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->transformations.push_back(std::unique_ptr<Transformation>(new xsltTransformation(doc.release(), std::vector<std::string>())));

    return SRCML_STATUS_OK;
}

/**
 * srcml_append_transform_xslt_filename
 * @param archive a srcml_archive
 * @param xslt_filename an XSLT program filename path
 *
 * Append the XSLT program filename path to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xslt_filename(srcml_archive* archive, const char* xslt_filename) {

    if (archive == NULL || xslt_filename == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlDoc> doc(xmlReadFile(xslt_filename, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_xslt_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_xslt_memory
 * @param archive a srcml_archive
 * @param xslt_buffer a buffer holding an XSLT
 * @param size the size of the passed buffer
*
 * Append the XSLT program in the buffer to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xslt_memory(srcml_archive* archive, const char* xslt_buffer, size_t size) {

    if (archive == NULL || xslt_buffer == 0 || size == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlDoc> doc(xmlReadMemory(xslt_buffer, (int)size, 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_xslt_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_xslt_FILE
 * @param archive a srcml_archive
 * @param xslt_file a FILE containing an XSLT program
 *
 * Append the XSLT program in FILE to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xslt_FILE(srcml_archive* archive, FILE* xslt_file) {

    if (archive == NULL || xslt_file == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    xmlRegisterDefaultInputCallbacks();
    std::unique_ptr<xmlDoc> doc(xmlReadIO(xmlFileRead, 0, xslt_file, 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_xslt_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_xslt_fd
 * @param archive a srcml_archive
 * @param xslt_fd a file descriptor containing an XSLT program
 *
 * Append the XSLT program in fd to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xslt_fd(srcml_archive* archive, int xslt_fd) {

    if (archive == NULL || xslt_fd < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlDoc> doc(xmlReadFd(xslt_fd, 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_xslt_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_internal
 * @param archive a srcml archive
 * @param relaxng_filename a RelaxNG schema filename path
 *
 * Append the RelaxNG schema filename path to the list
 * of transformation/queries.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
static int srcml_append_transform_relaxng_internal(srcml_archive* archive, std::unique_ptr<xmlDoc> doc) {

    if (archive == NULL || doc == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->transformations.push_back(std::unique_ptr<Transformation>(new relaxngTransformation(doc.get())));

    return SRCML_STATUS_OK;
}

/**
 * srcml_append_transform_relaxng_filename
 * @param archive a srcml archive
 * @param relaxng_filename a RelaxNG schema filename path
 *
 * Append the RelaxNG schema filename path to the list
 * of transformation/queries.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_relaxng_filename(srcml_archive* archive, const char* relaxng_filename) {

    if (archive == NULL || relaxng_filename == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlDoc> doc(xmlReadFile(relaxng_filename, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_relaxng_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_relaxng_memory
 * @param archive a srcml archive
 * @param relaxng_buffer a buffer holding a RelaxNG schema
 * @param size the size of the passed buffer
 *
 * Append the RelaxNG schema in the buffer to the list
 * of transformation/queries.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_relaxng_memory(srcml_archive* archive, const char* relaxng_buffer, size_t size) {

    if (archive == NULL || relaxng_buffer == 0 || size == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlDoc> doc(xmlReadMemory(relaxng_buffer, (int)size, 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_relaxng_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_relaxng_FILE
 * @param archive a srcml archive
 * @param relaxng_file a FILE containing aRelaxNG schema
 *
 * Append the RelaxNG schema in FILE to the list
 * of transformation/queries.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_relaxng_FILE(srcml_archive* archive, FILE* relaxng_file) {

    if (archive == NULL || relaxng_file == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    xmlRegisterDefaultInputCallbacks();
    std::unique_ptr<xmlDoc> doc(xmlReadIO(xmlFileRead, 0, relaxng_file, 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_relaxng_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_relaxng_fd
 * @param archive a srcml archive
 * @param relaxng_fd a file descriptor containing a RelaxNG schema
 *
 * Append the RelaxNG schema in fd to the list
 * of transformation/queries.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_relaxng_fd(srcml_archive* archive, int relaxng_fd) {

    if (archive == NULL || relaxng_fd < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlDoc> doc(xmlReadFd(relaxng_fd, 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_append_transform_relaxng_internal(archive, std::move(doc));
}

/**
 * srcml_append_transform_param
 * @param archive a srcml archive
 * @param xpath_param_name name of a parameter
 * @param xpath_param_value value of the named parameter
 *
 * Append the parameter to the last transformation.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status errors code on failure.
 */
int srcml_append_transform_param(srcml_archive* archive, const char* xpath_param_name, const char* xpath_param_value) {

    if (archive == NULL || xpath_param_name == NULL || xpath_param_value == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;
    if (archive->transformations.size() == 0)
        return SRCML_STATUS_NO_TRANSFORMATION;

    archive->transformations.back()->xsl_parameters.push_back(xpath_param_name);
    archive->transformations.back()->xsl_parameters.push_back(xpath_param_value);

    return SRCML_STATUS_OK;
}

/**
 * srcml_append_transform_stringparam
 * @param archive a srcml archive
 * @param xpath_param_name name of a parameter
 * @param xpath_param_value value of the named parameter will be wrapped in "
 *
 * Append the parameter to the last transformation with the value wrapped in ".
 *
 * @returns Returns SRCML_STATUS_OK on success and a status errors code on failure.
 */
int srcml_append_transform_stringparam(srcml_archive* archive, const char* xpath_param_name, const char* xpath_param_value) {

    if (archive == NULL || xpath_param_name == NULL || xpath_param_value == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;
    if (archive->transformations.size() == 0)
        return SRCML_STATUS_NO_TRANSFORMATION;

    archive->transformations.back()->xsl_parameters.push_back(xpath_param_name);

    std::string parenvalue("\"");
    parenvalue += xpath_param_value;
    parenvalue += '\"';

    archive->transformations.back()->xsl_parameters.push_back(parenvalue);

    return SRCML_STATUS_OK;
}

/**
 * srcml_clear_transforms
 * @param archive an archive
 *
 * Remove all transformations from archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_clear_transforms(srcml_archive* archive) {

    if (archive == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // cleanup the transformations
    archive->transformations.clear();

    return SRCML_STATUS_OK;
}

static bool usesURI(xmlNode* cur_node, std::string_view URI);

static bool usesURIChildren(xmlNode* a_node, std::string_view URI) {

    for (xmlNode* cur_node = a_node; cur_node; cur_node = cur_node->next) {

        if (cur_node->ns && cur_node->ns->prefix && URI == (const char*) cur_node->ns->href) {
            return true;
        }

        if (usesURIChildren(cur_node->children, URI))
            return true;
    }

    return false;
}

static bool usesURI(xmlNode* cur_node, std::string_view URI) {

    if (cur_node->ns && cur_node->ns->prefix && URI == (const char*) cur_node->ns->href) {
        return true;
    }

    return usesURIChildren(cur_node->children, URI);
}

/**
 * srcml_unit_apply_transforms
 * @param iarchive an input srcml archive
 * @param oarchive and output srcml archive
 *
 * Apply appended transformations inorder added and consecutively.
 * Intermediate results are stored in a temporary file.
 * Transformations are cleared.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_unit_apply_transforms(struct srcml_archive* archive, struct srcml_unit* unit, struct srcml_transform_result** presult) {

    if (archive == nullptr || unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // unit stays the same for no transformation
    if (archive->transformations.empty())
        return SRCML_STATUS_OK;

    srcml_transform_result* result = nullptr;
    if (presult) {
        *presult = new srcml_transform_result;
        result = *presult;
        result->type = SRCML_RESULT_NONE;
        result->boolValue = false;
    }

    // find the position of the item attribute, if it exists
    // the attribute array consists of { name1, value1, name2, value2, ... }
    auto currentItemPosition = unit->attributes.size();
    for (std::size_t i = 0; i < unit->attributes.size(); i += 2) {
        if (unit->attributes[i].name == "item"sv) {
            currentItemPosition = i;
            break;
        }
    }

    // create a DOM of the unit
    std::shared_ptr<xmlDoc> doc(xmlReadMemory(unit->srcml.data(), (int) unit->srcml.size(), 0, 0, 0), [](xmlDoc* doc) { xmlFreeDoc(doc); });
    if (doc == nullptr)
        return SRCML_STATUS_ERROR;

    // apply transformations sequentially on the results from the previous transformation
    std::unique_ptr<xmlNodeSet> fullresults(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc.get())));
    if (fullresults == nullptr)
        return SRCML_STATUS_ERROR;

    // final result of all applied transformations
    TransformationResult lastresult;
    std::shared_ptr<xmlDoc> curdoc(doc);
    for (const auto& trans : archive->transformations) {

        // preserve the fullresults to iterate through
        // collect results from this transformation applied to the potentially multiple
        // results of the previous transformation step
        std::unique_ptr<xmlNodeSet> pr(xmlXPathNodeSetCreate(0));
        if (pr == nullptr)
            return SRCML_STATUS_ERROR;
        fullresults.swap(pr);

        for (int i = 0; i < pr->nodeNr; ++i) {

            xmlDocSetRootElement(curdoc.get(), pr->nodeTab[i]);

            lastresult = trans->apply(curdoc.get(), 0);
            std::unique_ptr<xmlNodeSet> results(std::move(lastresult.nodeset));
            if (results == nullptr)
                break;

            xmlXPathNodeSetMerge(fullresults.get(), results.get());
        }

        // necessary to avoid access to free'd memory later on
        // does NOT cause a memory leak
        pr->nodeNr = 0;

        // result of the transformation may be a new doc
        if (lastresult.doc && lastresult.doc.get() != doc.get()) {
            curdoc = lastresult.doc;
        }

        // if there are no results, then we can't apply further transformations
        // but there still might be results in the scalar values
        if (fullresults->nodeNr == 0) {
            result->units.clear();
            break;
        }
    }

    if (result) {
        result->type = lastresult.nodeType;
    }

    if (lastresult.nodeType != SRCML_RESULT_UNITS) {
        // remove all nodes in the fullresults nodeset
        // valgrind shows free accessing these nodes after they have been freed
        // by the xmlDoc
        fullresults->nodeNr = 0;
    }

    // handle non-nodeset results
    switch (lastresult.nodeType) {
    case SRCML_RESULT_STRING:
        if (result != nullptr) {
            result->stringValue = lastresult.stringValue;
            return SRCML_STATUS_OK;
        }
        return SRCML_STATUS_ERROR;

    case SRCML_RESULT_BOOLEAN:
        if (result != nullptr) {
            result->boolValue = lastresult.boolValue;
            return SRCML_STATUS_OK;
        }
        return SRCML_STATUS_ERROR;

    case SRCML_RESULT_NUMBER:
        if (result != nullptr) {
            result->numberValue = lastresult.numberValue;
            return SRCML_STATUS_OK;
        }
        return SRCML_STATUS_ERROR;
    };

    if (result == nullptr) {
        return SRCML_STATUS_OK;
    }

    // create units out of the transformation results
    result->type = lastresult.nodeType;

    for (int i = 0; i < fullresults->nodeNr; ++i) {

        // create a new unit to store the results in
        auto nunit = srcml_unit_clone(unit);
        nunit->read_body = nunit->read_header = true;
        if (!lastresult.unitWrapped) {

            // remove the hash since not valid for partial query results
            nunit->hash = std::nullopt;

            // update or add item attribute
            if (currentItemPosition < unit->attributes.size()) {
                if (fullresults->nodeNr > 1) {
                    nunit->attributes[currentItemPosition].value += '-';
                    nunit->attributes[currentItemPosition].value += std::to_string(i + 1);
                }
            } else {
                nunit->attributes.push_back({"", "", "item", std::to_string(i + 1)});
            }
        }

        // when no namespace, use the starting namespaces
        if (!nunit->namespaces)
            nunit->namespaces = starting_namespaces;

        // mark unused cpp and omp until we examine the query result
        auto itcpp = findNSURI(*nunit->namespaces, SRCML_CPP_NS_URI);
        if (itcpp != nunit->namespaces->end()) {
            itcpp->flags &= ~NS_USED;
        }
        auto itomp = findNSURI(*nunit->namespaces, SRCML_OPENMP_NS_URI);
        if (itomp != nunit->namespaces->end()) {
            itomp->flags &= ~NS_USED;
        }

        // special cases where the nodes are not written to the tree
#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4061)
#endif
        switch (fullresults->nodeTab[i]->type) {
        case XML_COMMENT_NODE:

            for (auto c : "<!--"sv)
                nunit->srcml += c;
            nunit->srcml.append((const char*) fullresults->nodeTab[i]->content);
            for (auto c : "-->"sv)
                nunit->srcml += c;
            break;

        case XML_TEXT_NODE:

            nunit->srcml.append((const char*) fullresults->nodeTab[i]->content);
            break;

        case XML_ATTRIBUTE_NODE:

            nunit->srcml.append((const char*) fullresults->nodeTab[i]->name);
            nunit->srcml += '=';
            nunit->srcml += '"';
            nunit->srcml.append((const char*) fullresults->nodeTab[i]->children->content);
            nunit->srcml += '"';
            break;

        default:

            // dump the result tree to the string using an output buffer that writes to a std::string
            xmlOutputBufferPtr output = xmlOutputBufferCreateIO([](void* context, const char* buffer, int len) noexcept {

                ((std::string*) context)->append(buffer, static_cast<std::size_t>(len));

                return len;

            }, 0, &(nunit->srcml), 0);
            xmlNodeDumpOutput(output, curdoc.get(), fullresults->nodeTab[i], 0, 0, 0);

            // very important to flush to make sure the unit contents are all present
            // also performs a free of resources
            xmlOutputBufferClose(output);

            // update the cpp namespace if actually used
            if (usesURI(fullresults->nodeTab[i], SRCML_CPP_NS_URI)) {

                if (itcpp != nunit->namespaces->end()) {
                    itcpp->flags |= NS_USED;
                } else {
                    nunit->namespaces->emplace_back(SRCML_CPP_NS_DEFAULT_PREFIX, SRCML_CPP_NS_URI, NS_USED | NS_STANDARD);
                }
            }

            // update the openmp namespace if actually used
            if (usesURI(fullresults->nodeTab[i], SRCML_OPENMP_NS_URI)) {

                if (itomp != nunit->namespaces->end()) {
                    itomp->flags |= NS_USED;
                } else {
                    nunit->namespaces->emplace_back(SRCML_OPENMP_NS_DEFAULT_PREFIX, SRCML_OPENMP_NS_URI, NS_USED | NS_STANDARD);
                }
            }

            break;
        }

        // mark inside the units
        nunit->content_begin = lastresult.unitWrapped ? (int) nunit->srcml.find_first_of('>') + 1 : 0;
        nunit->content_end =   lastresult.unitWrapped ? (int) nunit->srcml.find_last_of('<') + 1  : (int) nunit->srcml.size() + 1;
        nunit->insert_begin = 0;
        nunit->insert_end = 0;

        // update the unit attributes with the transformed result based on the root tag
        if (lastresult.unitWrapped) {
            xmlSAXHandler roottagsax;
            memset(&roottagsax, 0, sizeof(roottagsax));
            roottagsax.initialized    = XML_SAX2_MAGIC;
            roottagsax.startElementNs = [](void* ctx, const xmlChar* /* localname */, const xmlChar* /* prefix */, const xmlChar* /* URI */,
                             int /* nb_namespaces */, const xmlChar** /* namespaces */,
                             int nb_attributes, int /* nb_defaulted */, const xmlChar** attributes) {

                auto ctxt = (xmlParserCtxtPtr) ctx;
                if (ctxt == nullptr)
                    return;
                auto unit = (srcml_unit*) ctxt->_private;
                if (unit == nullptr)
                    return;

                unit_update_attributes(unit, nb_attributes, attributes);
            };

            // extract the start tag, turning it into an empty tag
            // note: it may be an empty tag already
            std::string starttag = nunit->srcml.substr(0, static_cast<std::size_t>(nunit->content_begin - 1));
            if (starttag.back() != '/')
                starttag += '/';
            starttag += '>';

            // parse the start tag updating the unit
            xmlParserCtxtPtr context = xmlCreateMemoryParserCtxt(starttag.data(), (int) starttag.size());
            auto save_private = context->_private;
            context->_private = nunit;
            auto save_sax = context->sax;
            context->sax = &roottagsax;

            // parse our single-element unit
            xmlParseDocument(context);

            // restore state and free
            context->_private = save_private;
            context->sax = save_sax;
            xmlFreeParserCtxt(context);
        }

        // store in the returned results
        result->units.push_back(nunit);
    }
#ifdef _MSC_VER
#   pragma warning(pop)
#endif

    // remove all nodes in the fullresults nodeset
    // valgrind shows free accessing these nodes after they have been freed
    // by the xmlDoc
    fullresults->nodeNr = 0;

    return SRCML_STATUS_OK;
}

/**
 * Free the resources in a tranformation result.
 * @param results Struct of result
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_transform_free(struct srcml_transform_result* result) {

    for (auto unit : result->units) {
        srcml_unit_free(unit);
    }

    delete result;

    return SRCML_STATUS_OK;
}

/**
 * @param result A srcml transformation result
 * @return The type of the transformation result
 */
LIBSRCML_DECL int srcml_transform_get_type(struct srcml_transform_result* result) {

    return result->type;
}

/**
 * @param result A srcml transformation result
 * @return The number of units in the transformation result
 */
int srcml_transform_get_unit_size(struct srcml_transform_result* result) {

    // make sure correct result type
    if (result->type != SRCML_RESULT_UNITS)
       return 0;

    return (int) result->units.size();
}

/**
 * @param result A srcml transformation result
 * @param pos The index in the units
 * @return The unit in the transformation result at that index
 */
struct srcml_unit* srcml_transform_get_unit(struct srcml_transform_result* result, int index) {

    // make sure correct result type
    if (result->type != SRCML_RESULT_UNITS)
        return 0;

    if (index >= (int) result->units.size())
        return 0;

    return result->units[static_cast<std::size_t>(index)];
}

/**
 * @param result A srcml transformation result
 * @return The transformation result string
 */
const char* srcml_transform_get_string(struct srcml_transform_result* result) {

    // make sure correct result type
    if (result->type != SRCML_RESULT_STRING)
        return 0;

    if (!result->stringValue)
        return 0;

    return result->stringValue->data();
}

/**
 * @param result A srcml transformation result
 * @return The transformation result number
 */
double srcml_transform_get_number(struct srcml_transform_result* result) {

    // make sure correct result type
    if (result->type != SRCML_RESULT_NUMBER)
        return 0;

    if (!result->numberValue)
        return 0;

    return *(result->numberValue);
}

/**
 * @param result A srcml transformation result
 * @return The transformation result boolean
 */
int srcml_transform_get_bool(struct srcml_transform_result* result) {

    // make sure correct result type
    if (result->type != SRCML_RESULT_BOOLEAN)
        return -1;

    if (!result->boolValue)
        return -1;

    return *(result->boolValue);
}
