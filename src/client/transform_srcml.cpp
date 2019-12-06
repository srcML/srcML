/**
 * @file transform_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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

        // check first if namespace is already declared on the output archive
        element_uri = srcml_archive_get_uri_from_prefix(out_arch, element->prefix->c_str());

        // if not, check on the input archive
        if (!element_uri) {
            element_uri = srcml_archive_get_uri_from_prefix(in_arch, element->prefix->c_str());
        }

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

    // save for future xpath normalization
    std::string nxpath = transform_input;

    // Call appropriate XPath transform
    if (element && attribute) {

        int status = srcml_append_transform_xpath_element(in_arch, nxpath.c_str(),
            element->prefix->c_str(),
            element_uri,
            element->name->c_str());
        if (status != SRCML_STATUS_OK)
            return -1;

        return srcml_append_transform_xpath_attribute(in_arch, nxpath.c_str(),
            attribute->prefix->c_str(),
            attribute_uri,
            attribute->name->c_str(),
            attribute->value->c_str());

    } else if (element) {

        return srcml_append_transform_xpath_element(in_arch, nxpath.c_str(),
            element->prefix->c_str(),
            element_uri,
            element->name->c_str());

    } else if (attribute) {

        return srcml_append_transform_xpath_attribute(in_arch, nxpath.c_str(),
            attribute->prefix->c_str(),
            attribute_uri,
            attribute->name->c_str(),
            attribute->value->c_str());

    } else {

        // for xpath individual results the output archive is always a non-solo unit
        srcml_archive_disable_solitary_unit(out_arch);

        return srcml_append_transform_xpath(in_arch, nxpath.c_str());
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
