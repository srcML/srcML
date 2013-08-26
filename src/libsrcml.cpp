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

  SRCML_ARCHIVE_TYPE type;

  const char* filename;
  const char * encoding;
  const char * language;
  const char * directory;
  const char * version;
  const char *** attributes;
  OPTION_TYPE options;
  int tabstop;
  int num_namespaces;
  const char * prefixes[32];
  const char * namespaces[32];
  srcMLTranslator * translator;
  int num_registered;
  struct pair registered_languages[47];
};

struct srcml_unit {
  /* Have to remember which archive the unit is from */
  struct srcml_archive* archive;
  const char * language;
  const char * filename;
  const char * directory;
  const char * version;
  const char * unit;
  srcMLTranslator * translator;
  int * num_registered;
  struct pair * registered_languages;
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
struct srcml_archive* srcml_create_archive()

{
  struct srcml_archive * archive = (struct srcml_archive*) malloc(sizeof(struct srcml_archive));
  memset(archive, 0, sizeof(struct srcml_archive));
  archive->prefixes[0] = SRCML_SRC_NS_PREFIX_DEFAULT;
  archive->prefixes[1] = SRCML_CPP_NS_PREFIX_DEFAULT;
  archive->prefixes[2] = SRCML_ERR_NS_PREFIX_DEFAULT;
  archive->prefixes[3] = SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT;
  archive->prefixes[4] = SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT;
  archive->prefixes[5] = SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT;
  archive->prefixes[6] = SRCML_EXT_POSITION_NS_PREFIX_DEFAULT;

  archive->namespaces[0] = SRCML_SRC_NS_URI;
  archive->namespaces[1] = SRCML_CPP_NS_URI;
  archive->namespaces[2] = SRCML_ERR_NS_URI;
  archive->namespaces[3] = SRCML_EXT_LITERAL_NS_URI;
  archive->namespaces[4] = SRCML_EXT_OPERATOR_NS_URI;
  archive->namespaces[5] = SRCML_EXT_MODIFIER_NS_URI;
  archive->namespaces[6] = SRCML_EXT_POSITION_NS_URI;

  archive->num_namespaces = 7;
  Language::register_standard_file_extensions(archive->num_registered, archive->registered_languages);

  return archive;

}

/* free srcml archive
   allocated by srcml_create_archive() */
void srcml_free_archive(struct srcml_archive * archive) { free(archive); }

/* clone the setup of an existing archive
   client will have to free it using srcml_archive_free() */
struct srcml_archive* srcml_clone_archive(const struct srcml_archive* archive) {

  // probably memory leaks between here and free.  Who is responsible for freeing internal copied structures?

  struct srcml_archive * new_archive = srcml_create_archive();
  new_archive->type = archive->type;
  new_archive->filename = strdup(archive->filename);
  new_archive->encoding = strdup(archive->encoding);
  new_archive->language = strdup(archive->language);
  new_archive->directory = strdup(archive->directory);
  new_archive->version = strdup(archive->version);

  int length = 0;
  for(const char *** p = archive->attributes; (*p)[0]; ++length, ++p)
    ;
  new_archive->attributes = (const char ***)malloc((length + 1) * sizeof(const char **));
  for(int pos = 0; archive->attributes[pos][0]; ++pos) {

    new_archive->attributes[pos][0] = strdup(archive->attributes[pos][0]);
    new_archive->attributes[pos][1] = strdup(archive->attributes[pos][1]);

  }
  //new_archive->attributes[length][0] = 0, new_archive->attributes[length][1] = 0;

  new_archive->options = archive->options;
  new_archive->tabstop = archive->tabstop;

  new_archive->num_namespaces = archive->num_namespaces;
  int pos = 0;
  for(; archive->namespaces[pos]; ++pos) {

    new_archive->namespaces[pos] = strdup(archive->namespaces[pos]);
    new_archive->prefixes[pos] = strdup(archive->prefixes[pos]);

  }
  //new_archive->namespaces[pos] = 0, new_archive->prefixes[pos] = 0;

  // TODO make complete translator copy
  new_archive->translator = archive->translator;

  new_archive->num_registered = archive->num_registered;
  for(int i = 0; i < new_archive->num_registered; ++i) {
    
    new_archive->registered_languages[i].s = archive->registered_languages[i].s;
    new_archive->registered_languages[i].n = archive->registered_languages[i].n;
  }
  //new_archive->registered_languages[new_archive->num_registered].s = 0;
  //new_archive->registered_languages[new_archive->num_registered].n = 0;

  return new_archive;

}

/* setup options for srcml archive */
int srcml_set_encoding  (struct srcml_archive* archive, const char* encoding) {

  archive->encoding = encoding;
  return SRCML_STATUS_OK;

}

int srcml_set_language  (struct srcml_archive* archive, const char* language) {

  archive->language = language;
  return SRCML_STATUS_OK;

}
int srcml_set_filename  (struct srcml_archive* archive, const char* filename) {

  archive->filename = filename;
  return SRCML_STATUS_OK;

}
int srcml_set_directory (struct srcml_archive* archive, const char* directory) {

  archive->directory = directory;
  return SRCML_STATUS_OK;

}
int srcml_set_version   (struct srcml_archive* archive, const char* version) {

  archive->version = version;
  return SRCML_STATUS_OK;

}
int srcml_set_attributes(struct srcml_archive* archive, const char** attr[2]) {

  archive->attributes = attr;
  return SRCML_STATUS_OK;

}
int srcml_set_options   (struct srcml_archive* archive, int options) {

  archive->options = options;
  return SRCML_STATUS_OK;

}
int srcml_set_option    (struct srcml_archive* archive, int option) {

  archive->options |= option;
  return SRCML_STATUS_OK;

}
int srcml_clear_option  (struct srcml_archive* archive, int option) {

  archive->options &= ~option;
  return SRCML_STATUS_OK;

}
int srcml_set_tabstop   (struct srcml_archive* archive, int tabstop) {

  archive->tabstop = tabstop;
  return SRCML_STATUS_OK;

}

int srcml_register_file_extension(struct srcml_archive* archive, const char* extension, const char* language) {

  Language::registerUserExt(extension, language, archive->num_registered, archive->registered_languages);
  return SRCML_STATUS_OK;

}

int srcml_register_namespace(struct srcml_archive* archive, const char* prefix, const char* ns) {

  // check if overidding a default prefix
  if(strcmp(ns, SRCML_SRC_NS_URI) == 0) {
    archive->prefixes[0] = prefix;
  } else if(strcmp(ns, SRCML_CPP_NS_URI) == 0) {
    archive->prefixes[1] = prefix;
  } else if(strcmp(ns, SRCML_ERR_NS_URI) == 0) {
    archive->prefixes[2] = prefix;
  } else if(strcmp(ns, SRCML_EXT_LITERAL_NS_URI) == 0) {
    archive->prefixes[3] = prefix;
  } else if(strcmp(ns, SRCML_EXT_OPERATOR_NS_URI) == 0) {
    archive->prefixes[4] = prefix;
  } else if(strcmp(ns, SRCML_EXT_MODIFIER_NS_URI) == 0) {
    archive->prefixes[5] = prefix;
  } else if(strcmp(ns, SRCML_EXT_POSITION_NS_URI) == 0) {
    archive->prefixes[6] = prefix;
  } else {

    // TODO make dynamicly growing.
    archive->prefixes[archive->num_namespaces] = prefix;
    archive->namespaces[archive->num_namespaces] = ns;
    ++archive->num_namespaces;

    archive->prefixes[archive->num_namespaces] = 0;
    archive->namespaces[archive->num_namespaces] = 0;

  }

  return SRCML_STATUS_OK;

}

/* open a srcML archive for output */
int srcml_write_open_filename(struct srcml_archive* archive, const char* srcml_filename) {

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language),
                                            0, archive->encoding,
                                            srcml_filename,
                                            archive->options,
                                            archive->directory,
                                            archive->filename,
                                            archive->version,
                                            archive->prefixes,
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

int srcml_write_open_memory  (struct srcml_archive* archive, char* buffer, size_t buffer_size) { return 0; }
int srcml_write_open_FILE    (struct srcml_archive* archive, FILE* srcml_file) { return 0; }
int srcml_write_open_fd      (struct srcml_archive* archive, int srcml_fd) { return 0; }

/* open a srcML archive for reading */
int srcml_read_open_filename(struct srcml_archive* archive, const char* srcml_filename) { return 0; }
int srcml_read_open_memory  (struct srcml_archive* archive, const char* buffer, size_t buffer_size) { return 0; }
int srcml_read_open_FILE    (struct srcml_archive* archive, FILE* srcml_file) { return 0; }
int srcml_read_open_fd      (struct srcml_archive* archive, int srcml_fd) { return 0; }

/* setup options for srcml unit */
int srcml_unit_set_language (struct srcml_unit* unit, const char* language) {

  unit->language = language;
  return SRCML_STATUS_OK;

}

int srcml_unit_set_filename (struct srcml_unit* unit, const char* filename) {

  unit->filename = filename;
  return SRCML_STATUS_OK;

}

int srcml_unit_set_directory(struct srcml_unit* unit, const char* directory) {

  unit->directory = directory;
  return SRCML_STATUS_OK;

}

int srcml_unit_set_version  (struct srcml_unit* unit, const char* version) {

  unit->version = version;
  return SRCML_STATUS_OK;

}

const char* srcml_unit_get_language (const struct srcml_unit* unit) {

  return unit->language;

}

const char* srcml_unit_get_filename (const struct srcml_unit* unit) {

  return unit->filename;

}

const char* srcml_unit_get_directory(const struct srcml_unit* unit) {

  return unit->filename;

}

const char* srcml_unit_get_version  (const struct srcml_unit* unit) {

  return unit->version;

}

/* Convert to srcml and append to the archive */
int srcml_parse_unit_archive (struct srcml_archive* archive, struct srcml_unit* unit) { return 0; }
int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename) {

  int lang = unit->language ? srcml_check_language(unit->language) : Language::getLanguageFromFilename(src_filename, *unit->num_registered, unit->registered_languages);

  xmlBuffer * output_buffer = xmlBufferCreate();
  unit->translator->setInput(src_filename);

  unit->translator->translate_separate(src_filename, unit->directory, unit->filename, unit->version, lang, output_buffer);
  int length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n') 
    --length;
  unit->unit = (const char *)strndup((const char *)output_buffer->content, length);
  xmlBufferFree(output_buffer);

  return SRCML_STATUS_OK;

}
int srcml_parse_unit_memory  (struct srcml_archive* archive, char* src_buffer, size_t buffer_size) { return 0; }
int srcml_parse_unit_FILE    (struct srcml_archive* archive, FILE* src_file) { return 0; }
int srcml_parse_unit_fd      (struct srcml_archive* archive, int src_fd) { return 0; }

int srcml_write_unit(struct srcml_archive* archive, const struct srcml_unit* unit) {

  archive->translator->add_unit(unit->unit);

  return SRCML_STATUS_OK;
}

/* Read the next unit from the archive */
const struct srcml_unit* srcml_read_unit_archive (struct srcml_archive* archive) { return 0; }
const struct srcml_unit* srcml_read_archive_current_unit(const struct srcml_archive* archive) { return 0; }
int srcml_read_unit_filename(struct srcml_archive* archive, const char* src_filename) { return 0; }
int srcml_read_unit_memory  (struct srcml_archive* archive, const char* src_buffer, size_t buffer_size) { return 0; }
int srcml_read_unit_FILE    (struct srcml_archive* archive, FILE* srcml_file) { return 0; }
int srcml_read_unit_fd      (struct srcml_archive* archive, int srcml_fd) { return 0; }

/* close the srcML archive */
void srcml_write_close(struct srcml_archive* archive) {}
void srcml_read_close (struct srcml_archive* archive) {}
void srcml_close_archive(struct srcml_archive * archive) {

  archive->translator->close();

}

/* free the srcML archive data */
void srcml_write_free(struct srcml_archive* archive) {}
void srcml_read_free (struct srcml_archive* archive) {}

struct srcml_unit * srcml_create_unit(struct srcml_archive * archive) {

  struct srcml_unit * unit = (struct srcml_unit *)malloc(sizeof(struct srcml_unit));
  memset(unit, 0, sizeof(struct srcml_unit));
  unit->translator = archive->translator;
  unit->num_registered = &archive->num_registered;
  unit->registered_languages = archive->registered_languages;
  return unit;

}

int srcml_free_unit(struct srcml_unit* unit) {

  if(unit->unit)
    free((void *)unit->unit);

  free(unit);

  return SRCML_STATUS_OK;

}

const char* srcml_get_encoding (const struct srcml_archive* archive) {

  return archive->encoding;

}

const char* srcml_get_language (const struct srcml_archive* archive) {

  return archive->language;

}

const char* srcml_get_filename (const struct srcml_archive* archive) {

  return archive->filename;

}

const char* srcml_get_directory(const struct srcml_archive* archive) {

  return archive->directory;

}

const char* srcml_get_version  (const struct srcml_archive* archive) {

  return archive->version;

}

int         srcml_get_options  (const struct srcml_archive* archive) {

  return  archive->options;

}

int         srcml_get_tabstop  (const struct srcml_archive* archive) {

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
int srcml_add_transform_xpath(struct srcml_archive* archive, const char* xpath_string) { return 0; }
int srcml_add_transform_xslt(struct srcml_archive* archive, const char* xlst_filename) { return 0; }
int srcml_add_transform_relaxng(struct srcml_archive* archive, const char* xlst_filename) { return 0; }
int srcml_apply_transforms(struct srcml_archive* iarchive, struct srcml_archive* oarchive) { return 0; }
