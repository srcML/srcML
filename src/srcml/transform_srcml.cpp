/**
 * @file transform_srcml.hpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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
#include <boost/foreach.hpp>

 int apply_xpath(srcml_archive* in_arch, const std::string& transform_input, const std::pair< boost::optional<element>, boost::optional<attribute> >& xpath_support) {
 	// FIRST IS ELEMENT / SECOND IS ATTRIBUTE

    const char* element_uri = srcml_archive_get_uri_from_prefix(in_arch, xpath_support.first->prefix->c_str());
    const char* attribute_uri = srcml_archive_get_uri_from_prefix(in_arch, xpath_support.second->prefix->c_str());

    // Must have a uri for the element
    if (xpath_support.first && !(element_uri))
        return -1;

    // Must have a uri for the attribute
    if (xpath_support.second && !(attribute_uri))
        return -1;


 	// Check for element
 	if (xpath_support.first){
 		// See if an attribute is present as well
 		if (xpath_support.second) {
 			return srcml_append_transform_xpath_element (in_arch, transform_input.c_str(),
                                                            xpath_support.first->prefix->c_str(),
                                                            element_uri,
                                                            xpath_support.first->name->c_str(),
                                                            xpath_support.second->prefix->c_str(),
                                                            attribute_uri,
                                                            xpath_support.second->name->c_str(),
                                                            xpath_support.second->value->c_str());
 		}
 		else {
 			return srcml_append_transform_xpath_element (in_arch, transform_input.c_str(),
                                                            xpath_support.first->prefix->c_str(),
                                                            element_uri,
                                                            xpath_support.first->name->c_str(),
                                                            NULL, NULL, NULL, NULL);
 		}
 	}

 	// Check for attribute
 	if (xpath_support.second) {
 		return srcml_append_transform_xpath_attribute (in_arch, transform_input.c_str(),
                                                            xpath_support.second->prefix->c_str(),
                                                            attribute_uri,
                                                            xpath_support.second->name->c_str(),
                                                            xpath_support.second->value->c_str());
 	}

 	return srcml_append_transform_xpath(in_arch, transform_input.c_str());
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
void transform_srcml(const srcml_request_t& srcml_request,
                     const srcml_input_t& input_sources,
                     const srcml_output_dest& output) {

	// Convert output into srcml archive
	int status;
	srcml_archive* out_arch = srcml_create_archive();
    if (contains<int>(output))
        status = srcml_write_open_fd(out_arch, output);
    else if (contains<FILE*>(output))
        status = srcml_write_open_FILE(out_arch, output);
    else
        status = srcml_write_open_filename(out_arch, output.c_str());
    if (status != SRCML_STATUS_OK)
        throw status;

    // Convert inputs into srcml archive
	BOOST_FOREACH(const srcml_input_src& input, input_sources) {
        srcml_archive* in_arch = srcml_create_archive();
        if (contains<int>(input))
            status = srcml_read_open_fd(in_arch, input);
        else if (contains<FILE*>(input))
            status = srcml_read_open_FILE(in_arch, input);
        else
            status = srcml_read_open_filename(in_arch, input.c_str());
        if (status != SRCML_STATUS_OK)
            throw status;

		// iterate through all transformations added during cli parsing
		int xpath_index = -1;
		BOOST_FOREACH(const std::string& trans, srcml_request.transformations) {
			std::string protocol;
			std::string resource;
			src_prefix_split_uri(trans, protocol, resource);

			if (protocol == "xpath") {
				if (apply_xpath(in_arch, resource, srcml_request.xpath_query_support.at(++xpath_index)) != SRCML_STATUS_OK)
					throw status;
			}
			else if (protocol == "xslt") {
		        if (apply_xslt(in_arch, resource) != SRCML_STATUS_OK)
		            throw status;
				
				std::cerr << protocol << " : " << resource << "\n"; // Debug Printout
			}
			else if (protocol == "xpathparam") {
				std::cerr << protocol << " : " << resource << "\n"; // Stub
			}
			else if (protocol == "relaxng") {
		        if (apply_relaxng(in_arch, resource) != SRCML_STATUS_OK)
		            throw status;

				std::cerr << protocol << " : " << resource << "\n"; //Debug Printout
			}
		}
		srcml_apply_transforms(in_arch, out_arch);

		srcml_close_archive(in_arch);
		srcml_free_archive(in_arch);
	}

	srcml_close_archive(out_arch);
	srcml_free_archive(out_arch);
}
