/**
 * @file Transformation.hpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_TRANSFORMATION_HPP
#define INCLUDED_TRANSFORMATION_HPP

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <string>
#include <vector>
#include <libxml2_utilities.hpp>
#include <memory>
#include <boost/optional.hpp>
#include <srcml.h>

struct TransformationResult {
    TransformationResult(xmlNodeSetPtr nodeset = nullptr, bool wrapped = false)
        : nodeset(nodeset), unitWrapped(wrapped) {}
    int nodeType = 0;
    std::unique_ptr<xmlNodeSet> nodeset;
    double numberValue = 0.0;
    bool boolValue = false;
    std::string stringValue;
    bool unitWrapped = false;
    std::shared_ptr<xmlDoc> doc;
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
