/**
 * @file srcmlns.hpp
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

/**
 * Definitions of srcML namespace URI's
 */

#ifndef INCLUDED_SRCMLNS_HPP
#define INCLUDED_SRCMLNS_HPP

#include <string>
#include <array>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <string>

enum {
  NS_REQUIRED   = 1 << 0, // required for all srcML, and must be on the root
  NS_ROOT       = 1 << 1, // belongs on root tag only
  NS_USED       = 1 << 2, // was the namespace used
  NS_REGISTERED = 1 << 3, // registered by the user
  NS_STANDARD   = 1 << 4, // standard uri for srcML
};

struct Namespace {
    std::string prefix;
    std::string uri;
    mutable int flags;
    const std::string& getPrefix() const { flags |= NS_USED; return prefix; }
};

namespace nstags
{
    struct prefix {};
    struct uri {};
    struct position {};
}

// data structure for namespaces, prefixes and uri's
typedef boost::multi_index::multi_index_container<Namespace,
    boost::multi_index::indexed_by<
        // default access, indexing
        boost::multi_index::random_access<
            boost::multi_index::tag<nstags::position>
        >,
        // view based on prefix
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag<nstags::prefix>,
          boost::multi_index::member<Namespace, std::string, &Namespace::prefix>
        >,
        // view based on uri
        boost::multi_index::hashed_non_unique<
          boost::multi_index::tag<nstags::uri>,
          boost::multi_index::member<Namespace, std::string, &Namespace::uri>
        >
    >
> Namespaces;

/** xml declaration standalone attribute */
const char* const XML_DECLARATION_STANDALONE = "yes";

/** xml declaration version attribute= */
const char* const XML_VERSION = "1.0";

// srcML namespace URIs
/** Default srcML namespace uri and default prefix */
const char* const SRCML_SRC_NS_URI = "http://www.srcML.org/srcML/src";

const char* const SRCML_SRC_NS_DEFAULT_PREFIX = "";

/** srcML preprocessor namespace uri and default prefix */
const char* const SRCML_CPP_NS_URI = "http://www.srcML.org/srcML/cpp";

const char* const SRCML_CPP_NS_DEFAULT_PREFIX = "cpp";

/** srcML error namespace uri and default prefix */
const char* const SRCML_ERROR_NS_URI = "http://www.srcML.org/srcML/error";

const char* const SRCML_ERROR_NS_DEFAULT_PREFIX = "err";

/** srcML position namespace uri and default prefix */
const char* const SRCML_POSITION_NS_URI = "http://www.srcML.org/srcML/position";

const char* const SRCML_POSITION_NS_DEFAULT_PREFIX = "pos";

/** srcML OpenMP namespace uri and default prefix */
const char* const SRCML_OPENMP_NS_URI = "http://www.srcML.org/srcML/openmp";

const char* const SRCML_OPENMP_NS_DEFAULT_PREFIX = "omp";

/** srcDiff namespace uri and default prefix */
const char* const SRCML_DIFF_NS_URI = "http://www.srcML.org/srcDiff";

const char* const SRCML_DIFF_NS_DEFAULT_PREFIX = "diff";

/** default namespaces */
const Namespaces default_namespaces = {
    { SRCML_SRC_NS_DEFAULT_PREFIX,      SRCML_SRC_NS_URI,      NS_STANDARD | NS_REQUIRED },
    { SRCML_CPP_NS_DEFAULT_PREFIX,      SRCML_CPP_NS_URI,      NS_STANDARD },
    { SRCML_ERROR_NS_DEFAULT_PREFIX,    SRCML_ERROR_NS_URI,    NS_STANDARD | NS_ROOT },
    { SRCML_POSITION_NS_DEFAULT_PREFIX, SRCML_POSITION_NS_URI, NS_STANDARD | NS_ROOT },
    { SRCML_OPENMP_NS_DEFAULT_PREFIX,   SRCML_OPENMP_NS_URI,   NS_STANDARD },
};

const Namespaces starting_namespaces = {
    { SRCML_SRC_NS_DEFAULT_PREFIX,      SRCML_SRC_NS_URI,      NS_STANDARD | NS_REQUIRED },
};

// srcML standard attributes
/** revision attribute */
const char* const UNIT_ATTRIBUTE_REVISION = "revision";

/** language attribute */
const char* const UNIT_ATTRIBUTE_LANGUAGE = "language";

/**url attribute */
const char* const UNIT_ATTRIBUTE_URL = "url";

/** filename attribute */
const char* const UNIT_ATTRIBUTE_FILENAME = "filename";

/** version attribute */
const char* const UNIT_ATTRIBUTE_VERSION = "version";

/** options attribute */
const char* const UNIT_ATTRIBUTE_OPTIONS = "options";

/** timestamp attribute */
const char* const UNIT_ATTRIBUTE_TIMESTAMP = "timestamp";

/** hash checksum attribute */
const char* const UNIT_ATTRIBUTE_HASH = "hash";

/** hash checksum attribute */
const char* const UNIT_ATTRIBUTE_SOURCE_ENCODING = "src-encoding";

/** item (XPath) attribute */
const char* const UNIT_ATTRIBUTE_ITEM = "item";

/** item (XPath) attribute */
const char* const UNIT_ATTRIBUTE_PATH = "item";

const std::array<std::string, 2> SRCML_URI_PREFIX = {{ "http://www.srcML.org/", "http://www.sdml.info/" }};

// check if uri is same as srcML uri regardless of uri prefix
bool is_srcml_namespace(const std::string& uri, const std::string& srcml_uri);

// normalize the uri prefix to http://www.srcML.org/
std::string& srcml_uri_normalize(std::string & uri);

// merge in the other namespace
Namespaces& operator +=(Namespaces& ns, const Namespaces& otherns);

// is a srcdiff archive
bool issrcdiff(const Namespaces& namespaces);

#endif
