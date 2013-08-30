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

#include "../srcMLTranslator.hpp"
#include "../Language.hpp"
#include "../Options.hpp"
#include "../srcMLUtility.hpp"
#include "srcml_types.hpp"

#if defined(__GNUG__) && !defined(__MINGW32__)
#include <dlfcn.h>
#endif

std::string srcml_error;

// global archive for use with srcml() function.  Defaulted values.
srcml_archive global_archive = { SRCML_ARCHIVE_RW, 0, 0, 0, 0, 0, 0, std::vector<std::string>(), 0,
                                 4, std::vector<std::string>(), std::vector<std::string>(), std::vector<pair>(),
                                 0, 0, 0, std::vector<transform>() };

// version of libsrcml as string
const char * srcml_version_string() {

  return SRCML_VERSION_STRING;

}

// version of libsrcml as number
int srcml_version_number() {

  return SRCML_VERSION_NUMBER;

}

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

    bool is_xml = false;
    int len = strlen(input_filename);
    if((len > 4 && input_filename[len - 1] == 'l' && input_filename[len - 2] == 'm'
        && input_filename[len - 3] == 'x' && input_filename[len - 4] == '.')
       || (global_archive.language && strcmp(global_archive.language->c_str(), "xml") == 0))
      is_xml = true;

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

/*

  Global archive functions

*/

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
