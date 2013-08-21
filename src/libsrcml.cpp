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


// Includes for translator
#include "srcMLTranslator.hpp"
#include "Language.hpp"
#include "Options.hpp"

struct srcml_archive {
    const char* filename;
};

struct srcml_entry {
    /* Have to remember which archive the unit is from */
    struct srcml_archive* archive;
};

/* translates to/from srcML */
int srcml(const char* input_filename, const char* output_filename, const char* language) {
  static struct {
    const char * language_string;
    int language_int;

  } language2int[] = {   {"C", Language::LANGUAGE_C}
							 , {"C++", Language::LANGUAGE_CXX}
							 , {"C#", Language::LANGUAGE_CSHARP}
							 , {"Java", Language::LANGUAGE_JAVA}
							 , {0, 0}};

  int lang = 0;
  for(int i = 0; language2int[i].language_string; ++i)
    if(strcmp(language2int[i].language_string, language) == 0) {

      lang = language2int[i].language_int;
      break;

    }

  OPTION_TYPE options = OPTION_LITERAL | OPTION_OPERATOR | OPTION_MODIFIER;
  options |= lang == Language::LANGUAGE_JAVA ? 0 : OPTION_CPP;

  srcMLTranslator translator(lang, output_filename, options); 
  //translator.translate(input_filename, 0, 0, 0, lang);
  //translator.close();

  return SRCML_STATUS_OK;
}

/* source-code language is supported */
int srcml_check_language(const char* language) { return strcmp(language, "C++") == 0; }

/* null-terminated array of supported source-code languages */
const char** srcml_language_list() {
    static const char* langs[] = { "C", "C++", "C#", "Java", 0 };
    return langs;
}

/* currently registered language for a file extension
   Full filename can be provided, and extension will be extracted */
const char * srcml_check_extension(const char* filename) { return 0; }

/* currently supported format, e.g., tar.gz
   Full filename can be provided, and extension will be extracted */
int srcml_check_format(const char* format) { return 1; }

/* particular encoding is supported, both for input and output */
int srcml_check_encoding(const char* encoding) { return 1; }

/* prefix for an XML namespace */
const char* srcml_check_prefix(const char* namespace_uri) { return "src"; }

/* namespace for an XML prefix */
const char* srcml_check_namespace(const char* prefix) { return "http://www.sdml.info/srcML/src"; }

/* whether various features are available in this installation */
int srcml_check_xslt() { return 1; }
int srcml_check_exslt() { return 1; }

/* string describing last error */
const char* srcml_error_string() { return ""; }

/* create a new srcml archive
   client will have to free it using srcml_free() */
struct srcml_archive* srcml_create_archive() { return (struct srcml_archive*) malloc(sizeof(struct srcml_archive)); }

#if 0

/* clone the setup of an existing archive
   client will have to free it using srcml_archive_free() */
struct srcml_archive* srcml_clone_archive(const struct srcml_archive*);

/* setup options for srcml archive */
int srcml_set_encoding  (struct srcml_archive*, const char* encoding);
int srcml_set_language  (struct srcml_archive*, const char* language);
int srcml_set_filename  (struct srcml_archive*, const char* filename);
int srcml_set_directory (struct srcml_archive*, const char* directory);
int srcml_set_version   (struct srcml_archive*, const char* version);
int srcml_set_attributes(struct srcml_archive*, const char* attr[][2]);
int srcml_set_options   (struct srcml_archive*, int option);
int srcml_set_option    (struct srcml_archive*, int option);
int srcml_clear_option  (struct srcml_archive*, int option);
int srcml_set_tabstop   (struct srcml_archive*, int tabstop);
int srcml_register_file_extension(struct srcml_archive*, const char* extension, const char* language);
int srcml_register_namespace(struct srcml_archive*, const char* prefix, const char* ns);

/* open a srcML archive for output */
int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
int srcml_write_open_memory  (struct srcml_archive*, char* buffer, size_t buffer_size);
int srcml_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_write_open_fd      (struct srcml_archive*, int srcml_fd);

/* open a srcML archive for reading */
int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);

/* setup options for srcml unit */
int srcml_entry_set_language (struct srcml_entry*, const char* language);
int srcml_entry_set_filename (struct srcml_entry*, const char* filename);
int srcml_entry_set_directory(struct srcml_entry*, const char* directory);
int srcml_entry_set_version  (struct srcml_entry*, const char* version);

const char* srcml_entry_get_language (const struct srcml_entry*);
const char* srcml_entry_get_filename (const struct srcml_entry*);
const char* srcml_entry_get_directory(const struct srcml_entry*);
const char* srcml_entry_get_version  (const struct srcml_entry*);

/* Convert to srcml and append to the archive */
int srcml_write_entry_archive (struct srcml_archive*, struct srcml_entry*);
int srcml_write_entry_filename(struct srcml_archive*, const char* src_filename);
int srcml_write_entry_memory  (struct srcml_archive*, char* src_buffer, size_t buffer_size);
int srcml_write_entry_FILE    (struct srcml_archive*, FILE* src_file);
int srcml_write_entry_fd      (struct srcml_archive*, int src_fd);

/* Read the next entry from the archive */
const struct srcml_entry* srcml_read_entry_archive (struct srcml_archive*);
const struct srcml_entry* srcml_read_archive_current_entry(const struct srcml_archive*);
int srcml_read_entry_filename(struct srcml_archive*, const char* src_filename);
int srcml_read_entry_memory  (struct srcml_archive*, const char* src_buffer, size_t buffer_size);
int srcml_read_entry_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_read_entry_fd      (struct srcml_archive*, int srcml_fd);

/* close the srcML archive */
void srcml_write_close(struct srcml_archive*);
void srcml_read_close (struct srcml_archive*);

/* free the srcML archive data */
void srcml_write_free(struct srcml_archive*);
void srcml_read_free (struct srcml_archive*);

void srcml_free_archive_entry(struct srcml_entry*);

const char* srcml_get_encoding (const struct srcml_archive*);
const char* srcml_get_language (const struct srcml_archive*);
const char* srcml_get_filename (const struct srcml_archive*);
const char* srcml_get_directory(const struct srcml_archive*);
const char* srcml_get_version  (const struct srcml_archive*);
int         srcml_get_options  (const struct srcml_archive*);
int         srcml_get_tabstop  (const struct srcml_archive*);

/* srcML attributes with namespaces (header read only)*/
const char** srcml_info(const char* srcml_filename);

/* srcML attributes with namespaces and number of units (complete file read) */
const char** srcml_longinfo(const char* srcml_filename);

/* srcML attributes with namespaces of a particular unit in an archive */
const char** srcml_info_unit(const char* srcml_filename, int unit);

/* list of filenames */
const char** srcml_list(const char* srcml_filename);

/* srcML XPath query and XSLT transform functions */
int srcml_add_transform_xpath(struct srcml_archive*, const char* xpath_string);
int srcml_add_transform_xslt(struct srcml_archive*, const char* xlst_filename);
int srcml_add_transform_relaxng(struct srcml_archive*, const char* xlst_filename);
int srcml_apply_transforms(struct srcml_archive* iarchive, struct srcml_archive* oarchive);

#endif
