// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_sax2_utilities.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCML_SAX2_UTILITIES_HPP
#define INCLUDED_SRCML_SAX2_UTILITIES_HPP

#include <srcml_types.hpp>
#include <srcml.h>

#include <libxml/parserInternals.h>

int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char * context_element, const char * xpath,
                const char * prefix, const char * uri, const char * element, const char * attr_prefix, const char * attr_uri, const char * attr_name, const char * attr_value,
                OPTION_TYPE options, srcml_archive* oarchive);

int srcml_relaxng(xmlParserInputBufferPtr input_buffer, xmlDocPtr relaxng, OPTION_TYPE options, srcml_archive* oarchive);

#endif
