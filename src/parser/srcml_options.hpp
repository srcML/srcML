// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_options.hpp
 *
 * @copyright Copyright (C) 2022 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCML_OPTIONS_HPP
#define INCLUDED_SRCML_OPTIONS_HPP

/** Do not issue an XML declaration (default: include XML declaration */
const unsigned int SRCML_PARSER_OPTION_NO_XML_DECL       = 1<<1;
/** Include line/column position attributes */
const unsigned int SRCML_PARSER_OPTION_POSITION          = 1<<2;
/** Markup preprocessor elements (default for C, C++) */
const unsigned int SRCML_PARSER_OPTION_CPP               = 1<<3;
/** Leave as text preprocessor else parts (default: markup) */
const unsigned int SRCML_PARSER_OPTION_CPP_TEXT_ELSE     = 1<<4;
/** Markup preprocessor @code #if 0 @endcode sections (default: leave as text) */
const unsigned int SRCML_PARSER_OPTION_CPP_MARKUP_IF0    = 1<<5;
/** Encode the original source encoding as an attribute */
const unsigned int SRCML_PARSER_OPTION_STORE_ENCODING    = 1<<6;

/** Include any XML namespace declarations */
const unsigned int SRCML_OPTION_NAMESPACE_DECL    = 1<<8;
/** Not sure what this used for */
const unsigned int SRCML_OPTION_XPATH_TOTAL       = 1<<9;
/** Extra processing of @code#line@endcode for position information */
const unsigned int SRCML_OPTION_LINE              = 1<<10;
/** Parser output special tokens for debugging the parser */
const unsigned int SRCML_OPTION_DEBUG             = 1<<11;
/** Is a fragment, i.e., no unit element */
const unsigned int SRCML_OPTION_FRAGMENT          = 1<<12;
/** User requested cpp */
const unsigned int SRCML_OPTION_CPP_DECLARED      = 1<<13;
 /** Create an archive */
const unsigned int SRCML_OPTION_ARCHIVE           = 1<<14;
 /** Output hash attribute on each unit (default: on) */
const unsigned int SRCML_OPTION_HASH              = 1<<15;

/** All default enabled options */
const unsigned int SRCML_OPTION_DEFAULT_INTERNAL  = (SRCML_OPTION_ARCHIVE | SRCML_OPTION_HASH | SRCML_OPTION_NAMESPACE_DECL);

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
#define isoption(options, flag) (((options) & (flag)) > 0)

#endif
