/*
  srcml_archive.cpp

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

#include "srcml.h"
#include "srcml_types.hpp"

#include "../srcmlns.hpp"

/* currently registered language for a file extension
   Full filename can be provided, and extension will be extracted */
const char * srcml_archive_check_extension(srcml_archive * archive, const char* filename) {

  Language language(Language::getLanguageFromFilename(filename, archive->registered_languages));
  return language.getLanguageString();

}

/* create a new srcml archive
   client will have to free it using srcml_free_archive() */
srcml_archive* srcml_create_archive()

{
  srcml_archive * archive;
  try {

    archive = new srcml_archive;

  } catch(...) { return 0; }

  memset(archive, 0, sizeof(srcml_archive));
  archive->type = SRCML_ARCHIVE_INVALID;

  // default prefixes
  srcml_archive_register_namespace(archive, SRCML_SRC_NS_PREFIX_DEFAULT, SRCML_SRC_NS_URI);
  srcml_archive_register_namespace(archive, SRCML_CPP_NS_PREFIX_DEFAULT, SRCML_CPP_NS_URI);
  srcml_archive_register_namespace(archive, SRCML_ERR_NS_PREFIX_DEFAULT, SRCML_ERR_NS_URI);
  srcml_archive_register_namespace(archive, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT, SRCML_EXT_LITERAL_NS_URI);
  srcml_archive_register_namespace(archive, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT, SRCML_EXT_OPERATOR_NS_URI);
  srcml_archive_register_namespace(archive, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT, SRCML_EXT_MODIFIER_NS_URI);
  srcml_archive_register_namespace(archive, SRCML_EXT_POSITION_NS_PREFIX_DEFAULT, SRCML_EXT_POSITION_NS_URI);

  Language::register_standard_file_extensions(archive->registered_languages);

  return archive;

}

/* free srcml archive
   allocated by srcml_create_archive() */
void srcml_free_archive(srcml_archive * archive) {

  if(archive->filename) delete archive->filename, archive->filename = 0;
  if(archive->encoding) delete archive->encoding, archive->encoding = 0;
  if(archive->language) delete archive->language, archive->language = 0;
  if(archive->directory) delete archive->directory, archive->directory = 0;
  if(archive->version) delete archive->version, archive->version = 0;

  delete archive;
}

/* clone the setup of an existing archive
   client will have to free it using srcml_free_archive() */
srcml_archive* srcml_clone_archive(const srcml_archive* archive) {

  srcml_archive * new_archive = srcml_create_archive();

  if(!new_archive) return 0;

  new_archive->type = archive->type;

  try {

    new_archive->filename = archive->filename ? new std::string(*archive->filename) : 0;
    new_archive->encoding = archive->encoding ? new std::string(*archive->encoding): 0;
    new_archive->language = archive->language ? new std::string(*archive->language) : 0;
    new_archive->directory = archive->directory ? new std::string(*archive->directory) : 0;
    new_archive->version = archive->version ? new std::string(*archive->version) : 0;

  } catch(...) {

    // deallocate any allocated before error.
    // if error version can not have been allocated.
    if(archive->filename) delete archive->filename;
    if(archive->encoding) delete archive->encoding;
    if(archive->language) delete archive->language;
    if(archive->directory) delete archive->directory;
    return 0;

  }

  try {

    for(int pos = 0; pos < archive->attributes.size(); ++pos)
      new_archive->attributes.push_back(archive->attributes.at(pos));

  } catch(...) {}

  new_archive->options = archive->options;
  new_archive->tabstop = archive->tabstop;

  // clear out those added by srcml_create_archive
  new_archive->prefixes.clear();
  new_archive->namespaces.clear();
  for(int pos = 0; pos < archive->namespaces.size(); ++pos) {

    try {

      new_archive->namespaces.push_back(archive->namespaces.at(pos));
      new_archive->prefixes.push_back(archive->prefixes.at(pos));

    } catch(...) {}

  }

  try {

    for(int i = 0; i < archive->registered_languages.size(); ++i)
      new_archive->registered_languages.push_back(archive->registered_languages.at(i));

  } catch(...) {}

  return new_archive;

}

/* setup options for srcml archive */
int srcml_archive_set_encoding(srcml_archive* archive, const char* encoding) {

  if(archive->encoding) delete archive->encoding;
  try {

    archive->encoding = encoding ? new std::string(encoding) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_archive_set_language(srcml_archive* archive, const char* language) {

  if(archive->language) delete archive->language;
  try {

    archive->language = language ? new std::string(language) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_archive_set_filename(srcml_archive* archive, const char* filename) {

  if(archive->filename) delete archive->filename;
  try {

    archive->filename = filename ? new std::string(filename) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_archive_set_directory (srcml_archive* archive, const char* directory) {

  if(archive->directory) delete archive->directory;
  try {

    archive->directory = directory ? new std::string(directory) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_archive_set_version(srcml_archive* archive, const char* version) {

  if(archive->version) delete archive->version;
  try {

    archive->version = version ? new std::string(version) : 0;

  } catch(...) { return SRCML_STATUS_ERROR; }
  return SRCML_STATUS_OK;

}

int srcml_archive_set_attributes(srcml_archive* archive, const char** attr[2]) {

  archive->attributes.clear();

  while((*attr)[0]) {

    if((*attr)[0] == 0 || (*attr)[1] == 0) {
      archive->attributes.clear();
      return SRCML_STATUS_ERROR;

    }

    archive->attributes.push_back((*attr)[0]);
    archive->attributes.push_back((*attr)[1]);
    ++attr;
  }

  return SRCML_STATUS_OK;

}

int srcml_archive_set_options(srcml_archive* archive, int options) {

  archive->options = options;
  return SRCML_STATUS_OK;

}

int srcml_archive_set_option    (srcml_archive* archive, int option) {

  archive->options |= option;
  return SRCML_STATUS_OK;

}

int srcml_archive_clear_option(srcml_archive* archive, int option) {

  archive->options &= ~option;
  return SRCML_STATUS_OK;

}

int srcml_archive_set_tabstop(srcml_archive* archive, int tabstop) {

  archive->tabstop = tabstop;
  return SRCML_STATUS_OK;

}

int srcml_archive_register_file_extension(srcml_archive* archive, const char* extension, const char* language) {

  Language::registerUserExt(extension, language, archive->registered_languages);
  return SRCML_STATUS_OK;

}

int srcml_archive_register_namespace(srcml_archive* archive, const char* prefix, const char* ns) {
  try {

    for(int i = 0; i < archive->prefixes.size(); ++i)
      if(archive->namespaces.at(i) == ns) {

        archive->prefixes.at(i) = prefix;
        return SRCML_STATUS_OK;
      }

  } catch(...) { return SRCML_STATUS_ERROR; }

  archive->prefixes.push_back(prefix);
  archive->namespaces.push_back(ns);

  return SRCML_STATUS_OK;

}

const char* srcml_archive_get_encoding(const srcml_archive* archive) {

  return archive->encoding ? archive->encoding->c_str() : 0;

}

const char* srcml_archive_get_language(const srcml_archive* archive) {

  return archive->language ? archive->language->c_str() : 0;

}

const char* srcml_archive_get_filename(const srcml_archive* archive) {

  return archive->filename ? archive->filename->c_str() : 0;

}

const char* srcml_archive_get_directory(const srcml_archive* archive) {

  return archive->directory ? archive->directory->c_str() : 0;

}

const char* srcml_archive_get_version(const srcml_archive* archive) {

  return archive->version ? archive->version->c_str() : 0;

}

int srcml_archive_get_options(const srcml_archive* archive) {

  return  archive->options;

}

int srcml_archive_get_tabstop(const srcml_archive* archive) {

  return archive->tabstop;

}

/* open a srcML archive for output */
int srcml_write_open_filename(srcml_archive* archive, const char* srcml_filename) {

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->options |= SRCML_OPTION_ARCHIVE;
  try {

    archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                              0, archive->encoding ? archive->encoding->c_str() : "UTF-8",
                                              srcml_filename,
                                              archive->options,
                                              archive->directory ? archive->directory->c_str() : 0,
                                              archive->filename ? archive->filename->c_str() : 0,
                                              archive->version ? archive->version->c_str() : 0,
                                              (const char **)&archive->prefixes.front(),
                                              archive->tabstop);

  } catch(...) { return SRCML_STATUS_ERROR; }

  return SRCML_STATUS_OK;

}

int srcml_write_open_memory(srcml_archive* archive, char** buffer) {

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->options |= SRCML_OPTION_ARCHIVE;
  try {

    archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                              0, archive->encoding ? archive->encoding->c_str() : "UTF-8",
                                              buffer,
                                              archive->options,
                                              archive->directory ? archive->directory->c_str() : 0,
                                              archive->filename ? archive->filename->c_str() : 0,
                                              archive->version ? archive->version->c_str() : 0,
                                              (const char **)&archive->prefixes.front(),
                                              archive->tabstop);

  } catch(...) { return SRCML_STATUS_ERROR; }

  return SRCML_STATUS_OK;

}

int srcml_write_open_FILE(srcml_archive* archive, FILE* srcml_file) {

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;

  xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
  if(writer == NULL) {

    xmlOutputBufferClose(output_buffer);
    return SRCML_STATUS_ERROR;

  }

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->options |= SRCML_OPTION_ARCHIVE;
  try {

    archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                              0, archive->encoding ? archive->encoding->c_str() : "UTF-8",
                                              writer,
                                              archive->options,
                                              archive->directory ? archive->directory->c_str() : 0,
                                              archive->filename ? archive->filename->c_str() : 0,
                                              archive->version ? archive->version->c_str() : 0,
                                              (const char **)&archive->prefixes.front(),
                                              archive->tabstop);

  } catch(...) { 

    xmlOutputBufferClose(output_buffer);
    xmlFreeTextWriter(writer);
    return SRCML_STATUS_ERROR;

  }

  return SRCML_STATUS_OK;

}

int srcml_write_open_fd(srcml_archive* archive, int srcml_fd) {

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));
  if(output_buffer == NULL) return SRCML_STATUS_ERROR;

  xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
  if(writer == NULL) {

    xmlOutputBufferClose(output_buffer);
    return SRCML_STATUS_ERROR;

  }

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->options |= SRCML_OPTION_ARCHIVE;
  try {

    archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                              0, archive->encoding ? archive->encoding->c_str() : "UTF-8",
                                              writer,
                                              archive->options,
                                              archive->directory ? archive->directory->c_str() : 0,
                                              archive->filename ? archive->filename->c_str() : 0,
                                              archive->version ? archive->version->c_str() : 0,
                                              (const char **)&archive->prefixes.front(),
                                              archive->tabstop);

  } catch(...) { 

    xmlOutputBufferClose(output_buffer);
    xmlFreeTextWriter(writer);
    return SRCML_STATUS_ERROR;

  }

  return SRCML_STATUS_OK;

}

void srcml_read_internal(srcml_archive * archive) {

  archive->type = SRCML_ARCHIVE_READ;

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  bool done = !archive->reader->readRootUnitAttributes(&language, &filename, &directory, &version,
                                                       archive->attributes, archive->prefixes,
                                                       archive->namespaces,
                                                       archive->options,
                                                       archive->tabstop);
  if(!done) {

    archive->language = language;
    archive->filename = filename;
    archive->directory = directory;
    archive->version = version;

  }

}

/* open a srcML archive for reading */
int srcml_read_open_filename(srcml_archive* archive, const char* srcml_filename) {

  archive->input = xmlParserInputBufferCreateFilename(srcml_filename, xmlParseCharEncoding(0));
  try {

    archive->reader = new srcMLReader(srcml_filename);

  } catch(...) {

    xmlFreeParserInputBuffer(archive->input);
    return SRCML_STATUS_ERROR;

  }

  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_memory(srcml_archive* archive, const char* buffer, size_t buffer_size) {

  archive->input = xmlParserInputBufferCreateMem(buffer, buffer_size, xmlParseCharEncoding(0));
  try {

    archive->reader = new srcMLReader(archive->input);

  } catch(...) {

    xmlFreeParserInputBuffer(archive->input);
    return SRCML_STATUS_ERROR;

  }

  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_FILE(srcml_archive* archive, FILE* srcml_file) {

  archive->input = xmlParserInputBufferCreateFile(srcml_file, xmlParseCharEncoding(0));
  try {

    archive->reader = new srcMLReader(archive->input);

  } catch(...) {

    xmlFreeParserInputBuffer(archive->input);
    return SRCML_STATUS_ERROR;

  }

  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_fd(srcml_archive* archive, int srcml_fd) {

  archive->input = xmlParserInputBufferCreateFd(srcml_fd, xmlParseCharEncoding(0));
  try {

    archive->reader = new srcMLReader(archive->input);

  } catch(...) {

    xmlFreeParserInputBuffer(archive->input);
    return SRCML_STATUS_ERROR;

  }

  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

/* Read the next unit from the archive */
const srcml_unit* srcml_read_unit_archive(srcml_archive* archive) {

  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

  return srcml_read_unit(archive);

}

const srcml_unit* srcml_read_archive_current_unit(const srcml_archive* archive) {

  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  bool done = !archive->reader->readUnitAttributes(&language, &filename, &directory, &version);

  srcml_unit * unit = 0;
  if(!done) {
    unit = srcml_create_unit((srcml_archive *)archive);
    unit->language = language;
    unit->filename = filename;
    unit->directory = directory;
    unit->version = version;
  }

  return unit;

}

int srcml_write_unit(srcml_archive* archive, const srcml_unit* unit) {

  if(archive->type != SRCML_ARCHIVE_WRITE && archive->type != SRCML_ARCHIVE_RW) return 0;
  archive->translator->add_unit(unit->unit->c_str());

  return SRCML_STATUS_OK;
}

/* Read the next unit from the archive */
srcml_unit* srcml_read_unit(srcml_archive* archive) {

  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  archive->reader->readUnitAttributes(&language, &filename, &directory, &version);
  std::string * read_unit = archive->reader->readsrcML();

  srcml_unit * unit = 0;
  if(read_unit) {
    unit = srcml_create_unit(archive);
    unit->unit = read_unit;
    unit->language = language;
    unit->filename = filename;
    unit->directory = directory;
    unit->version = version;
  }

  return unit;
}

/* close the srcML archive */
void srcml_write_close(srcml_archive* archive) {

  //if(archive->type != SRCML_ARCHIVE_WRITE && archive->type != SRCML_ARCHIVE_RW);
  srcml_close_archive(archive);

}

void srcml_read_close(srcml_archive* archive) {

  //if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW);
  srcml_close_archive(archive);

}

void srcml_close_archive(srcml_archive * archive) {

  if(archive->translator) archive->translator->close();
  if(archive->translator) delete archive->translator, archive->translator = 0;
  if(archive->reader) delete archive->reader, archive->reader = 0;
  if(archive->input) xmlFreeParserInputBuffer(archive->input), archive->input = 0;

}

/* free the srcML archive data */
void srcml_write_free(srcml_archive* archive) {

  //if(archive->type != SRCML_ARCHIVE_WRITE && archive->type != SRCML_ARCHIVE_RW);
  srcml_free_archive(archive);

}

void srcml_read_free(srcml_archive* archive) {

  //if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW);
  srcml_free_archive(archive);

}
