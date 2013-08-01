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

#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

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
#define SRCML_OPTION_COMPRESS  1<<1
#define SRCML_OPTION_CPP
#define SRCML_OPTION_CPP_TEXT_ELSE
#define SRCML_OPTION_CPP_MARKUP_IF0
#define SRCML_OPTION_APPLY_ROOT

/*
  src2srcml - translate from source code to the srcML format

  NOTE:  Overloaded names until parameters are determined
  NOTE:  "converted"? "transformed"? Other term?
*/

struct srcml_archive;
struct srcml_unit; // alternatively: srcml_archive_unit

/* source code file converted to an individual srcML file 
   For setting attributes, encoding, etc, use src2srcml_filename_filename */
int src2srcml(const char* src_filename, const char* srcml_filename);

/* source code file converted to an individual srcML file */
/* NOTE:  considered using "file" instead of "filename", but libarchive did that, and deprecated it later */
int src2srcml_filename_filename(char* src_filename, char* srcml_filename, int options, char* language, char* src_encoding, char* attributes[][2]);

/* list of source code files converted to a srcML archive file */
int src2srcml_filelist_filename(char* src_filenames[], int size, char* srcmlfilename, char* language, char* src_encoding, int options);

/* source code file converted to an individual srcML memory */
int src2srcml_filename_memory(char* src_filename, char** srcml_memory, char* language, char* src_encoding, int options);

/* source code memory converted to a srcML file */
int src2srcml_memory_filename(char* src_memory, char* srcml_filename, char* language, char* src_encoding, int options);

/* source code memory to the individual srcML format */
int src2srcml_memory_memory(char* src_memory, char** srcml_memory, char* language, char* src_encoding, int options);

/*
  Write a srcML archive file by file
*/

/* create a new srcML archive data structure */
srcml_archive* src2srcml_new_archive();

/* setup options for srcml archive */
int src2srcml_set_archive_encoding  (struct srcml_archive*, const char* encoding);
int src2srcml_set_archive_language  (struct srcml_archive*, const char* language);
int src2srcml_set_archive_filename  (struct srcml_archive*, const char* filename);
int src2srcml_set_archive_directory (struct srcml_archive*, const char* directory);
int src2srcml_set_archive_version   (struct srcml_archive*, const char* version);
int src2srcml_set_archive_attributes(struct srcml_archive*, const char* attributes[][2]);
int src2srcml_set_archive_options   (struct srcml_archive*, int option);
int src2srcml_archive_register_file_extension(struct srcml_archive*, const char* extension, const char* language);

/* open a srcML archive for output */
int src2srcml_open_filename(struct srcml_archive*, const char* srcml_filename);
int src2srcml_open_memory(struct srcml_archive*, const char* buffer, size_t buffer_size);
int src2srcml_open_FILE(struct srcml_archive*, FILE* srcml_file);
int src2srcml_open_fd(struct srcml_archive*, int srcml_fd);

//srcml_unit* src2srcml_create_unit
struct srcml_archive_unit* src2srcml_new_unit(struct srcml_archive*);

/* setup options for srcml unit */
int src2srcml_set_unit_language (struct srcml_archive*, const char* language);
int src2srcml_set_unit_filename (struct srcml_archive*, const char* filename);
int src2srcml_set_unit_directory(struct srcml_archive*, const char* directory);
int src2srcml_set_unit_version  (struct srcml_archive*, const char* version);

/* write a src file to the srcML archive after translation */
int src2srcml_write_unit_filename(struct srcml_archive* archive, const char* src_filename);
int src2srcml_write_unit_memory(struct srcml_archive* archive, const char* buffer, size_t buffer_size);

/* write a buffer to the srcML archive after translation */
int src2srcml_write_unit_memory(struct srcml_archive* archive, const char* buffer, size_t buffer_size, const char* language, const char* attributes[][2]);

/* close the srcML archive */
void src2srcml_close(struct srcml_archive* arcdata);

/* free the srcML archive data */
void src2srcml_free(struct srcml_archive*);

/*
  srcml2src - translate from the srcML format back to source code
  NOTE: mirror of src2srcml
*/

/* srcML file converted back to a source code file, or directory of source-code files for an archive */
int srcml2src(const char* srcml_filename, const char* src_filename);

int srcml2src_filename_filename(const char* srcml_filename, const char* src_filename, int options, const char* src_encoding, int unit);

/* srcml utility functions NOTE:  need memory forms of these */

const char* src2srcml_get_encoding (struct srcml_archive*);
const char* src2srcml_get_language (struct srcml_archive*);
const char* src2srcml_get_filename (struct srcml_archive*);
const char* src2srcml_get_directory(struct srcml_archive*);
const char* src2srcml_get_version  (struct srcml_archive*);
int         src2srcml_get_options  (struct srcml_archive*);

/* srcML attributes with namespaces */
const char* [2][2] srcml_info(const char* srcml_filename);

/* srcML attributes with namespaces and number of units */
const char* [][2] srcml_longinfo(const char* srcml_filename);

/* srcML attributes with namespaces of a particular unit in an archive */
const char* [][2] srcml_info_unit(const char* srcml_filename, int unit);

/* list of filenames */
const char* [] srcml_list(const char* srcml_filename);

/* srcML XPath query and XSLT transform functions */

/* XPath (as a string) is applied to the input srcml with the query result stored in the output srcml */
int srcml_xpath_filename_filename(const char* input_srcml_filename, const char* xpath_string, const char* output_srcml_filename, int options);
int srcml_xpath_filename_memory(const char* input_srcml_filename, const char* xpath_string, char** output_buffer, size_t* size, int options);
int srcml_xpath_memory_filename(char* input_srcml_buffer, size_t size, const char* xpath_string, const char* output_srcml_filename, int options);
int srcml_xpath_memory_memory(char* input_srcml_buffer, size_t size, const char* xpath_string, char** output_buffer, size_t* size, int options);

/* XSLT file is applied to the input srcml with the transformed result stored in the output srcml */
int srcml_xslt_filename_filename(const char* input_srcml_filename, const char* xslt_filename, const char* output_srcml_filename, int options, const char* xsltparams[][2]);

/* RelaxNG file is applied to the input srcml with the transformed (???) result stored in the output srcml */
int srcml_relaxng_filename_filename(const char* input_srcml_filename, const char* relaxng_filename, const char* output_srcml_filename, int options, const char* xsltparams[][2]);

/* Pipeline of XPath, XSLT, and RelaxNG transformations on the input srcml filename with the transformed result stored in the
   output srcml filename. XPath is a string, while XSLT and RelaxNG are filenames.  Transforms are applied in the order given */
int srcml_transform_filename_filename(const char* input_srcml_filename, const char* transformation[], size_t transformation_size, const char* output_srcml_filename, int options);

#ifdef __cplusplus
//}
#endif

#endif
