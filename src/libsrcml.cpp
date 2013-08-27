/*
  libsrcml.cpp

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

/*
  Implementation of srcml functions from the namespace srcml_*
*/

#include "srcml.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#include <vector>
#include <string>

#include "srcMLTranslator.hpp"
#include "Language.hpp"
#include "Options.hpp"
#include "srcmlns.hpp"
#include "srcMLUtility.hpp"

#if defined(__GNUG__) && !defined(__MINGW32__)
#include <dlfcn.h>
#endif

char srcml_error[512] = { 0 };

struct uridata {
  const char * uri;
  const char * prefix;

  // Might want to remove
  int option;
  char const * const description;
};

enum SRCML_ARCHIVE_TYPE { SRCML_ARCHIVE_READ, SRCML_ARCHIVE_WRITE };

struct registered_language {

  const char * extension;
  int language;

};

struct srcml_archive {

  // archive type read/write
  SRCML_ARCHIVE_TYPE type;

  // srcML archive attributes
  std::string * filename;
  std::string * encoding;
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

  // TODO not use buffer to hold results
  xmlBuffer * buffer;
  FILE * output_file;
  int fd;
};

struct srcml_unit {
  /* Have to remember which archive the unit is from */
  srcml_archive* archive;

  std::string * language;
  std::string * filename;
  std::string * directory;
  std::string * version;
  std::string * unit;
};

/* translates to/from srcML */
int srcml(const char* input_filename, const char* output_filename, const char* language) {

  if(!input_filename) {

    snprintf(srcml_error, 512, "No input file provided");
    return  SRCML_STATUS_ERROR;

  }

  Language::register_standard_file_extensions();
  int lang = language ? srcml_check_language(language) : Language::getLanguageFromFilename(input_filename);

  if(lang) {

    OPTION_TYPE options = OPTION_LITERAL | OPTION_OPERATOR | OPTION_MODIFIER;
    options |= lang == Language::LANGUAGE_JAVA ? 0 : OPTION_CPP;

    srcMLTranslator translator(lang, output_filename, options);
    int error = 0;

    try {

      translator.setInput(input_filename);
      translator.translate(input_filename, 0, input_filename, 0, lang);

    } catch (FileError) {


      error = 1;
      snprintf(srcml_error, 512, "Error converting '%s' to srcML.", input_filename);

    }

    translator.close();

    if(error)
      return  SRCML_STATUS_ERROR;

  } else {

    int is_xml = 0;
    int len = strlen(input_filename);
    if((len > 4 && input_filename[len - 1] == 'l' && input_filename[len - 2] == 'm'
        && input_filename[len - 3] == 'x' && input_filename[len - 4] == '.')
       || (language && strcmp(language, "xml") == 0))
      is_xml = 1;

    // not xml or handled language
    if(!is_xml) {

      if(language)
        snprintf(srcml_error, 512, "Language '%s' is not supported.", language);
      else
        snprintf(srcml_error, 512, "No language provided.");

      return SRCML_STATUS_ERROR;

    }

    OPTION_TYPE options = 0;
    srcMLUtility utility(input_filename, "UTF-8", options, "");
    utility.extract_text(0, output_filename, 1);

  }



  return SRCML_STATUS_OK;
}

/* source-code language is supported */
int srcml_check_language(const char* language) { return language == 0 ? 0 : Language::getLanguage(language); }

/* null-terminated array of supported source-code languages */
const char** srcml_language_list() {
  static const char* langs[] = { "C", "C++", "C#", "Java", 0 };
  return langs;
}

/* currently registered language for a file extension
   Full filename can be provided, and extension will be extracted */
const char * srcml_check_extension(const char* filename) {

  Language language(Language::getLanguageFromFilename(filename));
  return language.getLanguageString();

}

/* currently supported format, e.g., tar.gz
   Full filename can be provided, and extension will be extracted */
int srcml_check_format(const char* format) {

  static const char * const regex = "(zx\\.|zg\\.|2zb\\.|rat\\.)*";

  // reversed copy of the path
  int length = strlen(format);

  char * reverse = (char *)malloc((length + 1) * sizeof(char));
  for(int i = 0; i < length; ++i)
    reverse[i] = format[length - i - 1];
  reverse[length] = 0;

  // setup the regular expression
  static regex_t preg = { 0 };
  static int errorcode = regcomp(&preg, regex, REG_EXTENDED);

  // evalue the regex
  regmatch_t pmatch[3];
  errorcode = errorcode || regexec(&preg, reverse, 3, pmatch, 0);

  // minus 1 to remove starting .
  int ext_len = pmatch[0].rm_eo - pmatch[0].rm_so - 1;
  free(reverse);
  return ext_len;

  //char * extension = (char *)malloc(ext_len * sizeof(char));
  // extract the extension from the path, reversing as we go
  //for(int i = 0; i < ext_len; ++i)
  //extension[i] = reverse[pmatch[0].rm_eo - i - 2];
  //extension[ext_len] = 0;

  //return 1;
}

/* particular encoding is supported, both for input and output */
int srcml_check_encoding(const char* encoding) { return xmlParseCharEncoding(encoding) > 0; }

struct uridata uris[] = {

  { SRCML_SRC_NS_URI,          SRCML_SRC_NS_PREFIX_DEFAULT, 0,               "primary srcML namespace" },
  { SRCML_CPP_NS_URI,          SRCML_CPP_NS_PREFIX_DEFAULT, OPTION_CPP,      "namespace for cpreprocessing elements" }
  ,
  { SRCML_ERR_NS_URI,          SRCML_ERR_NS_PREFIX_DEFAULT, OPTION_DEBUG,    "namespace for srcML debugging elements" },
  { SRCML_EXT_LITERAL_NS_URI,  SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT, OPTION_LITERAL,  "namespace for optional literal elements" },
  { SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT, OPTION_OPERATOR, "namespace for optional operator element"},
  { SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT, OPTION_MODIFIER, "namespace for optional modifier element"},
  { SRCML_EXT_POSITION_NS_URI, SRCML_EXT_POSITION_NS_PREFIX_DEFAULT, OPTION_POSITION, "namespace for optional position element and attributes" },
};

/* prefix for an XML namespace */
const char* srcml_check_prefix(const char* namespace_uri) {

  for(int i = 0; uris[i].uri; ++i)
    if(strcmp(uris[i].uri, namespace_uri) == 0)
      return uris[i].prefix;

  return 0;

}

/* namespace for an XML prefix */
const char* srcml_check_namespace(const char* prefix) {

  // handle default prefix
  if(prefix == 0) return uris[0].uri;
  for(int i = 1; uris[i].uri; ++i)
    if(strcmp(uris[i].prefix, prefix) == 0)
      return uris[i].uri;

  return 0;

}

/* whether various features are available in this installation */
int srcml_check_xslt() {
#ifdef LIBXSLT_VERSION
  return 1;
#else
  void* handle = dlopen("libxslt.so", RTLD_LAZY);
  if (!handle)
    handle = dlopen("libxslt.dylib", RTLD_LAZY);

  if(!handle) return 0;

  dlclose(handle);
  return 1;
#endif

}

int srcml_check_exslt() {
#ifdef LIBXSLT_VERSION
  return 1;
#else
  void* handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle)
    handle = dlopen("libexslt.dylib", RTLD_LAZY);

  if(!handle) return 0;

  dlclose(handle);
  return 1;
#endif
}

/* string describing last error */
const char* srcml_error_string() { return srcml_error; }

/* create a new srcml archive
   client will have to free it using srcml_free() */
srcml_archive* srcml_create_archive()

{
  srcml_archive * archive = new srcml_archive;//(srcml_archive*) malloc(sizeof(srcml_archive));
  memset(archive, 0, sizeof(srcml_archive));
  archive->prefixes.push_back(SRCML_SRC_NS_PREFIX_DEFAULT);
  archive->prefixes.push_back(SRCML_CPP_NS_PREFIX_DEFAULT);
  archive->prefixes.push_back(SRCML_ERR_NS_PREFIX_DEFAULT);
  archive->prefixes.push_back(SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT);
  archive->prefixes.push_back(SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT);
  archive->prefixes.push_back(SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT);
  archive->prefixes.push_back(SRCML_EXT_POSITION_NS_PREFIX_DEFAULT);

  archive->namespaces.push_back(SRCML_SRC_NS_URI);
  archive->namespaces.push_back(SRCML_CPP_NS_URI);
  archive->namespaces.push_back(SRCML_ERR_NS_URI);
  archive->namespaces.push_back(SRCML_EXT_LITERAL_NS_URI);
  archive->namespaces.push_back(SRCML_EXT_OPERATOR_NS_URI);
  archive->namespaces.push_back(SRCML_EXT_MODIFIER_NS_URI);
  archive->namespaces.push_back(SRCML_EXT_POSITION_NS_URI);

  Language::register_standard_file_extensions(archive->registered_languages);

  return archive;

}

/* free srcml archive
   allocated by srcml_create_archive() */
void srcml_free_archive(srcml_archive * archive) { 

  if(archive->filename) delete archive->filename;
  if(archive->encoding) delete archive->encoding;
  if(archive->language) delete archive->language;
  if(archive->directory) delete archive->directory;
  if(archive->version) delete archive->version;
  if(archive->translator) delete archive->translator;

  delete archive;
}

/* clone the setup of an existing archive
   client will have to free it using srcml_archive_free() */
srcml_archive* srcml_clone_archive(const srcml_archive* archive) {

  // probably memory leaks between here and free.  Who is responsible for freeing internal copied structures?

  srcml_archive * new_archive = srcml_create_archive();
  new_archive->type = archive->type;
  new_archive->filename = archive->filename;
  new_archive->encoding = archive->encoding;
  new_archive->language = archive->language;
  new_archive->directory = archive->directory;
  new_archive->version = archive->version;

  for(int pos = 0; pos < archive->attributes.size(); ++pos)
    new_archive->attributes.push_back(archive->attributes.at(pos));

  new_archive->options = archive->options;
  new_archive->tabstop = archive->tabstop;

  for(int pos = 0; pos < archive->namespaces.size(); ++pos) {

    new_archive->namespaces.push_back(archive->namespaces.at(pos));
    new_archive->prefixes.push_back(archive->prefixes.at(pos));

  }

  // TODO make complete translator copy
  new_archive->translator = archive->translator;

  for(int i = 0; i < archive->registered_languages.size(); ++i)    
    new_archive->registered_languages.push_back(archive->registered_languages.at(i));

  return new_archive;

}

/* setup options for srcml archive */
int srcml_archive_set_encoding  (srcml_archive* archive, const char* encoding) {

  if(archive->encoding) delete archive->encoding;
  archive->encoding = new std::string(encoding);
  return SRCML_STATUS_OK;

}

int srcml_archive_set_language  (srcml_archive* archive, const char* language) {

  if(archive->language) delete archive->language;
  archive->language = new std::string(language);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_filename  (srcml_archive* archive, const char* filename) {

  if(archive->filename) delete archive->filename;
  archive->filename = new std::string(filename);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_directory (srcml_archive* archive, const char* directory) {

  if(archive->directory) delete archive->directory;
  archive->directory = new std::string(directory);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_version   (srcml_archive* archive, const char* version) {

  if(archive->version) delete archive->version;
  archive->version = new std::string(version);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_attributes(srcml_archive* archive, const char** attr[2]) {

  // TODO fix archive->attributes = attr;
  return SRCML_STATUS_OK;

}
int srcml_archive_set_options   (srcml_archive* archive, int options) {

  archive->options = options;
  return SRCML_STATUS_OK;

}
int srcml_archive_set_option    (srcml_archive* archive, int option) {

  archive->options |= option;
  return SRCML_STATUS_OK;

}
int srcml_archive_clear_option  (srcml_archive* archive, int option) {

  archive->options &= ~option;
  return SRCML_STATUS_OK;

}
int srcml_archive_set_tabstop   (srcml_archive* archive, int tabstop) {

  archive->tabstop = tabstop;
  return SRCML_STATUS_OK;

}

int srcml_archive_register_file_extension(srcml_archive* archive, const char* extension, const char* language) {

  Language::registerUserExt(extension, language, archive->registered_languages);
  return SRCML_STATUS_OK;

}

int srcml_archive_register_namespace(srcml_archive* archive, const char* prefix, const char* ns) {

  // check if overidding a default prefix
  if(strcmp(ns, SRCML_SRC_NS_URI) == 0) {
    archive->prefixes.at(0) = prefix;
  } else if(strcmp(ns, SRCML_CPP_NS_URI) == 0) {
    archive->prefixes.at(1) = prefix;
  } else if(strcmp(ns, SRCML_ERR_NS_URI) == 0) {
    archive->prefixes.at(2) = prefix;
  } else if(strcmp(ns, SRCML_EXT_LITERAL_NS_URI) == 0) {
    archive->prefixes.at(3) = prefix;
  } else if(strcmp(ns, SRCML_EXT_OPERATOR_NS_URI) == 0) {
    archive->prefixes.at(4) = prefix;
  } else if(strcmp(ns, SRCML_EXT_MODIFIER_NS_URI) == 0) {
    archive->prefixes.at(5) = prefix;
  } else if(strcmp(ns, SRCML_EXT_POSITION_NS_URI) == 0) {
    archive->prefixes.at(6) = prefix;
  } else {

    // TODO search and replace all previous namespaces

    archive->prefixes.push_back(prefix);
    archive->namespaces.push_back(ns);

  }

  return SRCML_STATUS_OK;

}

/* open a srcML archive for output */
int srcml_write_open_filename(srcml_archive* archive, const char* srcml_filename) {

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            srcml_filename,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

int srcml_write_open_memory  (srcml_archive* archive, char* buffer, size_t buffer_size) {

  archive->buffer = xmlBufferCreate();
  if(archive->buffer->content)
    free(archive->buffer->content);
  archive->buffer->content = (xmlChar *)buffer;

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            archive->buffer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

// TODO not use buffer to hold results
int srcml_write_open_FILE    (srcml_archive* archive, FILE* srcml_file) { 

  archive->buffer = xmlBufferCreate();

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            archive->buffer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  archive->output_file = srcml_file;

  return SRCML_STATUS_OK;

}

// TODO not use buffer to hold results
int srcml_write_open_fd      (srcml_archive* archive, int srcml_fd) { 

  archive->buffer = xmlBufferCreate();

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            archive->buffer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  archive->fd = srcml_fd;

  return SRCML_STATUS_OK;

 }

/* open a srcML archive for reading */
int srcml_read_open_filename(srcml_archive* archive, const char* srcml_filename) { return 0; }
int srcml_read_open_memory  (srcml_archive* archive, const char* buffer, size_t buffer_size) { return 0; }
int srcml_read_open_FILE    (srcml_archive* archive, FILE* srcml_file) { return 0; }
int srcml_read_open_fd      (srcml_archive* archive, int srcml_fd) { return 0; }

/* setup options for srcml unit */
int srcml_unit_set_language (srcml_unit* unit, const char* language) {

  if(unit->language) delete unit->language;
  unit->language = new std::string(language);
  return SRCML_STATUS_OK;

}

int srcml_unit_set_filename (srcml_unit* unit, const char* filename) {

  if(unit->filename) delete unit->filename;
  unit->filename = new std::string(filename);
  return SRCML_STATUS_OK;

}

int srcml_unit_set_directory(srcml_unit* unit, const char* directory) {

  if(unit->directory) delete unit->directory;
  unit->directory = new std::string(directory);
  return SRCML_STATUS_OK;

}

int srcml_unit_set_version  (srcml_unit* unit, const char* version) {

  if(unit->version) delete unit->version;
  unit->version = new std::string(version);
  return SRCML_STATUS_OK;

}

const char* srcml_unit_get_language (const srcml_unit* unit) {

  return unit->language->c_str();

}

const char* srcml_unit_get_filename (const srcml_unit* unit) {

  return unit->filename->c_str();

}

const char* srcml_unit_get_directory(const srcml_unit* unit) {

  return unit->directory->c_str();

}

const char* srcml_unit_get_version  (const srcml_unit* unit) {

  return unit->version->c_str();

}

/* Convert to srcml and append to the archive */
int srcml_parse_unit_archive (srcml_archive* archive, srcml_unit* unit) { return 0; }
int srcml_parse_unit_filename(srcml_unit* unit, const char* src_filename) {

  int lang = unit->language ? srcml_check_language(unit->language->c_str()) : Language::getLanguageFromFilename(src_filename, unit->archive->registered_languages;

  xmlBuffer * output_buffer = xmlBufferCreate();
  unit->archive->translator->setInput(src_filename);

  unit->archive->translator->translate_separate(src_filename, unit->directory ? unit->directory->c_str() : 0,
                                       unit->filename ? unit->filename->c_str() : 0,
                                       unit->version ? unit->version->c_str() : 0, lang, output_buffer);

  int length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n') 
    --length;
  if(unit->unit) delete unit->unit;
  unit->unit = new std::string((const char *)output_buffer->content, length);
  xmlBufferFree(output_buffer);

  return SRCML_STATUS_OK;

}
int srcml_parse_unit_memory  (srcml_unit* unit, char* src_buffer, size_t buffer_size) { 

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  xmlBuffer * output_buffer = xmlBufferCreate();
  unit->archive->translator->setInputString(src_buffer, (int)buffer_size);

  unit->archive->translator->translate_separate(0, unit->directory ? unit->directory->c_str() : 0,
                                       unit->filename ? unit->filename->c_str() : 0,
                                       unit->version ? unit->version->c_str() : 0, lang, output_buffer);

  int length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n') 
    --length;
  if(unit->unit) delete unit->unit;
  unit->unit = new std::string((const char *)output_buffer->content, length);
  xmlBufferFree(output_buffer);

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_FILE    (srcml_unit* unit, FILE* src_file) { return 0; }
int srcml_parse_unit_fd      (srcml_unit* unit, int src_fd) { return 0; }

int srcml_write_unit(srcml_archive* archive, const srcml_unit* unit) {

  archive->translator->add_unit(unit->unit->c_str());

  return SRCML_STATUS_OK;
}

/* Read the next unit from the archive */
const srcml_unit* srcml_read_unit_archive (srcml_archive* archive) { return 0; }
const srcml_unit* srcml_read_archive_current_unit(const srcml_archive* archive) { return 0; }
int srcml_read_unit_filename(srcml_archive* archive, const char* src_filename) { return 0; }
int srcml_read_unit_memory  (srcml_archive* archive, const char* src_buffer, size_t buffer_size) { return 0; }
int srcml_read_unit_FILE    (srcml_archive* archive, FILE* srcml_file) { return 0; }
int srcml_read_unit_fd      (srcml_archive* archive, int srcml_fd) { return 0; }

/* close the srcML archive */
void srcml_write_close(srcml_archive* archive) {}
void srcml_read_close (srcml_archive* archive) {}
void srcml_close_archive(srcml_archive * archive) {

  archive->translator->close();
  if(archive->output_file)
    fputs((char *)archive->buffer->content, archive->output_file);
  if(archive->fd)
    write(archive->fd, (char *)archive->buffer->content, archive->buffer->use);

  if(archive->buffer) {

    if(!archive->output_file && !archive->fd)
      archive->buffer->content = 0;

    xmlBufferFree(archive->buffer);
    archive->buffer = 0;

  }

}

/* free the srcML archive data */
void srcml_write_free(srcml_archive* archive) {}
void srcml_read_free (srcml_archive* archive) {}

srcml_unit * srcml_create_unit(srcml_archive * archive) {

  srcml_unit * unit = new srcml_unit;//(srcml_unit *)malloc(sizeof(srcml_unit));
  memset(unit, 0, sizeof(srcml_unit));
  unit->archive = archive;

  return unit;

}

int srcml_free_unit(srcml_unit* unit) {

  if(unit->language) delete unit->language;
  if(unit->filename) delete unit->filename;
  if(unit->directory) delete unit->directory;
  if(unit->version) delete unit->version;
  if(unit->unit) delete unit->unit;

  delete unit;

  return SRCML_STATUS_OK;

}

const char* srcml_get_encoding (const srcml_archive* archive) {

  return archive->encoding->c_str();

}

const char* srcml_get_language (const srcml_archive* archive) {

  return archive->language->c_str();

}

const char* srcml_get_filename (const srcml_archive* archive) {

  return archive->filename->c_str();

}

const char* srcml_get_directory(const srcml_archive* archive) {

  return archive->directory->c_str();

}

const char* srcml_get_version  (const srcml_archive* archive) {

  return archive->version->c_str();

}

int         srcml_get_options  (const srcml_archive* archive) {

  return  archive->options;

}

int         srcml_get_tabstop  (const srcml_archive* archive) {

  return archive->tabstop;

}

/* srcML attributes with namespaces (header read only)*/
const char** srcml_info(const char* srcml_filename) { return 0; }

/* srcML attributes with namespaces and number of units (complete file read) */
const char** srcml_longinfo(const char* srcml_filename) { return 0; }

/* srcML attributes with namespaces of a particular unit in an archive */
const char** srcml_info_unit(const char* srcml_filename, int unit) { return 0; }

/* list of filenames */
const char** srcml_list(const char* srcml_filename) { return 0; }

/* srcML XPath query and XSLT transform functions */
int srcml_add_transform_xpath(srcml_archive* archive, const char* xpath_string) { return 0; }
int srcml_add_transform_xslt(srcml_archive* archive, const char* xlst_filename) { return 0; }
int srcml_add_transform_relaxng(srcml_archive* archive, const char* xlst_filename) { return 0; }
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) { return 0; }
