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
#include <string_view>
#include <array>
#include <vector>

using namespace ::std::literals::string_view_literals;

enum {
  NS_REQUIRED   = 1 << 0, // required for all srcML, and must be on the root
  NS_ROOT       = 1 << 1, // belongs on root tag only
  NS_USED       = 1 << 2, // was the namespace used
  NS_REGISTERED = 1 << 3, // registered by the user
  NS_STANDARD   = 1 << 4, // standard uri for srcML
};

struct Namespace {
    Namespace(std::string_view prefix, std::string_view uri, int flags)
        : prefix(prefix), uri(uri), flags(flags) {}
    std::string prefix;
    std::string uri;
    mutable int flags;
    std::string_view getPrefix() const { flags |= NS_USED; return prefix; }
};

typedef std::vector<Namespace> Namespaces;

/** xml declaration standalone attribute */
constexpr inline auto XML_DECLARATION_STANDALONE = "yes"sv;

/** xml declaration version attribute= */
constexpr inline auto XML_VERSION = "1.0"sv;

// srcML namespace URIs
/** Default srcML namespace uri and default prefix */
constexpr inline auto SRCML_SRC_NS_URI = "http://www.srcML.org/srcML/src"sv;

constexpr inline auto SRCML_SRC_NS_DEFAULT_PREFIX = ""sv;

/** srcML preprocessor namespace uri and default prefix */
constexpr inline auto SRCML_CPP_NS_URI = "http://www.srcML.org/srcML/cpp"sv;

constexpr inline auto SRCML_CPP_NS_DEFAULT_PREFIX = "cpp"sv;

/** srcML error namespace uri and default prefix */
constexpr inline auto SRCML_ERROR_NS_URI = "http://www.srcML.org/srcML/error"sv;

constexpr inline auto SRCML_ERROR_NS_DEFAULT_PREFIX = "err"sv;

/** srcML position namespace uri and default prefix */
constexpr inline auto SRCML_POSITION_NS_URI = "http://www.srcML.org/srcML/position"sv;

constexpr inline auto SRCML_POSITION_NS_DEFAULT_PREFIX = "pos"sv;

/** srcML OpenMP namespace uri and default prefix */
constexpr inline auto SRCML_OPENMP_NS_URI = "http://www.srcML.org/srcML/openmp"sv;

constexpr inline auto SRCML_OPENMP_NS_DEFAULT_PREFIX = "omp"sv;

/** srcDiff namespace uri and default prefix */
constexpr inline auto SRCML_DIFF_NS_URI = "http://www.srcML.org/srcDiff"sv;

constexpr inline auto SRCML_DIFF_NS_DEFAULT_PREFIX = "diff"sv;

/** default namespaces */
const Namespaces default_namespaces = {
    Namespace(SRCML_SRC_NS_DEFAULT_PREFIX,      SRCML_SRC_NS_URI,      NS_STANDARD | NS_REQUIRED),
    Namespace(SRCML_CPP_NS_DEFAULT_PREFIX,      SRCML_CPP_NS_URI,      NS_STANDARD),
    Namespace(SRCML_ERROR_NS_DEFAULT_PREFIX,    SRCML_ERROR_NS_URI,    NS_STANDARD | NS_ROOT),
    Namespace(SRCML_POSITION_NS_DEFAULT_PREFIX, SRCML_POSITION_NS_URI, NS_STANDARD | NS_ROOT),
    Namespace(SRCML_OPENMP_NS_DEFAULT_PREFIX,   SRCML_OPENMP_NS_URI,   NS_STANDARD),
};

const Namespaces starting_namespaces = {
    Namespace(SRCML_SRC_NS_DEFAULT_PREFIX,     SRCML_SRC_NS_URI,      NS_STANDARD | NS_REQUIRED),
};

void addNamespace(Namespaces& namespaces, std::string_view uri, std::string_view prefix);

struct Attribute {
    Attribute(std::string_view uri, std::string_view prefix, std::string_view name, std::string_view value)
        : uri(uri), prefix(prefix), name(name), value(value) {}
    std::string uri;
    std::string prefix;
    std::string name;
    std::string value;
};

typedef std::vector<Attribute> Attributes;

void addAttribute(Attributes& attributes, std::string_view uri, std::string_view prefix, std::string_view name, std::string_view value);

// srcML standard attributes
/** revision attribute */
constexpr inline auto UNIT_ATTRIBUTE_REVISION = "revision"sv;

/** language attribute */
constexpr inline auto UNIT_ATTRIBUTE_LANGUAGE = "language"sv;

/** url attribute */
constexpr inline auto UNIT_ATTRIBUTE_URL = "url"sv;

/** filename attribute */
constexpr inline auto UNIT_ATTRIBUTE_FILENAME = "filename"sv;

/** version attribute */
constexpr inline auto UNIT_ATTRIBUTE_VERSION = "version"sv;

/** options attribute */
constexpr inline auto UNIT_ATTRIBUTE_OPTIONS = "options"sv;

/** timestamp attribute */
constexpr inline auto UNIT_ATTRIBUTE_TIMESTAMP = "timestamp"sv;

/** hash checksum attribute */
constexpr inline auto UNIT_ATTRIBUTE_HASH = "hash"sv;

/** hash checksum attribute */
constexpr inline auto UNIT_ATTRIBUTE_SOURCE_ENCODING = "src-encoding"sv;

/** item (XPath) attribute */
constexpr inline auto UNIT_ATTRIBUTE_ITEM = "item"sv;

/** item (XPath) attribute */
constexpr inline auto UNIT_ATTRIBUTE_PATH = "item"sv;

const std::array<std::string, 2> SRCML_URI_PREFIX = {{ "http://www.srcML.org/", "http://www.sdml.info/" }};

// normalize the uri prefix to http://www.srcML.org/
std::string& srcml_uri_normalize(std::string& uri);

// merge in the other namespace
Namespaces& operator +=(Namespaces& ns, const Namespaces& otherns);

Namespaces::iterator findNSURI(Namespaces& namespaces, std::string_view uri);

Namespaces::iterator findNSPrefix(Namespaces& namespaces, std::string_view prefix);

Namespaces::const_iterator findNSURI(const Namespaces& namespaces, std::string_view uri);

Namespaces::const_iterator findNSPrefix(const Namespaces& namespaces, std::string_view prefix);

// is a srcdiff archive
bool issrcdiff(const Namespaces& namespaces);

#endif
