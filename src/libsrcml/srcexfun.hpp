/**
 * @file srcexfun.hpp
 *
 * @copyright Copyright (C) 2009-2014 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_SRCEXFUN_HPP
#define INCLUDED_SRCEXFUN_HPP

#include <libxml/xpath.h>

#include <string>
#include <vector>

/** Properties are pair of strings */
typedef std::pair<std::string, std::string> PROPERTIES_TYPE[32];

void setPosition(int n);

void setRootAttributes(const xmlChar** attributes, int pnb_attributes);

void setRootAttributes(PROPERTIES_TYPE&);

void xsltsrcMLRegister();

void xpathsrcMLRegister(xmlXPathContextPtr context);

/**
 * xpath_ext_function
 * 
 * Data class for xpath extention functions.
 */
struct xpath_ext_function {

    /** prefix for extension function */
    std::string prefix;

    /** name of extension function */
    std::string name;
    /** the xpath expression */
    std::string expr;
};

void xpathRegisterExtensionFunction(const std::string& uri, const std::string & name, const std::string & xpath);

const std::vector<xpath_ext_function> getXPathExtensionFunctions();

#endif
