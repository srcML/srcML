/**
 * @file srcml.h
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

/**
 * @mainpage srcML Project
 *
 * @section libsrcml
 * 
 * The srcml functions available in libsrcml for the purposes of:
 *
 * * Converting source code to the srcML format
 * * Converting the srcML format back to source code
 * * Querying and transformation in the srcML format
 * 
 * The general categories:
 * 
 * * The srcML convenience function srcml() for conversion of
 *   individual source files to srcML, and back. Includes helper
 *   functions to set options in the form srcml_set_*() and srcml_get_*()
 * 
 * * Query functions of the form srcml_check_*() to see the
 *   capabilities of the library. E.g., what languages are supported
 * 
 * * API for full control over the construction of srcML archives
 *   from multiple input source-code files, srcml_archive_*() and srcml_unit_*()
 */

#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

#include <stddef.h> /* size_t */
#include <stdio.h> /* FILE * */

#ifdef __cplusplus
extern "C" {
#endif

/** Function export macro */
#if defined(WIN32) && !defined(__MINGW32__)
#define LIBSRCML_DECL __declspec(dllexport)
#else
#define LIBSRCML_DECL
#endif

/* Header file version macros */
/** number representing libsrcml version */
#define SRCML_VERSION_NUMBER 9005
/** string containing libsrcml version */
#define SRCML_VERSION_STRING "0.9.5"

/* Cleanup the globally allocated items */
LIBSRCML_DECL void srcml_cleanup_globals();

/* Library version functions */
LIBSRCML_DECL int srcml_version_number();

LIBSRCML_DECL const char* srcml_version_string();

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
/** string for language C# */
#define SRCML_LANGUAGE_OBJECTIVE_C "Objective-C"
/** string for language XML */
#define SRCML_LANGUAGE_XML    "xml"

/* Options */
/** Create an archive */
#define SRCML_OPTION_ARCHIVE           1<<0
/** Include line/column position attributes */
#define SRCML_OPTION_POSITION          1<<1
/** Markup preprocessor elements (default for C, C++, C#) */
#define SRCML_OPTION_CPP_NOMACRO       1<<2
/** Markup preprocessor elements (default for C, C++) */
#define SRCML_OPTION_CPP               1<<2 | 1<<3 
/** Issue an XML declaration */
#define SRCML_OPTION_XML_DECL          1<<4
/** Include any XML namespace declarations */
#define SRCML_OPTION_NAMESPACE_DECL    1<<5
/** Leave as text preprocessor else parts (default: markup) */
#define SRCML_OPTION_CPP_TEXT_ELSE     1<<6
/** Markup preprocessor @code #if 0 @endcode sections (default: leave as text) */
#define SRCML_OPTION_CPP_MARKUP_IF0    1<<7
/** Apply transformations to the entire srcML file (default: each unit */
#define SRCML_OPTION_APPLY_ROOT        1<<8
/** Nest if in else if intead of elseif tag */
#define SRCML_OPTION_NESTIF            1<<9
/** Output hash attribute on each unit (default: on) */
#define SRCML_OPTION_HASH              1<<10
/** Wrap function/classes/etc with templates (default: on) */
#define SRCML_OPTION_WRAP_TEMPLATE     1<<11
/** output is interactive (good for editing applications) */
#define SRCML_OPTION_INTERACTIVE       1<<12
/** Not sure what this used for */
#define SRCML_OPTION_XPATH_TOTAL       1<<13
/** expression mode */
#define SRCML_OPTION_EXPRESSION        1<<14
/** Extra processing of @code#line@endcode for position information */
#define SRCML_OPTION_LINE              1<<15
/** additional cpp:if/cpp:endif checking */
#define SRCML_OPTION_CPPIF_CHECK       1<<16
/** debug time attribute */
#define SRCML_OPTION_DEBUG_TIMER       1<<17
/** turn on optional ternary operator markup */
#define SRCML_OPTION_TERNARY           1<<18
/** turn on optional ternary operator markup */
#define SRCML_OPTION_PSEUDO_BLOCK      1<<19
/** Turn on old optional markup behaviour */
#define SRCML_OPTION_OPTIONAL_MARKUP   1<<20
/** Markups literal in special namespace */
#define SRCML_OPTION_LITERAL           1<<21
/** Markups modifiers in special namespace */
#define SRCML_OPTION_MODIFIER          1<<22
/** Markups operator in special namespace */
#define SRCML_OPTION_OPERATOR          1<<23
/** Parser output special tokens for debugging the parser */
#define SRCML_OPTION_DEBUG             1<<24
/** Markups OpenMP in special namespace */
#define SRCML_OPTION_OPENMP            1<<25
/** Encode the original source encoding as an attribute */
#define SRCML_OPTION_STORE_ENCODING    1<<26

/** All default enabled options */
#define SRCML_OPTION_DEFAULT (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_HASH | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY)

/* srcml status messages */
/** Return status indicating no errors */
#define SRCML_STATUS_OK                   0
/** Return status indicating errors occurred */
#define SRCML_STATUS_ERROR                1
/** Return status indicating an invalid argument */
#define SRCML_STATUS_INVALID_ARGUMENT     2
/** Return status indicating that their is some problem with the input */
#define SRCML_STATUS_INVALID_INPUT        3
/** Return status indicating an invalid read I/O operation (such as write on read only archive) */
#define SRCML_STATUS_INVALID_IO_OPERATION 4
/** Return status indicating that their is some problem with the input */
#define SRCML_STATUS_IO_ERROR             5
/** Return status indicating an unitialized unit */
#define SRCML_STATUS_UNINITIALIZED_UNIT   6
/** Return status indicating an unset language */
#define SRCML_STATUS_UNSET_LANGUAGE       7
/** Return status inidicating their are no transformations */
#define SRCML_STATUS_NO_TRANSFORMATION    8

/** Source-code end of line done automatically */
#define SRCML_UNPARSE_OPTION_AUTO   0
/** Source-code end of line done accoring to operating system */
#define SRCML_UNPARSE_OPTION_NATIVE 0
/** Source-code end of line is new line only */
#define SRCML_UNPARSE_OPTION_LF     1
/** Source-code end of line is carriage return only */
#define SRCML_UNPARSE_OPTION_CR     2
/** Source-code end of line is carriage return and new line */
#define SRCML_UNPARSE_OPTION_CRLF   3

/* libsrcml data structures */
struct srcml_archive;
struct srcml_unit;

/*
  Convenience function for translating to and from the srcML format

  * Translates from source code to srcML if the input_filename
  extension is for source code, e.g., .c, .cpp, .java Language
  determined by file extension if language is not set with
  srcml_set_language()

  * Translates from srcML to source code if the input_filename
  extension is '.xml'.

  * Input filenames can be for a file, a compressed file, a
  source-code archive (e.g., .tar), or a URI that is for any of these.
  Additionally, the input filename can be for a directory.

  * Options can be specified with the global srcml_set_*(),
  srcml_clear_*(), and srcml_register_*()

  * Options can be queried with the global srcml_get_*() and
  srcml_check_*()
*/
LIBSRCML_DECL int srcml(const char* input_filename, const char* output_filename);

/*
  Global settings.  Can be used with convenience function srcml()
*/
LIBSRCML_DECL int srcml_set_src_encoding          (const char* encoding);
LIBSRCML_DECL int srcml_set_xml_encoding          (const char* encoding);
LIBSRCML_DECL int srcml_set_language              (const char* language);
LIBSRCML_DECL int srcml_set_filename              (const char* filename);
LIBSRCML_DECL int srcml_set_url                   (const char* url);
LIBSRCML_DECL int srcml_set_version               (const char* version);
LIBSRCML_DECL int srcml_set_timestamp             (const char* timestamp);
LIBSRCML_DECL int srcml_set_hash                  (const char* hash);
LIBSRCML_DECL int srcml_set_options               (unsigned long long option);
LIBSRCML_DECL int srcml_enable_option             (unsigned long long option);
LIBSRCML_DECL int srcml_disable_option            (unsigned long long option);
LIBSRCML_DECL int srcml_set_tabstop               (size_t tabstop);
LIBSRCML_DECL int srcml_register_file_extension   (const char* extension, const char* language);
LIBSRCML_DECL int srcml_register_namespace        (const char* prefix, const char* ns);
LIBSRCML_DECL int srcml_set_processing_instruction(const char* target, const char* data); 
LIBSRCML_DECL int srcml_register_macro            (const char* token, const char* type);
LIBSRCML_DECL int srcml_unparse_set_eol           (size_t eol);

LIBSRCML_DECL const char*        srcml_get_src_encoding ();
LIBSRCML_DECL const char*        srcml_get_xml_encoding ();
LIBSRCML_DECL const char*        srcml_get_revision ();
LIBSRCML_DECL const char*        srcml_get_language ();
LIBSRCML_DECL const char*        srcml_get_filename ();
LIBSRCML_DECL const char*        srcml_get_url();
LIBSRCML_DECL const char*        srcml_get_version  ();
LIBSRCML_DECL const char*        srcml_get_timestamp();
LIBSRCML_DECL const char*        srcml_get_hash();
LIBSRCML_DECL unsigned long long srcml_get_options  ();
LIBSRCML_DECL size_t             srcml_get_tabstop  ();
LIBSRCML_DECL const char*        srcml_get_processing_instruction_target(); 
LIBSRCML_DECL const char*        srcml_get_processing_instruction_data  (); 

/*
  XML namespaces
*/

/* Number of declared XML namespaces */
LIBSRCML_DECL size_t             srcml_get_namespace_size();

/* Prefix of the namespace at that position, where empty namespace is an empty string, and 0 is invalid position */
LIBSRCML_DECL const char*        srcml_get_namespace_prefix(size_t pos);

/* Prefix of the namespace with this namespace uri */
LIBSRCML_DECL const char*        srcml_get_prefix_from_uri(const char* namespace_uri);

/* URI of the namespace at that position, where 0 is invalid position */
LIBSRCML_DECL const char*        srcml_get_namespace_uri(size_t pos);

/* URI of the namespace with this namespace prefix */
LIBSRCML_DECL const char*        srcml_get_uri_from_prefix(const char* prefix);

LIBSRCML_DECL size_t             srcml_get_macro_list_size();
LIBSRCML_DECL const char*        srcml_get_macro_token(size_t pos);
LIBSRCML_DECL const char*        srcml_get_macro_token_type(const char* token);
LIBSRCML_DECL const char*        srcml_get_macro_type(size_t pos);

/* Source-code language is supported */
LIBSRCML_DECL int srcml_check_language(const char* language);

/* Query supported source-code languages */
LIBSRCML_DECL size_t srcml_get_language_list_size();
LIBSRCML_DECL const char* srcml_get_language_list(size_t pos);

/* Currently registered language for a file extension
   When full filename is given, the extension is extracted */
LIBSRCML_DECL const char* srcml_check_extension(const char* filename);

/* Particular encoding is supported, both for input and output */
LIBSRCML_DECL int srcml_check_encoding(const char* encoding);

/* Whether various features are available in this installation */
LIBSRCML_DECL int srcml_check_xslt();
LIBSRCML_DECL int srcml_check_exslt();

/* Description of last error */
LIBSRCML_DECL const char* srcml_error_string();

/*
  Full libsrcml API
*/

/* Currently registered language for a file extension
   When full filename is given, the extension is extracted */
LIBSRCML_DECL const char* srcml_archive_check_extension(struct srcml_archive* archive, const char* filename);

/* Create a new srcml archive
   Client is responsible for freeing memory using srcml_archive_free() */
LIBSRCML_DECL struct srcml_archive* srcml_archive_create();

/* Clone the setup of an existing archive
   Client is responsible for freeing memory using srcml_archive_free() */
LIBSRCML_DECL struct srcml_archive* srcml_archive_clone(const struct srcml_archive*);

/* Open a srcML archive for output */
LIBSRCML_DECL int srcml_archive_write_open_filename(struct srcml_archive*, const char* srcml_filename, unsigned short compression);
LIBSRCML_DECL int srcml_archive_write_open_memory  (struct srcml_archive*, char** buffer, size_t * size);
LIBSRCML_DECL int srcml_archive_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
LIBSRCML_DECL int srcml_archive_write_open_fd      (struct srcml_archive*, int srcml_fd);
LIBSRCML_DECL int srcml_archive_write_open_io      (struct srcml_archive*, void * context, int (*write_callback)(void * context, const char * buffer, size_t len), int (*close_callback)(void * context));

/* Setup options for srcml archive */
LIBSRCML_DECL int srcml_archive_set_xml_encoding       (struct srcml_archive*, const char* encoding);
LIBSRCML_DECL int srcml_archive_set_src_encoding       (struct srcml_archive*, const char* encoding);
LIBSRCML_DECL int srcml_archive_set_language           (struct srcml_archive*, const char* language);
LIBSRCML_DECL int srcml_archive_set_url                (struct srcml_archive*, const char* url);
LIBSRCML_DECL int srcml_archive_set_version            (struct srcml_archive*, const char* version);

LIBSRCML_DECL int srcml_archive_set_options               (struct srcml_archive*, unsigned long long option);
LIBSRCML_DECL int srcml_archive_enable_option             (struct srcml_archive*, unsigned long long option);
LIBSRCML_DECL int srcml_archive_disable_option            (struct srcml_archive*, unsigned long long option);
LIBSRCML_DECL int srcml_archive_set_tabstop               (struct srcml_archive*, size_t tabstop);
LIBSRCML_DECL int srcml_archive_register_file_extension   (struct srcml_archive*, const char* extension, const char* language);
LIBSRCML_DECL int srcml_archive_register_namespace        (struct srcml_archive*, const char* prefix, const char* uri);
LIBSRCML_DECL int srcml_archive_set_processing_instruction(struct srcml_archive*, const char* target, const char* data); 
LIBSRCML_DECL int srcml_archive_register_macro            (struct srcml_archive*, const char* token, const char* type);  

/* Query of the options for srcml archive */
LIBSRCML_DECL const char*        srcml_archive_get_xml_encoding                 (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_src_encoding                 (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_revision                     (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_language                     (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_url                          (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_version                      (const struct srcml_archive*);
LIBSRCML_DECL unsigned long long srcml_archive_get_options                      (const struct srcml_archive*);
LIBSRCML_DECL size_t             srcml_archive_get_tabstop                      (const struct srcml_archive*);
LIBSRCML_DECL size_t             srcml_archive_get_namespace_size               (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_namespace_prefix             (const struct srcml_archive*, size_t pos);
LIBSRCML_DECL const char*        srcml_archive_get_prefix_from_uri              (const struct srcml_archive*, const char* namespace_uri);
LIBSRCML_DECL const char*        srcml_archive_get_namespace_uri                (const struct srcml_archive*, size_t pos);
LIBSRCML_DECL const char*        srcml_archive_get_uri_from_prefix              (const struct srcml_archive*, const char* prefix);
LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_target(const struct srcml_archive*); 
LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_data  (const struct srcml_archive*); 
LIBSRCML_DECL size_t             srcml_archive_get_macro_list_size              (const struct srcml_archive*);
LIBSRCML_DECL const char*        srcml_archive_get_macro_token                  (const struct srcml_archive*, size_t pos);
LIBSRCML_DECL const char*        srcml_archive_get_macro_token_type             (const struct srcml_archive*, const char* token);
LIBSRCML_DECL const char*        srcml_archive_get_macro_type                   (const struct srcml_archive*, size_t pos);

/* Create a new srcml unit.
   Client is responsible for freeing memory using srcml_unit_free() */
LIBSRCML_DECL struct srcml_unit* srcml_unit_create(struct srcml_archive* archive);

/* Setup options for srcml unit */
LIBSRCML_DECL int srcml_unit_set_src_encoding (struct srcml_unit*, const char* language);
LIBSRCML_DECL int srcml_unit_set_language     (struct srcml_unit*, const char* language);
LIBSRCML_DECL int srcml_unit_set_filename     (struct srcml_unit*, const char* filename);
LIBSRCML_DECL int srcml_unit_set_url          (struct srcml_unit*, const char* url);
LIBSRCML_DECL int srcml_unit_set_version      (struct srcml_unit*, const char* version);
LIBSRCML_DECL int srcml_unit_set_timestamp    (struct srcml_unit*, const char* timestamp);
LIBSRCML_DECL int srcml_unit_set_hash         (struct srcml_unit*, const char* hash);
LIBSRCML_DECL int srcml_unit_unparse_set_eol  (struct srcml_unit*, size_t eol);

/* Convert to srcml.  Files/buffer can be compressed, but not a
   source archive format (e.g., not .tar) */
LIBSRCML_DECL int srcml_unit_parse_filename(struct srcml_unit* unit, const char* src_filename);
LIBSRCML_DECL int srcml_unit_parse_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
LIBSRCML_DECL int srcml_unit_parse_FILE    (struct srcml_unit*, FILE* src_file);
LIBSRCML_DECL int srcml_unit_parse_fd      (struct srcml_unit*, int src_fd);
LIBSRCML_DECL int srcml_unit_parse_io      (struct srcml_unit*, void * context, int (*read_callback)(void * context, char * buffer, size_t len), int (*close_callback)(void * context));

/* Append unit to an archive */
LIBSRCML_DECL int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);

/* Iteratively build a unit */
LIBSRCML_DECL int srcml_write_start_unit(struct srcml_unit*);
LIBSRCML_DECL int srcml_write_end_unit(struct srcml_unit*);
LIBSRCML_DECL int srcml_write_start_element(struct srcml_unit*, const char * prefix, const char * name, const char * uri);
LIBSRCML_DECL int srcml_write_end_element(struct srcml_unit*);
LIBSRCML_DECL int srcml_write_namespace(struct srcml_unit*, const char * prefix, const char * uri);
LIBSRCML_DECL int srcml_write_attribute(struct srcml_unit*, const char * prefix, const char * name, const char * uri, const char * content);
LIBSRCML_DECL int srcml_write_string(struct srcml_unit*, const char * content);
 
/* Free allocated unit */
LIBSRCML_DECL void srcml_unit_free(struct srcml_unit*);

/* Close the srcML archive */
LIBSRCML_DECL void srcml_archive_close(struct srcml_archive*);

/* Free the srcML archive data */
LIBSRCML_DECL void srcml_archive_free(struct srcml_archive*);

/* Open a srcML archive for reading */
LIBSRCML_DECL int srcml_archive_read_open_filename(struct srcml_archive*, const char* srcml_filename);
LIBSRCML_DECL int srcml_archive_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
LIBSRCML_DECL int srcml_archive_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
LIBSRCML_DECL int srcml_archive_read_open_fd      (struct srcml_archive*, int srcml_fd);
LIBSRCML_DECL int srcml_archive_read_open_io      (struct srcml_archive*, void * context, int (*read_callback)(void * context, char * buffer, size_t len), int (*close_callback)(void * context));

/* Read the next unit from the archive
   Returns 0 if there are no more units */
LIBSRCML_DECL struct srcml_unit* srcml_read_unit_header(struct srcml_archive*);
LIBSRCML_DECL struct srcml_unit* srcml_read_unit_xml(struct srcml_archive*);
LIBSRCML_DECL struct srcml_unit* srcml_read_unit(struct srcml_archive*);

/* srcDiff processing */
LIBSRCML_DECL struct srcml_unit* srcml_read_unit_revision(struct srcml_archive*, size_t revision_number);

/* Query options of srcml unit */
LIBSRCML_DECL const char* srcml_unit_get_src_encoding  (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_revision      (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_language      (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_filename      (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_url           (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_version       (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_timestamp     (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_hash          (const struct srcml_unit*);
LIBSRCML_DECL const char* srcml_unit_get_xml_fragment  (struct srcml_unit*);
LIBSRCML_DECL int srcml_unit_get_xml_standalone(struct srcml_unit*, const char* xml_encoding, char** xml_buffer, size_t* buffer_size);

/* Convert from srcML to source code */
LIBSRCML_DECL int srcml_unit_unparse_filename(struct srcml_unit*, const char* src_filename, unsigned short compression);
LIBSRCML_DECL int srcml_unit_unparse_memory  (struct srcml_unit*, char** src_buffer, size_t * src_size);
LIBSRCML_DECL int srcml_unit_unparse_FILE    (struct srcml_unit*, FILE* srcml_file);
LIBSRCML_DECL int srcml_unit_unparse_fd      (struct srcml_unit*, int srcml_fd);
LIBSRCML_DECL int srcml_unit_unparse_io      (struct srcml_unit*, void * context, int (*write_callback)(void * context, const char * buffer, size_t len), int (*close_callback)(void * context));

/* Free memmory buffer */
LIBSRCML_DECL void srcml_memory_free(char * buffer);

/* srcML XPath query and XSLT transform functions */
LIBSRCML_DECL int srcml_clear_transforms                 (struct srcml_archive*);
LIBSRCML_DECL int srcml_append_transform_xpath           (struct srcml_archive*, const char* xpath_string);
LIBSRCML_DECL int srcml_append_transform_xpath_attribute (struct srcml_archive*, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* attr_name, const char* attr_value);
LIBSRCML_DECL int srcml_append_transform_xpath_element   (struct srcml_archive*, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element,
                                                            const char* attr_prefix, const char* attr_namespace_uri,
                                                            const char* attr_name, const char* attr_value);

#ifdef WITH_LIBXSLT
LIBSRCML_DECL int srcml_append_transform_xslt_filename   (struct srcml_archive*, const char* xslt_filename);
LIBSRCML_DECL int srcml_append_transform_xslt_memory     (struct srcml_archive*, const char* xslt_buffer, size_t size);
LIBSRCML_DECL int srcml_append_transform_xslt_FILE       (struct srcml_archive*, FILE* xslt_file);
LIBSRCML_DECL int srcml_append_transform_xslt_fd         (struct srcml_archive*, int xslt_fd);
#endif
LIBSRCML_DECL int srcml_append_transform_relaxng_filename(struct srcml_archive*, const char* relaxng_filename);
LIBSRCML_DECL int srcml_append_transform_relaxng_memory  (struct srcml_archive*, const char* relaxng_buffer, size_t size);
LIBSRCML_DECL int srcml_append_transform_relaxng_FILE    (struct srcml_archive*, FILE* relaxng_file);
LIBSRCML_DECL int srcml_append_transform_relaxng_fd      (struct srcml_archive*, int relaxng_fd);
LIBSRCML_DECL int srcml_append_transform_param           (struct srcml_archive*, const char* param_name, const char* param_value);
LIBSRCML_DECL int srcml_append_transform_stringparam     (struct srcml_archive*, const char* param_name, const char* param_value);
LIBSRCML_DECL int srcml_apply_transforms                 (struct srcml_archive* iarchive, struct srcml_archive* oarchive);

/* Augment read to use user defined xml handlers */
/*
LIBSRCML_DECL int srcml_set_xml_handler_start_unit(struct srcml_archive*);
LIBSRCML_DECL int srcml_set_xml_handler_start_element(struct srcml_archive*);
LIBSRCML_DECL int srcml_set_xml_handler_characters(struct srcml_archive*);
LIBSRCML_DECL int srcml_set_xml_handler_cdata(struct srcml_archive*);
LIBSRCML_DECL int srcml_set_xml_handler_comment(struct srcml_archive*);
LIBSRCML_DECL int srcml_set_xml_handler_end_element(struct srcml_archive*);
LIBSRCML_DECL int srcml_set_xml_handler_end_unit(struct srcml_archive*);

LIBSRCML_DECL int srcml_xml_handler_parse_archive(struct srcml_archive*);
LIBSRCML_DECL int srcml_xml_handler_release(struct srcml_archive*);
LIBSRCML_DECL int srcml_xml_handler_stop(struct srcml_archive*);
*/

#ifdef __cplusplus
}
#endif

#endif
