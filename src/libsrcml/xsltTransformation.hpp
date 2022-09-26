/**
 * @file xsltTransformation.hpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
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

 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_XSLTTRANSFORMATION_HPP
#define INCLUDED_XSLTTRANSFORMATION_HPP

#include <Transformation.hpp>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>

#ifdef _MSC_VER
#include <io.h>
#endif

/**
 * xsltTransformation
 *
 * Extends unit_dom to execute XSLT program and write results.
 */
class xsltTransformation : public Transformation {
public :

    /**
     * xsltTransformation
     * @param a_context_element an element that provides a context
     * @param options list of srcML options
     * @param stylesheet an XSLT stylesheet
     * @param params XSLT parameters
     *
     * Constructor.  Dynamically loads XSLT functions.
     */
    xsltTransformation(/*OPTION_TYPE& options,*/ xmlDocPtr xslt, const std::vector<std::string>& params);

    /**
     * ~xsltTransformation
     *
     * Destructor.  Closes dynamically loaded library.
     */
    virtual ~xsltTransformation();

    /**
     * apply
     *
     * Apply XSLT program, writing results.
     *
     * @returns true on success false on failure.
     */
    virtual TransformationResult apply(xmlDocPtr doc, int position) const;

private :
    xsltStylesheetPtr stylesheet = nullptr;
    const std::vector<std::string> params;
};

#endif
