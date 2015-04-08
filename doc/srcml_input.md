srcml(1) -- srcML format conversion, query, and manipulation


## SYNOPSIS

`srcml` [opts] [input]

`srcml` [opts] [input] [opts] [out]

`srcml` [opts] [input] [command]


## DESCRIPTION

The program `srcml` allows you to work with the srcML format. The srcML format
allows the use of XML for addressing, querying, and transformation of source
code. Source code can be translated into the XML source-code representation
srcML, where the code can be queried and manipulated using standard XML tools,
many of which are available directly from the `srcml` program. Code in the
srcML format can be converted back to source code.

In the srcML format, all text from the original source-code file is preserved
including white-space, comments, and preprocessor statements. No preprocessing
of the source code is done. In addition, the tool can be applied to individual
source-code files, or code fragments.

The conversion to the srcML format uses a custom parser that is fast and
tolerant to incomplete source code and uncompilable code.

Standard input may be specified by using the character - in the place of
an input source-code file or providing no input source-code file. Similarly,
standard output may be specified with the - character for the output srcML
file or by not providing an output srcML file.

A source-code language was be specified when input is from standard input.


## OPTIONS

Describe options.

`-HELP_FLAG_SHORT`, `--HELP_FLAG_LONG`
: Output the help and exit.

`-VERSION_FLAG_SHORT`, `--VERSION_FLAG_LONG`
: Output the version of `srcml` then exit.

`--MAX_THREADS_FLAG_LONG`=[threads]
: Sets the maximum number of threads `srcml` can spawn.

`-COMPRESS_FLAG_SHORT`, `--COMPRESS_FLAG_LONG`
: Output is in compressed gzip format. This format can be directly, and
automatically, read as input to `srcml`.

`-OUTPUT_FLAG_SHORT`, `--OUTPUT_FLAG_LONG`=[output-srcML-file]
: Write to output srcML file or URI. By default, it is [stdout://-]
and writes to standard output.

`-OUTPUT_XML_FLAG_SHORT`,`--OUTPUT_XML_FLAG_LONG`
: Outputs XML in srcML format. This is the default when the input is source
code or files containing source code.

`-OUTPUT_SRC_FLAG_SHORT`, `--OUTPUT_SRC_FLAG_LONG`
: Outputs text in source code format. This is the default when the input
is in srcML format.

`--TO_DIR_FLAG_LONG` = [directory]
: Extract all files from srcML and create them in the file system.

`-EXPRESSION_MODE_FLAG_SHORT`, `--EXPRESSION_MODE_FLAG_LONG`
: Translates a single, standalone expression.

`-ARCHIVE_FLAG_SHORT`, `--ARCHIVE_FLAG_LONG`
: Creates a srcML archive, which can contain multiple files in the srcML
format. Default with when provided more than one file or a directory as
input.

`--FILES_FROM_LONG`
: Treats the input (either a file or URI) as a list of source files.
Each file is separately translated and collectively stored into a
single srcML archive. The list has a single filename on each line
starting at the beginning of the line. Blank lines and lines that
begin with the character '\#' are ignored. As with input and output
files, using the character - in place of a file name takes the input
list from standard input.

`-SRC_ENCODING_FLAG_SHORT`, `--SRC_ENCODING_FLAG_LONG`=[<encoding>]
: Sets the input encoding of the source-code file to encoding. The
default is to try to automatically determine this when possible.
Used for any necessary source-code text translation to the encoding used
in the srcML file. Possible encodings can be obtained by using the command
`iconv -l`.

`-XML_ENCODING_FLAG_SHORT`, `--XML_ENCODING_FLAG_LONG`=[encoding]
: Sets the xml encoding of the output srcML file to encoding. The
default is UTF-8. Possible encodings can be obtained by using the
command `iconv -l`.

`--NO_XML_DECL_LONG`
: No output of the default XML declaration. Useful when the output is
to be placed inside another XML document.

`--NO_NAMESPACE_DECL_LONG`
: No output of namespace declarations. Useful when the output is to be
placed inside another XML document.

`--IN_ORDER_FLAG_LONG`
: Enables strict output ordering. Useful for comparison when the output
is an archive of multiple srcML documents.

`--EXTERNAL_LONG`=[arg]
: Runs a user defined external script or application on `srcml` client
output.

`-INTERACTIVE_FLAG_SHORT`, `--INTERACTIVE_FLAG_LONG`
: Default is to use buffered output for speed. For interactive applications
output is issued as soon as parsed. For input from terminal, interactive
is default.

`-DEBUG_FLAG_SHORT`, `--DEBUG_FLAG_LONG`
: When translation errors occur, `srcml` preserves all text, but may
issue incorrect markup. In debug mode the text with the translation
error is marked with a special set of tags with the prefix
SRCML_ERR_NS_PREFIX_DEFAULT from the namespace
SRCML_ERR_NS_URI. Debug mode can also be indicated by defining a
prefix for this namespace URL, e.g.,
`--XMLNS_FLAG:="SRCML_ERR_NS_URI"`.

`-VERBOSE_FLAG_SHORT`, `--VERBOSE_FLAG_LONG`
: Conversion and status information to stderr, including encodings
used. Especially useful with for monitoring progress of the the `TODO`
option, a directory, or source-code archive (e.g. tar.gz). The
signal SIGUSR1 can be used to toggle this option.

`-QUIET_FLAG_SHORT`, `--QUIET_FLAG_LONG`
: Supresses status messages. `TODO Better description`.


## METADATA OPTIONS

This set of options allows control over various metadata stored in the
srcML document.

`-LANGUAGE_FLAG_SHORT`, `--LANGUAGE_FLAG_LONG`=[<language>]
: The programming language of the source-code file. Allowable values are
C, C++, C\#, Java, or AspectJ. The language affects parsing, the allowed
markup, and what is considered a keyword. The value is also stored
individually as an attribute in each unit element.

If not specified, the programming language is based on the file
extension. If the file extension is not available or not in the standard
list, then the program will terminate.

`-DIRECTORY_FLAG_SHORT`, `--DIRECTORY_FLAG_LONG`=[<directory>]
: The value of the directory attribute is typically obtained from the path
of the input filename. This option allows you to specify a different
directory for standard input or where the directory is not contained in
the input path

`-FILENAME_FLAG_SHORT`, `--FILENAME_FLAG_LONG`=[<filename>]
: The value of the filename attribute is typically obtained from the input
filename. This option allows you to specify a different filename for
standard input or where the filename is not contained in the input path.

`-SRCVERSION_FLAG_SHORT`, `--SRCVERSION_FLAG_LONG`=[<version>]
: Sets the value of the attribute version to version. This is a
purely-descriptive attribute, where the value has no interpretation by
`srcml`.

`-XML_ENCODING_FLAG_SHORT`, `--XML_ENCODING_FLAG_LONG`=[<encoding>]
: Sets the output xml encoding to <encoding>. If no <encoding> is specified,
UTF-8 is used.

`-SRC_ENCODING_FLAG_SHORT`, `--SRC_ENCODING_FLAG_LONG`=[<encoding>]
: Describe src encoding

`--HASH_FLAG_LONG`
: Describe adding hash to srcML output.

`--REGISTER_EXTENSION_FLAG_LONG`=[extention=language]
: Sets the extensions to associate with a given language. Note: the
extensions do not contain the '.'.

The programming language of the source-code file. Allowable values are
C, C++, C\#, Java, or AspectJ. The language affects parsing, the allowed
markup, and what is considered a keyword. The value is also stored
individually as an attribute in each unit element.

A common use of this is C++ files that use the .h extension for header
files. By default these will be translated as C source-code files. This
option can be used to override this behaviour.

The rest of the metadata options are for optional attributes. For a
srcML archive this option sets the attribute on the root element.

`-INFO_FLAG_SHORT`, `--INFO_FLAG_LONG`
: Display most metadata, except the unit count (file count) in a srcML
archive, then exit.

`-LONG_INFO_FLAG_SHORT`, `--LONG_INFO_FLAG_LONG`
: Display all metadata, excetp the unit count (file count) in a srcML
archive, then exit.

`--LIST_FLAG_LONG`
: List all files in the srcML archive and exit.

`-PREFIX_FLAG_SHORT`, `--PREFIX_FLAG_LONG`=[URI]
: Display a prefix given by a [URI] and exit. See URI AND PREFIXES.

`--SHOW_UNIT_COUNT_FLAG_LONG`
: Display the number of srcML files (units) in an archive and exit.

`-UNIT_OPTION_SHORT`, `--UNIT_OPTION_LONG` [num]
: Extract individual unit number from srcML.


## SRCML -> SRC OPTIONS

`--SHOW_LANGUAGE_FLAG_LONG`
: Display srcML unit's language and exit.

`--SHOW_DIRECTORY_FLAG_LONG`
: Display srcML unit's directory and exit.

`--SHOW_FILENAME_FLAG_LONG`
: Display srcML unit's filename and exit.

`--SHOW_SRC_VERSION_FLAG_LONG`
: Display srcML unit's version and exit.

`--SHOW_TIMESTAMP_FLAG_LONG`
: Display srcML unit's timestamp and exit.

`--SHOW_HASH_FLAG_LONG`
: Display srcML unit's hash and exit.

`--SHOW_XML_ENCODING_FLAG_LONG`
: Display srcML unit's xml encoding and exit.


## MARKUP EXTENSIONS

Each extensions to the srcML markup has its own namespace. These are
indicated in the srcML document by the declaration of the specific
extension namespace. These flags make it easier to declare.

`--LITERAL_FLAG`
: Additional markup of literal values using the element literal with the
prefix "SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT" in the namespace
"SRCML_EXT_LITERAL_NS_URI".

Can also be specified by declaring a prefix for literal namespace using
the `TODO` option, e.g.,
`XMLNS_FLAG:SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT="SRCML_EXT_LITERAL_NS_URI"`

`--XML_PROCESSING_FLAG_LONG`=[arg]
: Add [arg] as an XML processing instruction. `TODO - Example`


## LINE/COLUMN POSITION

Optional line and column attributes are used to indicate the position of
an element in the original source code document. Both the line and
column start at 1. The column position is based on the tab settings with
a default tab size of 8. Other tab sizes can be set using the
TABS_FLAG.

`TODO`
: Insert line and column attributes into each start element. These
attributes have a default prefix of
"SRCML_EXT_POSITION_NS_PREFIX_DEFAULT" in the namespace
"SRCML_EXT_POSITION_NS_URI".

`--TABS_FLAG`=[tab-size]
: Set the tab size. Default is 8. Use of this option automatically
turns on the position attributes.


## URI AND PREFIX

The following options can be used to change the prefixes.

`XMLNS_FLAG=URI`
: Sets the URI for the default namespace.

`XMLNS_FLAG:PREFIX=URI`
: Sets the namespace prefix PREFIX for the namespace URI.
There is a set of standard URIs for the elements in srcML, each with a
predefined prefix. The predefined URIs and prefixes for them include
(given in xmlns notation):

  * SRCML_CPP_NS_PREFIX_DEFAULT=SRCML_CPP_NS_URI
  * SRCML_ERR_NS_PREFIX_DEFAULT=SRCML_ERR_NS_URI
  * SRCML_EXT_POSITION_NS_PREFIX_DEFAULT=SRCML_EXT_POSITION_NS_URI

These options are an alternative way to turn on options by declaring the
URI for an option. See the MARKUP EXTENSIONS for examples.


## CPP MARKUP OPTIONS

This set of options allows control over how preprocessing regions are
handled, i.e., whether parsing and markup occur. In all cases the text
is preserved.

`--CPP_FLAG_LONG`
: Turns on parsing and markup of preprocessor statements in non-C/C++
languages such as Java. Can also be enabled by defining a prefix for
this cpp namespace URL, e.g.,
`XMLNS_FLAG:SRCML_CPP_NS_PREFIX_DEFAULT="SRCML_CPP_NS_URI"`.

`--CPP_MARKUP_ELSE_FLAG_LONG`
: Place markup in \#else and \#elif regions. Default.

`--CPP_TEXT_ELSE_FLAG_LONG`
: Only place text in \#else and \#elif regions leaving out markup.

`--CPP_MARKUP_IF0_FLAG_LONG`
: Place markup in \#if 0 regions.

`--CPP_TEXT_IF0_FLAG_LONG`
: Only place text in \#if 0 regions leaving out markup. Default.


## QUERY AND TRANSFORM

`--APPLY_ROOT_FLAG_LONG`
: Apply an XSLT program or Xpath query to the root element.

`--RELAXNG_OPTION_LONG`=[file | URI]
: Output individual units that match the RELAXNG file or URI.

`--XPATH_OPTION_LONG`=[expression]
: Apply Xpath [expression] query to each individual unit.

`--XPATH_PARAM_LONG` [NAME]=[VAL]
: Pass a parameter NAME and VAL to the XSLT program.

`--XSLT_LONG`=[file | URI]
: Apply a transformation from an XSLT file to each individual unit.

`--ATTRIBUTE_LONG`=[arg]
: Add attribute [arg] to the Xpath query.

`--ELEMENT_LONG`=[arg]
: Add element [arg] to the Xpath query.


## SIGNAL PROCESSING

The following signals may be used to control srcml:

SIGUSR1
: Toggles verbose option. Useful with multiple input files as in the
`TODO` option.

SIGINT
: Completes current file translation (and output) with multiple input
  files. The input file currently being translated is allowed to
  complete, the complex document is closed, and then the program
  stops. More than one SIGINT causes default behavior.

  This special SIGINT handling only occurs with multiple input files
  in srcML archives.


## EXAMPLES

To translate the C++ source-code file main.cpp into the srcML file
main.cpp.xml:

`srcml` main.cpp -OUTPUT_FLAG_SHORT main.cpp.xml

To translate a C source-code file main.c into the srcML file main.c.xml:

`srcml` -LANGUAGE_FLAG_SHORT=C main.c -OUTPUT_FLAG_SHORT main.c.xml

To translate a Java source-code file main.java into the srcML file
main.java.xml:

`srcml` -LANGUAGE_FLAG_SHORT=Java main.java -OUTPUT_FLAG_SHORT
main.java.xml

To specify the directory, filename, and version for an input file from
standard input:

`srcml` -DIRECTORY_FLAG_SHORT=src -FILENAME_FLAG_SHORT=main.cpp
-VERSION_FLAG_SHORT=1 -OUTPUT_FLAG_SHORT main.cpp.xml

To translate a source-code file in ISO-8859-1 encoding into a srcML file
with UTF-8 encoding:

`srcml` -SRC_ENCODING_FLAG_SHORT=ISO-8859-1 -XML_ENCODING_FLAG_SHORT=UTF-8 main.cpp
-OUTPUT_FLAG_SHORT main.cpp.xml


## RETURN STATUS

STATUS_SUCCESS: Normal

STATUS_ERROR: Error

STATUS_INPUTFILE_PROBLEM: Problem with input file

STATUS_UNKNOWN_OPTION: Unknown option

STATUS_UNKNOWN_ENCODING: Unknown encoding

STATUS_INVALID_LANGUAGE: Invalid language

STATUS_INVALID_OPTION_COMBINATION: Invalid combination of options

STATUS_TERMINATED: Incomplete output due to termination


## CAVEATS

Translation is performed based on local information with no symbol
table. For non-CFG languages, i.e., C/C++, and with macros this may lead
to incorrect markup.

Line endings are normalized in XML formats including srcML.


## BUGS

Java language mode does may not contain every Java language element.

Libxml2 directly supports many encodings beyond UTF-8, UTF-16, and
ISO-8859-1 through iconv. However, the BOM (Byte Order Mark) immediately
before the XML declaration may not be processed correctly by `srcml`
and by other libxml2-based tools (e.g., xmllint). Use the LE or BE
version of the encoding, e.g., UTF-32BE, UTF-32LE, instead.


## SEE ALSO

`srcml`(1)


## AUTHORS

Written by Michael L. Collard


## COPYRIGHT

Copyright (C) 2013-2015 srcML, LLC. (www.srcML.org)

The srcML Toolkit is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

The srcML Toolkit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the srcML Toolkit; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
