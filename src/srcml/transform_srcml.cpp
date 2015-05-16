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
#include <boost/foreach.hpp>

 int apply_xpath(srcml_archive* in_arch, const std::string& transform_input, const std::pair< boost::optional<element>, boost::optional<attribute> >& xpath_support, const std::map<std::string,std::string>& xmlns_namespaces) {
 	// FIRST IS ELEMENT / SECOND IS ATTRIBUTE

 	// Check for element
 	if (xpath_support.first){

        // check first if namespace is already declared
        const char* element_uri = srcml_archive_get_uri_from_prefix(in_arch, xpath_support.first->prefix->c_str());
        
        // if not declared, check for xmlns from cli
        if (!element_uri) {
            std::map<std::string,std::string>::const_iterator it = xmlns_namespaces.find(*(xpath_support.first->prefix));
            if (it != xmlns_namespaces.end())            
                element_uri = it->second.c_str();
        }

        // make sure we found it somewhere
        if (!element_uri) {
            std::cerr << "srcml: no uri exists for prefix \"" << xpath_support.first->prefix->c_str() << "\"\n";
            return -1;
        }

 		// See if an attribute is present as well
 		if (xpath_support.second) {
            const char* attribute_uri = srcml_archive_get_uri_from_prefix(in_arch, xpath_support.second->prefix->c_str());
            
            // if not declared, check for xmlns from cli
            if (!attribute_uri) {
                std::map<std::string,std::string>::const_iterator it = xmlns_namespaces.find(*(xpath_support.second->prefix));
                if (it != xmlns_namespaces.end())            
                    attribute_uri = it->second.c_str();
            }

            if (!attribute_uri) {
                std::cerr << "srcml: no uri exists for prefix \"" << xpath_support.second->prefix->c_str() << "\"\n";
                return -1;
            }

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
        const char* attribute_uri = srcml_archive_get_uri_from_prefix(in_arch, xpath_support.second->prefix->c_str());
        
        // if not declared, check for xmlns from cli
        if (!attribute_uri) {
            std::map<std::string,std::string>::const_iterator it = xmlns_namespaces.find(*(xpath_support.second->prefix));
            if (it != xmlns_namespaces.end())            
                attribute_uri = it->second.c_str();
        }

        if (!attribute_uri) {
            std::cerr << "srcml: no uri exists for prefix \"" << xpath_support.second->prefix->c_str() << "\"\n";
            return -1;
        }
 		
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
	srcml_archive* out_arch = srcml_archive_create();
    if (contains<int>(output))
        status = srcml_archive_write_open_fd(out_arch, output);
    else if (contains<FILE*>(output))
        status = srcml_archive_write_open_FILE(out_arch, output);
    else
        status = srcml_archive_write_open_filename(out_arch, output.c_str(), 0);
    if (status != SRCML_STATUS_OK) {
        std::cerr << "srcml: error with output archive for transformation\n";
        exit(-1);
    }

    // register xml namespaces
    std::map<std::string, std::string>::const_iterator itr;
    for (itr = srcml_request.xmlns_namespaces.begin(); itr != srcml_request.xmlns_namespaces.end(); ++itr){
        srcml_archive_register_namespace(out_arch, (*itr).first.c_str(), (*itr).second.c_str());
    }

    // Convert inputs into srcml archive
	BOOST_FOREACH(const srcml_input_src& input, input_sources) {
        srcml_archive* in_arch = srcml_archive_create();
        if (contains<int>(input))
            status = srcml_archive_read_open_fd(in_arch, input);
        else if (contains<FILE*>(input))
            status = srcml_archive_read_open_FILE(in_arch, input);
        else
            status = srcml_archive_read_open_filename(in_arch, input.c_str());
        if (status != SRCML_STATUS_OK) {
            std::cerr << "srcml: error with input archive for transformation\n";
            exit(-1);
        }

        // see if we have any XPath output
        bool isxpath = false;
        BOOST_FOREACH(const std::string& trans, srcml_request.transformations) {
            std::string protocol;
            std::string resource;
            src_prefix_split_uri(trans, protocol, resource);

            if (protocol == "xpath") {
                isxpath = true;
                break;
            }
        }

        // for non-archive input, then we want non-archive output, fool
        if (!isxpath && !(srcml_archive_get_options(in_arch) & SRCML_OPTION_ARCHIVE)) {
            srcml_archive_disable_option(out_arch, SRCML_OPTION_ARCHIVE);
        }

        // copy input xml namespaces
        // TODO: This assumes namespaces on first input. Need to open all, figure out
        // output namespaces, then process
        //if (srcml_archive_get_options(in_arch) & SRCML_OPTION_ARCHIVE) {
            for (int i = 0; i < (int)srcml_archive_get_namespace_size(in_arch); ++i) {

                // do not register the srcML namespace, unless the prefix is different
                if (std::string(srcml_archive_get_namespace_uri(in_arch, i)) == "http://www.srcML.org/srcML/src")
                    continue;

                srcml_archive_register_namespace(out_arch, srcml_archive_get_namespace_prefix(in_arch, i), srcml_archive_get_namespace_uri(in_arch, i));
            }
        //}

		// iterate through all transformations added during cli parsing
		int xpath_index = -1;
        BOOST_FOREACH(const std::string& trans, srcml_request.transformations) {
            std::string protocol;
            std::string resource;
            src_prefix_split_uri(trans, protocol, resource);

            if (protocol == "xpath") {
                // TODO: FIX BUG
				if (apply_xpath(in_arch, resource, srcml_request.xpath_query_support.at(++xpath_index), srcml_request.xmlns_namespaces) != SRCML_STATUS_OK) {
					std::cerr << "srcml: error with xpath transformation\n";
                    exit(-1);
                }
			}
			else if (protocol == "xslt") {
		        if (apply_xslt(in_arch, resource) != SRCML_STATUS_OK) {
		            std::cerr << "srcml: error with xslt transformation\n";
                    exit(-1);
                }
				
				//std::cerr << protocol << " : " << resource << "\n"; // Debug Printout
			}
			else if (protocol == "xpathparam") {
				//std::cerr << protocol << " : " << resource << "\n"; // Stub
			}
			else if (protocol == "relaxng") {
		        if (apply_relaxng(in_arch, resource) != SRCML_STATUS_OK) {
		            std::cerr << "srcml: error with relaxng transformation\n";
                    exit(-1);
                }

				//std::cerr << protocol << " : " << resource << "\n"; //Debug Printout
			}
		}
		srcml_apply_transforms(in_arch, out_arch);

		srcml_archive_close(in_arch);
		srcml_archive_free(in_arch);
	}

	srcml_archive_close(out_arch);
	srcml_archive_free(out_arch);
}
