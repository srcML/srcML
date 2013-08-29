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
#include "srcMLReader.hpp"

#if defined(__GNUG__) && !defined(__MINGW32__)
#include <dlfcn.h>
#endif

std::string srcml_error;

enum SRCML_ARCHIVE_TYPE { SRCML_ARCHIVE_RW, SRCML_ARCHIVE_READ, SRCML_ARCHIVE_WRITE };

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

  // TODO  Used for memory function.  May want to try and remove in future
  xmlBuffer * buffer;
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

srcml_archive global_archive = { SRCML_ARCHIVE_RW, 0, 0, 0, 0, 0, 0, std::vector<std::string>(), 0,
                                 4, std::vector<std::string>(), std::vector<std::string>(), std::vector<pair>(),
                                 0, 0, 0, 0 };

const char * srcml_version() {

  return SRCML_VERSION_STRING;

}

/* prefix for an XML namespace */
/* translates to/from srcML */
int srcml(const char* input_filename, const char* output_filename) {

  if(!input_filename) {

    srcml_error = "No input file provided";
    return  SRCML_STATUS_ERROR;

  }

  if(global_archive.registered_languages.size() == 0)
    Language::register_standard_file_extensions(global_archive.registered_languages);
  int lang = global_archive.language ? srcml_check_language(global_archive.language->c_str()) : Language::getLanguageFromFilename(input_filename, global_archive.registered_languages);

  if(lang) {

    OPTION_TYPE & options = global_archive.options;
    options |= lang == Language::LANGUAGE_JAVA ? 0 : OPTION_CPP;

    srcMLTranslator translator(lang, output_filename, options);
    int error = 0;

    try {

      translator.setInput(input_filename);
      translator.translate(0, 
                           global_archive.directory ? global_archive.directory->c_str() : 0,
                           global_archive.filename ? global_archive.filename->c_str() : output_filename,
                           global_archive.version ? global_archive.version->c_str() : 0,
                           lang);
      options &= ~OPTION_CPP;

    } catch (FileError) {


      error = 1;
      srcml_error = "Error converting '";
      srcml_error += input_filename;
      srcml_error += "' to srcML.";

    }

    translator.close();

    if(error)
      return  SRCML_STATUS_ERROR;

  } else {

    int is_xml = 0;
    int len = strlen(input_filename);
    if((len > 4 && input_filename[len - 1] == 'l' && input_filename[len - 2] == 'm'
        && input_filename[len - 3] == 'x' && input_filename[len - 4] == '.')
       || (global_archive.language && strcmp(global_archive.language->c_str(), "xml") == 0))
      is_xml = 1;

    // not xml or handled language
    if(!is_xml) {

      if(global_archive.language) {
        srcml_error = "Language '";
        srcml_error += global_archive.language->c_str();
        srcml_error += "' is not supported.";
      } else
        srcml_error = "No language provided.";

      return SRCML_STATUS_ERROR;

    }

    OPTION_TYPE & options = global_archive.options;
    srcMLUtility utility(input_filename, global_archive.encoding ? global_archive.encoding->c_str() : "UTF-8", options, "");
    utility.extract_text(0, output_filename, 1);

  }



  return SRCML_STATUS_OK;
}

int srcml_set_encoding(const char* encoding) {

  return srcml_archive_set_encoding(&global_archive, encoding);

}

int srcml_set_language(const char* language) {

  return srcml_archive_set_language(&global_archive, language);

}

int srcml_set_filename(const char* filename) {

  return srcml_archive_set_filename(&global_archive, filename);

}

int srcml_set_directory(const char* directory) {

  return srcml_archive_set_directory(&global_archive, directory);

}

int srcml_set_version(const char* version) {

  return srcml_archive_set_version(&global_archive, version);

}

int srcml_set_options(int option) {

  return srcml_archive_set_options(&global_archive, option);

}

int srcml_set_option(int option) {

  return srcml_archive_set_option(&global_archive, option);

}

int srcml_clear_option(int option) {

  return srcml_archive_set_option(&global_archive, option);

}

int srcml_set_tabstop(int tabstop) {

  return srcml_archive_set_tabstop(&global_archive, tabstop);

}

int srcml_register_file_extension(const char* extension, const char* language) {
  
  return srcml_archive_register_file_extension(&global_archive, extension, language);

}

int srcml_register_namespace(const char* prefix, const char* ns) {

  return srcml_archive_register_namespace(&global_archive, prefix, ns);

}

const char* srcml_get_encoding() {

  return srcml_archive_get_encoding(&global_archive);

}

const char* srcml_get_language() {

  return srcml_archive_get_language(&global_archive);

}

const char* srcml_get_filename() {

  return srcml_archive_get_filename(&global_archive);

}

const char* srcml_get_directory() {

  return srcml_archive_get_directory(&global_archive);

}

const char* srcml_get_version() {

  return srcml_archive_get_version(&global_archive);

}

int srcml_get_options() {

  return srcml_archive_get_options(&global_archive);

}

int srcml_get_tabstop() {

  return srcml_archive_get_tabstop(&global_archive);

}

int srcml_get_namespace_size() {

  return global_archive.namespaces.size();

}

const char* srcml_get_prefix(int pos) {

  return global_archive.prefixes.at(pos).c_str();

}

const char* srcml_get_prefix_uri(const char* namespace_uri) {

  for(int i = 0; i < global_archive.prefixes.size(); ++i)
    if(global_archive.namespaces.at(i) == namespace_uri)
      return global_archive.prefixes.at(i).c_str();

  return 0;
}

const char* srcml_get_namespace(int pos) {


  return global_archive.namespaces.at(pos).c_str();

}

const char* srcml_get_namespace_prefix(const char* prefix) {

  for(int i = 0; i < global_archive.namespaces.size(); ++i)
    if(global_archive.prefixes.at(i) == prefix)
      return global_archive.namespaces.at(i).c_str();

  return 0;

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

const char * srcml_archive_check_extension(srcml_archive * archive, const char* filename) {

  Language language(Language::getLanguageFromFilename(filename, archive->registered_languages));
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

/* prefix for an XML namespace */
const char* srcml_check_prefix(const char* namespace_uri) {

  for(int i = 0; global_archive.namespaces.size(); ++i)
    if(global_archive.namespaces.at(i) == namespace_uri)
      return global_archive.prefixes.at(i).c_str();

  return 0;

}

/* namespace for an XML prefix */
const char* srcml_check_namespace(const char* prefix) {

  for(int i = 0; global_archive.prefixes.size(); ++i)
    if(global_archive.prefixes.at(i) == prefix)
      return global_archive.namespaces.at(i).c_str();

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
const char* srcml_error_string() { return srcml_error.c_str(); }

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

  if(archive->filename) delete archive->filename, archive->filename = 0;
  if(archive->encoding) delete archive->encoding, archive->encoding = 0;
  if(archive->language) delete archive->language, archive->language = 0;
  if(archive->directory) delete archive->directory, archive->directory = 0;
  if(archive->version) delete archive->version, archive->version = 0;

  delete archive;
}

/* clone the setup of an existing archive
   client will have to free it using srcml_archive_free() */
srcml_archive* srcml_clone_archive(const srcml_archive* archive) {

  // probably memory leaks between here and free.  Who is responsible for freeing internal copied structures?

  srcml_archive * new_archive = srcml_create_archive();
  new_archive->type = archive->type;
  new_archive->filename = archive->filename ? new std::string(*archive->filename) : 0;
  new_archive->encoding = archive->encoding ? new std::string(*archive->encoding): 0;
  new_archive->language = archive->language ? new std::string(*archive->language) : 0;
  new_archive->directory = archive->directory ? new std::string(*archive->directory) : 0;
  new_archive->version = archive->version ? new std::string(*archive->version) : 0;

  for(int pos = 0; pos < archive->attributes.size(); ++pos)
    new_archive->attributes.push_back(archive->attributes.at(pos));

  new_archive->options = archive->options;
  new_archive->tabstop = archive->tabstop;

  new_archive->prefixes.clear();
  new_archive->namespaces.clear();
  for(int pos = 0; pos < archive->namespaces.size(); ++pos) {

    new_archive->namespaces.push_back(archive->namespaces.at(pos));
    new_archive->prefixes.push_back(archive->prefixes.at(pos));

  }

  for(int i = 0; i < archive->registered_languages.size(); ++i)
    new_archive->registered_languages.push_back(archive->registered_languages.at(i));

  return new_archive;

}

/* setup options for srcml archive */
int srcml_archive_set_encoding(srcml_archive* archive, const char* encoding) {

  if(archive->encoding) delete archive->encoding;
  archive->encoding = new std::string(encoding);
  return SRCML_STATUS_OK;

}

int srcml_archive_set_language(srcml_archive* archive, const char* language) {

  if(archive->language) delete archive->language;
  archive->language = new std::string(language);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_filename(srcml_archive* archive, const char* filename) {

  if(archive->filename) delete archive->filename;
  archive->filename = new std::string(filename);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_directory (srcml_archive* archive, const char* directory) {

  if(archive->directory) delete archive->directory;
  archive->directory = new std::string(directory);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_version(srcml_archive* archive, const char* version) {

  if(archive->version) delete archive->version;
  archive->version = new std::string(version);
  return SRCML_STATUS_OK;

}
int srcml_archive_set_attributes(srcml_archive* archive, const char** attr[2]) {

  archive->attributes.clear();

  while((*attr)[0]) {

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

  for(int i = 0; i < archive->prefixes.size(); ++i)
    if(archive->namespaces.at(i) == ns) {

      archive->prefixes.at(i) = prefix;
      return SRCML_STATUS_OK;
    }

  archive->prefixes.push_back(prefix);
  archive->namespaces.push_back(ns);

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

int srcml_write_open_memory(srcml_archive* archive, char* buffer, size_t buffer_size) {

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

int srcml_write_open_FILE(srcml_archive* archive, FILE* srcml_file) {

  xmlTextWriterPtr writer = xmlNewTextWriter(xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0)));

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            writer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

int srcml_write_open_fd(srcml_archive* archive, int srcml_fd) {

  xmlTextWriterPtr writer = xmlNewTextWriter(xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0)));


  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            writer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

void srcml_read_internal(srcml_archive * archive) {

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  int done = !archive->reader->readRootUnitAttributes(&language, &filename, &directory, &version,
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


  archive->type = SRCML_ARCHIVE_READ;
  archive->reader = new srcMLReader(srcml_filename);
  srcml_read_internal(archive);
 
  return SRCML_STATUS_OK;

}

int srcml_read_open_memory(srcml_archive* archive, const char* buffer, size_t buffer_size) { 

  archive->type = SRCML_ARCHIVE_READ;
  archive->input = xmlParserInputBufferCreateMem(buffer, buffer_size, xmlParseCharEncoding(0));
  archive->reader = new srcMLReader(archive->input);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_FILE(srcml_archive* archive, FILE* srcml_file) { 

  archive->type = SRCML_ARCHIVE_READ;
  archive->input = xmlParserInputBufferCreateFile(srcml_file, xmlParseCharEncoding(0));
  archive->reader = new srcMLReader(archive->input);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

 }

int srcml_read_open_fd(srcml_archive* archive, int srcml_fd) { 

  archive->type = SRCML_ARCHIVE_READ;
  archive->input = xmlParserInputBufferCreateFd(srcml_fd, xmlParseCharEncoding(0));
  archive->reader = new srcMLReader(archive->input);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

/* setup options for srcml unit */
int srcml_unit_set_language(srcml_unit* unit, const char* language) {

  if(unit->language) delete unit->language;
  unit->language = new std::string(language);
  return SRCML_STATUS_OK;

}

int srcml_unit_set_filename(srcml_unit* unit, const char* filename) {

  if(unit->filename) delete unit->filename;
  unit->filename = new std::string(filename);
  return SRCML_STATUS_OK;

}

int srcml_unit_set_directory(srcml_unit* unit, const char* directory) {

  if(unit->directory) delete unit->directory;
  unit->directory = new std::string(directory);
  return SRCML_STATUS_OK;

}

int srcml_unit_set_version(srcml_unit* unit, const char* version) {

  if(unit->version) delete unit->version;
  unit->version = new std::string(version);
  return SRCML_STATUS_OK;

}

const char* srcml_unit_get_language(const srcml_unit* unit) {

  return unit->language ? unit->language->c_str() : 0;

}

const char* srcml_unit_get_filename(const srcml_unit* unit) {

  return unit->filename ? unit->filename->c_str() : 0;

}

const char* srcml_unit_get_directory(const srcml_unit* unit) {

  return unit->directory ? unit->directory->c_str() : 0;

}

const char* srcml_unit_get_version  (const srcml_unit* unit) {

  return unit->version ? unit->version->c_str() : 0;

}


void srcml_parse_unit_internal(srcml_unit * unit, int lang) {

  xmlBuffer * output_buffer = xmlBufferCreate();
  unit->archive->translator->translate_separate(0, unit->directory ? unit->directory->c_str() : 0,
                                                unit->filename ? unit->filename->c_str() : 0,
                                                unit->version ? unit->version->c_str() : 0, lang, output_buffer);

  int length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n')
    --length;
  if(unit->unit) delete unit->unit;
  unit->unit = new std::string((const char *)output_buffer->content, length);
  xmlBufferFree(output_buffer);

}

/* Convert to srcml and append to the archive */
// TODO What is this supposed to do.  Right now append unit to archive, that is noop.
int srcml_parse_unit_archive (srcml_archive* archive, srcml_unit* unit) { 

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_filename(srcml_unit* unit, const char* src_filename) {

  int lang = unit->language ? srcml_check_language(unit->language->c_str()) : Language::getLanguageFromFilename(src_filename, unit->archive->registered_languages);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  unit->archive->translator->setInput(src_filename);

  srcml_parse_unit_internal(unit, lang);


  return SRCML_STATUS_OK;

}

int srcml_parse_unit_memory(srcml_unit* unit, char* src_buffer, size_t buffer_size) {

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  unit->archive->translator->setInputString(src_buffer, (int)buffer_size);
  srcml_parse_unit_internal(unit, lang);

  unit->archive->options = save_options;

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_FILE(srcml_unit* unit, FILE* src_file) {

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFile(src_file, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
  unit->archive->translator->setInput(input);

  srcml_parse_unit_internal(unit, lang);
  xmlFreeParserInputBuffer(input);

  unit->archive->options = save_options;

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_fd(srcml_unit* unit, int src_fd) { 

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFd(src_fd, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
  unit->archive->translator->setInput(input);

  srcml_parse_unit_internal(unit, lang);
  xmlFreeParserInputBuffer(input);

  unit->archive->options = save_options;

  return SRCML_STATUS_OK;

}

int srcml_write_unit(srcml_archive* archive, const srcml_unit* unit) {

  archive->translator->add_unit(unit->unit->c_str());

  return SRCML_STATUS_OK;
}

/* Read the next unit from the archive */
const srcml_unit* srcml_read_unit_archive(srcml_archive* archive) {

  return srcml_read_unit(archive);

}

const srcml_unit* srcml_read_archive_current_unit(const srcml_archive* archive) {

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  int done = !archive->reader->readUnitAttributes(&language, &filename, &directory, &version); 

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

int srcml_unparse_unit_filename(srcml_unit* unit, const char* src_filename) { 

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(src_filename, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"), unit->archive->options & SRCML_OPTION_COMPRESS);
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_memory(srcml_unit* unit, const char* src_buffer, size_t buffer_size) {

  xmlBufferPtr buffer = xmlBufferCreate();
  buffer->content = (xmlChar *)src_buffer;
  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  buffer->content = 0;
  xmlBufferFree(buffer);

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_FILE(srcml_unit* unit, FILE* srcml_file) { 

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_fd(srcml_unit* unit, int srcml_fd) { 

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  return SRCML_STATUS_OK;

}

/* Read the next unit from the archive */
srcml_unit* srcml_read_unit(srcml_archive* archive) {

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

  srcml_close_archive(archive);

}

void srcml_read_close(srcml_archive* archive) {

  srcml_close_archive(archive);

}

void srcml_close_archive(srcml_archive * archive) {

  if(archive->translator) archive->translator->close();
  if(archive->translator) delete archive->translator, archive->translator = 0;
  if(archive->reader) delete archive->reader, archive->reader = 0;
  if(archive->input) xmlFreeParserInputBuffer(archive->input), archive->input = 0;

  if(archive->buffer) {

    archive->buffer->content = 0;

    xmlBufferFree(archive->buffer);
    archive->buffer = 0;

  }

}

/* free the srcML archive data */
void srcml_write_free(srcml_archive* archive) {

  srcml_free_archive(archive);

}

void srcml_read_free(srcml_archive* archive) { 

  srcml_free_archive(archive);

}

srcml_unit * srcml_create_unit(srcml_archive * archive) {

  srcml_unit * unit = new srcml_unit;//(srcml_unit *)malloc(sizeof(srcml_unit));
  memset(unit, 0, sizeof(srcml_unit));
  unit->archive = archive;

  return unit;

}

int srcml_free_unit(srcml_unit* unit) {

  if(unit->language) delete unit->language, unit->language = 0;
  if(unit->filename) delete unit->filename, unit->filename = 0;
  if(unit->directory) delete unit->directory, unit->directory = 0;
  if(unit->version) delete unit->version, unit->version = 0;
  if(unit->unit) delete unit->unit, unit->unit = 0;

  delete unit;

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
int srcml_add_transform_relaxng(srcml_archive* archive, const char* relaxng_filename) { return 0; }
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) { return 0; }
