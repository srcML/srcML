srcML format conversion, query, and manipulation


## SYNOPSIS

`srcml` \[[input-options\]][INPUT OPTIONS] \[[metadata-options\]][METADATA OPTIONS] \[[metadata-accessors\]][METADATA OPTIONS] | \[[xml-form\]][XML FORM] \[[transformations\]][TRANSFORMATIONS] \[[output-options\]][OUTPUT OPTIONS] \[[general-options\]][GENERAL OPTIONS] \[input\] \[output\]


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


## INPUT OPTIONS

The following describe options on the input file or URI. The format of the
input affects the format of the output, unless otherwise specified in the
[output options][OUTPUT OPTIONS]. If the input is
source-code files, then the output is expected to be in srcML format. If
the input is in srcML format, then the output is in source-code format.

`--files-from`
: Treats the input (either a file or URI) as a list of source files.
Each file is separately translated and collectively stored into a
single srcML archive. The list has a single filename on each line
starting at the beginning of the line. Blank lines and lines that
begin with the character '\#' are ignored. As with input and output
files, using the character '-' in place of a file name takes the input
list from standard input.

`-l`, `--language=<language>`
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

`--register-ext="<extension>=<language>"`
: Sets the <extension> to associate with a given <language>. Note
that the extensions do not contain the period character '.'.

	A common use of this is C++ files that use the .h extension for header
files. By default these will be translated as C source-code files. This
option can be used to override this behaviour.

`-t`, `--src-encoding=<encoding>`
: Sets the input encoding of the source-code file to encoding. The
default is to try to automatically determine this when possible.
Used for any necessary source-code text translation to the encoding used
in the srcML file. Possible encodings can be obtained by using the command
`iconv -l`.



## OUTPUT OPTIONS

`-o`, `--output=<output-srcML-file>`
: Write to output srcML file, URI, or source code file.
By default, it is [stdout://-] and writes to standard output.

`-r`, `--archive`
: Creates a srcML archive, which can contain multiple files in the srcML
format. Default with when provided more than one file or a directory as
input.

`-X`,`--output-xml`
: Outputs XML in srcML format. This is the default when the input is source
code or files containing source code.

`-S`, `--output-src`
: Outputs text in source code format. This is the default when the input
is in srcML format.

`-z`, `--compress`
: Output is in compressed gzip format. This format can be directly, and
automatically, read as input to `srcml`.

`--to-dir=<directory>`
: Extract all files from srcML and create them in the file system at
<directory>.

`--in-order`
: Enables strict output ordering. Useful for comparison when the output
is an archive of multiple srcML documents.

`--line-ending="<environment>"`
: Set line endings for a specific <environment>. Acceptable environments
are "Unix" or "Windows".

`-c`, `--interactive`
: Default is to use buffered output for speed. For interactive applications
output is issued as soon as parsed. For input from terminal, interactive
is default.



## MARKUP OPTIONS

### POSITION

Optional line and column attributes are used to indicate the position of
an element in the original source code document. Both the line and
column start at 1. The column position is based on the tab settings with
a default tab size of 8. Other tab sizes can be set using the
tabs.

`--position`
: Insert line and column attributes into each start element. These
attributes have a default prefix of
"pos" in the namespace
"http://www.sdml.info/srcML/position".

`--tabs=<tabsize>`
: Set the tab size. Default is 8. Use of this option automatically
turns on the position attributes.


### CPP

This set of options allows control over how preprocessing regions are
handled, i.e., whether parsing and markup occur. In all cases the text
is preserved.

`--cpp`
: Turns on parsing and markup of preprocessor statements in non-C/C++
languages such as Java. Can also be enabled by defining a prefix for
this cpp namespace URL, for example,
`--xmlns:cpp="http://www.sdml.info/srcML/cpp"`.

`--cpp-no-markup-else`
: Only place text in \#else and \#elif regions, leaving out markup.
The default is to markup these regions.

`--cpp-markup-if0`
: Place markup in \#if 0 regions. The default is to only place text
in these regions, leaving out markup.



## XML FORM

The following options control the format of the XML.

`-x`, `--xml-encoding=<encoding>`
: Sets the xml encoding of the output srcML file to encoding. The
default is UTF-8. Possible encodings can be obtained by using the
command `iconv -l`. The attribute is stored only on the root element.

`--no-xml-declaration`
: No output of the default XML declaration. Useful when the output is
to be placed inside another XML document.

`--no-namespace-decl`
: No output of namespace declarations. Useful when the output is to be
placed inside another XML document.

`--xml-processing=<arg>`
: Add <arg> as an XML processing instruction.


The following options can be used to change the prefixes.
Each extensions to the srcML markup has its own namespace. These are
indicated in the srcML document by the declaration of the specific
extension namespace. These flags make it easier to declare, and are
an alternative way to turn on options by declaring the URI for an option.

`--xmlns=<uri>`
: Sets the URI for the default namespace.

`--xmlns:<prefix>=<uri>`
: Sets the namespace prefix PREFIX for the namespace URI.
There is a set of standard URIs for the elements in srcML, each with a
predefined prefix. The predefined URIs and prefixes for them include
(given in xmlns notation):

	* cpp=http://www.sdml.info/srcML/cpp
	* err=http://www.sdml.info/srcML/srcerr
	* pos=http://www.sdml.info/srcML/position



## METADATA OPTIONS

This set of options allows control over various metadata stored in the
srcML document.

`-f`, `--filename=<filename>`
: The value of the filename attribute is typically obtained from the input
filename. This option allows you to specify a different filename for
standard input or where the filename is not contained in the input path.

`-s`, `--src-version=<version>`
: Sets the value of the attribute version to version. This is a
purely-descriptive attribute, where the value has no interpretation by
`srcml`. The attribute is applied to the root element, and in the case
of a srcML archive, it is also applied to each unit in the archive.

`--hash`
: The value of the hash attribute is a SHA-1 hash generated based on
the contents of the source-code file. This is enabled by default when
working with srcML archives.

`--timestamp`
: Sets the timestamp of the output srcML file to the last modified
time of the input source-code file. For a source archive, this is the
last modified time based on the archive files. For source files from
a file system, it is the modified timestamp from the file system. For
inputs from a URL, it is the current time (i.e., last access time).
For inputs from version control, it is the last modified time based
on the metadata from the last commit.

`-p`, `--prefix=<uri>`
: Display a prefix given by a <URI> and exit. See [XML FORM][].

`-i`, `--info=[<format>]`
: Display most metadata, except the unit count (file count) in a srcML
archive, then exit. A format may be specified that allows you to
specify which information to show. It works similar to printf, with
the exception of optional header and footer information declared
before and after curly-brackets, respectively.

	Three types of placeholders exist: (1) those that hold metadata
on a per archive basis, (2) those that hold metadata on a per unit
basis, and (3) information left within the header or footer of the
output which is thus displayed only once.

	- %h: hash attribute on the unit
	- %d: directory attribute on the unit
	- %f: file name attribute on the unit
	- %v: version attribute on the unit
	- %x: XML encoding attribute on the unit
	- %s: source encoding attribute on the unit
	- %i: index of the unit within the archive
	- %D: directory attribute on the archive
	- %F: file attribute on the archive
	- %V: version attribute on the archive
	- %X: XML encoding on the archive
	- %S: source encoding attribute on the archive
	- %C: total number of units
	
	If the format string contains any placeholders for an attribute
on the unit, then all information in the string is provided on a per
unit basis, thus duplicating information in each unit for the per archive
attributes. If the lowest granularity of the placeholders is for an 
attribute on the archive, then all information in the string is provided
on a per archive basis.
	
	If you wish to display information in either a header or footer, or both,
a delimiter (curly-brackets) is used to separate the first header section
from the main per unit/archive section, and from the last footer section.
For example, provided an archive with two units, the format string "The
directory was %d with a filename of %f" would show something like this:

	The directory was sub with a filename of a.cpp
	The directory was sub with a filename of b.cpp

	In another example, the format string "%C { %d %f }" would show:
	2
	sub a.cpp
	sub b.cpp


## TRANSFORMATIONS

`--apply-root`
: Apply an XSLT program or Xpath query to the root element.

`--relaxng=<file|uri>`
: Output individual units that match the RELAXNG <file> or <uri>.

`--xpath=<expression>`
: Apply Xpath <expression> query to each individual unit.

`--xpathparam <parameter>=<value>`
: Pass a <parameter> name and its <value> to the XSLT program.

`--xslt=<file|uri>`
: Apply a transformation from an XSLT file to each individual unit.

`--attribute=<arg>`
: Add attribute <arg> to the Xpath query.

`--element=<arg>`
: Add element <arg> to the Xpath query.

`-U`, `--unit <n>`
: Extract individual <n>th unit from srcML archive.

`--external=<arg>`
: Runs a user defined external script or application on `srcml` client
output.



## GENERAL OPTIONS

`-h`, `--help`
: Output the help and exit.

`-V`, `--version`
: Output the version of `srcml` then exit.

`--max-threads=<num>`
: Sets the maximum number of threads `srcml` can spawn.

`-v`, `--verbose=[<format>]`
: Conversion and status information to stderr, including encodings
used. Especially useful with for monitoring progress of the the `TODO`
option, a directory, or source-code archive (e.g. tar.gz). The
signal SIGUSR1 can be used to toggle this option.

	A format may be specified for the verbose output. See `-i`
from [METADATA OPTIONS][] for formatting placeholders. In addition, the
following placeholders may be used:

	- %T: files translated
	- %S: files skipped
	- %E: files with errors

`-q`, `--quiet`
: Supresses status messages. `TODO Better description`.



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

`srcml main.cpp -o main.cpp.xml`

To translate a C source-code file main.c into the srcML file main.c.xml:

`srcml -l=C main.c -o main.c.xml`

To translate a Java source-code file main.java into the srcML file
main.java.xml:

`srcml -l=Java main.java -o
main.java.xml`

To specify the directory, filename, and version for an input file from
standard input:

`srcml -d=src -f=main.cpp
-V=1 -o main.cpp.xml`

To translate a source-code file in ISO-8859-1 encoding into a srcML file
with UTF-8 encoding:

`srcml -t=ISO-8859-1 -x=UTF-8 main.cpp
-o main.cpp.xml`


## RETURN STATUS

* 0: Normal
* idk: Error
* idk: Problem with input file
* idk: Unknown option
* idk: Unknown encoding
* idk: Invalid language
* idk: Invalid combination of options
* idk: Incomplete output due to termination



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
