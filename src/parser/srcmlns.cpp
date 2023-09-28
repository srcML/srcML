// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcmlns.cpp
 *
 * @copyright Copyright (C) 2005-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <srcmlns.hpp>
#include <algorithm>

void addNamespace(Namespaces& namespaces, std::string_view uri, std::string_view prefix) {

    // lookup by uri, if it already exists, update the prefix. If it doesn't exist, add it
    // prefix must be unique
    auto ituri = findNSURI(namespaces, uri);
    auto itprefix = findNSPrefix(namespaces, prefix);
    if (ituri != namespaces.end() && itprefix != namespaces.end()) {
        // existing prefix/uri, change prefix of existing namespace
        ituri->prefix = prefix;
    } else if (ituri != namespaces.end()) {
        // change prefix for existing uri
        ituri->prefix = prefix;
    } else {
        // add new namespace
        namespaces.emplace_back(prefix, uri, NS_REGISTERED);
    }
}

// namespace form of immediate add
// * Update prefixes
// * Add an new uri's
// * Or flags
Namespaces& operator+=(Namespaces& namespaces, const Namespaces& otherns) {

    for (const auto& ns : otherns) {

        // find where the new URI is in the default URI list, or not
        auto it = findNSURI(namespaces, ns.uri);
        if (it != namespaces.end()) {

            // update the default prefix, but only the default prefix
            auto default_it = findNSURI(default_namespaces, ns.uri);
            if (default_it == default_namespaces.end() || it->prefix == default_it->prefix) {
                it->prefix = ns.prefix;
                it->flags |= ns.flags;
            }

        } else {

            // create a new entry for this URI
            namespaces.emplace_back(ns.prefix, ns.uri, ns.flags);
        }
    }

    return namespaces;
}

/**
 * srcml_uri_normalize
 * @param uri the uri to normalize
 *
 * Normalize the uri to www.srcML.org
 *
 * @returns the normalized uri.
 */
std::string& srcml_uri_normalize(std::string& uri) {

    if (uri.substr(0, SRCML_URI_PREFIX[1].size()) == SRCML_URI_PREFIX[1])
        uri = SRCML_URI_PREFIX[0] + uri.substr(SRCML_URI_PREFIX[1].size());

    return uri;
}

Namespaces::iterator findNSURI(Namespaces& namespaces, std::string_view uri) {

    auto ns = std::find_if(namespaces.begin(), namespaces.end(), [uri](const Namespace& nsarg)->bool {
        return nsarg.uri == uri; });

    return ns;
}

Namespaces::iterator findNSPrefix(Namespaces& namespaces, std::string_view prefix) {

    // find the last one (yes, could use reverse iterators, but then conversion to return type)
    auto it = namespaces.end();
    for (auto p = namespaces.begin(); p != namespaces.end(); ++p) {
        if (p->prefix == prefix)
            it = p;
    }

    return it;
}

Namespaces::const_iterator findNSURI(const Namespaces& namespaces, std::string_view uri) {

    return std::find_if(namespaces.cbegin(), namespaces.cend(), [uri](const Namespace& nsarg)->bool {
        return nsarg.uri == uri; });
}

Namespaces::const_iterator findNSPrefix(const Namespaces& namespaces, std::string_view prefix) {

    // find the last one (yes, could use reverse iterators, but then conversion to return type)
    auto it = namespaces.cend();
    for (auto p = namespaces.cbegin(); p != namespaces.cend(); ++p) {
        if (p->prefix == prefix)
            it = p;
    }

    return it;
}

bool issrcdiff(const Namespaces& namespaces) {

    return findNSURI(namespaces, SRCML_DIFF_NS_URI) != namespaces.end();
}

void addAttribute(Attributes& attributes, std::string_view uri, std::string_view prefix, std::string_view name, std::string_view value) {

    // if we already have the attribute, then just update the value
    // otherwise create a new one
    auto itprefix = std::find_if(attributes.begin(), attributes.end(), [uri, prefix, name](const Attribute& nsarg)->bool {
        return nsarg.uri == uri && nsarg.prefix == prefix && nsarg.name == name; });

    if (itprefix != attributes.end()) {
        // update value of existing attribute
        itprefix->value = value;
    } else {
        // add new attribute
        attributes.emplace_back(uri, prefix, name, value);
    }
}
