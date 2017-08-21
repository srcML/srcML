/**
 * @file srcmlns.hpp
 *
 * @copyright Copyright (C) 2005-2012 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * Definitions of srcML namespace URI's
 */

#ifndef INCLUDED_SRCMLNS_HPP
#define INCLUDED_SRCMLNS_HPP

#include <string>
#include <array>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <string>

struct Namespace {
    std::string prefix;
    std::string uri;
    bool used;
};

namespace nstags
{
    struct prefix {};
    struct uri {};
    struct position {};
}

typedef boost::multi_index::multi_index_container<Namespace,
  	boost::multi_index::indexed_by<
  		// default access, indexing
    	boost::multi_index::random_access<
      		boost::multi_index::tag<nstags::position>
    	>,
    	// view based on prefix
	    boost::multi_index::hashed_unique<
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
/** Default srcML namespace uri */
const char* const SRCML_SRC_NS_URI = "http://www.srcML.org/srcML/src";

/** srcML preprocessor namespace uri */
const char* const SRCML_CPP_NS_URI = "http://www.srcML.org/srcML/cpp";

/** srcML error namespace uri */
const char* const SRCML_ERR_NS_URI = "http://www.srcML.org/srcML/error";

/** srcDiff namespace uri */
const char* const SRCML_DIFF_NS_URI = "http://www.srcML.org/srcDiff";

/** Default srcML namespace prefix */
const char* const SRCML_SRC_NS_PREFIX_DEFAULT = "";

/** srcML preprocessor namespace prefix */
const char* const SRCML_CPP_NS_PREFIX_DEFAULT = "cpp";

/** srcML error namespace prefix */
const char* const SRCML_ERR_NS_PREFIX_DEFAULT = "err";

/** srcDiff namespace prefix */
const char* const SRCML_DIFF_NS_PREFIX_DEFAULT = "diff";

/** srcML position namespace uri */
const char* const SRCML_EXT_POSITION_NS_URI = "http://www.srcML.org/srcML/position";

/** srcML OpenMP namespace uri */
const char* const SRCML_EXT_OPENMP_NS_URI = "http://www.srcML.org/srcML/openmp";

/** srcML operator namespace prefix */
const char* const SRCML_EXT_POSITION_NS_PREFIX_DEFAULT = "pos";

/** srcML OpenMP namespace prefix */
const char* const SRCML_EXT_OPENMP_NS_PREFIX_DEFAULT = "omp";

// srcML standard attributes
/** revision attribute @todo see if still needed */
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
bool is_srcml_namespace(const std::string & uri, const std::string & srcml_uri);

// normalize the uri prefix to http://www.srcML.org/
std::string & srcml_uri_normalize(std::string & uri);

#endif
