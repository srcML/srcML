// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Transformation.hpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef INCLUDED_TRANSFORMATION_HPP
#define INCLUDED_TRANSFORMATION_HPP

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <string>
#include <vector>
#include <libxml2_utilities.hpp>
#include <memory>
#include <optional>
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
