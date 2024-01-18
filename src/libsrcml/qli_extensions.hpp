// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file qli_extensions.hpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#ifndef QLI_EXTENSION_FUNCTIONS_HPP
#define QLI_EXTENSION_FUNCTIONS_HPP

#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

void add_element(xmlXPathParserContext* context, int nargs);

void match_element(xmlXPathParserContext* context, int nargs);

void clear_elements(xmlXPathParserContext* context, int nargs);

void is_valid_element(xmlXPathParserContext* context, int nargs);

void intersect(xmlXPathParserContext* ctxt, int nargs);

void difference(xmlXPathParserContext* ctxt, int nargs);

void debug_print(xmlXPathParserContext* ctxt, int nargs);

#endif
