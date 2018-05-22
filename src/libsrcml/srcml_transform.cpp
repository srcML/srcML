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

#include <srcml_macros.hpp>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_utilities.hpp>
#include <srcml_translator.hpp>

#include <stdio.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _MSC_BUILD
#include <unistd.h>
#else
#include <io.h>
#endif

#define DLLOAD

#ifdef DLLOAD
#include <dlfcn.h>
#else
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>
#endif

#include <sax2_srcsax_handler.hpp>
#include <srcml_sax2_reader.hpp>

#include <libxml/parser.h>
#include <libxml/xmlIO.h>

#include <xsltTransformation.hpp>
#include <xpathTransformation.hpp>

xpath_arguments null_arguments;

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

    return srcml_append_transform_xpath_element_attribute(archive, xpath_string, 0, 0, 0, 0, 0, 0, 0);
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
int srcml_append_transform_xpath_attribute (struct srcml_archive* archive, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* attr_name, const char* attr_value) {

    return srcml_append_transform_xpath_element_attribute(archive, xpath_string, 0, 0, 0, prefix, namespace_uri, attr_name, attr_value);
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
int srcml_append_transform_xpath_element (struct srcml_archive* archive, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element) {

    return srcml_append_transform_xpath_element_attribute(archive, xpath_string, prefix, namespace_uri, element, 0, 0, 0, 0);
}

/**
 * srcml_append_transform_xpath_element_attribute
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
int srcml_append_transform_xpath_element_attribute (struct srcml_archive* archive, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element,
                                                            const char* attr_prefix, const char* attr_namespace_uri,
                                                            const char* attr_name, const char* attr_value) {
    if(archive == NULL || xpath_string == 0) return SRCML_STATUS_INVALID_ARGUMENT;
//    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;

    xpathTransformation* trans = new xpathTransformation(archive, xpath_string, prefix, namespace_uri, element,
            attr_prefix, attr_namespace_uri, attr_name, attr_value);

    archive->ntransformations.push_back(trans);

    return SRCML_STATUS_OK;
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

    if(archive == NULL || doc == 0) return SRCML_STATUS_INVALID_ARGUMENT;
  //  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;

    xsltTransformation* trans = new xsltTransformation(doc, std::vector<std::string>());

    archive->ntransformations.push_back(trans);

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

    if(archive == NULL || xslt_filename == 0) return SRCML_STATUS_INVALID_ARGUMENT;
  //  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;

    xmlDocPtr doc = xmlReadFile(xslt_filename, 0, 0);

    return srcml_append_transform_xslt_internal(archive, doc);
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

    if(archive == NULL || xslt_buffer == 0 || size == 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlDocPtr doc = xmlReadMemory(xslt_buffer, (int)size, 0, 0, 0);

    return srcml_append_transform_xslt_internal(archive, doc);
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

    if(archive == NULL || xslt_file == 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlRegisterDefaultInputCallbacks();
    xmlDocPtr doc = xmlReadIO(xmlFileRead, 0, xslt_file, 0, 0, 0);

    return srcml_append_transform_xslt_internal(archive, doc);
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

    if(archive == NULL || xslt_fd < 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlDocPtr doc = xmlReadFd(xslt_fd, 0, 0, 0);

    return srcml_append_transform_xslt_internal(archive, doc);
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

    if(archive == NULL || doc == 0) return SRCML_STATUS_INVALID_ARGUMENT;
//    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;

    transform tran = { SRCML_RELAXNG, std::vector<std::string>(), null_arguments, doc, 0, 0, 0 };

    archive->transformations.push_back(tran);

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

    if(archive == NULL || relaxng_filename == 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlDocPtr doc = xmlReadFile(relaxng_filename, 0, 0);

    return srcml_append_transform_relaxng_internal(archive, doc);
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

    if(archive == NULL || relaxng_buffer == 0 || size == 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlDocPtr doc = xmlReadMemory(relaxng_buffer, (int)size, 0, 0, 0);

    return srcml_append_transform_relaxng_internal(archive, doc);
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

    if(archive == NULL || relaxng_file == 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlRegisterDefaultInputCallbacks();
    xmlDocPtr doc = xmlReadIO(xmlFileRead, 0, relaxng_file, 0, 0, 0);

    return srcml_append_transform_relaxng_internal(archive, doc);
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

    if(archive == NULL || relaxng_fd < 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlDocPtr doc = xmlReadFd(relaxng_fd, 0, 0, 0);

    return srcml_append_transform_relaxng_internal(archive, doc);
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

    if(archive == NULL || xpath_param_name == NULL || xpath_param_value == NULL) return SRCML_STATUS_INVALID_ARGUMENT;
    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;
    if(archive->transformations.size() == 0) return SRCML_STATUS_NO_TRANSFORMATION;

    archive->transformations.back().xsl_parameters.pop_back();
    archive->transformations.back().xsl_parameters.push_back(xpath_param_name);
    archive->transformations.back().xsl_parameters.push_back(xpath_param_value);

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

    if(archive == NULL || xpath_param_name == NULL || xpath_param_value == NULL) return SRCML_STATUS_INVALID_ARGUMENT;
    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;
    if(archive->transformations.size() == 0) return SRCML_STATUS_NO_TRANSFORMATION;

    archive->transformations.back().xsl_parameters.pop_back();
    archive->transformations.back().xsl_parameters.push_back(xpath_param_name);

    size_t xpath_param_value_length = strlen(xpath_param_value);
    char * string_value = new char[xpath_param_value_length + 3];
    string_value[0] = '"';
    strncpy(string_value + 1, xpath_param_value, xpath_param_value_length);
    string_value[xpath_param_value_length + 1] = '"';
    string_value[xpath_param_value_length + 2] = 0;

    archive->transformations.back().xsl_parameters.push_back(string_value);

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

    for (const auto* p : archive->ntransformations) {

        delete p;
    }
    archive->ntransformations.clear();

    return SRCML_STATUS_OK;
}

/**
 * srcml_apply_transforms
 * @param iarchive an input srcml archive
 * @param oarchive and output srcml archive
 *
 * Apply appended transformations inorder added and consecutively.
 * Intermediate results are stored in a temporary file.
 * Transformations are cleared.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */

int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) {

    return srcml_apply_transforms_verbose(iarchive, oarchive, 0);
}

int srcml_unit_apply_transforms(struct srcml_archive* archive, struct srcml_unit* unit, struct srcml_unit*** units) {

    // unit stays the same for no transformation
    if (archive->ntransformations.empty())
        return 0;

    std::string nssrcml = unit->srcml.substr(0, 5);
    if (unit->archive->options & SRCML_OPTION_ARCHIVE)
        nssrcml.append(" xmlns=\"http://www.srcML.org/srcML/src\"");
    nssrcml.append(unit->srcml.substr(5));

    // create a DOM of the unit
    xmlDocPtr doc = xmlReadMemory(nssrcml.c_str(), (int) nssrcml.size(), 0, 0, 0);

    // apply transformations serially on the results from the previous transformation
    bool hasUnitWrapper = false;
    xmlNodeSetPtr fullresults = xmlXPathNodeSetCreate(xmlDocGetRootElement(doc));
    for (auto* trans : archive->ntransformations) {

        // preserve the fullresults to iterate through
        xmlNodeSetPtr pr = fullresults;

        // collect results from this transformation applied to the potentially multiple
        // results of the previous transformation step
        fullresults = xmlXPathNodeSetCreate(0);

        for (int i = 0; i < pr->nodeNr; ++i) {
            xmlDocSetRootElement(doc, pr->nodeTab[i]);

            xmlNodeSetPtr results = trans->apply(doc, 0);

            for (int i = 0; i < results->nodeNr; ++i)
                xmlXPathNodeSetAdd(fullresults, results->nodeTab[i]);
        }

        hasUnitWrapper = trans->hasUnitWrapper();

        if (!fullresults->nodeNr)
            break;
    }

    // create units out of the transformation results
    srcml_unit** newunits = new srcml_unit*[fullresults->nodeNr + 1];
    newunits[fullresults->nodeNr] = 0;

    for (int i = 0; i < fullresults->nodeNr; ++i) {

        doc->children = fullresults->nodeTab[i];

        bool isunit = strcmp((const char*) doc->children->name, "unit") == 0;

        auto nunit = srcml_unit_clone(unit);
        nunit->read_body = nunit->read_header = true;
        if (!isunit) {
            nunit->attributes.push_back("item");
            nunit->attributes.push_back(std::to_string(i + 1));
            nunit->hash = boost::optional<std::string>();
        }

        // dump the result tree to the string using an output buffer that writes to a std::string
        nunit->srcml.clear();
        xmlOutputBufferPtr output = xmlOutputBufferCreateIO([](void* context, const char* buffer, int len) {

            ((std::string*) context)->append(buffer, len);

            return len;

        }, 0, &(nunit->srcml), 0);
        xmlNodeDumpOutput(output, doc, xmlDocGetRootElement(doc), 0, 0, 0);

        // very important to flush to make sure the unit contents are all present
        xmlOutputBufferClose(output);

        // mark inside the units
        // @todo Not being done right
        nunit->content_begin = hasUnitWrapper ? (int) nunit->srcml.find_first_of('>') + 1 : 0;
        nunit->content_end =   hasUnitWrapper ? (int) nunit->srcml.find_last_of('<') + 1  : (int) nunit->srcml.size() + 1;

        newunits[i] = nunit;
    }

    if (units) {
        *units = newunits;
    }

    xmlFreeDoc(doc);

    return 1;
}

int srcml_apply_transforms_verbose(srcml_archive* iarchive, srcml_archive* oarchive, apply_transforms_callback* ) {

    if(iarchive == NULL || oarchive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;
    if((iarchive->type != SRCML_ARCHIVE_READ && iarchive->type != SRCML_ARCHIVE_RW)
        || (oarchive->type != SRCML_ARCHIVE_WRITE && oarchive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_INVALID_IO_OPERATION;

    int status = SRCML_STATUS_OK;
    for(std::vector<transform>::size_type i = 0; i < iarchive->transformations.size(); ++i) {

        xmlParserInputBufferPtr pinput = iarchive->input;

        auto ctxt = iarchive->reader->control.getContext()->libxml2_context;
        auto state = (sax2_srcsax_handler*) ctxt->_private;
        state->collect_unit_body = false;

        try {

            switch(iarchive->transformations[i].type) {
            case SRCML_XPATH: {

                auto save = oarchive->transformations;
                oarchive->transformations = iarchive->transformations;
                status = srcml_xpath(pinput, "src:unit",
                                    optional_get_c_str(iarchive->transformations[i].arguments.str),
                                    optional_get_c_str(iarchive->transformations[i].arguments.prefix), optional_get_c_str(iarchive->transformations[i].arguments.uri),
                                    optional_get_c_str(iarchive->transformations[i].arguments.element),
                                    optional_get_c_str(iarchive->transformations[i].arguments.attr_prefix), optional_get_c_str(iarchive->transformations[i].arguments.attr_uri),
                                    optional_get_c_str(iarchive->transformations[i].arguments.attr_name), optional_get_c_str(iarchive->transformations[i].arguments.attr_value),
                                    oarchive->options, oarchive);
                oarchive->transformations = save;
                break;
            }

#ifdef WITH_LIBXSLT
            /*
            case SRCML_XSLT: {

                status = srcml_xslt(pinput,
                                   iarchive->transformations[i].doc,
                                   iarchive->transformations[i].xsl_parameters, 0, oarchive->options, oarchive);
                break;
            }
            */
#endif

            case SRCML_RELAXNG: {

                status = srcml_relaxng(pinput,
                                      iarchive->transformations[i].doc,
                                      oarchive->options, oarchive);
                break;
            }

            default : break;
            }

        } catch(...) {

            return SRCML_STATUS_INVALID_INPUT;
        }

        break;
    }

    //srcml_clear_transforms(iarchive);

    return status;
}

