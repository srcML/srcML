/**
 * @file Options.hpp
 *
 * @copyright Copyright (C) 2005-2014 SDML (www.srcML.org)
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

/*
  Options for translation output.
*/

#ifndef INCLUDED_OPTION_HPP
#define INCLUDED_OPTION_HPP

#ifdef __GNUC__

/** size of option type in GNU */
typedef unsigned long long OPTION_TYPE;

#else

/** size of option type in non-GNU */
typedef unsigned __int64 OPTION_TYPE;

#endif

#ifdef __GNUC__

/** size of options literal in GNU */
#define ull(a) a##ULL

#else

/** size of options literal in non-GNU */
#define ull(a) a##i64

#endif

/**
 * isoption
 * @param options the set options
 * @param flag options to query if set
 *
 * Predicate to query if options flag are set in options.
 *
 * @returns if options flag are set in options.
 */
inline bool isoption(OPTION_TYPE options, OPTION_TYPE flag) {
    return (flag & options) > 0;
}

// begin SRCML_OPTIONS

// end SRCML_OPTIONS

#endif
