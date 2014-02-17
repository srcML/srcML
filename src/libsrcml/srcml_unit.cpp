/**
 * @file srcml_unit.cpp
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_utilities.hpp>
#include <srcMLSAX2Reader.hpp>

/******************************************************************************
 *                                                                            *
 *                           Set up functions                                 *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unit_set_language
 * @param unit a srcml unit
 * @param language a language
 * 
 * Set the language for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR
 * on failure.
 */
int srcml_unit_set_language(srcml_unit* unit, const char* language) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  unit->language = language ? std::string(language) : boost::optional<std::string>();

  return SRCML_STATUS_OK;

}

/**
 * srcml_unit_set_filename
 * @param unit a srcml unit
 * @param filename the name of a file
 * 
 * Set the filename attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR
 * on failure.
 */
int srcml_unit_set_filename(srcml_unit* unit, const char* filename) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  unit->filename = filename ? std::string(filename) : boost::optional<std::string>();

  return SRCML_STATUS_OK;

}

/**
 * srcml_unit_set_directory
 * @param unit a srcml unit
 * @param directory a directory path
 * 
 * Set the directory attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR
 * on failure.
 */
int srcml_unit_set_directory(srcml_unit* unit, const char* directory) {

  if(unit == NULL) return SRCML_STATUS_ERROR;


  unit->directory = directory ? std::string(directory) : boost::optional<std::string>();

  return SRCML_STATUS_OK;

}

/**
 * srcml_unit_set_version
 * @param unit a srcml unit
 * @param version a version string
 * 
 * Set the version attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR
 * on failure.
 */
int srcml_unit_set_version(srcml_unit* unit, const char* version) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  unit->version = version ? std::string(version) : boost::optional<std::string>();

  return SRCML_STATUS_OK;

}

/******************************************************************************
 *                                                                            *
 *                           Accessor functions                               *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unit_get_language
 * @param unit a srcml unit
 * 
 * Get the language for the srcml unit
 *
 * @returns langauge on success and NULL on failure.
 */
const char* srcml_unit_get_language(const struct srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->language ? unit->language->c_str() : 0;

}

/**
 * srcml_unit_get_filename
 * @param unit a srcml unit
 * 
 * Get the filename attribute for the srcml unit.
 *
 * @returns filename attribute on success and NULL on failure.
 */
const char* srcml_unit_get_filename(const struct srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->filename ? unit->filename->c_str() : 0;

}

/**
 * srcml_unit_get_directory
 * @param unit a srcml unit
 * 
 * Get the directory attribute for the srcml unit.
 *
 * @returns directory attribute on successand NULL on failure.
 */
const char* srcml_unit_get_directory(const struct srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->directory ? unit->directory->c_str() : 0;

}

/**
 * srcml_unit_get_version
 * @param unit a srcml unit
 * 
 * Get the version for the srcml unit.
 *
 * @returns version on success and NULL on failure.
 */
const char* srcml_unit_get_version(const struct srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->version ? unit->version->c_str() : 0;

}

/**
 * srcml_unit_get_xml
 * @param unit a srcml unit
 * 
 * Get the parsed or collected srcml from an archive.
 * If only the attributes were collected from a read,
 * then read in the xml and return that value.
 *
 * @returns the unit srcML on success and NULL on failure.
 */
const char* srcml_unit_get_xml(struct srcml_unit* unit) {

  if(unit == NULL || (!unit->unit && !unit->read_header)) return 0;

  if(!unit->unit && (unit->archive->type == SRCML_ARCHIVE_READ || unit->archive->type == SRCML_ARCHIVE_RW))
    unit->archive->reader->readsrcML(unit->unit);

  return unit->unit ? unit->unit->c_str() : 0;

}

/******************************************************************************
 *                                                                            *
 *                           Unit parsing functions                           *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_parse_unit_internal
 * @param unit a srcml unit
 * @param lang an interger representation of a language.
 *
 * Function for internal use for parsing functions. Creates
 * output buffer, translates a current input and places the
 * contents into the unit.
 * 
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_parse_unit_internal(srcml_unit * unit, int lang, xmlParserInputBufferPtr input, OPTION_TYPE translation_options) {

  xmlBuffer * output_buffer = xmlBufferCreate();
  try {

    unit->archive->translator->translate_separate(unit->directory ? unit->directory->c_str() : 0,
                                                  unit->filename ? unit->filename->c_str() : 0,
                                                  unit->version ? unit->version->c_str() : 0, lang, input, output_buffer,
						  translation_options);
  } catch(...) {

    xmlBufferFree(output_buffer);
    return SRCML_STATUS_ERROR;

  }


  size_t length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n')
    --length;

  int status = SRCML_STATUS_OK;

  unit->unit = std::string((const char *)output_buffer->content, length);

  xmlBufferFree(output_buffer);

  return status;

}

/**
 * srcml_parse_unit_filename
 * @param unit a unit to parse the results to
 * @param src_filename name of a file to parse into srcML
 *
 * Convert to srcML the contents of src_filename and 
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_parse_unit_filename(srcml_unit* unit, const char* src_filename) {

  if(unit == NULL || src_filename == NULL || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int file_lang = Language::getLanguageFromFilename(src_filename, unit->archive->registered_languages);
  int lang = unit->language ? srcml_check_language(unit->language->c_str()) :
    (file_lang != Language::LANGUAGE_NONE && file_lang != 0 ? file_lang : srcml_check_language("C++"));



  OPTION_TYPE translation_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    translation_options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
  else if (lang == Language::LANGUAGE_CSHARP)
    translation_options |= SRCML_OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFilename(src_filename,
				      unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
  int status = srcml_parse_unit_internal(unit, lang, input, translation_options);

  xmlFreeParserInputBuffer(input);

  return status;

}

/**
 * srcml_parse_unit_memory
 * @param unit a unit to parse the results to
 * @param src_buffer buffer containing source code to parse into srcML
 * @param buffer_size size of the buffer to parse
 *
 * Convert to srcML the contents of buffer up to size buffer_size and
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_parse_unit_memory(srcml_unit* unit, const char* src_buffer, size_t buffer_size) {

  if(unit == NULL || src_buffer == NULL || buffer_size <= 0 || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : "C++");

  OPTION_TYPE translation_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    translation_options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
  else if (lang == Language::LANGUAGE_CSHARP)
    translation_options |= SRCML_OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem(src_buffer, (int)buffer_size, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

  int status = srcml_parse_unit_internal(unit, lang, input, translation_options);
  xmlFreeParserInputBuffer(input);

  return status;

}

/**
 * srcml_parse_unit_FILE
 * @param unit a unit to parse the results to
 * @param src_file a FILE opened for reading
 *
 * Convert to srcML the contents of src_file and
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_parse_unit_FILE(srcml_unit* unit, FILE* src_file) {

  if(unit == NULL || src_file == NULL || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : "C++");

  OPTION_TYPE translation_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    translation_options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
  else if (lang == Language::LANGUAGE_CSHARP)
    translation_options |= SRCML_OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFile(src_file, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

  int status = srcml_parse_unit_internal(unit, lang, input, translation_options);
  input->context = 0;
  input->readcallback = 0;
  input->closecallback = 0;
  xmlFreeParserInputBuffer(input);

  return status;

}

/**
 * srcml_parse_unit_fd
 * @param unit a unit to parse the results to
 * @param src_fd a file descriptor open for reading
 *
 * Convert to srcML the contents of src_fd
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_parse_unit_fd(srcml_unit* unit, int src_fd) {

  if(unit == NULL || src_fd < 0 || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : "C++");

  OPTION_TYPE translation_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    translation_options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
  else if (lang == Language::LANGUAGE_CSHARP)
    translation_options |= SRCML_OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFd(src_fd, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

  int status = srcml_parse_unit_internal(unit, lang, input, translation_options);
  input->context = 0;
  input->readcallback = 0;
  input->closecallback = 0;
  xmlFreeParserInputBuffer(input);

  return status;

}

/******************************************************************************
 *                                                                            *
 *                           Unit unparsing functions                         *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unparse_unit_filename
 * @param unit a srcml unit
 * @param src_filename name of a file to output contents of unit as source
 * 
 * Convert the srcML in unit into source code and place it into the file
 * src_filename.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_unparse_unit_filename(srcml_unit* unit, const char* src_filename) {

  if(unit == NULL || src_filename == NULL || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW) || (!unit->unit && !unit->read_header)) return SRCML_STATUS_ERROR;

  if(!unit->unit)
    unit->archive->reader->readsrcML(unit->unit);

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(src_filename, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "UTF-8"), unit->archive->options & SRCML_OPTION_COMPRESS);
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;
  int status = srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options);

  return status;

}

/**
 * srcml_unparse_unit_memory
 * @param unit a srcml unit
 * @param src_buffer an output buffer address
 * 
 * Convert the srcML in unit into source code and place it into the address
 * pointed to by src_buffer.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.  src_buffer is allocated in the function and
 * needs to be freed after finished using.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_unparse_unit_memory(srcml_unit* unit, char** src_buffer, int * src_size) {

  if(unit == NULL || src_buffer == NULL || src_size == NULL || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW) || (!unit->unit && !unit->read_header)) return SRCML_STATUS_ERROR;

  if(!unit->unit)
    unit->archive->reader->readsrcML(unit->unit);

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlBufferPtr buffer = xmlBufferCreate();
  if(buffer == NULL) return SRCML_STATUS_ERROR;
  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "UTF-8"));
  if(output_buffer == NULL) {

    xmlBufferFree(buffer);
    return SRCML_STATUS_ERROR;

  }

  if(srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options) == SRCML_STATUS_ERROR) {

    xmlBufferFree(buffer);
    return SRCML_STATUS_ERROR;

  }
  
  (*src_buffer) = strdup((const char *)buffer->content);
  xmlBufferFree(buffer);
  if(!buffer->content && !(*src_buffer)) return SRCML_STATUS_ERROR;

  *src_size = (int)strlen(*src_buffer);

  return SRCML_STATUS_OK;

}

/**
 * srcml_unparse_unit_FILE
 * @param unit a srcml unit
 * @param srcml_file FILE opened for writing
 * 
 * Convert the srcML in unit into source code and place it into the FILE
 * srcml_file.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_unparse_unit_FILE(srcml_unit* unit, FILE* srcml_file) {

  if(unit == NULL || srcml_file == NULL || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW) || (!unit->unit && !unit->read_header)) return SRCML_STATUS_ERROR;

  if(!unit->unit)
    unit->archive->reader->readsrcML(unit->unit);

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "UTF-8"));
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;
  int status = srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options);

  return status;

}

/**
 * srcml_unparse_unit_fd
 * @param unit a srcml unit
 * @param srcml_fd file descriptor opened for writing
 * 
 * Convert the srcML in unit into source code and place it into the file
 * descriptor srcml_fd.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_unparse_unit_fd(srcml_unit* unit, int srcml_fd) {

  if(unit == NULL || srcml_fd < 0 || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW) || (!unit->unit && !unit->read_header)) return SRCML_STATUS_ERROR;

  if(!unit->unit)
    unit->archive->reader->readsrcML(unit->unit);

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "UTF-8"));
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;
  int status = srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options);

  return status;

}

/******************************************************************************
 *                                                                            *
 *                       Unit creation/cleanup functions                      *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_create_unit
 * @param archive a srcml archvie
 * 
 * Create a srcml_unit tied to the srcml_archive archive
 *
 * @returns unit on success and on failure returns NULL
 */
srcml_unit * srcml_create_unit(srcml_archive * archive) {

  if(archive == NULL) return 0;

  srcml_unit * unit;
  try {

    unit = new srcml_unit;

  } catch(...) { return 0; }
  unit->archive = archive;
  unit->read_header = false;

  return unit;

}

/**
 * srcml_free_unit
 * @param unit a srcml unit
 * 
 * Free the contents of a srcml_unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_free_unit(srcml_unit* unit) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  delete unit;

  return SRCML_STATUS_OK;

}

