/**
 * @file srcmlns.cpp
 *
 * @copyright Copyright (C) 2005-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <srcmlns.hpp>

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
