/**
 * @file srcml.h
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  Ideas/standards used in this API (For internal use)
  * srcml functions form the namespace srcml_*
  * Only types that are passed as parameters:
  - srcml_archive* (struct srcml_archive*)
  - srcml_unit* (struct srcml_unit*)
  - char*
  - int
*/

#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

#include <stddef.h> /* size_t */
#include <stdio.h> /* FILE * */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define __LIBSRCML_DECL __declspec(dllexport)
#else
#define __LIBSRCML_DECL
#endif

/* Header file version macros */
/** number representing libsrcml version */
#define SRCML_VERSION_NUMBER 10000
/** string containing libsrcml version */
#define SRCML_VERSION_STRING "libsrcml 0.9"

/* Cleanup the globally allocated items */
__LIBSRCML_DECL void srcml_cleanup_globals();

/* Library version functions */
__LIBSRCML_DECL int srcml_version_number();

__LIBSRCML_DECL const char* srcml_version_string();

/* Core language set */
/** srcML language not set */
#define SRCML_LANGUAGE_NONE   0
/** string for language C */
#define SRCML_LANGUAGE_C      "C"
/** string for language C++ */
#define SRCML_LANGUAGE_CXX    "C++"
/** string for language Java */
#define SRCML_LANGUAGE_JAVA   "Java"
/** string for language C# */
#define SRCML_LANGUAGE_CSHARP "C#"
/** string for language XML */
#define SRCML_LANGUAGE_XML    "xml"

/* Options */
/** Create an archive */
#define SRCML_OPTION_ARCHIVE        1<<0
/** Markups literal in special namespace */
#define SRCML_OPTION_LITERAL           1<<1
/** Markups modifiers in special namespace */
#define SRCML_OPTION_MODIFIER          1<<2
/** Markups operator in special namespace */
#define SRCML_OPTION_OPERATOR          1<<3
/** Markups operator in special namespace */
#define SRCML_OPTION_DEBUG             1<<4
/** Include line/column position attributes */
#define SRCML_OPTION_POSITION          1<<5
/** Markup preprocessor elements (default for C, C++) */
#define SRCML_OPTION_CPP               1<<6
/** Markup preprocessor elements (default for C, C++, C#) */
#define SRCML_OPTION_CPP_NOMACRO       1<<7
/** Issue an XML declaration */
#define SRCML_OPTION_XML_DECL       1<<8
/** Include any XML namespace declarations */
#define SRCML_OPTION_NAMESPACE_DECL 1<<9
/** Leave as text preprocessor else parts (default: markup) */
#define SRCML_OPTION_CPP_TEXT_ELSE     1<<10
/** Markup preprocessor @code #if 0 @endcode sections (default: leave as text) */
#define SRCML_OPTION_CPP_MARKUP_IF0    1<<11
/** Apply transformations to the entire srcML file (default: each unit */
#define SRCML_OPTION_APPLY_ROOT        1<<12
/** Compress the output file */
#define SRCML_OPTION_COMPRESS          1<<13
/** Nest if in else if intead of elseif tag */
#define SRCML_OPTION_NESTIF            1<<14

/* srcml status messages */
/** Return status indicating no errors */
#define SRCML_STATUS_OK    0
/** Return status indicating errors occurred */
#define SRCML_STATUS_ERROR 1

/* srcML data structures */
struct srcml_archive;
struct srcml_unit;

/*
  Convenience function for translating to and from the srcML format

  * Translates from source code to srcML if the input_filename
  extension is for source code, e.g., .c, .cpp, .java
  Language determined by file extension if language is not
  set with srcml_set_language()
  
  * Translates from srcML to source code if the input_filename
  extension is '.xml'.

  * Input filenames can be for a file, a compressed file, a
  source-code archive (e.g., .tar), or a URI that is for any of
  these.  Additionally, the input filename can be for a directory.

  * Options can be specified with the global srcml_set_*(),
  srcml_clear_(), and srcml_register_*()

  * Options can be queried with the global srcml_get_*() and
  srcml_check_*()
*/
__LIBSRCML_DECL int srcml(const char* input_filename, const char* output_filename);

/*
  Global settings.  Can be used with convenience function srcml()
*/
__LIBSRCML_DECL int srcml_set_encoding           (const char* encoding);
__LIBSRCML_DECL int srcml_set_language           (const char* language);
__LIBSRCML_DECL int srcml_set_filename           (const char* filename);
__LIBSRCML_DECL int srcml_set_directory          (const char* directory);
__LIBSRCML_DECL int srcml_set_version            (const char* version);
__LIBSRCML_DECL int srcml_set_options            (unsigned long long option);
__LIBSRCML_DECL int srcml_enable_option          (unsigned long long option);
__LIBSRCML_DECL int srcml_disable_option         (unsigned long long option);
__LIBSRCML_DECL int srcml_set_tabstop            (int tabstop);
__LIBSRCML_DECL int srcml_register_file_extension(const char* extension, const char* language);
__LIBSRCML_DECL int srcml_register_namespace     (const char* prefix, const char* ns);
__LIBSRCML_DECL int srcml_register_macro         (const char* token, const char* type);

__LIBSRCML_DECL const char*        srcml_get_encoding ();
__LIBSRCML_DECL const char*        srcml_get_language ();
__LIBSRCML_DECL const char*        srcml_get_filename ();
__LIBSRCML_DECL const char*        srcml_get_directory();
__LIBSRCML_DECL const char*        srcml_get_version  ();
__LIBSRCML_DECL unsigned long long srcml_get_options  ();
__LIBSRCML_DECL int                srcml_get_tabstop  ();
__LIBSRCML_DECL int                srcml_get_namespace_size();
__LIBSRCML_DECL const char*        srcml_get_prefix(int pos);
__LIBSRCML_DECL const char*        srcml_get_prefix_uri(const char* namespace_uri);
__LIBSRCML_DECL const char*        srcml_get_namespace(int pos);
__LIBSRCML_DECL const char*        srcml_get_namespace_prefix(const char* prefix);
__LIBSRCML_DECL int                srcml_get_macro_list_size();
__LIBSRCML_DECL const char*        srcml_get_macro_token(int pos);
__LIBSRCML_DECL const char*        srcml_get_macro_token_type(const char* namespace_uri);
__LIBSRCML_DECL const char*        srcml_get_macro_type(int pos);

/* source-code language is supported */
__LIBSRCML_DECL int srcml_check_language(const char* language);

/* null-terminated array of supported source-code languages */
__LIBSRCML_DECL const char** srcml_language_list();

/* currently registered language for a file extension
   When full filename is given, the extension is extracted */
__LIBSRCML_DECL const char* srcml_check_extension(const char* filename);

/* currently supported format, e.g., tar.gz
   When full filename is given, the extension is extracted */
__LIBSRCML_DECL int srcml_check_format(const char* format);

/* particular encoding is supported, both for input and output */
__LIBSRCML_DECL int srcml_check_encoding(const char* encoding);

/* whether various features are available in this installation */
__LIBSRCML_DECL int srcml_check_xslt();
__LIBSRCML_DECL int srcml_check_exslt();

/* string describing last error */
__LIBSRCML_DECL const char* srcml_error_string();

/*
  Full libsrcml API
*/

/* currently registered language for a file extension
   When full filename is given, the extension is extracted */
__LIBSRCML_DECL const char* srcml_archive_check_extension(struct srcml_archive* archive, const char* filename);

/* create a new srcml archive
   client is responsible for freeing it using srcml_free_archive() */
__LIBSRCML_DECL struct srcml_archive* srcml_create_archive();

/* clone the setup of an existing archive
   client is responsible for freeing it using srcml_free_archive() */
__LIBSRCML_DECL struct srcml_archive* srcml_clone_archive(const struct srcml_archive*);

/* open a srcML archive for output */
__LIBSRCML_DECL int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
__LIBSRCML_DECL int srcml_write_open_memory  (struct srcml_archive*, char** buffer, int * size);
__LIBSRCML_DECL int srcml_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
__LIBSRCML_DECL int srcml_write_open_fd      (struct srcml_archive*, int srcml_fd);

/* setup options for srcml archive */
__LIBSRCML_DECL int srcml_archive_set_encoding           (struct srcml_archive*, const char* encoding);
__LIBSRCML_DECL int srcml_archive_set_language           (struct srcml_archive*, const char* language);
__LIBSRCML_DECL int srcml_archive_set_filename           (struct srcml_archive*, const char* filename);
__LIBSRCML_DECL int srcml_archive_set_directory          (struct srcml_archive*, const char* directory);
__LIBSRCML_DECL int srcml_archive_set_version            (struct srcml_archive*, const char* version);

__LIBSRCML_DECL int srcml_archive_set_options            (struct srcml_archive*, unsigned long long option);
__LIBSRCML_DECL int srcml_archive_enable_option          (struct srcml_archive*, unsigned long long option);
__LIBSRCML_DECL int srcml_archive_disable_option         (struct srcml_archive*, unsigned long long option);
__LIBSRCML_DECL int srcml_archive_set_tabstop            (struct srcml_archive*, int tabstop);
__LIBSRCML_DECL int srcml_archive_register_file_extension(struct srcml_archive*, const char* extension, const char* language);
__LIBSRCML_DECL int srcml_archive_register_namespace     (struct srcml_archive*, const char* prefix, const char* ns);
__LIBSRCML_DECL int srcml_archive_register_macro         (struct srcml_archive*, const char* token, const char* type);

/* query options for srcml archive */
__LIBSRCML_DECL const char*        srcml_archive_get_encoding        (const struct srcml_archive*);
__LIBSRCML_DECL const char*        srcml_archive_get_language        (const struct srcml_archive*);
__LIBSRCML_DECL const char*        srcml_archive_get_filename        (const struct srcml_archive*);
__LIBSRCML_DECL const char*        srcml_archive_get_directory       (const struct srcml_archive*);
__LIBSRCML_DECL const char*        srcml_archive_get_version         (const struct srcml_archive*);
__LIBSRCML_DECL unsigned long long srcml_archive_get_options         (const struct srcml_archive*);
__LIBSRCML_DECL int                srcml_archive_get_tabstop         (const struct srcml_archive*);
__LIBSRCML_DECL int                srcml_archive_get_namespace_size(const struct srcml_archive*);
__LIBSRCML_DECL const char*        srcml_archive_get_prefix(const struct srcml_archive*, int pos);
__LIBSRCML_DECL const char*        srcml_archive_get_prefix_uri(const struct srcml_archive*, const char* namespace_uri);
__LIBSRCML_DECL const char*        srcml_archive_get_namespace(const struct srcml_archive*, int pos);
__LIBSRCML_DECL const char*        srcml_archive_get_namespace_prefix(const struct srcml_archive*, const char* prefix);
__LIBSRCML_DECL int                srcml_archive_get_macro_list_size (const struct srcml_archive*);
__LIBSRCML_DECL const char*        srcml_archive_get_macro_token     (const struct srcml_archive*, int pos);
__LIBSRCML_DECL const char*        srcml_archive_get_macro_token_type(const struct srcml_archive*, const char* namespace_uri);
__LIBSRCML_DECL const char*        srcml_archive_get_macro_type      (const struct srcml_archive*, int pos);

/* create a new srcml unit
   client is responsible for freeing it using srcml_free_unit() */
__LIBSRCML_DECL struct srcml_unit* srcml_create_unit(struct srcml_archive* archive);

/* Setup options for srcml unit */
__LIBSRCML_DECL int srcml_unit_set_language (struct srcml_unit*, const char* language);
__LIBSRCML_DECL int srcml_unit_set_filename (struct srcml_unit*, const char* filename);
__LIBSRCML_DECL int srcml_unit_set_directory(struct srcml_unit*, const char* directory);
__LIBSRCML_DECL int srcml_unit_set_version  (struct srcml_unit*, const char* version);

/* Convert to srcml.  Files/buffer can be compressed, but not a
   source archive format (e.g., not .tar)
*/
__LIBSRCML_DECL int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
__LIBSRCML_DECL int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
__LIBSRCML_DECL int srcml_parse_unit_FILE    (struct srcml_unit*, FILE* src_file);
__LIBSRCML_DECL int srcml_parse_unit_fd      (struct srcml_unit*, int src_fd);

/* Append unit to an archive */
__LIBSRCML_DECL int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);

/* Free allocated unit */
__LIBSRCML_DECL int srcml_free_unit(struct srcml_unit*);

/* close the srcML archive */
__LIBSRCML_DECL void srcml_close_archive(struct srcml_archive*);

/* free the srcML archive data */
__LIBSRCML_DECL void srcml_free_archive(struct srcml_archive*);

/* open a srcML archive for reading */
__LIBSRCML_DECL int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
__LIBSRCML_DECL int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
__LIBSRCML_DECL int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
__LIBSRCML_DECL int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);

/* Read the next unit from the archive
   Return 0 if there are no more unit */
__LIBSRCML_DECL struct srcml_unit* srcml_read_unit_header(struct srcml_archive*);
__LIBSRCML_DECL struct srcml_unit* srcml_read_unit(struct srcml_archive*);

/* Read over the next unit from the archive
   Returns 0 if no unit to skip */
__LIBSRCML_DECL int srcml_skip_unit(struct srcml_archive*);

/* Read a unit at a specific position in an archive
   Unit numbers start at 1
   Returns 0 if pos unit does not exist */
__LIBSRCML_DECL struct srcml_unit* srcml_read_unit_position(struct srcml_archive*, int pos);

/* Query options of srcml unit */
__LIBSRCML_DECL const char* srcml_unit_get_language (const struct srcml_unit*);
__LIBSRCML_DECL const char* srcml_unit_get_filename (const struct srcml_unit*);
__LIBSRCML_DECL const char* srcml_unit_get_directory(const struct srcml_unit*);
__LIBSRCML_DECL const char* srcml_unit_get_version  (const struct srcml_unit*);
__LIBSRCML_DECL const char* srcml_unit_get_xml      (const struct srcml_unit*);

/* Convert from srcML to source code */
__LIBSRCML_DECL int srcml_unparse_unit_filename(struct srcml_unit*, const char* src_filename);
__LIBSRCML_DECL int srcml_unparse_unit_memory  (struct srcml_unit*, char** src_buffer, int * src_size);
__LIBSRCML_DECL int srcml_unparse_unit_FILE    (struct srcml_unit*, FILE* srcml_file);
__LIBSRCML_DECL int srcml_unparse_unit_fd      (struct srcml_unit*, int srcml_fd);

// @todo Consider removing this and making srcml client iterate over the values
// Problems with memory handling
__LIBSRCML_DECL const char** srcml_list(const char* srcml_filename);

/* srcML XPath query and XSLT transform functions */
__LIBSRCML_DECL int srcml_clear_transforms(struct srcml_archive*);
__LIBSRCML_DECL int srcml_append_transform_xpath(struct srcml_archive*, const char* xpath_string);
__LIBSRCML_DECL int srcml_append_transform_xslt(struct srcml_archive*, const char* xslt_filename);
__LIBSRCML_DECL int srcml_append_transform_relaxng(struct srcml_archive*, const char* relaxng_filename);
__LIBSRCML_DECL int srcml_apply_transforms(struct srcml_archive* iarchive, struct srcml_archive* oarchive);

/* Augment read to use user defined xml handlers */
__LIBSRCML_DECL int srcml_set_xml_handler_start_unit(struct srcml_archive*);
__LIBSRCML_DECL int srcml_set_xml_handler_start_element(struct srcml_archive*);
__LIBSRCML_DECL int srcml_set_xml_handler_characters(struct srcml_archive*);
__LIBSRCML_DECL int srcml_set_xml_handler_cdata(struct srcml_archive*);
__LIBSRCML_DECL int srcml_set_xml_handler_comment(struct srcml_archive*);
__LIBSRCML_DECL int srcml_set_xml_handler_end_element(struct srcml_archive*);
__LIBSRCML_DECL int srcml_set_xml_handler_end_unit(struct srcml_archive*);

__LIBSRCML_DECL int srcml_xml_handler_parse_archive(struct srcml_archive*);
__LIBSRCML_DECL int srcml_xml_handler_release(struct srcml_archive*);
__LIBSRCML_DECL int srcml_xml_handler_stop(struct srcml_archive*);

#ifdef __cplusplus
}
#endif

#endif
