/**
 * @file Transformation.hpp
 *
 * @copyright Copyright (C) 2018 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_TRANSFORMATION_HPP
#define INCLUDED_TRANSFORMATION_HPP

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <boost/optional.hpp>

struct TransformationResult {
    TransformationResult(xmlNodeSetPtr nodeset = nullptr, bool wrapped = false)
        : nodeset(nodeset), unitWrapped(wrapped) {}
    int nodeType = 0;
    xmlNodeSetPtr nodeset = nullptr;
    double numberValue;
    bool boolValue;
    std::string stringValue;
    bool unitWrapped = false;
};

/**
 * Transformation
 *
 * Base for all unit transformations
 */
class Transformation {
public :

    /**
     * apply
     *
     * Apply transformation to doc, and returning new doc
     * 
     * @returns document on success, NULL on failure.
     */
    virtual TransformationResult apply(xmlDocPtr doc, int position) const = 0;

    virtual ~Transformation() {}

      /** XSLT parameters */
    std::vector<std::string> xsl_parameters;
};

#endif