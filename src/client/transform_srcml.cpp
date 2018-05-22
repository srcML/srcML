/**
 * @file transform_srcml.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <transform_srcml.hpp>
#include <src_prefix.hpp>
#include <input_file.hpp>
#include <srcml.h>
#include <string>
#include <SRCMLStatus.hpp>

int apply_xpath(srcml_archive* in_arch, srcml_archive* out_arch, const std::string& transform_input, const std::pair< boost::optional<element>, boost::optional<attribute> >& xpath_support, const std::map<std::string,std::string>& xmlns_namespaces) {

    auto element = xpath_support.first;
    auto attribute = xpath_support.second;

    // Check element namespace
    char const * element_uri = 0;
    if (element){

        // check first if namespace is already declared
        element_uri = srcml_archive_get_uri_from_prefix(out_arch, element->prefix->c_str());

        // if not declared, check for xmlns from cli
        if (!element_uri) {
            auto it = xmlns_namespaces.find(*(element->prefix));
            if (it != xmlns_namespaces.end())
                element_uri = it->second.c_str();
        }

        // make sure we found it somewhere
        if (!element_uri) {
            SRCMLstatus(WARNING_MSG, "srcml: no uri exists for prefix \"" + std::string(element->prefix->c_str()) + "\"");
            return -1;
        }
    }

    // Check attribute namespace
    char const * attribute_uri = 0;
    if (attribute) {
        attribute_uri = srcml_archive_get_uri_from_prefix(out_arch, attribute->prefix->c_str());

        // if not declared, check for xmlns from cli
        if (!attribute_uri) {
            auto it = xmlns_namespaces.find(*(attribute->prefix));
            if (it != xmlns_namespaces.end())
                attribute_uri = it->second.c_str();
        }

        if (!attribute_uri) {
            SRCMLstatus(WARNING_MSG, "srcml: no uri exists for prefix \"" + std::string(attribute->prefix->c_str()) + "\"");
            return -1;
        }
    }

    // Call appropriate XPath transform
    if (element && attribute) {

        return srcml_append_transform_xpath_element_attribute (in_arch, transform_input.c_str(),
            element->prefix->c_str(),
            element_uri,
            element->name->c_str(),
            attribute->prefix->c_str(),
            attribute_uri,
            attribute->name->c_str(),
            attribute->value->c_str());

    } else if (element) {

        return srcml_append_transform_xpath_element (in_arch, transform_input.c_str(),
            element->prefix->c_str(),
            element_uri,
            element->name->c_str());

    } else if (attribute) {

        return srcml_append_transform_xpath_attribute (in_arch, transform_input.c_str(),
            attribute->prefix->c_str(),
            attribute_uri,
            attribute->name->c_str(),
            attribute->value->c_str());

    } else {

        return srcml_append_transform_xpath(in_arch, transform_input.c_str());
    }
}

int apply_xslt(srcml_archive* in_arch, const std::string& transform_input) {

    // xslt has file input, which may need to be processed
    int status;
    srcml_input_src xslt_file(transform_input.c_str());
    input_file(xslt_file);

    if (contains<int>(xslt_file))
        status = srcml_append_transform_xslt_fd(in_arch, xslt_file);
    else if (contains<FILE*>(xslt_file))
        status = srcml_append_transform_xslt_FILE(in_arch, xslt_file);
    else
        status = srcml_append_transform_xslt_filename(in_arch, xslt_file.c_str());

    return status;
}

int apply_relaxng(srcml_archive* in_arch, const std::string& transform_input) {

	// relaxng has file input, which may need to be processed
	int status;
	srcml_input_src relaxng_file(transform_input.c_str());
	input_file(relaxng_file);

    if (contains<int>(relaxng_file))
        status = srcml_append_transform_relaxng_fd(in_arch, relaxng_file);
    else if (contains<FILE*>(relaxng_file))
        status = srcml_append_transform_relaxng_FILE(in_arch, relaxng_file);
    else
        status = srcml_append_transform_relaxng_filename(in_arch, relaxng_file.c_str());

    return status;
}

// transform srcml with query or transformation
void transform_srcml(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& /* input_sources */,
    const srcml_output_dest& /* output */) {
/*
	// create output archive
	int status;
	srcml_archive* out_arch = srcml_archive_create();

    // open all input sources early so xml namespaces can be setup for the output archive
    std::vector<srcml_archive*> inarchives;
    for (const auto& input : input_sources) {
        srcml_archive* in_arch = srcml_archive_create();
        if (contains<int>(input)) {
            status = srcml_archive_read_open_fd(in_arch, input);
        } else if (contains<FILE*>(input)) {
            status = srcml_archive_read_open_FILE(in_arch, input);
        } else {
            status = srcml_archive_read_open_filename(in_arch, input.c_str());
        }
        if (status != SRCML_STATUS_OK) {
            SRCMLstatus(ERROR_MSG, "srcml: error with input archive for transformation");
            exit(-1);
        }

        inarchives.push_back(in_arch);
    }

    // register xml namespaces from the input sources onto the output
    for (const auto in_arch : inarchives) {
        // register the xml namespaces on the output from this input source
        for (int i = 0; i < (int)srcml_archive_get_namespace_size(in_arch); ++i) {
            srcml_archive_register_namespace(out_arch, srcml_archive_get_namespace_prefix(in_arch, i), srcml_archive_get_namespace_uri(in_arch, i));
        }
    }

    // register xml namespaces directly from the user
    for (const auto& itr : srcml_request.xmlns_namespaces) {
        srcml_archive_register_namespace(out_arch, itr.first.c_str(), itr.second.c_str());
    }

    // open the output source. perform this after namespace registration so it appears
    // correctly in the output xml
    if (contains<int>(output))
        status = srcml_archive_write_open_fd(out_arch, output);
    else if (contains<FILE*>(output))
        status = srcml_archive_write_open_FILE(out_arch, output);
    else
        status = srcml_archive_write_open_filename(out_arch, output.c_str(), 0);
    if (status != SRCML_STATUS_OK) {
        SRCMLstatus(ERROR_MSG, "srcml: error with output archive for transformation");
        exit(-1);
    }

    // process the content of the input sources
    for (const auto in_arch : inarchives) {

        // copy over any url that we find
        // @todo Doesn't seem to work on the output archive
        auto url = srcml_archive_get_url(in_arch);
        if (url) {
            srcml_archive_set_url(out_arch, url);
        }

        // see if we have any XPath output
        bool isxpath = false;
        for (const auto& trans : srcml_request.transformations) {
            std::string protocol;
            std::string resource;
            src_prefix_split_uri(trans, protocol, resource);

            if (protocol == "xpath") {
                isxpath = true;
                break;
            }
        }

        // for non-archive input, then we want non-archive output, fool
        if ((inarchives.size() == 1) && !isxpath && !srcml_archive_is_full_archive(in_arch)) {
            srcml_archive_disable_full_archive(out_arch);
        }

		// iterate through all transformations added during cli parsing
        int xpath_index = -1;
        for (const auto& trans : srcml_request.transformations) {
            std::string protocol;
            std::string resource;
            src_prefix_split_uri(trans, protocol, resource);

            if (protocol == "xpath") {
                if (apply_xpath(in_arch, out_arch, resource, srcml_request.xpath_query_support[++xpath_index], srcml_request.xmlns_namespaces) != SRCML_STATUS_OK) {
                    SRCMLstatus(ERROR_MSG, "srcml: error with xpath transformation");
                    exit(-1);
                }

            } else if (protocol == "xslt") {
                if (apply_xslt(in_arch, resource) != SRCML_STATUS_OK) {
                    SRCMLstatus(ERROR_MSG, "srcml: error with xslt transformation");
                    exit(-1);
                }

            } else if (protocol == "xpathparam") {
				//std::cerr << protocol << " : " << resource << "\n"; // Stub

            } else if (protocol == "relaxng") {
                if (apply_relaxng(in_arch, resource) != SRCML_STATUS_OK) {
                    SRCMLstatus(ERROR_MSG, "srcml: error with relaxng transformation");
                    exit(-1);
                }
            }
        }

        int status = srcml_apply_transforms(in_arch, out_arch);

        srcml_archive_close(in_arch);
        srcml_archive_free(in_arch);

        if (status != SRCML_STATUS_OK)
            exit(1);
    }

    srcml_archive_close(out_arch);
    srcml_archive_free(out_arch);

    if (contains<int>(output)) {
        close(*(output.fd));
    }
*/
}
