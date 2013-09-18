/*
  srcml_types.hpp

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <Options.hpp>
#include <Language.hpp>
#include <srcMLTranslator.hpp>
#include <srcMLReader.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_TYPES_HPP
#define INCLUDED_SRCML_TYPES_HPP

/*
 * SRCML_TRANSORM_TYPE:
 * 
 * Transformation types, xpath, xslt, and relaxng
 */

enum SRCML_TRANSFORM_TYPE { SRCML_XPATH, SRCML_XSLT, SRCML_RELAXNG };

/*
 * transform:
 * @type: a transformation type
 * @transformation: the transformation to perform
 *
 * Struct to hold transformation information for latter application.
 */
struct transform {

  SRCML_TRANSFORM_TYPE type;
  std::string transformation;

};

/*
 * SRCML_ARCHIVE_TYPE:
 * 
 * Archive type read, write, or read/write.
 */
enum SRCML_ARCHIVE_TYPE { SRCML_ARCHIVE_INVALID, SRCML_ARCHIVE_RW, SRCML_ARCHIVE_READ, SRCML_ARCHIVE_WRITE };

/*
 * srcml_archive:
 * @filename: an attribute for a name of a file
 * @encoding: an attribute for an encoding
 * @xml_encoding: an attribute for an ecoding
 * @language: an attribute for a language
 * @directory: an attribute for a directory path
 * @version: an attribute for a version string
 * @attributes: an array of name-value attribute pairs
 * @options: srcml options
 * @tabstop: size of tabstop
 * @prefixes: an array of XML namespace prefixes
 * @namespaces: an array of XML namespaces
 * @registered_languages: an array of registered extension language pairs
 * @translator: a srcMLTranslator for writing and parsing
 * @reader: a srcMLReader for reading
 * @input: xmlParserInputBuffer for reading
 * @transformations: an array of transformations to apply
 *
 * Holds data for a srcML archive read/write.
*/
struct srcml_archive {

  // archive type read/write
  SRCML_ARCHIVE_TYPE type;

  // srcML archive attributes
  std::string * filename;
  std::string * encoding;
  std::string * xml_encoding;
  std::string * language;
  std::string * directory;
  std::string * version;
  std::vector<std::string>  attributes;

  // parsing options
  OPTION_TYPE options;

  // tabstop size
  int tabstop;

  // namespace/prefixes
  std::vector<std::string> prefixes;
  std::vector<std::string> namespaces;

  // registered language extensions
  std::vector<pair> registered_languages;

  // translator
  srcMLTranslator * translator;

  // utility
  srcMLReader * reader;
  xmlParserInputBufferPtr input;

  std::vector<transform> transformations;
};

/*
 * srcml_unit:
 * @archive: the archive the unit is created from
 * @language: an attribute for a language
 * @filename: an attribute name for a file
 * @directory: an attribute for a directory path
 * @version: an attribute for a version string
 * @unit: a buffer to store srcml from read and after parsing
 *
 * Hold data pertaining to a unit in an archive,
 * such as the transformed or collected unit.
*/
struct srcml_unit {
  // reference to archive part of
  srcml_archive* archive;

  // srcml attribute information
  std::string * language;
  std::string * filename;
  std::string * directory;
  std::string * version;

  // transformed unit or collected unit
  std::string * unit;
};

#endif
