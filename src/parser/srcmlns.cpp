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
            namespaces.push_back({ ns.prefix, ns.uri, ns.flags });
        }
    }

    return namespaces;
}

/**
 * is_srcml_namespace
 * @param uri the uri to compare
 * @param srcml_uri the srcML URI to compare to
 *
 * Compare uri to the srcML uri idependent of prefix
 * i.e., www.sdml.info or www.srcML.org.
 *
 * @returns if the two uris are equal.
 */
bool is_srcml_namespace(const std::string& uri, const std::string& srcml_uri) {

    if (uri == srcml_uri)
        return true;

    auto uri_suffix = uri;
    for (auto& prefix : SRCML_URI_PREFIX) {
        if (uri.substr(0, prefix.size()) == prefix) {
            uri_suffix = uri.substr(prefix.size());
            break;
        }
    }

    auto srcml_uri_suffix = srcml_uri;
    for (auto& prefix : SRCML_URI_PREFIX) {
        if (srcml_uri.substr(0, prefix.size()) == prefix) {
            srcml_uri_suffix = srcml_uri.substr(prefix.size());
            break;
        }
    }

    return uri_suffix == srcml_uri_suffix;
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

Namespaces::iterator findNSURI(Namespaces& namespaces, const std::string& uri) {

    auto ns = std::find_if(namespaces.begin(), namespaces.end(), [uri](const Namespace& nsarg)->bool {
        return nsarg.uri == uri; });

    return ns;
}

Namespaces::iterator findNSPrefix(Namespaces& namespaces, const std::string& prefix) {

    // find the last one (yes, could use reverse iterators, but then conversion to return type)
    auto it = namespaces.end();
    for (auto p = namespaces.begin(); p != namespaces.end(); ++p) {
        if (p->prefix == prefix)
            it = p;
    }

    return it;
}

Namespaces::const_iterator findNSURI(const Namespaces& namespaces, const std::string& uri) {

    return std::find_if(namespaces.cbegin(), namespaces.cend(), [uri](const Namespace& nsarg)->bool {
        return nsarg.uri == uri; });
}

Namespaces::const_iterator findNSPrefix(const Namespaces& namespaces, const std::string& prefix) {

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
