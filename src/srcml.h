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

  NOTE:  Perhaps change "unit" to "entry", e.g., "write_unit_*" -> "write_entry_*"
  NOTE:  Went with camel case as it is c, and libarchive uses camelcase, and I like the way that API feels
  NOTE:  We will probably not support all of this all at once.  Just want to work out the names so we don't
  have to change any later

  NOTE:  considered using "file" instead of "filename", but libarchive did that, and deprecated it later
*/

#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

#include <sys/types.h> /* size_t */
#include <stdio.h> /* FILE * */

#ifdef __cplusplus
//extern "C" {
#endif

/* srcml error messages */
#define SRCML_OK 0
#define SRCML_ERROR 1

/* srcml options */
#define SRCML_OPTION_LITERAL  1<<0
#define SRCML_OPTION_MODIFIER 1<<1
#define SRCML_OPTION_ARCHIVE  1<<2
#define SRCML_OPTION_POSITION  1<<2
#define SRCML_OPTION_COMPRESS  1<<1
#define SRCML_OPTION_NO_XML_DECLARATION
#define SRCML_OPTION_NO_NAMESPACE_DECLARATION
#define SRCML_OPTION_CPP
#define SRCML_OPTION_CPP_TEXT_ELSE
#define SRCML_OPTION_CPP_MARKUP_IF0
#define SRCML_OPTION_APPLY_ROOT

/* Set of languages */
#define SRCML_LANGUAGE_NONE   0
#define SRCML_LANGUAGE_C      "C"
#define SRCML_LANGUAGE_CXX    "C++"
#define SRCML_LANGUAGE_JAVA   "Java"
#define SRCML_LANGUAGE_CSHARP "C#"

/*
  src2srcml - translate from source code to the srcML format
*/

struct srcml_archive;

/* Translates from source code to srcML if the input_filename extension is for source code.
   Language determined by file extension if language argument is null.

   Translates from srcML back to source code if the input_filename extension is '.xml'. 
   Language argument should be null in this case.

   CLI equivalence:  srcml main.cpp -o main.cpp.xml
                     srcml main.cpp.xml -o main.cpp

   NOTE:  Should language be a string?  Or an enumerated type?  Keep in mind, future languages
   may be user added.  Probably string would be more flexible.  Perhaps provide constants
   for core languages
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

#if 0
/* Translates from source code to srcML. Language determined by file extension.
   For setting attributes, encoding, etc, use src2srcml_filename_filename()

   (Old) CLI equivalence:  src2srcml main.cpp -o main.cpp.xml
*/
int src2srcml(const char* src_filename, const char* srcml_filename);
#endif

/* Too many parameters.  Favoring set on full archive */
#if 0
/* Translates source code to srcML with full control of attr/options */
int src2srcml_filename_filename(char* src_filename, char* srcml_filename, int options, char* language, char* attr[][2]);
int src2srcml_filename_memory  (char* src_filename, char** srcml_buffer,  size_t size, int options, char* language, char* attr[][2]);
int src2srcml_memory_filename  (char* src_buffer, char* srcml_filename,   int options, char* language, char* attr[][2]);
int src2srcml_memory_memory    (char* src_buffer, char** srcml_buffer, size_t size,    int options, char* language, char* attr[][2]);

/* Translates a list of source code files to a srcML archive */
/* NOTE:  have src_encoding be part of attr? */
int src2srcml_filelist_filename(char* src_filelist[], int size, char* srcmlfilename, int options, char* language, char* src_encoding, char* attr[][2]);
int src2srcml_filelist_memory  (char* src_filelist[], int size, char** srcml_buffer, int options, char* language, char* src_encoding, char* attr[][2]);
int src2srcml_filelist_FILE    (char* src_filelist[], int size, FILE* srcml_file,    int options, char* language, char* src_encoding, char* attr[][2]);
int src2srcml_filelist_fd      (char* src_filelist[], int size, int srcml_fd,        int options, char* language, char* src_encoding, char* attr[][2]);
#endif

/*
  Full API for creating srcML archives
*/

/* create a new srcml archive
   client will have to free it using srcml_free() */
struct srcml_archive* srcml_write_new_archive();
struct srcml_archive* srcml_read_new_archive();

/* current entry in archive */
int srcml_has_entry(struct srcml_archive*);

/* clone the setup of an existing archive
   client will have to free it using src2srcml_free() */
struct srcml_archive* src2srcml_clone_archive(const struct srcml_archive*);
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
int srcml_write_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
int srcml_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_write_open_fd      (struct srcml_archive*, int srcml_fd);

/* open a srcML archive for reading */
int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);

/* setup options for srcml unit */
int srcml_unit_set_language (struct srcml_archive*, const char* language);
int srcml_unit_set_filename (struct srcml_archive*, const char* filename);
int srcml_unit_set_directory(struct srcml_archive*, const char* directory);
int srcml_unit_set_version (struct srcml_archive*, const char* version);

const char* srcml_unit_get_language (struct srcml_archive*);
const char* srcml_unit_get_filename (struct srcml_archive*);
const char* srcml_unit_get_directory(struct srcml_archive*);
const char* srcml_unit_get_version (struct srcml_archive*);

/* Convert to srcml and append to the archive */
int srcml_write_unit_filename(struct srcml_archive*, const char* src_filename);
int srcml_write_unit_memory  (struct srcml_archive*, const char* src_buffer, size_t buffer_size);
int srcml_write_unit_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_write_unit_fd      (struct srcml_archive*, int srcml_fd);

/* Read the next entry from the archive */
int srcml_read_unit_filename(struct srcml_archive*, const char* src_filename);
int srcml_read_unit_memory  (struct srcml_archive*, const char* src_buffer, size_t buffer_size);
int srcml_read_unit_FILE    (struct srcml_archive*, FILE* srcml_file);
int srcml_read_unit_fd      (struct srcml_archive*, int srcml_fd);

/* close the srcML archive */
void srcml_write_close(struct srcml_archive*);
void srcml_read_close(struct srcml_archive*);

/* free the srcML archive data */
void srcml_write_free(struct srcml_archive*);
void srcml_read_free(struct srcml_archive*);

#if 0
/*
  srcml2src - translate from the srcML format back to source code
  NOTE: mirror of src2srcml
*/


/* srcML file converted back to a source code file, or directory of source-code files for an archive */
int srcml2src(const char* srcml_filename, const char* src_filename);

#if 0
int srcml2src_filename_filename(const char* srcml_filename, const char* src_filename, int options, const char* src_encoding, int unit);
#endif
#endif

const char* srcml_get_encoding (const struct srcml_archive*);
const char* srcml_get_language (const struct srcml_archive*);
const char* srcml_get_filename (const struct srcml_archive*);
const char* srcml_get_directory(const struct srcml_archive*);
const char* srcml_get_version  (const struct srcml_archive*);
int         srcml_get_options  (const struct srcml_archive*);
int         srcml_get_tabstop  (const struct srcml_archive*);

/* srcml utility functions NOTE:  need memory forms of these */

/* srcML attributes with namespaces */
const char** srcml_info(const char* srcml_filename);

/* srcML attributes with namespaces and number of units */
const char** srcml_longinfo(const char* srcml_filename);

/* srcML attributes with namespaces of a particular unit in an archive */
const char** srcml_info_unit(const char* srcml_filename, int unit);

/* list of filenames */
const char** srcml_list(const char* srcml_filename);

/* srcML XPath query and XSLT transform functions */

int srcml_add_transform_xpath(struct srcml_archive*, const char* xpath_string);
int srcml_add_transform_xslt(struct srcml_archive*, const char* xlst_filename);

/* XPath (as a string) is applied to the input srcml with the query result stored in the output srcml */
int srcml_xpath_filename_filename(const char* input_srcml_filename, const char* xpath_string, const char* output_srcml_filename, int options);
int srcml_xpath_filename_memory(const char* input_srcml_filename, const char* xpath_string, char** output_buffer, size_t* size, int options);
int srcml_xpath_memory_filename(char* input_srcml_buffer, size_t input_size, const char* xpath_string, const char* output_srcml_filename, int options);
int srcml_xpath_memory_memory(char* input_srcml_buffer, size_t input_size, const char* xpath_string, char** output_buffer, size_t* size, int options);

/* XSLT file is applied to the input srcml with the transformed result stored in the output srcml */
int srcml_xslt_filename_filename(const char* input_srcml_filename, const char* xslt_filename, const char* output_srcml_filename, int options, const char* xsltparams[][2]);

/* RelaxNG file is applied to the input srcml with the transformed (???) result stored in the output srcml */
int srcml_relaxng_filename_filename(const char* input_srcml_filename, const char* relaxng_filename, const char* output_srcml_filename, int options, const char* xsltparams[][2]);

/* Pipeline of XPath, XSLT, and RelaxNG transformations on the input srcml filename with the transformed result
   stored in the output srcml filename. XPath is a string, while XSLT and RelaxNG are filenames.  Transformations are
   applied in the order given */
int srcml_transform_filename_filename(const char* input_srcml_filename, const char* transformation[], size_t transformation_size, const char* output_srcml_filename, int options);

#ifdef __cplusplus
//}
#endif

#endif
