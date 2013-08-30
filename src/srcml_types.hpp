
#include "Options.hpp"
#include "Language.hpp"
#include "srcMLTranslator.hpp"
#include "srcMLReader.hpp"

#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_TYPES_HPP
#define INCLUDED_SRCML_TYPES_HPP

// transformation types, xpath, xslt, and relaxng
enum SRCML_TRANSFORM_TYPE { SRCML_XPATH, SRCML_XSLT, SRCML_RELAXNG };

// struct to hold transformation information for latter application.
struct transform {

  SRCML_TRANSFORM_TYPE type;
  std::string transformation;

};

// archive type read, write, or read/write
enum SRCML_ARCHIVE_TYPE { SRCML_ARCHIVE_RW, SRCML_ARCHIVE_READ, SRCML_ARCHIVE_WRITE };

/*

  Holds data for a srcML archive read/write.

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

  Hold data pertaining to a unit in an archive,
  such as the transformed or collected unit.

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
