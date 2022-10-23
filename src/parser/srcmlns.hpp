// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcmlns.hpp
 *
 * @copyright Copyright (C) 2005-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Definitions of srcML namespace URI's
 */

#ifndef INCLUDED_SRCMLNS_HPP
#define INCLUDED_SRCMLNS_HPP

#include <string>
#include <array>
#include <vector>

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

typedef std::vector<Namespace> Namespaces;

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

// normalize the uri prefix to http://www.srcML.org/
std::string& srcml_uri_normalize(std::string & uri);

// merge in the other namespace
Namespaces& operator +=(Namespaces& ns, const Namespaces& otherns);

Namespaces::iterator findNSURI(Namespaces& namespaces, const std::string& uri);

Namespaces::iterator findNSPrefix(Namespaces& namespaces, const std::string& prefix);

Namespaces::const_iterator findNSURI(const Namespaces& namespaces, const std::string& uri);

Namespaces::const_iterator findNSPrefix(const Namespaces& namespaces, const std::string& prefix);

// is a srcdiff archive
bool issrcdiff(const Namespaces& namespaces);

#endif
