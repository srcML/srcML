srcML format conversion, query, and manipulation


## SYNOPSIS

`srcml` \[[srcML-options\]][CREATING SRCML] \[[transformations\]][TRANSFORMATIONS] \[[output-src-options\]][EXTRACTING SOURCE CODE] \[[general-options\]][GENERAL OPTIONS] \[input\] \[output\]


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

Standard input may be specified by using the character '-' in the place of
an input source-code file or providing no input source-code file. Similarly,
standard output may be specified with the '-' character for the output srcML
file or by not providing an output srcML file.

A source-code language must be specified when input is from standard input.


## GENERAL OPTIONS

`-HELP_FLAG_SHORT`, `--HELP_FLAG_LONG`
: Output the help and exit.

`-VERSION_FLAG_SHORT`, `--VERSION_FLAG_LONG`
: Output the version of `srcml` then exit.

`-VERBOSE_FLAG_SHORT`, `--VERBOSE_FLAG_LONG`
: Conversion and status information to stderr, including encodings
used. Especially useful with for monitoring progress of the from file list
option, a directory, or source-code archive (e.g. tar.gz). The
signal SIGUSR1 can be used to toggle this option.

`-QUIET_FLAG_SHORT`, `--QUIET_FLAG_LONG`
: Supresses status messages.

`-INFO_FLAG_SHORT`, `--INFO_FLAG_LONG`
: Display most metadata, except the unit count (file count) in a srcML
archive, then exit.

`--LONG_INFO_FLAG_LONG`
: Display most metadata including the unit (file) count in a srcML archive,
then exit.

`--MAX_THREADS_FLAG_LONG=<num>`
: Sets the maximum number of threads `srcml` can spawn.



## CREATING SRCML

The following describe options for creating srcML. The format of the
input affects the format of the output, unless otherwise specified.
If the input is source-code files, then the output is expected to be
in srcML format. If the input is in srcML format, then the output is
in source-code format.

`-LANGUAGE_FLAG_SHORT`, `--LANGUAGE_FLAG_LONG=<language>`
: The programming language of the source-code file. Allowable values are
C, C++, C\#, or Java. The language affects parsing, the allowed
markup, and what is considered a keyword. The value is also stored
individually as an attribute in each unit element.

	If not specified, the programming language is based on the file
extension. Language must be specified if using standard input. If the
file extension is not available or not in the standard list, then the
program will skip that file. This allows you to run `srcml` on a
project directory with many source-code files and config files, and
only the files with supported extensions will be parsed by `srcml`.

`--REGISTER_EXTENSION_FLAG_LONG="<extension>=<language>"`
: Sets the <extension> to associate with a given <language>. Note
that the extensions do not contain the period character '.'.

	A common use of this is C++ files that use the .h extension for header
files. By default these will be translated as C source-code files. This
option can be used to override this behavior.

`--SRC_ENCODING_FLAG_LONG=<encoding>`
: Sets the input encoding of the source-code file to <encoding>. The
default is to try to automatically determine this when possible.
Used for any necessary source-code text translation to the encoding used
in the srcML file. Possible encodings, such as "UTF-16", "ISO-10646-UCS-2",
or "ISO-8859-1" can be obtained by using the command
`iconv -l` on UNIX platforms.

`--FILES_FROM_LONG=<file>`
: Treats the input file as a list of source files.
Each file is separately translated and collectively stored into a
single srcML archive. The list has a single filename on each line
starting at the beginning of the line. Blank lines and lines that
begin with the character '\#' are ignored. As with input and output
files, using the character '-' in place of a file name takes the input
list from standard input.

`-OUTPUT_FLAG_SHORT`, `--OUTPUT_FLAG_LONG=<output-file>`
: Write to output srcML file, URI, or source code file.
By default, it writes to standard output.

`-OUTPUT_XML_FLAG_SHORT`,`--OUTPUT_XML_FLAG_LONG`
: Outputs XML in srcML format. This is the default when the input is source
code or files containing source code.

`-ARCHIVE_FLAG_SHORT`, `--ARCHIVE_FLAG_LONG`
: Creates a srcML archive, which can contain multiple files in the srcML
format. Default with when provided more than one file or a directory as
input.

`--IN_ORDER_FLAG_LONG`
: Enables strict output ordering. Useful for comparison when the output
is an archive of multiple srcML documents.

`-TEXT_FLAG_SHORT`, `--TEXT_FLAG_LONG=<text>`
: Processes the raw <text> string as input, respecting the escaped newline
charater. Useful for short text inputs. Equivalent to `echo <text> | srcml`
with the relevant language specification.


### MARKUP OPTIONS

Optional line and column attributes are used to indicate the position of
an element in the original source code document. Both the line and
column start at 1. The column position is based on the tab settings with
a default tab size of 8. Other tab sizes can be set using the
TABS_FLAG.

`--POSITION_FLAG_LONG`
: Insert line and column attributes into each start element. These
attributes have a default prefix of
"SRCML_EXT_POSITION_NS_PREFIX_DEFAULT" in the namespace
"SRCML_EXT_POSITION_NS_URI".

`--TABS_FLAG=<tabsize>`
: Set the tab size. Default is 8. Use of this option automatically
turns on the position attributes.


This set of options allows control over how preprocessing regions are
handled, i.e., whether parsing and markup occur. In all cases the text
is preserved.

`--CPP_FLAG_LONG`
: Turns on parsing and markup of preprocessor statements in non-C/C++
languages such as Java. Can also be enabled by defining a prefix for
this cpp namespace URL, for example,
`--XMLNS_FLAG:SRCML_CPP_NS_PREFIX_DEFAULT="SRCML_CPP_NS_URI"`.

`--CPP_NO_MARKUP_ELSE_FLAG_LONG`
: Only place text in \#else and \#elif regions, leaving out markup.
The default is to markup these regions.

`--CPP_MARKUP_IF0_FLAG_LONG`
: Place markup in \#if 0 regions. The default is to only place text
in these regions, leaving out markup.


### XML FORM

The following options control the format of the XML.

`-XML_ENCODING_FLAG_SHORT`, `--XML_ENCODING_FLAG_LONG=<encoding>`
: Sets the xml encoding of the output srcML file to encoding. The
default is UTF-8. Possible encodings can be obtained by using the
command `iconv -l` on UNIX platforms. The attribute is stored only
on the root element.

`--NO_XML_DECL_LONG`
: No output of the default XML declaration. Useful when the output is
to be placed inside another XML document.

`--NO_NAMESPACE_DECL_LONG`
: No output of namespace declarations. Useful when the output is to be
placed inside another XML document.

The following options can be used to change the prefixes.
Each extensions to the srcML markup has its own namespace. These are
indicated in the srcML document by the declaration of the specific
extension namespace. These flags make it easier to declare, and are
an alternative way to turn on options by declaring the URI for an option.

`--XMLNS_FLAG=<uri>`
: Sets the URI for the default namespace.

`--XMLNS_FLAG:<prefix>=<uri>`
: Sets the namespace prefix PREFIX for the namespace URI.
There is a set of standard URIs for the elements in srcML, each with a
predefined prefix. The predefined URIs and prefixes for them include
(given in xmlns notation):

	* SRCML_CPP_NS_PREFIX_DEFAULT=SRCML_CPP_NS_URI
	* SRCML_ERR_NS_PREFIX_DEFAULT=SRCML_ERR_NS_URI
	* SRCML_EXT_POSITION_NS_PREFIX_DEFAULT=SRCML_EXT_POSITION_NS_URI


### METADATA OPTIONS

This set of options allows control over various metadata stored in the
srcML document.

`-FILENAME_FLAG_SHORT`, `--FILENAME_FLAG_LONG=<filename>`
: The value of the filename attribute is typically obtained from the input
filename. This option allows you to specify a different filename for
standard input or where the filename is not contained in the input path.

`--URL_FLAG_LONG=<url>`
: The url attribute on the root element can be defined. This is purely
descriptive and has no interpretation by `srcml`. It is useful for specifying
a directory or defining the source protocol.

`-SRCVERSION_FLAG_SHORT`, `--SRCVERSION_FLAG_LONG=<version>`
: Sets the value of the attribute version to version. This is a
purely-descriptive attribute, where the value has no interpretation by
`srcml`. The attribute is applied to the root element, and in the case
of a srcML archive, it is also applied to each unit in the archive.

`--HASH_FLAG_LONG`
: The value of the hash attribute is a SHA-1 hash generated based on
the contents of the source-code file. This is enabled by default when
working with srcML archives.

`--TIMESTAMP_FLAG_LONG`
: Sets the timestamp of the output srcML file to the last modified
time of the input source-code archive. This is the
last modified time based on the archive files.

The following options allow viewing various metadata stored in the
srcML document.

`-PREFIX_FLAG_SHORT`, `--PREFIX_FLAG_LONG=<uri>`
: Display a prefix given by a <URI> and exit. See [XML FORM][].

`--SHOW_FILENAME_FLAG_LONG`
: Display the filename and exit.

`--SHOW_LANGUAGE_FLAG_LONG`
: Display language and exit.

`--SHOW_URL_FLAG_LONG`
: Display URL of the root element and exit.

`--SHOW_SRC_VERSION_FLAG_LONG`
: Display the source-code version attribute and exit.

`--SHOW_TIMESTAMP_FLAG_LONG`
: Display the timestamp attribute and exit.

`--SHOW_HASH_FLAG_LONG`
: Display the hash value and exit.

`--SHOW_ENCODING_FLAG_LONG`
: Display the XML encoding and exit.

`--SHOW_UNIT_COUNT_FLAG_LONG`
: Display the unit count and exit.


### EXAMPLES

srcml input.cpp
: Create a srcML unit from input.cpp, using C++ parsing rules,
and output to standard out.

echo "int a;" | srcml -LANGUAGE_FLAG_SHORT C++
: Create a srcML unit from standard in, using C++ parsing rules,
and output to standard out.

srcml dir.xml --SHOW_UNIT_COUNT_FLAG_LONG
: Create a srcML archive from all files contained in the dir directory, using
their extensions to determine the markup parsing rules, and
output the number of units contained in the archive to standard out.

srcml input.java --CPP_FLAG_LONG
: Create a srcML unit from input.java, using Java parsing
rules as well as C++ parsing rules for preprocessor directives.



## EXTRACTING SOURCE CODE

The following describe options that are only applicable for when
the output is desired in source-code format.

`-OUTPUT_FLAG_SHORT`, `--OUTPUT_FLAG_LONG=<output-file>`
: Write to output srcML file, URI, or source code file.
By default, it writes to standard output.

`-OUTPUT_SRC_FLAG_SHORT`, `--OUTPUT_SRC_FLAG_LONG`
: Outputs text in source code format. This is the default when the input
is in srcML format.

`--TO_DIR_FLAG_LONG=<directory>`
: Extract all files from srcML and create them in the file system at
<directory>.


### EXAMPLES

srcml dir/ -OUTPUT_FLAG_SHORT dir.xml
: Create a srcML archive from all files contained in the dir directory, using
their extensions to determine the markup parsing rules, and
write the resulting srcML archive to dir.xml.

srcml archive.xml --TO_DIR_FLAG_LONG=.
: Re-create all files based on the srcML units in archive.xml, using the current
directory as the root directory.



## TRANSFORMATIONS

`--APPLY_ROOT_FLAG_LONG`
: Apply an XSLT program or Xpath query to the root element.

`--RELAXNG_OPTION_LONG=<file|uri>`
: Output individual units that match the RELAXNG <file> or <uri>.

`--XPATH_OPTION_LONG=<expression>`
: Apply Xpath <expression> query to each individual unit.

`--XSLT_LONG=<file|uri>`
: Apply a transformation from an XSLT <file> or <uri> to each individual unit.

`--ATTRIBUTE_LONG=<arg>`
: Add attribute <arg> to the Xpath query.

`--ELEMENT_LONG=<arg>`
: Add element <arg> to the Xpath query.

`-UNIT_OPTION_SHORT`, `--UNIT_OPTION_LONG <n>`
: Extract individual <n>th unit from srcML archive.


### EXAMPLES

srcml a.cpp --XPATH_OPTION_LONG="//src:name" --ATTRIBUTE_LONG="cpp:foo=test"
: Convert a.cpp to srcML and add the attribute cpp:foo=test to all src:name
elements as found by the XPath query. Output the results to standard out.

srcml archive.xml --xpath "//src:unit/@filename"
: Execute the XPath query on archive.xml, outputting the filename attribute
of each unit in the archive to standard out.



## SIGNAL PROCESSING

The following signals may be used to control srcml:

SIGUSR1
: Toggles verbose option. Useful with multiple input files.

SIGINT
: Completes current file translation (and output) with multiple input
files. The input file currently being translated is allowed to
complete, the complex document is closed, and then the program
stops. More than one SIGINT causes default behavior.

	This special SIGINT handling only occurs with multiple input files
in srcML archives.



## RETURN STATUS


* STATUS_SUCCESS: Normal
* STATUS_ERROR: Error
* STATUS_INVALID_ARGUMENT: Invalid argument
* STATUS_INPUTFILE_PROBLEM: Invalid input
* SRCML_STATUS_INVALID_IO_OPERATION: Invalid read I/O operation (such as write on read only archive)
* SRCML_STATUS_IO_ERROR: Error with input
* SRCML_STATUS_UNINITIALIZED_UNIT: Uninitialized unit
* SRCML_STATUS_UNSET_LANGUAGE: Unset language
* SRCML_STATUS_NO_TRANSFORMATION: There are no transformations



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

Report bugs to collard@uakron.edu



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
