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

// @src2srcml
// @srcml2src
/** unit is nested a root unit */
const OPTION_TYPE OPTION_ARCHIVE      = ull(1) << __COUNTER__; /* 0 */

// @src2srcml
/** markup literal values */
const OPTION_TYPE OPTION_LITERAL     = ull(1) << __COUNTER__; /* 1 */

// @src2srcml
/** markup type modifiers */
const OPTION_TYPE OPTION_MODIFIER     = ull(1) << __COUNTER__; /* 2 */

// @src2srcml
/** markup operators */
const OPTION_TYPE OPTION_OPERATOR     = ull(1) << __COUNTER__; /* 3 */

// @src2srcml
/** issue debugging elements and declare debugging element namespace */
const OPTION_TYPE OPTION_DEBUG       = ull(1) << __COUNTER__; /* 4 */

// @src2srcml
/** additional position markup */
const OPTION_TYPE OPTION_POSITION    = ull(1) << __COUNTER__; /* 5 */

// @src2srcml
/** cpp preprocessor mode without macro */
const OPTION_TYPE OPTION_CPP_NOMACRO = ull(1) << __COUNTER__; /* 6 */
/** all cpp preprocessor mode */
const OPTION_TYPE OPTION_CPP = ull(1) << __COUNTER__ | OPTION_CPP_NOMACRO; /* 7 */

// @src2srcml
// @srcml2src
/** use xml declaration */
const OPTION_TYPE OPTION_XMLDECL = ull(1) << __COUNTER__; /* 8 */

// @src2srcml
// @srcml2src
/** use namespace declarations */
const OPTION_TYPE OPTION_NAMESPACEDECL = ull(1) << __COUNTER__; /* 9 */

// @src2srcml
/** cpp markup @code#else@endcode sections */
const OPTION_TYPE OPTION_CPP_TEXT_ELSE = ull(1) << __COUNTER__; /* 10 */

// @src2srcml
/** cpp markup @code#if 0@endcode sections */
const OPTION_TYPE OPTION_CPP_MARKUP_IF0 = ull(1) << __COUNTER__; /* 11 */

// @srcml2src
/** xslt all evaluation */
const OPTION_TYPE OPTION_APPLY_ROOT      = ull(1) << __COUNTER__; /* 12 */

// @src2srcml
// @srcml2src
/** output is compressed (gzip) */
const OPTION_TYPE OPTION_COMPRESSED  = ull(1) << __COUNTER__; /* 13 */

// @src2srcml
/** Option to nest if within each other instead of using elseif */
const OPTION_TYPE OPTION_NESTIF  = ull(1) << __COUNTER__; /* 14 */

// @src2srcml
/** option to output a hash attribute */
const OPTION_TYPE OPTION_HASH  = ull(1) << __COUNTER__; /* 15 */

// @src2srcml
/** option to output a hash attribute */
const OPTION_TYPE OPTION_WRAP_TEMPLATE  = ull(1) << __COUNTER__; /* 15 */

// end SRCML_OPTIONS

// @src2srcml
/** output is OPTION_TYPEeractive (good for editing applications) */
const OPTION_TYPE OPTION_INTERACTIVE = ull(1) << __COUNTER__;

/** ??? @todo check if used */
const OPTION_TYPE OPTION_XPATH_TOTAL = OPTION_CPP_TEXT_ELSE;

// @src2srcml
// @srcml2src
/** terminate after this file */
const OPTION_TYPE OPTION_TERMINATE = ull(1) << __COUNTER__;

// @src2srcml
/** expression mode */
const OPTION_TYPE OPTION_EXPRESSION = ull(1) << __COUNTER__;

// @src2srcml
/** think use @code#line@endcode info */
const OPTION_TYPE OPTION_LINE  = ull(1) << __COUNTER__;

// @src2srcml
/** specify pattern to treat as macro */
const OPTION_TYPE OPTION_MACRO_PATTERN  = ull(1) << __COUNTER__;

// @src2srcml
/** input of macro list */
const OPTION_TYPE OPTION_MACRO_LIST  = ull(1) << __COUNTER__;

// @src2srcml
/** additional cpp:if/cpp:endif checking */
const OPTION_TYPE OPTION_CPPIF_CHECK  = ull(1) << __COUNTER__;

// @src2srcml
/** debug time attribute */
const OPTION_TYPE OPTION_DEBUG_TIMER  = ull(1) << __COUNTER__;

#endif
