// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLException.hpp
 *
 * @copyright Copyright (C) 2004-2019 srcML, LLC. (www.srcML.org)
 *
 * Changes segmentation errors to an thrown exception.
 */

#ifndef SRCMLEXCEPTION_HPP
#define SRCMLEXCEPTION_HPP

/**
 * Segmentation_Fault
 *
 * Class for segmentation fault exception.
 */
class Segmentation_Fault : public std::exception {};

#endif
