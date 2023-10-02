// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file transform_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <transform_srcml.hpp>
#include <src_prefix.hpp>
#include <input_file.hpp>
#include <srcml.h>
#include <string>
#include <SRCMLStatus.hpp>

int apply_xpath(srcml_archive* in_arch, srcml_archive* out_arch, std::string_view transform_input, const std::pair< std::optional<element>, std::optional<attribute> >& xpath_support, const std::map<std::string,std::string>& xmlns_namespaces) {

    auto element = xpath_support.first;
    auto attribute = xpath_support.second;

    // Check element namespace
    char const * element_uri = 0;
    if (element){

        // check first if namespace is already declared on the output archive
        element_uri = srcml_archive_get_uri_from_prefix(out_arch, element->prefix->data());

        // if not, check on the input archive
        if (!element_uri) {
            element_uri = srcml_archive_get_uri_from_prefix(in_arch, element->prefix->data());
        }

        if (!element_uri) {
            auto it = xmlns_namespaces.find(*(element->prefix));
            if (it != xmlns_namespaces.end())
                element_uri = it->second.data();
        }

        // make sure we found it somewhere
        if (!element_uri) {
            SRCMLstatus(WARNING_MSG, "srcml: no uri exists for prefix \"" + *element->prefix + "\"");
            return -1;
        }
    }

    // Check attribute namespace
    char const * attribute_uri = 0;
    if (attribute) {
        attribute_uri = srcml_archive_get_uri_from_prefix(out_arch, attribute->prefix->data());

        // if not declared, check for xmlns from cli
        if (!attribute_uri) {
            auto it = xmlns_namespaces.find(*(attribute->prefix));
            if (it != xmlns_namespaces.end())
                attribute_uri = it->second.data();
        }

        if (!attribute_uri) {
            SRCMLstatus(WARNING_MSG, "srcml: no uri exists for prefix \"" + *attribute->prefix + "\"");
            return -1;
        }
    }

    // save for future xpath normalization
    std::string nxpath(transform_input);

    // Call appropriate XPath transform
    if (element && attribute) {

        int status = srcml_append_transform_xpath_element(in_arch, nxpath.data(),
            element->prefix->data(),
            element_uri,
            element->name->data());
        if (status != SRCML_STATUS_OK)
            return -1;

        return srcml_append_transform_xpath_attribute(in_arch, nxpath.data(),
            attribute->prefix->data(),
            attribute_uri,
            attribute->name->data(),
            attribute->value->data());

    } else if (element) {

        return srcml_append_transform_xpath_element(in_arch, nxpath.data(),
            element->prefix->data(),
            element_uri,
            element->name->data());

    } else if (attribute) {

        return srcml_append_transform_xpath_attribute(in_arch, nxpath.data(),
            attribute->prefix->data(),
            attribute_uri,
            attribute->name->data(),
            attribute->value->data());

    } else {

        // for xpath individual results the output archive is always a non-solo unit
        srcml_archive_disable_solitary_unit(out_arch);

        return srcml_append_transform_xpath(in_arch, nxpath.data());
    }
}

int apply_srcql(srcml_archive* in_arch, srcml_archive* out_arch, std::string_view transform_input, const std::pair< std::optional<element>, std::optional<attribute> >& srcql_support, const std::map<std::string,std::string>& xmlns_namespaces) {

    auto element = srcql_support.first;
    auto attribute = srcql_support.second;

    // Check element namespace
    char const * element_uri = 0;
    if (element){

        // check first if namespace is already declared on the output archive
        element_uri = srcml_archive_get_uri_from_prefix(out_arch, element->prefix->data());

        // if not, check on the input archive
        if (!element_uri) {
            element_uri = srcml_archive_get_uri_from_prefix(in_arch, element->prefix->data());
        }

        if (!element_uri) {
            auto it = xmlns_namespaces.find(*(element->prefix));
            if (it != xmlns_namespaces.end())
                element_uri = it->second.data();
        }

        // make sure we found it somewhere
        if (!element_uri) {
            SRCMLstatus(WARNING_MSG, "srcml: no uri exists for prefix \"" + *element->prefix + "\"");
            return -1;
        }
    }

    // Check attribute namespace
    char const * attribute_uri = 0;
    if (attribute) {
        attribute_uri = srcml_archive_get_uri_from_prefix(out_arch, attribute->prefix->data());

        // if not declared, check for xmlns from cli
        if (!attribute_uri) {
            auto it = xmlns_namespaces.find(*(attribute->prefix));
            if (it != xmlns_namespaces.end())
                attribute_uri = it->second.data();
        }

        if (!attribute_uri) {
            SRCMLstatus(WARNING_MSG, "srcml: no uri exists for prefix \"" + *attribute->prefix + "\"");
            return -1;
        }
    }

    // save for future srcql normalization
    std::string nsrcql(transform_input);

    // Call appropriate XPath transform
    if (element && attribute) {

        int status = srcml_append_transform_srcql_element(in_arch, nsrcql.data(),
            element->prefix->data(),
            element_uri,
            element->name->data());
        if (status != SRCML_STATUS_OK)
            return -1;

        return srcml_append_transform_srcql_attribute(in_arch, nsrcql.data(),
            attribute->prefix->data(),
            attribute_uri,
            attribute->name->data(),
            attribute->value->data());

    } else if (element) {

        return srcml_append_transform_srcql_element(in_arch, nsrcql.data(),
            element->prefix->data(),
            element_uri,
            element->name->data());

    } else if (attribute) {

        return srcml_append_transform_srcql_attribute(in_arch, nsrcql.data(),
            attribute->prefix->data(),
            attribute_uri,
            attribute->name->data(),
            attribute->value->data());

    } else {

        // for srcql individual results the output archive is always a non-solo unit
        srcml_archive_disable_solitary_unit(out_arch);

        return srcml_append_transform_srcql(in_arch, nsrcql.data());
    }
}

int apply_xslt(srcml_archive* in_arch, std::string_view transform_input) {

    // xslt has file input, which may need to be processed
    int status;
    srcml_input_src xslt_file(transform_input.data());
    input_file(xslt_file);

    if (contains<int>(xslt_file))
        status = srcml_append_transform_xslt_fd(in_arch, xslt_file);
    else if (contains<FILE*>(xslt_file))
        status = srcml_append_transform_xslt_FILE(in_arch, xslt_file);
    else
        status = srcml_append_transform_xslt_filename(in_arch, xslt_file.data());

    return status;
}

int apply_relaxng(srcml_archive* in_arch, std::string_view transform_input) {

    // relaxng has file input, which may need to be processed
    int status;
    srcml_input_src relaxng_file(transform_input.data());
    input_file(relaxng_file);

    if (contains<int>(relaxng_file))
        status = srcml_append_transform_relaxng_fd(in_arch, relaxng_file);
    else if (contains<FILE*>(relaxng_file))
        status = srcml_append_transform_relaxng_FILE(in_arch, relaxng_file);
    else
        status = srcml_append_transform_relaxng_filename(in_arch, relaxng_file.data());

    return status;
}
