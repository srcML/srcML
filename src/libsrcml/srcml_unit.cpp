/*
  srcml_unit.cpp

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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_utilities.hpp>

/* setup options for srcml unit */
int srcml_unit_set_language(srcml_unit* unit, const char* language) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  if(unit->language) delete unit->language;
  try {

    unit->language = language  ? new std::string(language) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_unit_set_filename(srcml_unit* unit, const char* filename) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  if(unit->filename) delete unit->filename;
  try {

    unit->filename = filename ? new std::string(filename) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_unit_set_directory(srcml_unit* unit, const char* directory) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  if(unit->directory) delete unit->directory;
  try {

    unit->directory = directory ? new std::string(directory) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_unit_set_version(srcml_unit* unit, const char* version) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  if(unit->version) delete unit->version;
  try {

    unit->version = version ? new std::string(version) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

const char* srcml_unit_get_language(const srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->language ? unit->language->c_str() : 0;

}

const char* srcml_unit_get_filename(const srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->filename ? unit->filename->c_str() : 0;

}

const char* srcml_unit_get_directory(const srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->directory ? unit->directory->c_str() : 0;

}

const char* srcml_unit_get_version  (const srcml_unit* unit) {

  if(unit == NULL) return 0;

  return unit->version ? unit->version->c_str() : 0;

}

int srcml_parse_unit_internal(srcml_unit * unit, int lang) {

  xmlBuffer * output_buffer = xmlBufferCreate();
  try {

    unit->archive->translator->translate_separate(0, unit->directory ? unit->directory->c_str() : 0,
                                                  unit->filename ? unit->filename->c_str() : 0,
                                                  unit->version ? unit->version->c_str() : 0, lang, output_buffer);
  } catch(...) {

    xmlBufferFree(output_buffer);
    return SRCML_STATUS_ERROR;

  }


  int length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n')
    --length;
  if(unit->unit) delete unit->unit;

  int status = SRCML_STATUS_OK;
  try {

    unit->unit = new std::string((const char *)output_buffer->content, length);

  } catch(...) { status = SRCML_STATUS_ERROR; }

  xmlBufferFree(output_buffer);

  return status;

}

/* Convert to srcml and append to the archive */
// TODO What is this supposed to do.  Right now append unit to archive, that is noop.
int srcml_parse_unit_archive (srcml_archive* archive, srcml_unit* unit) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_filename(srcml_unit* unit, const char* src_filename) {

  if(unit == NULL || src_filename == NULL || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = unit->language ? srcml_check_language(unit->language->c_str()) : Language::getLanguageFromFilename(src_filename, unit->archive->registered_languages);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= SRCML_OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= SRCML_OPTION_CPP_NOMACRO;

  try {

    unit->archive->translator->setInput(src_filename);

  } catch(...) {

    unit->archive->options = save_options;
    return SRCML_STATUS_ERROR;

  }

  int status = srcml_parse_unit_internal(unit, lang);

  unit->archive->options = save_options;

  return status;

}

int srcml_parse_unit_memory(srcml_unit* unit, const char* src_buffer, size_t buffer_size) {

  if(unit == NULL || src_buffer == NULL || buffer_size <= 0 || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= SRCML_OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= SRCML_OPTION_CPP_NOMACRO;

  try {

    unit->archive->translator->setInputString(src_buffer, (int)buffer_size);

  } catch(...) {

    unit->archive->options = save_options;
    return SRCML_STATUS_ERROR;

  }

  int status = srcml_parse_unit_internal(unit, lang);

  unit->archive->options = save_options;

  return status;

}

int srcml_parse_unit_FILE(srcml_unit* unit, FILE* src_file) {

  if(unit == NULL || src_file == NULL || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= SRCML_OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= SRCML_OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFile(src_file, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

  try {

    unit->archive->translator->setInput(input);

  } catch(...) {

    xmlFreeParserInputBuffer(input);
    unit->archive->options = save_options;
    return SRCML_STATUS_ERROR;

  }

  int status = srcml_parse_unit_internal(unit, lang);
  xmlFreeParserInputBuffer(input);

  unit->archive->options = save_options;

  return status;

}

int srcml_parse_unit_fd(srcml_unit* unit, int src_fd) {

  if(unit == NULL || src_fd < 0 || (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= SRCML_OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= SRCML_OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFd(src_fd, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

  try {

    unit->archive->translator->setInput(input);

  } catch(...) {

    xmlFreeParserInputBuffer(input);
    unit->archive->options = save_options;
    return SRCML_STATUS_ERROR;

  }

  int status = srcml_parse_unit_internal(unit, lang);
  xmlFreeParserInputBuffer(input);

  unit->archive->options = save_options;

  return status;

}

int srcml_unparse_unit_filename(srcml_unit* unit, const char* src_filename) {

  if(unit == NULL || src_filename == NULL || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(src_filename, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"), unit->archive->options & SRCML_OPTION_COMPRESS);
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;
  int status = srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options);

  return status;

}

int srcml_unparse_unit_memory(srcml_unit* unit, char** src_buffer) {

  if(unit == NULL || src_buffer == NULL || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlBufferPtr buffer = xmlBufferCreate();
  if(buffer == NULL) return SRCML_STATUS_ERROR;
  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
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

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_FILE(srcml_unit* unit, FILE* srcml_file) {

  if(unit == NULL || srcml_file == NULL || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;
  int status = srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options);

  return status;

}

int srcml_unparse_unit_fd(srcml_unit* unit, int srcml_fd) {

  if(unit == NULL || srcml_fd < 0 || (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW)) return SRCML_STATUS_ERROR;

  // Must read unit before unparse
  if(!unit->unit) return SRCML_STATUS_ERROR;

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;
  int status = srcml_extract_text(unit->unit->c_str(), unit->unit->size(), output_buffer, unit->archive->options);

  return status;

}

srcml_unit * srcml_create_unit(srcml_archive * archive) {

  if(archive == NULL) return 0;

  srcml_unit * unit;
  try {

    unit = new srcml_unit;

  } catch(...) { return 0; }
  memset(unit, 0, sizeof(srcml_unit));
  unit->archive = archive;

  return unit;

}

int srcml_free_unit(srcml_unit* unit) {

  if(unit == NULL) return SRCML_STATUS_ERROR;

  if(unit->language) delete unit->language, unit->language = 0;
  if(unit->filename) delete unit->filename, unit->filename = 0;
  if(unit->directory) delete unit->directory, unit->directory = 0;
  if(unit->version) delete unit->version, unit->version = 0;
  if(unit->unit) delete unit->unit, unit->unit = 0;

  delete unit;

  return SRCML_STATUS_OK;

}

