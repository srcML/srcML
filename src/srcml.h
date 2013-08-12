/*
  srcml.h

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
  srcml functions form the namespace srcml_*
*/

#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

#include <sys/types.h> /* size_t */
#include <stdio.h> /* FILE * */

/* Version number in include */
#define SRCML_VERSION VERSION
const char * srcml_version();

#ifdef __cplusplus
//extern "C" {
#endif

/* header file version macros */
#define SRCML_VERSION_NUMBER 1
#define SRCML_VERSION_STRING "libsrcml 0.9"

/* library version functions */
int srcml_version_number(void);
const char* srcml_version_string(void);

/* srcml data structures */
struct srcml_archive;
struct srcml_entry;

/* srcml core language set */
#define SRCML_LANGUAGE_NONE   0
#define SRCML_LANGUAGE_C      "C"
#define SRCML_LANGUAGE_CXX    "C++"
#define SRCML_LANGUAGE_JAVA   "Java"
#define SRCML_LANGUAGE_CSHARP "C#"

/* srcml options */
#define SRCML_OPTION_LITERAL  1<<0 /* Markups literal in special namespace */
#define SRCML_OPTION_MODIFIER 1<<1 /* Markups modifiers in special namespace */
#define SRCML_OPTION_ARCHIVE  1<<2
#define SRCML_OPTION_POSITION  1<<2
#define SRCML_OPTION_COMPRESS  1<<1
#define SRCML_OPTION_NO_XML_DECLARATION
#define SRCML_OPTION_NO_NAMESPACE_DECLARATION
#define SRCML_OPTION_CPP
#define SRCML_OPTION_CPP_TEXT_ELSE
#define SRCML_OPTION_CPP_MARKUP_IF0
#define SRCML_OPTION_APPLY_ROOT

/* srcml status messages */
#define SRCML_STATUS_OK 0
#define SRCML_STATUS_ERROR 1

/*
  Convenience Functions
*/

/* Translates from source code to srcML if the input_filename extension is for source code.
   Language determined by file extension if language argument is null.

   Translates from srcML back to source code if the input_filename extension is '.xml'. 
   Language argument should be null in this case.

   CLI equivalence:  srcml main.cpp -o main.cpp.xml
                     srcml main.cpp.xml -o main.cpp
*/
int srcml(const char* input_filename, const char* output_filename, const char* language);

/*
  srcML capabilities
*/

/* source-code language is supported */
int srcml_check_language(const char* language);

/* null-terminated array of supported source-code languages */
const char** srcml_language_list();

/* currently registered language for a file extension
   Full filename can be provided, and extension will be extracted */
int srcml_check_extension(const char* filename);

/* currently supported format, e.g., tar.gz
   Full filename can be provided, and extension will be extracted */
int srcml_check_format(const char* format);

/* particular encoding is supported, both for input and output */
int srcml_check_encoding(const char* encoding);

/* prefix for an XML namespace */
const char* srcml_check_prefix(const char* namespace_uri);

/* namespace for an XML prefix */
const char* srcml_check_namespace(const char* prefix);

/* whether various features are available in this installation */
int srcml_check_xslt();
int srcml_check_exslt();

/* string describing last error */
const char* srcml_error_string();

/*
  Full libsrcml archive API
*/

/* create a new srcml archive
   client will have to free it using srcml_free() */
struct srcml_archive* srcml_write_new_archive();

/* current entry in archive */
int srcml_has_entry(struct srcml_archive*);

/* clone the setup of an existing archive
   client will have to free it using srcml_archive_free() */
struct srcml_archive* srcml_archive_clone_setup(const struct srcml_archive*);

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

/* create a new entry for a srcml archive */
struct srcml_entry* srcml_new_entry(struct srcml_archive*);

/* setup options for srcml entry */
int srcml_entry_set_language (struct srcml_entry*, const char* language);
int srcml_entry_set_filename (struct srcml_entry*, const char* filename);
int srcml_entry_set_directory(struct srcml_entry*, const char* directory);
int srcml_entry_set_version  (struct srcml_entry*, const char* version);

/* Convert to srcml and append to the archive */
int srcml_write_entry_filename(struct srcml_archive*, const struct srcml_entry*, const char* src_filename);
int srcml_write_entry_memory  (struct srcml_archive*, const struct srcml_entry*, char* src_buffer, size_t buffer_size);
int srcml_write_entry_FILE    (struct srcml_archive*, const struct srcml_entry*, FILE* src_file);
int srcml_write_entry_fd      (struct srcml_archive*, const struct srcml_entry*, int src_fd);

/* free used entry */
int srcml_free_entry(struct srcml_entry*);

/* close the srcML archive */
void srcml_write_close(struct srcml_archive*);

/* free the srcML archive data */
void srcml_free_archive(struct srcml_archive*);



int srcml_write_entry_archive (struct srcml_archive*, const struct srcml_entry*);

/* open a srcML archive for reading */
int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);


const char* srcml_entry_get_language (const struct srcml_entry*);
const char* srcml_entry_get_filename (const struct srcml_entry*);
const char* srcml_entry_get_directory(const struct srcml_entry*);
const char* srcml_entry_get_version  (const struct srcml_entry*);

/* Read the next entry from the archive */
struct srcml_entry* srcml_read_entry_archive (struct srcml_archive*);
const struct srcml_entry* srcml_read_archive_current_entry(const struct srcml_archive*);
int srcml_read_entry_filename(struct srcml_archive*, const char* src_filename);
int srcml_read_entry_memory  (struct srcml_archive*, const char* src_buffer, size_t buffer_size);
int srcml_read_entry_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_read_entry_fd      (struct srcml_archive*, int srcml_fd);

void srcml_read_close (struct srcml_archive*);

void srcml_free_archive_entry(struct srcml_entry*);

struct srcml_archive* srcml_read_archive();

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

#ifdef __cplusplus
//}
#endif

#endif
