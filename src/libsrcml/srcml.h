/**
 * @file srcml.h
 *
 * @copyright Copyright (C) 2013-2023 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @mainpage libsrcml
 *
 * The functions in libsrcml are for the purposes of:
 *
 * * Converting source code to the srcML format
 * * Converting the srcML format back to source code
 * * Querying and transformation in the srcML format
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
#include <sys/types.h> /* ssize_t */
#include <stdio.h> /* FILE * */

#ifdef __cplusplus
extern "C" {
#endif

/* Function export macro */
#if defined(WIN32) && !defined(__MINGW32__)
#define LIBSRCML_DECL __declspec(dllexport)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#define LIBSRCML_DECL
#endif

/** @defgroup macros Macros
    @{ */

/**@{ @name Current Version */
/** Number representing libsrcml version */
#define SRCML_VERSION_NUMBER 1000
/** String containing libsrcml version */
#define SRCML_VERSION_STRING "1.0.0"
/**@}*/

/**@{ @name Status */
/** Return status indicating no errors */
#define SRCML_STATUS_OK                   0
/** Return status indicating general errors occurred */
#define SRCML_STATUS_ERROR                1
/** Return status indicating an invalid argument */
#define SRCML_STATUS_INVALID_ARGUMENT     2
/** Return status indicating that there is some problem with the input */
#define SRCML_STATUS_INVALID_INPUT        3
/** Return status indicating an invalid read I/O operation (such as write on read only archive) */
#define SRCML_STATUS_INVALID_IO_OPERATION 4
/** Return status indicating that there is some problem with the input */
#define SRCML_STATUS_IO_ERROR             5
/** Return status indicating an unitialized unit */
#define SRCML_STATUS_UNINITIALIZED_UNIT   6
/** Return status indicating an unset language */
#define SRCML_STATUS_UNSET_LANGUAGE       7
/** Return status indicating their are no transformations */
#define SRCML_STATUS_NO_TRANSFORMATION    8
/** Return status indicating a prefix is missing a declared namespace */
#define SRCML_STATUS_UNASSIGNED_PREFIX    9
/**@}*/

/**@{ @anchor Language @name Core Language Set */
/** Language not set */
#define SRCML_LANGUAGE_NONE   0
/** Language C */
#define SRCML_LANGUAGE_C      "C"
/** Language C++ */
#define SRCML_LANGUAGE_CXX    "C++"
/** Language C# */
#define SRCML_LANGUAGE_CSHARP "C#"
/** Language Java */
#define SRCML_LANGUAGE_JAVA   "Java"
/** Language XML */
#define SRCML_LANGUAGE_XML    "xml"
/**@}*/

/**@{ @name Options */
/** Do not issue an XML declaration (default: include XML declaration */
const unsigned int SRCML_OPTION_NO_XML_DECL       = 1<<1;
/** Include line/column position attributes */
const unsigned int SRCML_OPTION_POSITION          = 1<<2;
/** Markup preprocessor elements (default for C, C++) */
const unsigned int SRCML_OPTION_CPP               = 1<<3;
/** Leave as text preprocessor else parts (default: markup) */
const unsigned int SRCML_OPTION_CPP_TEXT_ELSE     = 1<<4;
/** Markup preprocessor @code #if 0 @endcode sections (default: leave as text) */
const unsigned int SRCML_OPTION_CPP_MARKUP_IF0    = 1<<5;
/** Encode the original source encoding as an attribute */
const unsigned int SRCML_OPTION_STORE_ENCODING    = 1<<6;
/**@}*/

/**@{ @name Source Output EOL Options */
/** Source-code end of line determined automatically */
#define SOURCE_OUTPUT_EOL_AUTO      0
/** Source-code end of line determined according to operating system */
#define SOURCE_OUTPUT_EOL_NATIVE    0
/** Source-code end of line is new line only */
#define SOURCE_OUTPUT_EOL_LF        1
/** Source-code end of line is carriage return only */
#define SOURCE_OUTPUT_EOL_CR        2
/** Source-code end of line is carriage return and new line */
#define SOURCE_OUTPUT_EOL_CRLF      3
/**@}*/
/**@}*/
/**@}*/

/**
 * @struct srcml_archive
 *
 * Encapsulates multiple srcML units into one srcML archive
 */
struct srcml_archive;

/**
 * @struct srcml_unit
 *
 * The srcML markup of one source-code file
 */
struct srcml_unit;

/** @defgroup utility Utility functions
    @{
 */

/**@{ @name Version */
/**
 * The current version of the library
 * @return Version of libsrcml as a number
 */
LIBSRCML_DECL int srcml_version_number();

/**
 * The current version of the library
 * @return Version of libsrcml as a string
 */
LIBSRCML_DECL const char* srcml_version_string();
/**@}*/

/**@{ @name Supported Languages */
/**
 * Checks if a source-code language is supported.
 * Accepted string values for the languages can be found in @ref Language
 * @param language The language to check support for as a string
 * @retval pos The numeric representation for that language
 * @retval 0 If the language is not supported
 */
LIBSRCML_DECL int srcml_check_language(const char* language);

/**
 * Check the current registered language for a file extension
 * @param filename The name of a file. When a full filename is given, the extension is extracted
 * @return The language name registered with that extension on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_check_extension(const char* filename);

/**
 * Gets the number of supported source-code languages
 * @return The number of source-code languages supported
 */
LIBSRCML_DECL size_t srcml_get_language_list_size();

/**
 * Gets the name of the supported language at a given position
 * @param pos The position of the language in the supported language list
 * @return The name of the supported source-code language on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_language_list(size_t pos);
/**@}*/

/**@{ @name Supported Encodings */
/**
 * Check if a particular encoding is supported for input and output
 * @param encoding The name of the encoding
 * @retval 0 if the encoding is not supported
 * @retval nonzero if the encoding is supported
 */
LIBSRCML_DECL int srcml_check_encoding(const char* encoding);
/**@}*/

/**@{ @name Optional Features */
/**
 * Check if XSLT is available
 * @retval 1 if XSLT is available
 * @retval 0 if it is unavailable
 */
LIBSRCML_DECL int srcml_check_xslt();

/**
 * Check if EXSLT is available
 * @retval 1 if EXSLT is available
 * @retval 0 if it is unavailable
 */
LIBSRCML_DECL int srcml_check_exslt();
/**@}*/

/**@{ @name Error Handling */
/**
 * Provides a description of the last error to occur
 * @return A string describing last recorded error
 */
LIBSRCML_DECL const char* srcml_error_string();
/**@}*/

/**@{ @name Memory */
/**
 * Free a memory buffer allocated by functions such as @ref srcml_archive_write_open_memory()
 * @param buffer The allocated buffer
 */
LIBSRCML_DECL void srcml_memory_free(char * buffer);
/**@}*/
/**@}*/
/**@}*/

/** @defgroup convenience Convenience functions

  Straightforward functions for translating single source-code files to and from the srcML format.
  A complete program to convert from a source-code file to srcML:

  @code
  #include <srcml.h>
  int main() {
    srcml("main.cpp", "main.cpp.xml");
    return 0;
  }
  @endcode

  A complete program to convert from srcML to a source-code file:

  @code
  #include <srcml.h>
  int main() {
    srcml("main.cpp.xml", "main.cpp");
    return 0;
  }
  @endcode

  * Only applies to individual source-code files

  * Options can be specified with the global srcml_set_*(),
  srcml_clear_*(), and srcml_register_*()

  * Options can be queried with the global srcml_get_*() and
  srcml_check_*()

  @{
 */
/**
 * Translate to and from the srcML format
 * @details Translates from source code to srcML if the input_filename
 * extension is for source code, e.g., .c, .cpp, .java Language
 * determined by file extension if language is not set with
 * srcml_set_language(). Translates from srcML to source code if the
 * input_filename extension is '.xml'
 * @param [in] input_filename The name of a source-code file or srcML file
 * @param [in] output_filename The name of the output srcML file or source-code file
 * @return SRCML_STATUS_OK on success
 * @return Status error on failure
 */
LIBSRCML_DECL int srcml(const char* input_filename, const char* output_filename);

/**@{ @name Global settings
      @brief To be used with the convenience function srcml()
*/
/**
 * Set the source encoding for the srcML
 * @param encoding An output encoding
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_src_encoding(const char* encoding);

/**
 * Set the xml encoding for the srcML
 * @param encoding An output encoding
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_xml_encoding(const char* encoding);

/**
 * Set the language used to parse for the srcML
 * @param language A supported source-code language
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_language(const char* language);

/**
 * Set the filename attribute for the srcML
 * @param filename Name of a file
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_filename(const char* filename);

/**
 * Set the url attribute for the srcML
 * @note The url is not checked for validity
 * @param url a url path
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_url(const char* url);

/**
 * Set the version attribute for the srcML
 * @note The version value is user-defined, and can be any value
 * @param version A version string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_version(const char* version);

/**
 * Set the timestamp attribute for the srcML
 * @param timestamp A timestamp string in any format
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_timestamp(const char* timestamp);

/**
 * Set options on the srcML, clearing all previously set options
 * @param option A srcML option
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_options(size_t option);

/**
 * Enable (set) a specific option on the srcML
 * @param option The srcML option(s)
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_enable_option(size_t option);

/**
 * Disable (unset) a specific option on the srcML
 * @param option The srcML option(s)
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_disable_option(size_t option);

/**
 * Set the size of the tabstop on the srcML
 * @param tabstop Tabstop size
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_tabstop(size_t tabstop);

/**
 * Associate an extension with a supported source-code language on the srcML
 * @param extension A source file extension
 * @param language A supported source code language
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_register_file_extension(const char* extension, const char* language);

/**
 * Add a new namespace or change the prefix of an existing namespace on the srcML
 * @param prefix An XML namespace prefix
 * @param ns An XML namespace
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_*  Status error code on failure
 */
LIBSRCML_DECL int srcml_register_namespace(const char* prefix, const char* ns);

/**
 * Set a processing instruction that will be output before the root
 * element of an archive
 * @param target the processing instruction's target
 * @param data the processing instruciton's data
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_set_processing_instruction(const char* target, const char* data);

/**
 * Set the end of line characters to be used for unparse
 * @param eol The kind of eol to use for unparse
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_set_eol(size_t eol);

/**
 * Set what revision in a srcDiff document to operate with
 * @param revision_number The revision to operate with
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_set_srcdiff_revision(size_t revision_number);

/**
 * @return The source encoding on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_src_encoding();

/**
 * @return The XML encoding on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_xml_encoding();

/**
 * @return The srcML revision attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_revision();

/**
 * @return The language attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_language();

/**
 * @return The filename attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_filename();

/**
 * @return The url attribute for the root unit on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_url();

/**
 * @return The version attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_version();

/**
 * @return The timestamp attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_timestamp();

/**
 * @return The hash attribute on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_hash();

/**
 * @return The loc of the source code on success
 * @return -1 on failure
 */
LIBSRCML_DECL int srcml_get_loc();

/**
 * @return The eol for to-src output (unparse)
 * @return NULL on failure
 */
LIBSRCML_DECL size_t srcml_get_eol();

/**
 * @return The currently set options on success
 * @return NULL on failure
 */
LIBSRCML_DECL int srcml_get_options();

/**
 * @return The tabstop size on success
 * @return NULL on failure
 */
LIBSRCML_DECL size_t srcml_get_tabstop();

/**
 * @return The processing instruction target
 */
LIBSRCML_DECL const char* srcml_get_processing_instruction_target();

/**
 * @return The processing instruction data
 */
LIBSRCML_DECL const char* srcml_get_processing_instruction_data();
/**@}*/

/**@{ @name XML Namespaces */
/**
 * @return Number of declared XML namespaces
 */
LIBSRCML_DECL size_t srcml_get_namespace_size();

/**
 * Get the prefix of the namespace at that position
 * @param pos The position to get the namespace prefix at
 * @return The prefix, where empty namespace is an empty string
 * @return 0 if given an invalid position
 */
LIBSRCML_DECL const char* srcml_get_namespace_prefix(size_t pos);

/**
 * Get the registered prefix for the given namespace
 * @param namespace_uri An XML namespace
 * @return The registered prefix for the given namespace
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_prefix_from_uri(const char* namespace_uri);

/**
 * @param pos position in namespaces
 * @return The namespace URI at that position on succcess
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_namespace_uri(size_t pos);

/**
 * @param prefix An XML prefix
 * @return The first namespace URI for the given prefix on success
 * @return NULL on failure
 */
LIBSRCML_DECL const char* srcml_get_uri_from_prefix(const char* prefix);

/**
 * Add the attribute to the archive
 * @param uri An XML namespace uri of the attribute
 * @param name The attribute name
 * @param value The attribute value
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_add_attribute(const char* uri, const char* name, const char* value);

/**
 * Number of custom attributes
 * @return The number of attributes or 0 if archive is NULL
 */
LIBSRCML_DECL size_t srcml_get_attribute_size();

/**
 * Prefix of the custom attribute at position pos
 * @param pos The attribute position
 * @return The prefix for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_get_attribute_prefix(size_t pos);

/**
 * Name of the custom attribute at position pos
 * @param pos The attribute position
 * @return The name for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_get_attribute_name(size_t pos);

/**
 * Value of the custom attribute at position pos
 * @param pos The attribute position
 * @return The value for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_get_attribute_value(size_t pos);

/**
 * Cleanup and free globally allocated items (usually by libxml2)
 */
LIBSRCML_DECL void srcml_cleanup_globals();
/**@}*/

/** @defgroup archive Archive
    @{
*/
/**
 * Create a new srcml archive
 * @note Archive must be freed using srcml_archive_free()
 * @return The created srcml_archive
 */
LIBSRCML_DECL struct srcml_archive* srcml_archive_create();

/**
 * Clone the setup of an existing archive
 * @note Archive must be freed using srcml_archive_free()
 * @param archive A srcml_archive
 * @return The cloned archive
 */
LIBSRCML_DECL struct srcml_archive* srcml_archive_clone(const struct srcml_archive* archive);

/**
 * Provides a code of the last error to occur for an archive
 * @param archive A srcml_archive
 * @return Error code for last recorded error
 */
LIBSRCML_DECL int srcml_archive_error_number(const struct srcml_archive* archive);

/**
 * Provides a description of the last error to occur for an archive
 * @param archive A srcml_archive
 * @return A string describing last recorded error
 */
LIBSRCML_DECL const char* srcml_archive_error_string(const struct srcml_archive* archive);

/**
 * Append the srcml_unit unit to the srcml_archive archive
 * @param archive A srcml_archive opened for writing
 * @param unit A srcml_unit to output
 * @note Can not mix with by element mode.
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_unit(struct srcml_archive* archive, struct srcml_unit* unit);

/**
 * Append the string to the srcml_archive archive
 * @param archive A srcml_archive opened for writing
 * @param s String to write
 * @param len Length of the string to write
 * @note Can not mix with by element mode.
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_string(struct srcml_archive* archive, const char* s, int len);

/**
 * Close a srcml_archive opened using srcml_archive_read_open_*() or srcml_archive_write_open_*().
 * The archive can be reopened.
 * @param archive An open srcml_archive
 */
LIBSRCML_DECL void srcml_archive_close(struct srcml_archive* archive);

/**
 * Free a srcml archive that was previously allocated
 * by using srcml_archive_create() or srcml_archive_clone(). The archive must be reallocated/re-created to use again.
 * @param archive A srcml_archive
 */
LIBSRCML_DECL void srcml_archive_free(struct srcml_archive* archive);

/**@{ @name Open for Write
      @brief Open a srcML archive for output */

/**
 * Open up a srcml_archive for writing to a given output file
 * @param archive A srcml_archive
 * @param srcml_filename Name of an output file
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_filename(struct srcml_archive* archive, const char* srcml_filename);

/**
 * Open up a srcml_archive for writing to a given memory buffer
 * @param archive A srcml_archive
 * @param buffer Location to return output string. The buffer is allocated
 * and set to the location the buffer points at. The buffer must be freed after use
 * @param size the size of the resulting buffer
 * @note The buffer must be freed after use
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_memory(struct srcml_archive* archive, char** buffer, size_t * size);

/**
 * Open up a srcml_archive for writing to a given FILE pointer
 * @param archive A srcml_archive
 * @param srcml_file FILE opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_FILE(struct srcml_archive* archive, FILE* srcml_file);

/**
 * Open up a srcml_archive for writing to a file descriptor
 * @param archive A srcml_archive
 * @param srcml_fd Output file descriptor
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_fd(struct srcml_archive* archive, int srcml_fd);

/**
 * Open up a srcml_archive for writing to an io context using writeand close callbacks
 * @param archive A srcml_archive
 * @param context An io context
 * @param write_callback A write callback function
 * @param close_callback A close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_write_open_io(struct srcml_archive* archive, void * context, int (*write_callback)(void * context, const char* buffer, int len), int (*close_callback)(void * context));
/**@}*/

/**@{ @name Open for Read
      @brief Open a srcML archive for reading */
/**
 * Open a srcML archive for reading from a filename
 * @param archive A srcml_archive
 * @param srcml_filename Name of an input file
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_read_open_filename(struct srcml_archive* archive, const char* srcml_filename);

/**
 * Open a srcML archive for reading from a buffer up until a buffer_size
 * @param archive A srcml_archive
 * @param buffer An input buffer
 * @param buffer_size Size of the input buffer
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_IO_ERROR
 */
LIBSRCML_DECL int srcml_archive_read_open_memory(struct srcml_archive* archive, const char* buffer, size_t buffer_size);

/**
 * Open a srcML archive for reading from a FILE
 * @param archive A srcml_archive
 * @param srcml_file A FILE opened for reading
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_IO_ERROR
 */
LIBSRCML_DECL int srcml_archive_read_open_FILE(struct srcml_archive* archive, FILE* srcml_file);

/**
 * Open a srcML archive for reading from a file descriptor
 * @param archive A srcml_archive
 * @param srcml_fd A file descriptor opened for reading
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_IO_ERROR
 */
LIBSRCML_DECL int srcml_archive_read_open_fd(struct srcml_archive* archive, int srcml_fd);

/**
 * Open a srcML archive for reading from the opened context, accessed via read and close callbacks
 * @param archive A srcml_archive
 * @param context An io context
 * @param read_callback A read callback function
 * @param close_callback A close callback function
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_IO_ERROR
 */
LIBSRCML_DECL int srcml_archive_read_open_io(struct srcml_archive* archive, void * context, int (*read_callback)(void * context, char* buffer, int len), int (*close_callback)(void * context));
/**@}*/

/**@{ @name Archive Options */

/**
 * Whether the archive is a single, non-nested unit, or an archive
 * @param archive A srcml_archive opened for reading or writing
 * @retval 1 Is a solitary unit
 * @retval 0 Is an archive that contains other units
 */
LIBSRCML_DECL int srcml_archive_is_solitary_unit(const struct srcml_archive* archive);

/**
 * Enable a single, solitary unit. This is only needed when each source-code file is to be
 * represented by an individual srcML file. Note that writing multiple units to this archive is an error.
 * @param archive A srcml_archive opened for writing
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_enable_solitary_unit(struct srcml_archive* archive);

/**
 * Disable the solitary unit. The full archive format allows for multiple units, and
 * is the default.
 * @param archive A srcml_archive opened for writing
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_disable_solitary_unit(struct srcml_archive* archive);

/**
 * Whether the hash attribute exists (in the case of a read), or would be added (in case of a write)
 * @param archive A srcml archive opened for reading or writing
 * @retval 1 Will include hash attribute
 * @retval 0 Does not include the hash attribute
 */
LIBSRCML_DECL int srcml_archive_has_hash(const struct srcml_archive* archive);

/**
 * Enable the hash attribute. This is the default.
 * @param archive A srcml_archive opened for writing
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_enable_hash(struct srcml_archive* archive);

/**
 * Disable the hash attribute
 * @param archive A srcml_archive opened for writing
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_disable_hash(struct srcml_archive* archive);

/**
 * Set the XML encoding of the srcML archive
 * @param archive The srcml_archive to set the encoding
 * @param encoding The encoding of the archive
 * @retval SRCML_STATUS_OK on success @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_xml_encoding(struct srcml_archive* archive, const char* encoding);

/**
 * Set the default source encoding for the srcML archive
 * @param archive The srcml_archive to set the source encoding for
 * @param encoding A source-code encoding
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_src_encoding(struct srcml_archive* archive, const char* encoding);

/**
 * Set the language of the srcML archive
 * @param archive A srcml_archive to set the source-code language on
 * @param language A source-code language
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_language(struct srcml_archive* archive, const char* language);

/**
 * Set all options for processing an archive, erasing all previously set options
 * @param archive The srcml_archive to set the options for
 * @param option A set of srcml options
 * @note Erases all previously set options
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_options(struct srcml_archive* archive, size_t option);

/**
 * Enable/set an option or options on an archive
 * @param archive A srcml_archive to enable options on
 * @param option An option, or multiple options by |ing each, to set on the archive
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_enable_option(struct srcml_archive* archive, size_t option);

/**
 * Remove an option or options from an archive
 * @param archive A srcml_archive to remove options from
 * @param option The option, or multiple options by |ing each, to clear from the archive
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_disable_option(struct srcml_archive* archive, size_t option);

/**
 * Set the tabstop size for position and column calculation
 * @param archive A srcml_archive
 * @param tabstop Size of a tabstop
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_tabstop(struct srcml_archive* archive, size_t tabstop);

/**
 * Set an extension to be associated with a given source-code language
 * @param archive A srcml_archive that associates the given extension with a language
 * @param extension A file extension
 * @param language A supported source-code language
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_register_file_extension(struct srcml_archive* archive, const char* extension, const char* language);

/**
 * Create a new namespace or change the prefix of an existing namespace
 * @param archive A srcml_archive
 * @param prefix An XML namespace prefix
 * @param uri An XML namespace uri
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_register_namespace(struct srcml_archive* archive, const char* prefix, const char* uri);

/**
 * Add the attribute to the archive
 * @param archive A srcml_archive
 * @param uri An XML namespace uri of the attribute
 * @param name The attribute name
 * @param value The attribute value
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_add_attribute(struct srcml_archive* archive, const char* uri, const char* name, const char* value);

/**
 * Number of custom attributes
 * @param archive A srcml_archive
 * @return The number of attributes or 0 if archive is NULL
 */
LIBSRCML_DECL size_t srcml_archive_get_attribute_size(const struct srcml_archive* archive);

/**
 * Prefix of the custom attribute at position pos
 * @param archive A srcml_archive
 * @param pos The attribute position
 * @return The prefix for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_attribute_prefix(const struct srcml_archive* archive, size_t pos);

/**
 * Name of the custom attribute at position pos
 * @param archive A srcml_archive
 * @param pos The attribute position
 * @return The name for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_attribute_name(const struct srcml_archive* archive, size_t pos);

/**
 * Value of the custom attribute at position pos
 * @param archive A srcml_archive
 * @param pos The attribute position
 * @return The value for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_attribute_value(const struct srcml_archive* archive, size_t pos);

/**
 * Set a processing instruction that will be output before the root element of the archive
 * @param archive A srcml_archive
 * @param target The processing instruction's target
 * @param data The processing instruciton's data
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_archive_set_processing_instruction(struct srcml_archive* archive, const char* target, const char* data);
/**@}*/

/**@{ @name Archive Optional Attributes */

/**
 * Set the root URL attribute of the srcML archive
 * @param archive A srcml_archive to set the root URL attribute on
 * @param url A url path
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_url(struct srcml_archive* archive, const char* url);

/**
 * Set the root version attribute of the srcML archive
 * @param archive A srcml_archive to set the root version attribute on
 * @param version A version string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_archive_set_version(struct srcml_archive* archive, const char* version);

/**@}*/

/**@{ @name Get Configuration */

/**
 * @param archive A srcml_archive
 * @return The currently set XML encoding, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_xml_encoding(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently default source encoding, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_src_encoding(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently set revision, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_revision(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently set language, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_language(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently set root url attribute, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_url(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently set root version attribute, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_version(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently set options
 */
LIBSRCML_DECL int srcml_archive_get_options(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The currently set tabstop size
 */
LIBSRCML_DECL size_t srcml_archive_get_tabstop(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @return The number of currently defined namespaces or 0 if archive is NULL
 */
LIBSRCML_DECL size_t srcml_archive_get_namespace_size(const struct srcml_archive* archive);

/**
 * @param archive A srcml_archive
 * @param pos The namespace position
 * @return The prefix for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_namespace_prefix(const struct srcml_archive* archive, size_t pos);

/**
 * @param archive A srcml_archive
 * @param namespace_uri An XML namespace URI
 * @return The registered prefix for the given namespace, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_prefix_from_uri(const struct srcml_archive* archive, const char* namespace_uri);

/**
 * @param archive A srcml_archive
 * @param pos The namespace position
 * @return The namespace at the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_namespace_uri(const struct srcml_archive* archive, size_t pos);

/**
 * @param archive A srcml_archive
 * @param prefix An XML prefix
 * @return The first namespace for the given prefix on success, or NULL
 */
LIBSRCML_DECL const char* srcml_archive_get_uri_from_prefix(const struct srcml_archive* archive, const char* prefix);

/**
 * @param archive A srcml archive
 * @return The processing instruction target
 */
LIBSRCML_DECL const char* srcml_archive_get_processing_instruction_target(const struct srcml_archive* archive);

/**
 * @param archive A srcml archive
 * @return The processing instruction data
 */
LIBSRCML_DECL const char* srcml_archive_get_processing_instruction_data(const struct srcml_archive* archive);

/**
 * Retrieve the currently registered language for a file extension
 * @param archive A srcml_archive
 * @param filename Name of a file. Given the full filename, the extension is extracted
 * @return The language for extension, or if 0 if no language.
 */
LIBSRCML_DECL const char* srcml_archive_check_extension(const struct srcml_archive* archive, const char* filename);
/**@}*/

/**@{ @name Read Unit
*/
/**
 * Read the next unit header from the archive
 * @param archive A srcml_archive open for reading
 * @return The read srcml_unit, with header information only, on success
 * @return NULL on failure
 */
LIBSRCML_DECL struct srcml_unit* srcml_archive_read_unit_header(struct srcml_archive* archive);

/**
 * Read the next unit from the archive
 * @param archive A srcml_archive open for reading
 * @return The read srcml_unit on success
 * @return NULL on failure
 */
LIBSRCML_DECL struct srcml_unit* srcml_archive_read_unit(struct srcml_archive* archive);

/**
 * Skip the next unit from the archive
 * @param archive A srcml_archive open for reading
 * @return 1 Succesfully skipped
 * @return NULL on failure
 */
LIBSRCML_DECL int srcml_archive_skip_unit(struct srcml_archive* archive);
/**@}*/

/**@{ @name XPath query and XSLT transformations */

/**
 * Append the XPath expression to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xpath_string An XPath expression
 * @note Currently, there is no way to specify context to the expression.
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_xpath(struct srcml_archive* archive, const char* xpath_string);

/**
 * Append the XPath expression to the list of transformations/queries.
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
LIBSRCML_DECL int srcml_append_transform_xpath_attribute(struct srcml_archive* archive, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* attr_name, const char* attr_value);

/**
 * Append the XPath expression to the list of transformations/queries.
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
LIBSRCML_DECL int srcml_append_transform_xpath_element(struct srcml_archive* archive, const char* xpath_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element);

/**
 * Append an XSLT program at the designated filename path to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xslt_filename An XSLT program filename path
 * @note Currently no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure.
 */
LIBSRCML_DECL int srcml_append_transform_xslt_filename(struct srcml_archive* archive, const char* xslt_filename);

/**
 * Append an XSLT program from a buffer to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xslt_buffer A buffer holding an XSLT
 * @param size Size of the given buffer
 * @note Currently no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure
 */
LIBSRCML_DECL int srcml_append_transform_xslt_memory(struct srcml_archive* archive, const char* xslt_buffer, size_t size);

/**
 * Append an XSLT program in a FILE to the list of transformations/queries.
 * @param archive A srcml_archive
 * @param xslt_file A FILE containing an XSLT program
 * @note Currently no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure
 */
LIBSRCML_DECL int srcml_append_transform_xslt_FILE(struct srcml_archive* archive, FILE* xslt_file);

/**
 * Append an XSLT program from a file descriptor to the list of transformations/queries
 * @param archive A srcml_archive
 * @param xslt_fd A file descriptor containing an XSLT program
 * @note Currently no way to specify parameters or context
 * Append the XSLT program in fd to the list
 * of transformation/queries.  As of yet no way to specify parameters or context
 * @return SRCML_STATUS_OK on success
 * @return Status error codes on failure
 */
LIBSRCML_DECL int srcml_append_transform_xslt_fd(struct srcml_archive* archive, int xslt_fd);

/**
 * Append the RelaxNG schema from a filename path to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_filename A RelaxNG schema filename path
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_filename(struct srcml_archive* archive, const char* relaxng_filename);

/**
 * Append the RelaxNG schema in the buffer to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_buffer A buffer holding a RelaxNG schema
 * @param size Size of the passed buffer
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_memory(struct srcml_archive* archive, const char* relaxng_buffer, size_t size);

/**
 * Append the RelaxNG schema in a FILE to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_file A FILE containing a RelaxNG schema
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_FILE(struct srcml_archive* archive, FILE* relaxng_file);

/**
 * Append the RelaxNG schema in a file descriptor to the list of transformations/queries
 * @param archive A srcml_archive
 * @param relaxng_fd A file descriptor containing a RelaxNG schema
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_relaxng_fd(struct srcml_archive* archive, int relaxng_fd);

/**
 * Append the srcQL query to the list of transformations/queries
 * @param archive A srcml_archive
 * @param srcql_string A srcQL query
 * @note Currently, there is no way to specify context to the expression.
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_srcql(struct srcml_archive* archive, const char* srcql_string);

/**
 * Append the srcQL query to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * archive marking the XPath results with a user provided attribute
 * @param archive A srcml_archive
 * @param srcql_string A srcQL query
 * @param prefix Attribute prefix
 * @param namespace_uri Attribute namespace
 * @param attr_name Attribute name
 * @param attr_value Attribute value
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_srcql_attribute(struct srcml_archive* archive, const char* srcql_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* attr_name, const char* attr_value);

/**
 * Append the srcQL query to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * archive marking the XPath results with a user provided element.
 * @param archive A srcml_archive
 * @param srcql_string A srcQL query
 * @param prefix Element prefix
 * @param namespace_uri Element namespace
 * @param element Element name
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_srcql_element(struct srcml_archive* archive, const char* srcql_string,
                                                            const char* prefix, const char* namespace_uri,
                                                            const char* element);

/**
 * Append an XSLT parameter to the last transformation
 * @param archive A srcml_archive
 * @param param_name Name of a parameter
 * @param param_value Value of the parameter
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_param(struct srcml_archive* archive, const char* param_name, const char* param_value);

/**
 * Append a string XSLT parameter to the last transformation, with the value wrapped in quotes
 * @param archive A srcml_archive
 * @param param_name Name of a parameter
 * @param param_value Value of the named parameter wrapped in quotes (")
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_append_transform_stringparam(struct srcml_archive* archive, const char* param_name, const char* param_value);

/**
 * @struct srcml_transform_result
 *
 * The result of a srcML transformation
 */
struct srcml_transform_result;

/**
 * Transformation result types
 */
#define SRCML_RESULT_NONE    0
#define SRCML_RESULT_UNITS   1
#define SRCML_RESULT_BOOLEAN 2
#define SRCML_RESULT_NUMBER  3
#define SRCML_RESULT_STRING  4

/**
 * Apply appended transformations from the archive to the unit consecutively in order. If parameter result is NULL,
 * result replaces the unit that the transformation was performed on. If parameter result is not NULL, results
 * are places in the proper field of the result, with the result_type parameter indicating which is appropriate.
 * If the result of the transformation is not a single unit, and the parameter result is NULL, that is considered an error.
 * @param archive Archive with the transformations declared
 * @param unit Unit to perform the transformation on
 * @param results Optional struct of different results types
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
LIBSRCML_DECL int srcml_unit_apply_transforms(struct srcml_archive* archive, struct srcml_unit* unit, struct srcml_transform_result** result);

/**
 * @param result A srcml transformation result
 * @return The type of the transformation result
 */
LIBSRCML_DECL int srcml_transform_get_type(struct srcml_transform_result* result);

/**
 * @param result A srcml transformation result
 * @return The number of units in the transformation result
 */
LIBSRCML_DECL int srcml_transform_get_unit_size(struct srcml_transform_result* result);

/**
 * @param result A srcml transformation result
 * @param pos The index in the units
 * @return The unit in the transformation result at that index
 */
LIBSRCML_DECL struct srcml_unit* srcml_transform_get_unit(struct srcml_transform_result* result, int index);

/**
 * @param result A srcml transformation result
 * @return The transformation result string
 */
LIBSRCML_DECL const char* srcml_transform_get_string(struct srcml_transform_result* result);

/**
 * @param result A srcml transformation result
 * @return The transformation result number
 */
LIBSRCML_DECL double srcml_transform_get_number(struct srcml_transform_result* result);

/**
 * @param result A srcml transformation result
 * @return The transformation result boolean
 */
LIBSRCML_DECL int srcml_transform_get_bool(struct srcml_transform_result* result);

/**
 * Free the resources in a tranformation result.
 * @param results Struct of result
 * @returns Returns SRCML_STATUS_OK on success and a status error codes on failure.
 */
LIBSRCML_DECL int srcml_transform_free(struct srcml_transform_result* result);

/**
 * Remove all appended transformations from the archive
 * @param archive A srcml_archive
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_clear_transforms(struct srcml_archive* archive);

/**@}*/
/**@}*/

/** @defgroup unit Unit
    @{
*/
/**
 * Create a new srcml_unit tied to the srcml archive
 * @param archive A srcml_archive
 * @note The unit must be freed using srcml_unit_free()
 * @returns Created srcml_unit on success
 * @return NULL on failure
 */
LIBSRCML_DECL struct srcml_unit* srcml_unit_create(struct srcml_archive* archive);

/**
 * Clone the setup of an existing unit
 * @note Unit must be freed using srcml_unit_free()
 * @param unit A srcml_unit
 * @return The cloned unit
 */
LIBSRCML_DECL struct srcml_unit* srcml_unit_clone(const struct srcml_unit* unit);

/**
 * Provides a code of the last error to occur for a unit
 * @param unit A srcml_unit
 * @return A code for the last recorded error
 */
LIBSRCML_DECL int srcml_unit_error_number(const struct srcml_unit* unit);

/**
 * Provides a description of the last error to occur for a unit
 * @param unit A srcml_unit
 * @return A string describing last recorded error
 */
LIBSRCML_DECL const char* srcml_unit_error_string(const struct srcml_unit* unit);

/**
 * Free an allocated unit
 * @param unit The srcml unit to free
 */
LIBSRCML_DECL void srcml_unit_free(struct srcml_unit* unit);

/**@{ @name Setup options */
/**
 * Set the source-code encoding for the srcml unit
 * @param unit A srcml_unit
 * @param encoding A source-code encoding
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_unit_set_src_encoding(struct srcml_unit* unit, const char* encoding);

/**
 * Set the source-code language for the srcml unit
 * @param unit A srcml_unit
 * @param language A supported source-code language
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_unit_set_language(struct srcml_unit* unit, const char* language);

/**
 * Set the filename attribute for the srcml unit
 * @param unit A srcml_unit
 * @param filename The name of a file
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_unit_set_filename(struct srcml_unit* unit, const char* filename);

/**
 * Set the version attribute for the srcml unit
 * @param unit A srcml_unit
 * @param version A version string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_unit_set_version(struct srcml_unit* unit, const char* version);

/**
 * Set the timestamp attribute for the srcml unit
 * @param unit A srcml_unit
 * @param timestamp A timestamp string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_unit_set_timestamp(struct srcml_unit* unit, const char* timestamp);

/**
 * Set the type of end of line to be used for unparse
 * @param unit A srcml_unit
 * @param eol The kind of eol to use for unparse
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
LIBSRCML_DECL int srcml_unit_set_eol(struct srcml_unit* unit, size_t eol);

/**
 * Create a new namespace or change the prefix of an existing namespace
 * @param unit A srcml_unit
 * @param prefix An XML namespace prefix
 * @param uri An XML namespace uri
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_register_namespace(struct srcml_unit* unit, const char* prefix, const char* uri);

/**
 * Add the attribute to the unit
 * @param unit A srcml_unit
 * @param uri An XML namespace uri of the attribute
 * @param name The attribute name
 * @param value The attribute value
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_add_attribute(struct srcml_unit* unit, const char* uri, const char* name, const char* value);

/**
 * Number of custom attributes
 * @param unit A srcml_unit
 * @return The number of attributes or 0 if unit is NULL
 */
LIBSRCML_DECL size_t srcml_unit_get_attribute_size(const struct srcml_unit* unit);

/**
 * Prefix of the custom attribute at position pos
 * @param unit A srcml_unit
 * @param pos The attribute position
 * @return The prefix for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_attribute_prefix(const struct srcml_unit* unit, size_t pos);

/**
 * Name of the custom attribute at position pos
 * @param unit A srcml_unit
 * @param pos The attribute position
 * @return The name for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_attribute_name(const struct srcml_unit* unit, size_t pos);

/**
 * Value of the custom attribute at position pos
 * @param unit A srcml_unit
 * @param pos The attribute position
 * @return The value for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_attribute_value(const struct srcml_unit* unit, size_t pos);

/**
 * @param unit A srcml_unit
 * @return The source-code encoding for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_src_encoding(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The revision for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_revision(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The source-code language for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_language(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The filename attribute on the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_filename(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The version for the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_version(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The timestamp attribute on the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_timestamp(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The hash attribute on the unit on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_hash(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The loc of the source code in the unit, or -1 on failure
 */
LIBSRCML_DECL int srcml_unit_get_loc(const struct srcml_unit* unit);

/**
 * @param unit A srcml unit
 * @return The eol for to-src output (unparse), or NULL
 */
LIBSRCML_DECL size_t srcml_unit_get_eol(struct srcml_unit* unit);

/**
 * Get a complete, valid XML of the srcML from this unit
 * The XML returned is a complete solo srcML unit
 * @note Do not free
 * @note String is valid until the unit is freed, or another srcml_unit_get_srcml*() is called
 * @param unit A srcml unit opened for reading
 * @return The standalone unit srcML on success and NULL on failure.
 */
LIBSRCML_DECL const char* srcml_unit_get_srcml(struct srcml_unit* unit);

/**
 * Get a fragment of the srcML from this unit
 * The XML returned is UTF-8 encoded XML. It is not well-formed XML, e.g., it is missing
 * the archive namespace declarations
 * @note Do not free
 * @note String is valid until the unit is freed, or another srcml_unit_get_srcml*() is called
 * @param unit A srcml unit opened for reading
 * @return The fragment unit srcML on success and NULL on failure.
 */
LIBSRCML_DECL const char* srcml_unit_get_srcml_outer(struct srcml_unit* unit);

/**
 * Get the srcML without the enclosing unit tags
 * The XML fragment returned is UTF-8 encoded XML. It is not well-formed XML, e.g., it is missing
 * the archive namespace declarations and may not have a single root.
 * @note Do not free
 * @note String is valid until the unit is freed, or another srcml_unit_get_srcml*() is called
 * @param unit A srcml unit opened for reading
 * @return The fragment unit srcML on success and NULL on failure.
 */
LIBSRCML_DECL const char* srcml_unit_get_srcml_inner(struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @return The number of currently defined namespaces or 0 if unit is NULL
 */
LIBSRCML_DECL size_t srcml_unit_get_namespace_size(const struct srcml_unit* unit);

/**
 * @param unit A srcml_unit
 * @param pos The namespace position
 * @return The prefix for the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_namespace_prefix(const struct srcml_unit* unit, size_t pos);

/**
 * @param unit A srcml_unit
 * @param namespace_uri An XML namespace URI
 * @return The registered prefix for the given namespace, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_prefix_from_uri(const struct srcml_unit* unit, const char* namespace_uri);

/**
 * @param unit A srcml_unit
 * @param pos The namespace position
 * @return The namespace at the given position, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_namespace_uri(const struct srcml_unit* unit, size_t pos);

/**
 * @param unit A srcml_unit
 * @param prefix An XML prefix
 * @return The first namespace for the given prefix on success, or NULL
 */
LIBSRCML_DECL const char* srcml_unit_get_uri_from_prefix(const struct srcml_unit* unit, const char* prefix);

/**@}*/

/**@{ @name Convert source code to srcML
      @brief Source code from a variety of input sources is converted into srcML, and stored in a unit
      @note Files/buffer can be compressed with gzip (i.e., .gz extension)
      */
/**
 * Convert the contents of the file with the name src_filename to srcML and store in the unit
 * @param unit A srcml_unit to parse the results to
 * @param src_filename Name of a file to parse into srcML
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_filename(struct srcml_unit* unit, const char* src_filename);

/**
 * Convert the contents of the src_buffer to srcML and store in the unit
 * @param unit A srcml_unit to parse the results to
 * @param src_buffer Buffer containing source code to parse into srcML
 * @param buffer_size Size of the buffer to parse
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_memory(struct srcml_unit* unit, const char* src_buffer, size_t buffer_size);

/**
 * Convert the contents of the source-code FILE* to srcML and store in the unit
 * @param unit A srcml_unit to parse the results to
 * @param src_file A FILE* opened for reading
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_FILE(struct srcml_unit* unit, FILE* src_file);

/**
 * Convert the contents of a file descriptor and stored in the unit
 * @param unit A srcml_unit to parse the results to
 * @param src_fd A file descriptor open for reading
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure.
 */
LIBSRCML_DECL int srcml_unit_parse_fd(struct srcml_unit* unit, int src_fd);

/**
 * Convert to srcML the contents from the opened context accessed via read and close callbacks and place it into a unit
 * @param unit A srcml_unit to parse the results to
 * @param context an io context
 * @param read_callback a read callback function
 * @param close_callback a close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_parse_io(struct srcml_unit* unit, void * context, ssize_t (*read_callback)(void * context, void * buffer, size_t len), int (*close_callback)(void * context));
/**@}*/

/**@{ @name Convert srcML to source code
      @brief srcML in a srcml unit is converted back to source code, and stored in a variety of output destinations
      */

/**
 * Get the source from this unit
 * @note The source is in UTF-8 encoding and does not follow the source encoding
 * @note If other encodings are needed, use srcml_unit_unparse_memory()
 * @param unit A srcml unit opened for reading
 * @return The source
 * @return Null on failure
 */
LIBSRCML_DECL const char* srcml_unit_get_src(struct srcml_unit* unit);

/**
 * Get the source size from this unit
 * @note The size of the source is for UTF-8 encoding and does not follow the source encoding
 * @note If the size of other encodings are needed, use srcml_unit_unparse_memory()
 * @param unit A srcml unit opened for reading
 * @return The source size
 * @return -1 on failure
 */
LIBSRCML_DECL ssize_t srcml_unit_get_src_size(struct srcml_unit* unit);

/**
 * Convert the srcML in a unit into source code and place it into a filename
 * If the srcML was not read in, but the attributes were, the XML is read in and that value is unparsed
 * @param unit A srcml unit opened for reading
 * @param src_filename Name of a file to output contents of unit as source
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_filename(struct srcml_unit* unit, const char* src_filename);

/**
 * Convert the srcML in a unit into source code and place it into a buffer
 * The buffer is allocated in the function and needs to be freed after using.
 * @param unit A srcml unit opened for reading
 * @param src_buffer An output buffer address
 * @param src_size The size of the resulting buffer
 * @note The output buffer must be freed after using
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_memory(struct srcml_unit* unit, char** src_buffer, size_t * src_size);

/**
 * Convert the srcML in a unit into source code and output to the FILE*
 * @param unit A srcml unit opened for reading
 * @param file FILE* opened for writing to output the source file
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_FILE(struct srcml_unit* unit, FILE* file);

/**
 * Convert the srcML in a unit into source code and output to the file descriptor
 * @param unit A srcml unit opened for reading
 * @param fd File descriptor opened for writing to output the source file
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_fd(struct srcml_unit* unit, int fd);

/**
 * Convert the srcML in a unit into source code and output using write callbacks
 * @param unit A srcml unit opened for reading
 * @param context
 * @param write_callback a write callback function
 * @param close_callback a close callback function
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_unit_unparse_io(struct srcml_unit* unit, void * context, int (*write_callback)(void * context, const char* buffer, int len), int (*close_callback)(void * context));
/**@}*/

/**@{ @name Iteratively build a unit */
/**
 * Write a start tag for a unit
 * @param unit A srcml unit opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_start_unit(struct srcml_unit* unit);

/**
 * Write an end tag for a unit
 * @param unit A srcml unit opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_end_unit(struct srcml_unit* unit);

/**
 * Write a start tag for a general element
 * @param unit A srcml unit opened for writing
 * @param prefix Element prefix
 * @param name Element name
 * @param uri URI of the prefix
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_start_element(struct srcml_unit* unit, const char* prefix, const char* name, const char* uri);

/**
 * Write an end tag for a general element
 * @param unit A srcml unit opened for writing
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_end_element(struct srcml_unit* unit);

/**
 * Write a namespace
 * @param unit A srcml unit opened for writing
 * @param prefix Namespace prefix
 * @param uri Namespace URI
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_namespace(struct srcml_unit* unit, const char* prefix, const char* uri);

/**
 * Write an attribute
 * @param unit A srcml unit opened for writing
 * @param prefix Element prefix
 * @param name Element name
 * @param uri URI of the prefix
 * @param content Value of the attribute
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_attribute(struct srcml_unit* unit, const char* prefix, const char* name, const char* uri, const char* content);

/**
 * Write a general string
 * @param unit A srcml unit opened for writing
 * @param content Null-terminated string to write
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_write_string(struct srcml_unit* unit, const char* content);
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
 * @return The current srcDiff revision number used for processing
 */
LIBSRCML_DECL size_t srcml_get_srcdiff_revision();

/**
 * @param archive A srcml_archive
 * @return The srcdiff revision number the archive is using for processing
 */
LIBSRCML_DECL size_t srcml_archive_get_srcdiff_revision(const struct srcml_archive* archive);

/**
 * Set what revision in a srcDiff archive to operate with
 * @param archive A srcml_archive
 * @param revision_number The operational srcdff revision
 * @return SRCML_STATUS_OK on success
 * @return Status error code on failure
 */
LIBSRCML_DECL int srcml_archive_set_srcdiff_revision(struct srcml_archive* archive, size_t revision_number);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif
