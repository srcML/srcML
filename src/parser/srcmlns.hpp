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

#include <vector>
#include <string>

/** xml declaration standalone attribute */
const char* const XML_DECLARATION_STANDALONE = "yes";

/** xml declaration version attribute= */
const char* const XML_VERSION = "1.0";

// srcML namespace URIs
/** Default srcML namespace uri */
const char* const SRCML_SRC_NS_URI = "http://www.sdml.info/srcML/src";

/** srcML preprocessor namespace uri */
const char* const SRCML_CPP_NS_URI = "http://www.sdml.info/srcML/cpp";

/** srcML error namespace uri */
const char* const SRCML_ERR_NS_URI = "http://www.sdml.info/srcML/srcerr";

/** srcDiff namespace uri */
const char* const SRCML_DIFF_NS_URI = "http://www.sdml.info/srcDiff";

/** Default srcML namespace prefix */
const char* const SRCML_SRC_NS_PREFIX_DEFAULT = "";

/** srcML preprocessor namespace prefix */
const char* const SRCML_CPP_NS_PREFIX_DEFAULT = "cpp";

/** srcML error namespace prefix */
const char* const SRCML_ERR_NS_PREFIX_DEFAULT = "err";

/** srcDiff namespace prefix */
const char* const SRCML_DIFF_NS_PREFIX_DEFAULT = "diff";

// srcML extension namespace URI's
/** srcML literal namespace uri */
const char* const SRCML_EXT_LITERAL_NS_URI = "http://www.sdml.info/srcML/literal";

/** srcML operator namespace uri */
const char* const SRCML_EXT_OPERATOR_NS_URI = "http://www.sdml.info/srcML/operator";

/** srcML modifier namespace uri */
const char* const SRCML_EXT_MODIFIER_NS_URI = "http://www.sdml.info/srcML/modifier";

/** srcML position namespace uri */
const char* const SRCML_EXT_POSITION_NS_URI = "http://www.sdml.info/srcML/position";

/** srcML OpenMP namespace uri */
const char* const SRCML_EXT_OPENMP_NS_URI = "http://www.sdml.info/srcML/OpenMP";

/** srcML literal namespace prefix */
const char* const SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT = "lit";

/** srcML operator namespace prefix */
const char* const SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT = "op";

/** srcML operator namespace prefix */
const char* const SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT = "type";

/** srcML operator namespace prefix */
const char* const SRCML_EXT_POSITION_NS_PREFIX_DEFAULT = "pos";

/** srcML OpenMP namespace prefix */
const char* const SRCML_EXT_OPENMP_NS_PREFIX_DEFAULT = "omp";

// srcML standard attributes
/** revision attribute @todo see if still needed */
const char* const UNIT_ATTRIBUTE_REVISION = "revision";

/** language attribute */
const char* const UNIT_ATTRIBUTE_LANGUAGE = "language";

/** directory attribute */
const char* const UNIT_ATTRIBUTE_DIRECTORY = "directory";

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

/** item (XPath) attribute */
const char* const UNIT_ATTRIBUTE_ITEM = "item";

/** item (XPath) attribute */
const char* const UNIT_ATTRIBUTE_PATH = "item";

#endif
