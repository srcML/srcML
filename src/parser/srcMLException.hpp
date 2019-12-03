/**
 * @file srcMLException.hpp
 *
 * @copyright Copyright (C) 2004-2019 srcML, LLC. (www.srcML.org)
 *
 * This include file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This include file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this include file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
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
