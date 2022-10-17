// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file relaxngTransformation.hpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef INCLUDED_RELAXNGTRANSFORMATION_HPP
#define INCLUDED_RELAXNGTRANSFORMATION_HPP

#include <libxml/parser.h>
#include <libxml/relaxng.h>

#include <libxml2_utilities.hpp>

#include <memory>
#include <vector>
#include <string>

#include <Transformation.hpp>

/**
 * relaxngTransformation
 *
 * Extends unit_dom to execute relaxng program and write results.
 */
class relaxngTransformation : public Transformation {
public :

    /**
     * relaxngTransformation
     * @param a_context_element an element that provides a context
     * @param options list of srcML options
     * @param stylesheet an relaxng stylesheet
     * @param params relaxng parameters
     *
     * Constructor.  Dynamically loads relaxng functions.
     */
    relaxngTransformation(/*OPTION_TYPE& options,*/ xmlDocPtr relaxng);

    /**
     * apply
     *
     * Apply relaxng program, writing results.
     *
     * @returns true on success false on failure.
     */
    virtual TransformationResult apply(xmlDocPtr doc, int position) const;

private :
    const std::unique_ptr<xmlRelaxNGParserCtxt> relaxng_parser_ctxt;
    const std::unique_ptr<xmlRelaxNG> rng;
};

#endif
