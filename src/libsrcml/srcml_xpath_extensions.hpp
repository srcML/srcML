// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_xpath_extensions.hpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#ifndef SRCML_XPATH_EXTENSION_FUNCTIONS_HPP
#define SRCML_XPATH_EXTENSION_FUNCTIONS_HPP

#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

// Predicates
void evaluate_boolean_xpath(xmlXPathParserContext* context, int nargs);

#endif

