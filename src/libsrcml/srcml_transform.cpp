/**
 * @file srcml_transform.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcml.h>
#include <srcml_types.hpp>

#ifdef _MSC_BUILD
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
//    if (archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW)
//        return SRCML_STATUS_INVALID_IO_OPERATION;

    xpathTransformation* trans = new xpathTransformation(archive, xpath_string, prefix, namespace_uri, element,
            attr_prefix, attr_namespace_uri, attr_name, attr_value);

    archive->transformations.emplace_back(trans);

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
 * Instead of outputting the results each in a separte unit tag.  Output the complete
 * archive marking the xpath results with a user provided attribute.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
int srcml_append_transform_xpath_attribute(struct srcml_archive* archive, const char* xpath_string,
                                            const char* prefix, const char* namespace_uri,
                                            const char* attr_name, const char* attr_value) {

    if (archive == nullptr || xpath_string == nullptr)
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

    return srcml_append_transform_xpath_internal(archive, xpath_string, prefix, namespace_uri, element, 0, 0, 0, 0);
}

#ifdef WITH_LIBXSLT
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
static int srcml_append_transform_xslt_internal(srcml_archive* archive, xmlDocPtr doc) {

    if (archive == NULL || doc == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;
  //  if (archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW)
 //   return SRCML_STATUS_INVALID_IO_OPERATION;

    xsltTransformation* trans = new xsltTransformation(doc, std::vector<std::string>());

    archive->transformations.emplace_back(trans);

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

    return srcml_append_transform_xslt_internal(archive, doc.get());
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

    return srcml_append_transform_xslt_internal(archive, doc.get());
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

    return srcml_append_transform_xslt_internal(archive, doc.get());
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

    return srcml_append_transform_xslt_internal(archive, doc.get());
}
#endif

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
static int srcml_append_transform_relaxng_internal(srcml_archive* archive, xmlDocPtr doc) {

    if (archive == NULL || doc == 0)
        return SRCML_STATUS_INVALID_ARGUMENT;
//    if (archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW)
//    return SRCML_STATUS_INVALID_IO_OPERATION;

    relaxngTransformation* trans = new relaxngTransformation(doc);

    archive->transformations.emplace_back(trans);

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

    return srcml_append_transform_relaxng_internal(archive, doc.get());
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

    return srcml_append_transform_relaxng_internal(archive, doc.get());
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

    return srcml_append_transform_relaxng_internal(archive, doc.get());
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

    return srcml_append_transform_relaxng_internal(archive, doc.get());
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
    if (archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW)
        return SRCML_STATUS_INVALID_IO_OPERATION;
    if (archive->transformations.size() == 0)
        return SRCML_STATUS_NO_TRANSFORMATION;

    archive->transformations.back()->xsl_parameters.pop_back();
    archive->transformations.back()->xsl_parameters.push_back(xpath_param_name);

    size_t xpath_param_value_length = strlen(xpath_param_value);
    char * string_value = new char[xpath_param_value_length + 3];
    string_value[0] = '"';
    strncpy(string_value + 1, xpath_param_value, xpath_param_value_length);
    string_value[xpath_param_value_length + 1] = '"';
    string_value[xpath_param_value_length + 2] = 0;

    archive->transformations.back()->xsl_parameters.push_back(string_value);

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
int srcml_unit_apply_transforms(struct srcml_archive* archive, struct srcml_unit* unit, struct srcml_transformation_result_t* result) {

    if (archive == nullptr || unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (result) {
        result->num_units = 0;
        result->units = nullptr;
        result->stringValue = nullptr;
    }

    // unit stays the same for no transformation
    if (archive->transformations.empty())
        return SRCML_STATUS_OK;

    // create a DOM of the unit
    std::unique_ptr<xmlDoc> doc(xmlReadMemory(unit->srcml.c_str(), (int) unit->srcml.size(), 0, 0, 0));
    if (doc == nullptr)
        return SRCML_STATUS_ERROR;

    // apply transformations sequentially on the results from the previous transformation
    std::unique_ptr<xmlNodeSet> fullresults(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc.get())));
    if (fullresults == nullptr)
        return SRCML_STATUS_ERROR;

    TransformationResult lastresult;
    for (const auto& trans : archive->transformations) {

        // preserve the fullresults to iterate through
        // collect results from this transformation applied to the potentially multiple
        // results of the previous transformation step
        std::unique_ptr<xmlNodeSet> pr(xmlXPathNodeSetCreate(0));
        if (pr == nullptr)
            return SRCML_STATUS_ERROR;
        fullresults.swap(pr);

        for (int i = 0; i < pr->nodeNr; ++i) {
            xmlDocSetRootElement(doc.get(), pr->nodeTab[i]);

            lastresult = trans->apply(doc.get(), 0);
            std::unique_ptr<xmlNodeSet> results(lastresult.nodeset);
            if (results == nullptr)
                break;
            xmlXPathNodeSetMerge(fullresults.get(), results.get());
        }

        // if there are no results, then we can't apply further transformations
        // but there still might be reults in the scalar values
        if (fullresults->nodeNr == 0) {
            result->units = 0;
            break;
        }
    }

    if (result) {
        result->type = lastresult.nodeType;
    }

    // handle non-nodeset results
    // @todo Implement these
    switch (lastresult.nodeType) {
    case SRCML_RESULTS_STRING:
        if (result != nullptr) {
            result->stringValue = strdup(lastresult.stringValue.c_str());
            return SRCML_STATUS_OK;
        }
        return SRCML_STATUS_ERROR;

    case SRCML_RESULTS_BOOLEAN:
        if (result != nullptr) {
            result->boolValue = lastresult.boolValue;
            return SRCML_STATUS_OK;
        }
        return SRCML_STATUS_ERROR;

    case SRCML_RESULTS_NUMBER:
        if (result != nullptr) {
            result->numberValue = lastresult.numberValue;
            return SRCML_STATUS_OK;
        }
        return SRCML_STATUS_ERROR;
    };

    if (result == nullptr)
        return SRCML_STATUS_OK;

    // create units out of the transformation results
    result->type = lastresult.nodeType;
    result->num_units = fullresults->nodeNr;
    result->units = new srcml_unit*[fullresults->nodeNr + 1];
    result->units[fullresults->nodeNr] = 0;

    for (int i = 0; i < fullresults->nodeNr; ++i) {

        // create a new unit to store the results in
        // @todo What happens for a single result?
        auto nunit = srcml_unit_clone(unit);
        nunit->read_body = nunit->read_header = true;
        if (!lastresult.unitWrapped) {
            nunit->attributes.push_back("item");
            nunit->attributes.push_back(std::to_string(i + 1));
            nunit->hash = boost::none;
        }

        // special case for XML comment as it does not get written to the tree
        switch (fullresults->nodeTab[i]->type) {
        case XML_COMMENT_NODE:

            nunit->srcml.assign("<!--");
            nunit->srcml.append((const char*) fullresults->nodeTab[i]->content);
            nunit->srcml.append("-->");
            break;

        case XML_TEXT_NODE:

            nunit->srcml.append((const char*) fullresults->nodeTab[i]->content);
            break;

        case XML_ATTRIBUTE_NODE:

            nunit->srcml.append((const char*) fullresults->nodeTab[i]->name);
            nunit->srcml.append("=\"");
            nunit->srcml.append((const char*) fullresults->nodeTab[i]->children->content);
            nunit->srcml.append("\"");
            break;

        default:

            // dump the result tree to the string using an output buffer that writes to a std::string
            doc->children = fullresults->nodeTab[i];
            xmlOutputBufferPtr output = xmlOutputBufferCreateIO([](void* context, const char* buffer, int len) {

                ((std::string*) context)->append(buffer, len);

                return len;

            }, 0, &(nunit->srcml), 0);
            xmlNodeDumpOutput(output, doc.get(), xmlDocGetRootElement(doc.get()), 0, 0, 0);

            // very important to flush to make sure the unit contents are all present
            // also performs a free of resources
            xmlOutputBufferClose(output);

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
            roottagsax.startElementNs = [](void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                             int nb_namespaces, const xmlChar** namespaces,
                             int nb_attributes, int /* nb_defaulted */, const xmlChar** attributes) {

                auto ctxt = (xmlParserCtxtPtr) ctx;
                if (ctxt == nullptr)
                    return;
                auto unit = (srcml_unit*) ctxt->_private;
                if (unit == nullptr)
                    return;

                unit_update_attributes(unit, nb_attributes, attributes);
            };

            // extract the start tag
            std::string starttag = nunit->srcml.substr(0, unit->content_begin) + "/>";

            // parse the start tag updating the unit
            xmlParserCtxtPtr context = xmlCreateMemoryParserCtxt(starttag.c_str(), (int) starttag.size());
            context->_private = nunit;
            context->sax = &roottagsax;

            // @todo Handle error
            xmlParseDocument(context);
        }

        result->units[i] = nunit;
    }

    return SRCML_STATUS_OK;
}
