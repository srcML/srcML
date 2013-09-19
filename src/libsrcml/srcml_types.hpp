/**
 * @file srcml_types.hpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
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

#include <Options.hpp>
#include <Language.hpp>
#include <srcMLTranslator.hpp>
#include <srcMLReader.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_TYPES_HPP
#define INCLUDED_SRCML_TYPES_HPP

/**
 * SRCML_TRANSORM_TYPE
 * 
 * Transformation types, xpath, xslt, and relaxng
 */
enum SRCML_TRANSFORM_TYPE { SRCML_XPATH, SRCML_XSLT, SRCML_RELAXNG };

/**
 * transform
 *
 * Struct to hold transformation information for latter application.
 */
struct transform {
  /** a transformation type */
  SRCML_TRANSFORM_TYPE type;
  /** the transformation to perform */
  std::string transformation;

};

/**
 * SRCML_ARCHIVE_TYPE
 * 
 * Archive type read, write, or read/write.
 */
enum SRCML_ARCHIVE_TYPE { SRCML_ARCHIVE_INVALID, SRCML_ARCHIVE_RW, SRCML_ARCHIVE_READ, SRCML_ARCHIVE_WRITE };

/**
 * srcml_archive
 *
 * Holds data for a srcML archive read/write.
 */
struct srcml_archive {

  /** type archive type read/write */
  SRCML_ARCHIVE_TYPE type;

  /** an attribute for a name of a file */
  std::string * filename;
  /** an attribute for an encoding */
  std::string * encoding;
  /** an attribute for an ecoding */
  std::string * xml_encoding;
  /** an attribute for a language */
  std::string * language;
  /** an attribute for a directory path */
  std::string * directory;
  /** an attribute for a version string */
  std::string * version;
  /** an array of name-value attribute pairs */
  std::vector<std::string>  attributes;

  /** srcml options */
  OPTION_TYPE options;

  /** size of tabstop */
  int tabstop;

  /** an array of XML namespace prefixes */
  std::vector<std::string> prefixes;
  /** namespaces an array of XML namespaces */
  std::vector<std::string> namespaces;

  /** an array of registered extension language pairs */
  std::vector<pair> registered_languages;

  /** a srcMLTranslator for writing and parsing */
  srcMLTranslator * translator;

  /** a srcMLReader for reading */
  srcMLReader * reader;
  /** xmlParserInputBuffer for reading */
  xmlParserInputBufferPtr input;

  /** an array of transformations to apply */
  std::vector<transform> transformations;
};

/**
 * srcml_unit
 *
 * Hold data pertaining to a unit in an archive,
 * such as the transformed or collected unit.
*/
struct srcml_unit {
  /** the archive the unit is created from */
  srcml_archive* archive;

  /** an attribute for a language */
  std::string * language;
  /** an attribute name for a file */
  std::string * filename;
  /** an attribute for a directory path */
  std::string * directory;
  /** an attribute for a version string */
  std::string * version;

  /** a buffer to store srcml from read and after parsing */
  std::string * unit;
};

#endif
