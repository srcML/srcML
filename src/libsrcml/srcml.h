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
 * @mainpage libsrcml
 * 
 * The srcml functions available in libsrcml are for the purposes of:
 *
 * * Converting source code to the srcML format
 * * Converting the srcML format back to source code
 * * Querying and transformation in the srcML format
 * 
 *
 * The API is broken up into the following sections:
 *    - @subpage macros : Options, status codes, and global values
 *    - @subpage utility : Checking library support and freeing memory
 *    - @subpage convenience : The simplistic single source-code file to srcML file conversion
 *    - @subpage archive : Operations on and involving core data structure of srcML
 *    - @subpage unit : Operations on and involving the building blocks of an archive
 *    - @subpage srcDiff : A syntactic differencing tool integrated into srcML
 */


#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

#include <stddef.h> /* size_t */
#include <stdio.h> /* FILE * */

#ifdef __cplusplus
extern "C" {
#endif

/* Function export macro */
#if defined(WIN32) && !defined(__MINGW32__)
#define LIBSRCML_DECL __declspec(dllexport)
#else
#define LIBSRCML_DECL
#endif


/** @defgroup macros Macros
    @{
*/
/**@{ @name Version */
/** Number representing libsrcml version */
#define SRCML_VERSION_NUMBER 9005
/** String containing libsrcml version */
#define SRCML_VERSION_STRING "0.9.5"
/**@}*/

/**@{ @name Status */
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
/**@}*/


/**@{ @name Core language set */
/** Language not set */
#define SRCML_LANGUAGE_NONE   0
/** Language C */
#define SRCML_LANGUAGE_C      "C"
/** Language C++ */
#define SRCML_LANGUAGE_CXX    "C++"
/** Language Java */
#define SRCML_LANGUAGE_JAVA   "Java"
/** Language C# */
#define SRCML_LANGUAGE_CSHARP "C#"
/** Language XML */
#define SRCML_LANGUAGE_XML    "xml"
/**@}*/ 


/**@{ @name Options */
/** Create an archive */
#define SRCML_OPTION_ARCHIVE           1<<0
/** Include line/column position attributes */
#define SRCML_OPTION_POSITION          1<<1
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
/** Expression mode */
#define SRCML_OPTION_EXPRESSION        1<<14
/** Extra processing of @code#line@endcode for position information */
#define SRCML_OPTION_LINE              1<<15
/** Additional cpp:if/cpp:endif checking */
#define SRCML_OPTION_CPPIF_CHECK       1<<16
/** Debug time attribute */
#define SRCML_OPTION_DEBUG_TIMER       1<<17
/** Turn on old optional markup behaviour */
#define SRCML_OPTION_OPTIONAL_MARKUP   1<<20
/** Parser output special tokens for debugging the parser */
#define SRCML_OPTION_DEBUG             1<<24
/** Markups OpenMP in special namespace */
#define SRCML_OPTION_OPENMP            1<<25
/** Encode the original source encoding as an attribute */
#define SRCML_OPTION_STORE_ENCODING    1<<26
/** All default enabled options */
#define SRCML_OPTION_DEFAULT (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_HASH | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY)
/**@}*/


/**@{ @name Unparse Options */
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
/**@}*/ 
/**@}*/
/**@}*/


/**
 * @struct srcml_archive
 * @brief Encapsulates multiple srcML units into one srcML archive
 */
struct srcml_archive;

/**
 * @struct srcml_unit
 * @brief The srcML markup of one source-code file
 */
struct srcml_unit;

/** @defgroup utility Utility functions
    @{
 */
/**
 * @brief Gets the current version of the library
 * @return Version of libsrcml as a number
 */
LIBSRCML_DECL int srcml_version_number();

/**
 * @brief Gets the current version of the library
 * @return Version of libsrcml as a string
 */
LIBSRCML_DECL const char* srcml_version_string();

/**
 * @brief Checks if a source-code language is supported
 * @param language The language to check support for
 * @retval 0 If the language is not supported
 * @retval pos The numeric representation for that language if supported
 */
LIBSRCML_DECL int srcml_check_language(const char* language);

/**
 * @brief Gets the number of supported source-code languages
 * @return The number of source-code languages supported
 */
LIBSRCML_DECL size_t srcml_get_language_list_size();

/**
 * @brief Gets the name of the supported language at a given position
 * @param pos The position of the language in the supported language list
 * @return The name of the supported source-code language on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_language_list(size_t pos);

/**
 * @brief Check the current registered language for a file extension
 * @param filename The name of a file. When a full filename is given, the extension is extracted
 * @return The language name registered with that extension on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_check_extension(const char* filename);

/**
 * @brief Check if a particular encoding is supported for input and output
 * @param encoding The name of the encoding
 * @retval 0 if the encoding is not supported
 * @retval nonzero if the encoding is supported
 */
LIBSRCML_DECL int srcml_check_encoding(const char* encoding);

/**
 * @brief Check if XSLT is available
 * @retval 1 if XSLT is available
 * @retval 0 if it is unavailable
 */
LIBSRCML_DECL int srcml_check_xslt();

/**
 * @brief Check if EXSLT is available
 * @retval 1 if EXSLT is available
 * @retval 0 if it is unavailable
 */
LIBSRCML_DECL int srcml_check_exslt();

/**
 * @brief Provides a description of the last error to occur
 * @return A string describing last recorded error
 */
LIBSRCML_DECL const char* srcml_error_string();

/**
 * @brief Free a memory buffer allocated by functions such as srcml_archive_write_open_memory
 * @param buffer The allocated buffer
 */
LIBSRCML_DECL void srcml_memory_free(char * buffer);
/**@}*/



/** @defgroup convenience Convenience functions

  Convenience functions for translating to and from the srcML format

  * Options can be specified with the global srcml_set_*(),
  srcml_clear_*(), and srcml_register_*()

  * Options can be queried with the global srcml_get_*() and
  srcml_check_*()

  @{
 */
/**
 * @brief Translate to and from the srcML format
 * @details Translates from source code to srcML if the input_filename
 * extension is for source code, e.g., .c, .cpp, .java Language
 * determined by file extension if language is not set with
 * srcml_set_language(). Translates from srcML to source code if the
 * input_filename extension is '.xml'
 * @param input_filename The name of a source-code file or srcML file
 * @param output_filename The name of the output srcML file or source-code file
 * @return SRCML_STATUS_OK on success
 * @return Status error on failure
 */
LIBSRCML_DECL int srcml(const char* input_filename, const char* output_filename);

/**@{ @name Global settings
      @brief Can be used with the convenience function srcml()
*/
/**
 * @brief Set the source encoding
 * @param encoding An output encoding
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_src_encoding          (const char* encoding);

/**
 * @brief Set the xml encoding
 * @param encoding An output encoding
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_xml_encoding          (const char* encoding);

/**
 * @brief Set the language used to parse
 * @param language A supported source-code language
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_language              (const char* language);

/**
 * @brief Set the filename attribute for the root unit
 * @param filename Name of a file
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_filename              (const char* filename);

/**
 * @brief Set the url attribute for the root unit
 * @param url a url path
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_url                   (const char* url);

/**
 * @brief Set the version attribute for the root unit
 * @param version A version string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_version               (const char* version);

/**
 * @brief Set the timestamp attribute for the root unit
 * @param timestamp A timestamp string in any format
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_timestamp             (const char* timestamp);

/**
 * @brief Set the hash attribute for the root unit
 * @param hash A unique hash value
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_hash                  (const char* hash);

/**
 * @brief Set options on the unit, clearing all previously set options
 * @param option A srcML option
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_options               (unsigned long long option);

/**
 * @brief Enable (set) an option or options
 * @param option The srcML option(s)
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_enable_option             (unsigned long long option);

/**
 * @brief Remove an option or options
 * @param option The srcML option(s)
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_disable_option            (unsigned long long option);

/**
 * @brief Set the size of the tabstop
 * @param tabstop Tabstop size
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_set_tabstop               (size_t tabstop);

/**
 * @brief Associate an extension with a supported source-code language
 * @param extension A source file extension
 * @param language A supported source code language
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_register_file_extension   (const char* extension, const char* language);

/**
 * @brief Add a new namespace or change the prefix of an existing namespace.
 * @param prefix An XML namespace prefix
 * @param ns An XML namespace
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_register_namespace        (const char* prefix, const char* ns);

/**
 * @brief Set a processing instruction that will be output before the root
 * element of an archive
 * @param target the processing instruction's target
 * @param data the processing instruciton's data
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_set_processing_instruction(const char* target, const char* data); 

/**
 * @brief Register a macro (token) to be processed as a special type
 * @param token Name of macro
 * @param type Macro type
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_register_macro            (const char* token, const char* type);

/**
 * @brief Set the end of line characters to be used for unparse
 * @param eol The kind of eol to use for unparse
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unparse_set_eol           (size_t eol);

/**
 * @brief Set what revision in a srcDiff document to operate with
 * @param revision_number The revision to operate with
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_set_srcdiff_revision      (size_t revision_number);

/**
 * @return The source encoding on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_src_encoding ();

/**
 * @return The XML encoding on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_xml_encoding ();

/**
 * @return The srcML revision attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_revision ();

/**
 * @return The language attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_language ();

/**
 * @return The filename attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_filename ();

/**
 * @return The url attribute for the root unit on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_url();

/**
 * @return The versiot attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_version  ();

/**
 * @return The timestamp attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_timestamp();

/**
 * @return The hash attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_hash();

/**
 * @return The currently set options on success
 * @return NULL on failure
 */
LIBSRCML_DECL unsigned long long srcml_get_options  ();

/**
 * @return The tabstop size on success
 * @return NULL on failure
 */
LIBSRCML_DECL size_t             srcml_get_tabstop  ();

/**
 * @return The processing instruction target
 */
LIBSRCML_DECL const char*        srcml_get_processing_instruction_target();

/**
 * @return The processing instruction data
 */
LIBSRCML_DECL const char*        srcml_get_processing_instruction_data  ();
/**@}*/


/**@{ @name XML Namespaces */
/**
 * @return Number of declared XML namespaces
 */
LIBSRCML_DECL size_t             srcml_get_namespace_size();

/**
 * @brief Get the prefix of the namespace at that position
 * @param pos The position to get the namespace prefix at
 * @return The prefix, where empty namespace is an empty string
 * @return 0 if given an invalid position
 */
LIBSRCML_DECL const char*        srcml_get_namespace_prefix(size_t pos);

/**
 * @brief Get the registered prefix for the given namespace
 * @param namespace_uri An XML namespace
 * @return The registered prefix for the given namespace
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_prefix_from_uri(const char* namespace_uri);

/**
 * @param pos position in namespaces
 * @return The namespace URI at that position on succcess
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_namespace_uri(size_t pos);

/**
 * @param prefix An XML prefix
 * @return The first namespace URI for the given prefix on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_uri_from_prefix(const char* prefix);
/**@}*/


/**@{ @name Macro Handling */
/**
 * @return The number of currently defined macros.
 */
LIBSRCML_DECL size_t             srcml_get_macro_list_size();

/**
 * @param pos Position in macr list
 * @return The registered token at the given pos on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_macro_token(size_t pos);

/**
 * @param token A macro token
 * @return The registered type for the given token on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_macro_token_type(const char* token);

/**
 * @param pos Position in macro list
 * @return The type at the given pos on succcess
 * @return NULL on failure
 */
LIBSRCML_DECL const char*        srcml_get_macro_type(size_t pos);
/**@}*/

/**
 * @brief Cleanup and free globally allocated items (usually by libxml2)
 */
LIBSRCML_DECL void srcml_cleanup_globals();
/**@}*/



/** @defgroup archive Archive
    @{
*/
/**
 * @brief Create a new srcml archive.
 * @note Archive must be freed using srcml_archive_free()
 * @return The created srcml_archive
 */
LIBSRCML_DECL struct srcml_archive* srcml_archive_create();

/**
 * @brief Clone the setup of an existing archive
 * @note Archive must be freed using srcml_archive_free()
 * @param srcml_archive A srcml_archive
 * @return The cloned archive
 */
LIBSRCML_DECL struct srcml_archive* srcml_archive_clone(const struct srcml_archive*);

/**
 * @brief Append the srcml_unit unit to the srcml_archive archive
 * If copying from a read and only the attributes have been read
 * read in the xml and output.
 * @param archive A srcml_archive opened for writing
 * @param unit A srcml_unit to output
 * @note Can not mix with by element mode.
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);

/**
 * @brief Close a srcml_archive opened using srcml_archive_read_open_* or srcml_archive_write_open_*.
 * The archive can be reopened.
 * @param archive An open srcml_archive
 */
LIBSRCML_DECL void srcml_archive_close(struct srcml_archive*);

/**
 * @brief Free a srcml archive that was previously allocated
 * by using srcml_archive_create(). The archive must be reallocated/re-created to use again.
 * @param archive A srcml_archive
 */
LIBSRCML_DECL void srcml_archive_free(struct srcml_archive*);

/**@{ @name Write
      @brief Open a srcML archive for output */
/**
 * @brief Open up a srcml_archive for writing to a given output file
 * @param archive A srcml_archive
 * @param srcml_filename Name of an output file
 * @param compression Amount of compression (0 for none to 9 as the max)
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_filename(struct srcml_archive*, const char* srcml_filename, unsigned short compression);

/**
 * @brief Open up a srcml_archive for writing to a given memory buffer
 * @param archive A srcml_archive
 * @param buffer Location to return output string. The buffer is allocated
 * and set to the location the buffer points at. The buffer must be freed after use
 * @param size the size of the resulting buffer
 * @note The buffer must be freed after use
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_memory  (struct srcml_archive*, char** buffer, size_t * size);

/**
 * @brief Open up a srcml_archive for writing to a given FILE pointer
 * @param archive A srcml_archive
 * @param srcml_file FILE opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);

/**
 * @brief Open up a srcml_archive for writing to a file descriptor
 * @param archive A srcml_archive
 * @param srcml_fd Output file descriptor
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_fd      (struct srcml_archive*, int srcml_fd);

/**
 * @brief Open up a srcml_archive for writing to an io context using writeand close callbacks
 * @param archive A srcml_archive
 * @param context An io context
 * @param write_callback A write callback function
 * @param close_callback A close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_io      (struct srcml_archive*, void * context, int (*write_callback)(void * context, const char * buffer, size_t len), int (*close_callback)(void * context));
/**@}*/


/**@{ @name Read
      @brief Open a srcML archive for reading */
/**
 * @brief Open a srcML archive for reading from a filename
 * @param archive A srcml_archive
 * @param srcml_filename Name of an input file
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_read_open_filename(struct srcml_archive*, const char* srcml_filename);

/**
 * @brief Open a srcML archive for reading from a buffer up until a buffer_size
 * @param archive A srcml_archive
 * @param buffer An input buffer
 * @param buffer_size Size of the input buffer
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);

/**
 * @brief Open a srcML archive for reading from a FILE
 * @param archive A srcml_archive
 * @param srcml_file A FILE opened for reading
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);

/**
 * @brief Open a srcML archive for reading from a file descriptor
 * @param archive A srcml_archive
 * @param srcml_fd A file descriptor opened for reading
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_read_open_fd      (struct srcml_archive*, int srcml_fd);

/**
 * @brief Open a srcML archive for reading from the opened context, accessed via read and close callbacks
 * @param archive A srcml_archive
 * @param context An io context
 * @param read_callback A read callback function
 * @param close_callback A close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_read_open_io      (struct srcml_archive*, void * context, int (*read_callback)(void * context, char * buffer, size_t len), int (*close_callback)(void * context));
/**@}*/


/**@{ @name Setup options */
/**
 * @brief Set the XML encoding of the srcML archive
 * @param archive The srcml_archive to set the encoding
 * @param encoding The encoding of the archive
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_xml_encoding       (struct srcml_archive*, const char* encoding);

/**
 * @brief Set the default source encoding for the srcML archive 
 * @param archive The srcml_archive to set the source encoding for
 * @param encoding A source-code encoding
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_src_encoding       (struct srcml_archive*, const char* encoding);

/**
 * @brief Set the language of the srcML archive
 * @param archive A srcml_archive to set the source-code language on
 * @param language A source-code language
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_language           (struct srcml_archive*, const char* language);

/**
 * @brief Set the root URL attribute of the srcML archive
 * @param archive A srcml_archive to set the root URL attribute on
 * @param url A url path
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_url                (struct srcml_archive*, const char* url);

/**
 * @brief Set the root version attribute of the srcML archive
 * @param archive A srcml_archive to set the root version attribute on
 * @param version A version string
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_version            (struct srcml_archive*, const char* version);

/**
 * @brief Set all options for processing an archive, erasing all previously set options
 * @param archive The srcml_archive to set the options for
 * @param option A set of srcml options
 * @note Erases all previously set options
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_options               (struct srcml_archive*, unsigned long long option);

/**
 * @brief Enable/set an option or options on an archive
 * @param archive A srcml_archive to enable options on
 * @param option An option, or multiple options by |ing each, to set on the archive
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_enable_option             (struct srcml_archive*, unsigned long long option);

/**
 * @brief Remove an option or options from an archive
 * @param archive A srcml_archive to remove options from
 * @param option The option, or multiple options by |ing each, to clear from the archive
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_disable_option            (struct srcml_archive*, unsigned long long option);

/**
 * @brief Set the tabstop size for position and column calculation
 * @param archive A srcml_archive
 * @param tabstop Size of a tabstop
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_archive_set_tabstop               (struct srcml_archive*, size_t tabstop);

/**
 * @brief Set an extension to be associated with a given source-code language
 * @param archive A srcml_archive that associates the given extension with a language
 * @param extension A file extension
 * @param language A supported source-code language
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_register_file_extension   (struct srcml_archive*, const char* extension, const char* language);

/**
 * @brief Create a new namespace or change the prefix of an existing namespace
 * @param archive A srcml_archive
 * @param prefix An XML namespace prefix
 * @param uri An XML namespace uri
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_register_namespace        (struct srcml_archive*, const char* prefix, const char* uri);

/**
 * @brief Set a processing instruction that will be output before the root element of the archive
 * @param archive A srcml_archive
 * @param target The processing instruction's target
 * @param data The processing instruciton's data
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_set_processing_instruction(struct srcml_archive*, const char* target, const char* data); 

/**
 * @brief Register a macro (token) to be processed as a special type.
 * @details Here is a list of the currently supported special types:
 * - src:macro     -> Treat the token as a standalone macro (will be marked with a macro tag)
 * - src:type      -> Treat the token as an identifier (will still be marked as macro tag)
 * - src:name      -> Treat the token as an identifier (will still be marked as macro tag)
 * - src:specifier -> Treat the token as a specifier (will be marked with a specifier tag around macro tag)
 * - src:label     -> Treat the token as a label (a goto label, will be marked with a label tag around macro tag)
 * - src:case      -> Treat as a case label (mark with case tag) either case keyword or case keyword and label
 * @param archive A srcml_archive
 * @param token Name of macro
 * @param type Macro type
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_register_macro            (struct srcml_archive*, const char* token, const char* type);

/**
 * @param archive A srcml_archive
 * @return The currently set XML encoding, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_xml_encoding                 (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently default source encoding, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_src_encoding                 (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently set revision, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_revision                     (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently set language, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_language                     (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently set root url attribute, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_url                          (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently set root version attribute, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_version                      (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently set options
 */
LIBSRCML_DECL unsigned long long srcml_archive_get_options                      (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The currently set tabstop size
 */
LIBSRCML_DECL size_t             srcml_archive_get_tabstop                      (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @return The number of currently defined namespaces or 0 if archive is NULL
 */
LIBSRCML_DECL size_t             srcml_archive_get_namespace_size               (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @param pos The namespace position
 * @return The prefix for the given position, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_namespace_prefix             (const struct srcml_archive*, size_t pos);

/**
 * @param archive A srcml_archive
 * @param namespace_uri An XML namespace URI
 * @return The registered prefix for the given namespace, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_prefix_from_uri              (const struct srcml_archive*, const char* namespace_uri);

/**
 * @param archive A srcml_archive
 * @param pos The namespace position
 * @return The namespace at the given position, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_namespace_uri                (const struct srcml_archive*, size_t pos);

/**
 * @param archive A srcml_archive
 * @param prefix An XML prefix
 * @return The first namespace for the given prefix on success, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_uri_from_prefix              (const struct srcml_archive*, const char* prefix);

/**
 * @param archive A srcml archive
 * @return The processing instruction target
 */
LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_target(const struct srcml_archive*); 

/**
 * @param archive A srcml archive
 * @return The processing instruction data
 */
LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_data  (const struct srcml_archive*); 

/**
 * @param archive A srcml_archive
 * @return The number of currently defined macros, or 0 if archive is NULL
 */
LIBSRCML_DECL size_t             srcml_archive_get_macro_list_size              (const struct srcml_archive*);

/**
 * @param archive A srcml_archive
 * @param pos A macro position
 * @return Token for the given position, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_macro_token                  (const struct srcml_archive*, size_t pos);

/**
 * @param archive A srcml_archive
 * @param token A macro token
 *
 * @returns The registered type for the given token, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_macro_token_type             (const struct srcml_archive*, const char* token);

/**
 * @param archive A srcml_archive
 * @param pos Position in macro list
 *
 * @returns The type at the given pos on succcess, or NULL
 */
LIBSRCML_DECL const char*        srcml_archive_get_macro_type                   (const struct srcml_archive*, size_t pos);

/**
 * @brief Retrieve the currently registered language for a file extension
 * @param archive A srcml_archive
 * @param filename Name of a file. Given the full filename, the extension is extracted
 * @return The language for extension, or if 0 if no language.
 */
LIBSRCML_DECL const char* srcml_archive_check_extension(const struct srcml_archive* archive, const char* filename);
/**@}*/


/**@{ @name Read archive's next unit
*/
/**
 * @brief Read the start unit tag header from the next archive 
 * @param archive A srcml_archive open for reading
 * @return The read srcml_unit, with header information only, on success
 * @return NULL on failure
 */
LIBSRCML_DECL struct srcml_unit* srcml_archive_read_unit_header(struct srcml_archive*);

/**
 * @brief Read the body of the unit from the archive (if not done already)
 * @note This is rarely needed as most functionality can be obtained by srcml_archive_read_unit_header()
 * @param unit The srcml_unit to read
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_read_body(struct srcml_unit*);
/**@}*/


/**@{ @name XPath query and XSLT transformations */
/**
 * @brief Remove all appended transformations from the archive which have not been applied yet
 * @param archive A srcml_archive
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_clear_transforms                 (struct srcml_archive*);

/**
 * @brief Append the XPath expression to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xpath_string An XPath expression
 * @note Currently, there is no way to specify context to the expression.
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_xpath           (struct srcml_archive*, const char* xpath_string);

/**
 * @brief Append the XPath expression to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * archive marking the XPath results with a user provided attribute
 * @param archive A srcml_archive
 * @param xpath_string An XPath expression
 * @param prefix Attribute prefix
 * @param namespace_uri Attribute namespace
 * @param attr_name Attribute name
 * @param attr_value Attribute value
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_xpath_attribute (struct srcml_archive*, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* attr_name, const char* attr_value);

/**
 * @brief Append the XPath expression to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * archive marking the XPath results with a user provided element.
 * @param archive A srcml_archive
 * @param xpath_string An XPath expression
 * @param prefix Element prefix
 * @param namespace_uri Element namespace
 * @param element Element name
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_xpath_element   (struct srcml_archive*, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element);

/**
 * @brief Append the XPath expression to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * archive marking the XPath results with a user provided element and attribute
 * @param archive A srcml_archive
 * @param xpath_string An XPath expression
 * @param prefix Element prefix
 * @param namespace_uri Element namespace
 * @param element Element name
 * @param attr_prefix An optional attribute prefix for the element
 * @param attr_namespace_uri An optional attribute namespace for the element
 * @param attr_name An optional attribute name for the element
 * @param attr_value An optional attribute value for the element
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_xpath_element_attribute  (struct srcml_archive*, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element,
                                                            const char* attr_prefix, const char* attr_namespace_uri,
                                                            const char* attr_name, const char* attr_value);
#ifdef WITH_LIBXSLT
/**
 * @brief Append an XSLT program at the designated filename path to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xslt_filename An XSLT program filename path
 * @note Currently no way to specify parameters or context
 * @return SRCML_STATUS_OK on success 
 * @return Status error codes on failure.
 */
LIBSRCML_DECL int srcml_append_transform_xslt_filename   (struct srcml_archive*, const char* xslt_filename);

/**
 * @brief Append an XSLT program from a buffer to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xslt_buffer A buffer holding an XSLT
 * @param size Size of the given buffer
 * @note Currently no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure
 */
LIBSRCML_DECL int srcml_append_transform_xslt_memory     (struct srcml_archive*, const char* xslt_buffer, size_t size);

/**
 * @brief Append an XSLT program in a FILE to the list of transformations/queries.
 * @param archive A srcml_archive
 * @param xslt_file A FILE containing an XSLT program
 * @note Currently no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure
 */
LIBSRCML_DECL int srcml_append_transform_xslt_FILE       (struct srcml_archive*, FILE* xslt_file);

/**
 * @brief Append an XSLT program from a file descriptor to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xslt_fd A file descriptor containing an XSLT program
 * @note Currently no way to specify parameters or context
 * Append the XSLT program in fd to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure
 */
LIBSRCML_DECL int srcml_append_transform_xslt_fd         (struct srcml_archive*, int xslt_fd);
#endif

/**
 * @brief Append the RelaxNG schema from a filename path to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_filename A RelaxNG schema filename path
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_filename(struct srcml_archive*, const char* relaxng_filename);

/**
 * @brief Append the RelaxNG schema in the buffer to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_buffer A buffer holding a RelaxNG schema
 * @param size Size of the passed buffer
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_memory  (struct srcml_archive*, const char* relaxng_buffer, size_t size);

/**
 * @brief Append the RelaxNG schema in a FILE to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_file A FILE containing a RelaxNG schema
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_FILE    (struct srcml_archive*, FILE* relaxng_file);

/**
 * @brief Append the RelaxNG schema in a file descriptor to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_fd A file descriptor containing a RelaxNG schema
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_fd      (struct srcml_archive*, int relaxng_fd);

/**
 * @brief Append an XSLT parameter to the last transformation
 * @param archive A srcml_archive
 * @param param_name Name of a parameter
 * @param param_value Value of the parameter
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_param           (struct srcml_archive*, const char* param_name, const char* param_value);

/**
 * @brief Append a string XSLT parameter to the last transformation, with the value wrapped in quotes
 * @param archive A srcml_archive
 * @param param_name Name of a parameter
 * @param param_value Value of the named parameter wrapped in quotes (")
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_stringparam     (struct srcml_archive*, const char* param_name, const char* param_value);

/**
 * @brief Apply all appended transformations/queries in the order that they were added, consecutaviely
 * Intermediate results are stored in a temporary file and transformations are cleared.
 * @param iarchive An input srcml_archive
 * @param oarchive An output srcml archive with the applied transformations/queries
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_apply_transforms                 (struct srcml_archive* iarchive, struct srcml_archive* oarchive);
/**@}*/
/**@}*/



/** @defgroup unit Unit
    @{
*/
/**
 * @brief Create a new srcml_unit tied to the srcml archive
 * @param archive A srcml_archive
 * @note The unit must be freed using srcml_unit_free()
 * @returns Created srcml_unit on success
 * @return NULL on failure
 */
LIBSRCML_DECL struct srcml_unit* srcml_unit_create(struct srcml_archive* archive);

/**
 * @brief Free an allocated unit
 * @param srcml_unit The srcml unit to free
 */
LIBSRCML_DECL void srcml_unit_free(struct srcml_unit*);


/**@{ @name Setup options */
/**
 * @brief Set the source-code encoding for the srcml unit
 * @param unit A srcml_unit
 * @param encoding A source-code encoding
 * @returns SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_set_src_encoding (struct srcml_unit*, const char* language);

/**
 * @brief Set the source-code language for the srcml unit
 * @param unit A srcml_unit
 * @param language A supported source-code language
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_set_language     (struct srcml_unit*, const char* language);

/**
 * @brief Set the filename attribute for the srcml unit
 * @param unit A srcml_unit
 * @param filename The name of a file
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_set_filename     (struct srcml_unit*, const char* filename);

/**
 * @brief Set the version attribute for the srcml unit
 * @param unit A srcml_unit
 * @param version A version string
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_set_version      (struct srcml_unit*, const char* version);

/**
 * @brief Set the timestamp attribute for the srcml unit
 * @param unit A srcml_unit
 * @param timestamp A timestamp string
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_set_timestamp    (struct srcml_unit*, const char* timestamp);

/**
 * @brief Set the hash attribute for the srcml unit
 * @param unit A srcml_unit
 * @param hash A hash string
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_set_hash         (struct srcml_unit*, const char* hash);

/**
 * @brief Set the type of end of line to be used for unparse
 * @param unit A srcml_unit
 * @param eol The kind of eol to use for unparse
 * @return SRCML_STATUS_OK on success
 * @return SRCML_STATUS_INVALID_ARGUMENT on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_set_eol  (struct srcml_unit*, size_t eol);

/**
 * @param unit A srcml_unit
 * @return The source-code encoding for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_src_encoding  (const struct srcml_unit*);

/**
 * @param unit A srcml_unit
 * @return The revision for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_revision      (const struct srcml_unit*);

/**
 * @param unit A srcml_unit
 * @return The source-code language for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_language      (const struct srcml_unit*);

/**
 * @param unit A srcml_unit
 * @return The filename attribute on the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_filename      (const struct srcml_unit*);

/**
 * @param unit A srcml_unit
 * @return The version for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_version       (const struct srcml_unit*);

/**
 * @param unit A srcml_unit
 * @return The timestamp attribute on the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_timestamp     (const struct srcml_unit*);

/**
 * @param unit A srcml_unit
 * @return The hash attribute on the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_hash          (const struct srcml_unit*);

/**
 * @brief Get a raw versio of the parsed or collected srcml from an archive.
 * If only the attributes were collected from a read,
 * then the XML is read in and that value is returned. The XML fragment returned
 * is the raw UTF-8 encoded XML stored internally and is not completely XML, so do not free.
 * @note Do not free
 * @param unit A srcml_unit
 * @return The raw unit srcML on success and NULL on failure.
 */
LIBSRCML_DECL const char* srcml_unit_get_xml_fragment  (struct srcml_unit*);

/**
 * @brief Get a formatted version of the the parsed or collected srcml from an archive.
 * If only the attributes were collected from a read,
 * then the XML is read in and that value is returned. The XML returned
 * is formatted version of the internally stored xml after
 * applying encoding, and appending of namespaces.  It is a complete standalone XML.
 * Must free when done using.
 * @note Must free when done using
 * @param unit A srcml_unit
 * @param xml_encoding The xml encoding to encode the unit
 * @param xml_buffer Buffer to return the standalone xml
 * @param buffer_size The size of the returned buffer
 * @return The formatted unit srcML on success and NULL on failure.
 */
LIBSRCML_DECL int srcml_unit_get_xml_standalone(struct srcml_unit*, const char* xml_encoding, char** xml_buffer, size_t* buffer_size);
/**@}*/


/**@{ @name Convert source code to srcML
      @brief Files/buffer can be compressed, but not a source archive format (e.g., not .tar)
      */
/**
 * @brief Convert to srcML the contents of a file and place it into a unit
 * @param unit A srcml_unit to parse the results to
 * @param src_fiename Name of a file to parse into srcML
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_filename(struct srcml_unit* unit, const char* src_filename);

/**
 * @brief Convert to srcML the contents of a buffer and place it into a unit
 * @param unit A srcml_unit to parse the results to
 * @param src_buffer Buffer containing source code to parse into srcML
 * @param buffer_size Size of the buffer to parse
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);

/**
 * @brief Convert to srcML the contents of a FILE and place it into a unit
 * @param unit A srcml_unit to parse the results to
 * @param src_file A FILE opened for reading
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_FILE    (struct srcml_unit*, FILE* src_file);

/**
 * @brief Convert to srcML the contents of a file descriptor and place it into a unit
 * @param unit A srcml_unit to parse the results to
 * @param src_fd A file descriptor open for reading
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_fd      (struct srcml_unit*, int src_fd);

/**
 * @brief Convert to srcML the contents from the opened context accessed via read and close callbacks and place it into a unit
 * @param unit A srcml_unit to parse the results to
 * @param context an io context
 * @param read_callback a read callback function
 * @param close_callback a close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_io      (struct srcml_unit*, void * context, int (*read_callback)(void * context, char * buffer, size_t len), int (*close_callback)(void * context));
/**@}*/


/**@{ @name Convert srcML to source code */
/**
 * @brief Convert the srcML in a unit into source code and place it into a filename
 * If the srcML was not read in, but the attributes were, the XML is read in and that value is unparsed
 * @param unit A srcml_unit
 * @param src_filename Name of a file to output contents of unit as source
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_filename(struct srcml_unit*, const char* src_filename, unsigned short compression);

/**
 * @brief Convert the srcML in a unit into source code and place it into a buffer
 * If the srcML was not read in, but the attributes were, the XML is read in and that value is unparsed.
 * The buffer is allocated in the function and needs to be freed after using.
 * @param unit A srcml_unit
 * @param src_buffer An output buffer address
 * @param src_size The size of the resulting buffer
 * @note The output buffer must be freed after using
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_memory  (struct srcml_unit*, char** src_buffer, size_t * src_size);

/**
 * @brief Convert the srcML in a unit into source code and place it into a FILE
 * If the srcML was not read in, but the attributes were, the XML is read in and that value is unparsed.
 * @param unit A srcml_unit
 * @param srcml_file FILE opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_FILE    (struct srcml_unit*, FILE* srcml_file);

/**
 * @brief Convert the srcML in a unit into source code and place it into a file descriptor
 * If the srcML was not read in, but the attributes were, the XML is read in and that value is unparsed.
 * @param unit A srcml_unit
 * @param srcml_fd File descriptor opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_fd      (struct srcml_unit*, int srcml_fd);

/**
 * @brief Convert the srcML in a unit into source code and place it into an opened io context using read/write callbacks.
 * If the srcML was not read in, but the attributes were, the XML is read in and that value is unparsed.
 * @param unit A srcml_unit
 * @param write_callback a write callback function
 * @param close_callback a close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_io      (struct srcml_unit*, void * context, int (*write_callback)(void * context, const char * buffer, size_t len), int (*close_callback)(void * context));
/**@}*/


/**@{ @name Iteratively build a unit */
/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_start_unit(struct srcml_unit*);

/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_end_unit(struct srcml_unit*);

/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_start_element(struct srcml_unit*, const char * prefix, const char * name, const char * uri);

/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_end_element(struct srcml_unit*);

/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_namespace(struct srcml_unit*, const char * prefix, const char * uri);

/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_attribute(struct srcml_unit*, const char * prefix, const char * name, const char * uri, const char * content);

/**
 * @todo Add documentation
 */
LIBSRCML_DECL int srcml_write_string(struct srcml_unit*, const char * content);
/**@}*/
/**@}*/



/** @defgroup srcDiff srcDiff
    @{
*/
/** Constant for original srcDiff revision number */
#define SRCDIFF_REVISION_ORIGINAL 0
/** Constant for modified srcDiff revision number */
#define SRCDIFF_REVISION_MODIFIED 1
/** Constant for an invalid srcDiff revision number */
#define SRCDIFF_REVISION_INVALID  2

/**
 * @return The current srcdiff revision number used for processing
 */
LIBSRCML_DECL size_t             srcml_get_srcdiff_revision();

/**
 * @param archive A srcml_archive
 * @return The srcdiff revision number the achive is using for processing
 */
LIBSRCML_DECL size_t             srcml_archive_get_srcdiff_revision             (const struct srcml_archive*);

/**
 * @brief Set what revision in a srcDiff archive to operate with
 * @param archive A srcml_archive
 * @param revision_number The operational srcdff revision
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_set_srcdiff_revision      (struct srcml_archive*, size_t revision_number);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif
