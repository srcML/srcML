srcml(1) -- srcML format conversion, query, and manipulation

## SYNOPSIS

`srcml` 
src2srcml
1
-hVnizcgv
LANGUAGE\_FLAG\_SHORT
language
DIRECTORY\_FLAG\_SHORT
directory
FILENAME\_FLAG\_SHORT
filename
SRCVERSION\_FLAG\_SHORT
version
ENCODING\_FLAG\_SHORT
encoding
SRC\_ENCODING\_FLAG\_SHORT
encoding
input-source-code-file
OUTPUT\_FLAG\_SHORT
output-srcML-file

## DESCRIPTION

The program `srcml` allows you to work with the srcML format. 
The srcML format allows the use of XML
for addressing, querying, and transformation of source code. 
Source code can be translated into the XML source-code representation srcML, where
the code can be queried and manipulated using standard XML tools, many of which
are available directly from the `srcml` program. Code in the srcML format can
be converted back to source code.

In the srcML format, all text
from the original source-code file is preserved including white-space,
comments, and preprocessor statements. No preprocessing of the source
code is done. In addition, the tool can be applied to individual
source-code files, or code fragments.

The conversion to the srcML format uses a custom parser that is fast and tolerant to 
incomplete source code and uncompilable code.

Standard input may be specified by using the character - in the place of
an input source-code file or providing no input source-code file.
Similarly, standard output may be specified with the - character for the
output srcML file or by not providing an output srcML file.

A source-code language was be specified when input is from standard
input.

OPTIONS
=======

`-h`, `--help`:
Output the help and exit.

`-v`, `--version`:
Output the version of `srcml` then exit.

`EXPRESSION_MODE_FLAG_SHORT, EXPRESSION_MODE_FLAG`:
Translates a single, standalone expression.

`COMPOUND_FLAG_SHORT`, `--archive`:
Creates a srcML archive, which can contain multiple files in the srcML format. Default with more then one input file, a directory, or the `--archive` option.

``:
Treats the input file as a list of source files. Each file is
separately translated and collectively stored into a single srcML
archive. The list has a single filename on each line starting at the
beginning of the line. Blank lines and lines that begin with the
character '\#' are ignored. As with input and output files, using
the character - in place of a file name takes the input list from
standard input.

`SRC_ENCODING_FLAG_SHORT, `--src-encoding`=[<encoding>]:
Sets the input encoding of the source-code file to encoding. The
default is to try to automatically determine this when possible.
Used for any necessary source-code text translation to the encoding used in the srcML file. Possible encodings can be obtained by using the command `iconv -l`.

`ENCODING_FLAG_SHORT, ENCODING_FLAG=encoding`:
Sets the xml encoding of the output srcML file to encoding. The
    default is UTF-8. Possible encodings can be obtained by using the
    command `iconv -l`.

``:
No output of the default XML declaration. Useful when the output is
    to be placed inside another XML document.

``:
No output of namespace declarations. Useful when the output is to be
    placed inside another XML document.

`COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG`:
Output is in compressed gzip format. This format can be directly,
    and automatically, read by `srcml2src`.

`INTERACTIVE_FLAG_SHORT, INTERACTIVE_FLAG`:
Default is to use buffered output for speed. For interactive
    applications output is issued as soon as parsed.

    For input from terminal, interactive is default.

`DEBUG_FLAG_SHORT, DEBUG_FLAG`:
When translation errors occur src2srcml preserves all text, but may
    issue incorrect markup. In debug mode the text with the translation
    error is marked with a special set of tags with the prefix
    SRCML\_ERR\_NS\_PREFIX\_DEFAULT from the namespace
    SRCML\_ERR\_NS\_URI. Debug mode can also be indicated by defining a
    prefix for this namespace URL, e.g.,
    `XMLNS_FL:

    ="SRCML_ERR_NS_URI"`.

`VERBOSE_FLAG_SHORT, VERBOSE_FLAG`:
Conversion and status information to stderr, including encodings
    used. Especially useful with for monitoring progress of the the ``
    option, a directory, or source-code archive (e.g. tar.gz). The
    signal SIGUSR1 can be used to toggle this option.

Metadata Options
================

This set of options allows control over various metadata stored in the
srcML document.

LANGUAGE\_FLAG\_SHORT, LANGUAGE\_FLAG=
language
The programming language of the source-code file. Allowable values are
C, C++, C\#, Java, or AspectJ. The language affects parsing, the allowed
markup, and what is considered a keyword. The value is also stored
individually as an attribute in each unit element.

If not specified, the programming language is based on the file
extension. If the file extension is not available or not in the standard
list, then the program will terminate.

REGISTER\_EXTENSION\_FLAG=
extention=language
Sets the extensions to associate with a given language. Note: the
extensions to not contain the '.'.

The programming language of the source-code file. Allowable values are
C, C++, C\#, Java, or AspectJ. The language affects parsing, the allowed
markup, and what is considered a keyword. The value is also stored
individually as an attribute in each unit element.

A common use of this is C++ files that use the .h extension for header
files. By default these will be translated as C source-code files. This
option can be used to override this behaviour.

The rest of the metadata options are for optional attributes. For a
srcML archive this option sets the attribute on the root element.

DIRECTORY\_FLAG\_SHORT, DIRECTORY\_FLAG=
directory
The value of the directory attribute is typically obtained from the path
of the input filename. This option allows you to specify a different
directory for standard input or where the directory is not contained in
the input path.

FILENAME\_FLAG\_SHORT, FILENAME\_FLAG=
filename
The value of the filename attribute is typically obtained from the input
filename. This option allows you to specify a different filename for
standard input or where the filename is not contained in the input path.

SRCVERSION\_FLAG\_SHORT, SRCVERSION\_FLAG=
version
Sets the value of the attribute version to version. This is a
purely-descriptive attribute, where the value has no interpretation by
src2srcml.

Markup Extensions
=================

Each extensions to the srcML markup has its own namespace. These are
indicated in the srcML document by the declaration of the specific
extension namespace. These flags make it easier to declare.

LITERAL\_FLAG
Additional markup of literal values using the element literal with the
prefix "SRCML\_EXT\_LITERAL\_NS\_PREFIX\_DEFAULT" in the namespace
"SRCML\_EXT\_LITERAL\_NS\_URI".

Can also be specified by declaring a prefix for literal namespace using
the `` option, e.g.,
`XMLNS_FLAG:SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT="SRCML_EXT_LITERAL_NS_URI"`

OPERATOR\_FLAG
Additional markup of operators values using the element operator with
the prefix "SRCML\_EXT\_OPERATOR\_NS\_PREFIX\_DEFAULT" in the namespace
"SRCML\_EXT\_OPERATOR\_NS\_URI".

Can also be specified by declaring a prefix for operator namespace using
the `` option, e.g.,
`XMLNS_FLAG:SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT="SRCML_EXT_OPERATOR_NS_URI"`

MODIFIER\_FLAG
Additional markup of type modifiers using the element modifier with the
prefix "SRCML\_EXT\_MODIFIER\_NS\_PREFIX\_DEFAULT" in the namespace
"SRCML\_EXT\_MODIFIER\_NS\_URI".

Can also be specified by declaring a prefix for the modifier namespace
using the `` option, e.g.,
`XMLNS_FLAG:SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT="SRCML_EXT_MODIFIER_NS_URI"`

LINE/COLUMN POSITION
====================

Optional line and column attributes are used to indicate the position of
an element in the original source code document. Both the line and
column start at 1. The column position is based on the tab settings with
a default tab size of 8. Other tab sizes can be set using the
TABS\_FLAG.

``

:   Insert line and column attributes into each start element. These
    attributes have a default prefix of
    "SRCML\_EXT\_POSITION\_NS\_PREFIX\_DEFAULT" in the namespace
    "SRCML\_EXT\_POSITION\_NS\_URI".

``

:   Set the tab size. Default is 8. Use of this option automatically
    turns on the position attributes.

URI AND PREFIX
==============

There is a set of standard URIs for the elements in srcML, each with a
predefined prefix. The predefined URIs and prefixes for them include
(given in xmlns notation):

  -------- ----------------------------------------------------------------
  PREFIX   URI

  (nil)    SRCML\_SRC\_NS\_URI

  SRCML\_C SRCML\_CPP\_NS\_URI
  PP\_NS\_ 
  PREFIX\_ 
  DEFAULT  

  SRCML\_E SRCML\_ERR\_NS\_URI
  RR\_NS\_ 
  PREFIX\_ 
  DEFAULT  

  SRCML\_E SRCML\_EXT\_LITERAL\_NS\_URI
  XT\_LITE 
  RAL\_NS\ 
  _PREFIX\ 
  _DEFAULT 

  SRCML\_E SRCML\_EXT\_OPERATOR\_NS\_URI
  XT\_OPER 
  ATOR\_NS 
  \_PREFIX 
  \_DEFAUL 
  T        

  SRCML\_E SRCML\_EXT\_MODIFIER\_NS\_URI
  XT\_MODI 
  FIER\_NS 
  \_PREFIX 
  \_DEFAUL 
  T        

  SRCML\_E SRCML\_EXT\_POSITION\_NS\_URI
  XT\_POSI 
  TION\_NS 
  \_PREFIX 
  \_DEFAUL 
  T        
  -------- ----------------------------------------------------------------

The following options can be used to change the prefixes.

`XMLNS_FLAG=URI`

:   Sets the URI for the default namespace.

`XMLNS_FLAG:PREFIX=URI`

:   Sets the namespace prefix PREFIX for the namespace URI.

These options are an alternative way to turn on options by declaring the
URI for an option. See the MARKUP EXTENSIONS for examples.

CPP MARKUP OPTIONS
==================

This set of options allows control over how preprocessing regions are
handled, i.e., whether parsing and markup occur. In all cases the text
is preserved.

``

:   Turns on parsing and markup of preprocessor statements in non-C/C++
    languages such as Java. Can also be enabled by defining a prefix for
    this cpp namespace URL, e.g.,
    `XMLNS_FLAG:SRCML_CPP_NS_PREFIX_DEFAULT="SRCML_CPP_NS_URI"`.

``

:   Place markup in \#else and \#elif regions. Default.

``

:   Only place text in \#else and \#elif regions leaving out markup.

``

:   Place markup in \#if 0 regions.

``

:   Only place text in \#if 0 regions leaving out markup. Default.

SIGNAL PROCESSING
=================

The following signals may be used to control src2srcml:

SIGUSR1

:   Toggles verbose option. Useful with multiple input files as in the
    `` option.

SIGINT

:   Completes current file translation (and output) with multiple input
    files. The input file currently being translated is allowed to
    complete, the complex document is closed, and then the program
    stops. More than one SIGINT causes default behavior.

    This special SIGINT handling only occurs with multiple input files
    in srcML archives.

USAGE
=====

To translate the C++ source-code file main.cpp into the srcML file
main.cpp.xml:

`src2srcml` main.cpp OUTPUT\_FLAG\_SHORT main.cpp.xml

To translate a C source-code file main.c into the srcML file main.c.xml:

`src2srcml` LANGUAGE\_FLAG=C main.c OUTPUT\_FLAG\_SHORT main.c.xml

To translate a Java source-code file main.java into the srcML file
main.java.xml:

`src2srcml` LANGUAGE\_FLAG=Java main.java OUTPUT\_FLAG\_SHORT
main.java.xml

To specify the directory, filename, and version for an input file from
standard input:

`src2srcml` DIRECTORY\_FLAG=src FILENAME\_FLAG=main.cpp
VERSION\_FLAG=1 - OUTPUT\_FLAG\_SHORT main.cpp.xml

To translate a source-code file in ISO-8859-1 encoding into a srcML file
with UTF-8 encoding:

`src2srcml` SRC\_ENCODING\_FLAG=ISO-8859-1 ENCODING\_FLAG=UTF-8 main.cpp
OUTPUT\_FLAG\_SHORT main.cpp.xml

RETURN STATUS
=============

STATUS\_SUCCESS: Normal

STATUS\_ERROR: Error

STATUS\_INPUTFILE\_PROBLEM: Problem with input file

STATUS\_UNKNOWN\_OPTION: Unknown option

STATUS\_UNKNOWN\_ENCODING: Unknown encoding

STATUS\_INVALID\_LANGUAGE: Invalid language

STATUS\_INVALID\_OPTION\_COMBINATION: Invalid combination of options

STATUS\_TERMINATED: Incomplete output due to termination

CAVEATS
=======

Translation is performed based on local information with no symbol
table. For non-CFG languages, i.e., C/C++, and with macros this may lead
to incorrect markup.

Line endings are normalized in XML formats including srcML.

BUGS
====

Java language mode does may not contain every Java language element.

Libxml2 directly supports many encodings beyond UTF-8, UTF-16, and
ISO-8859-1 through iconv. However, the BOM (Byte Order Mark) immediately
before the XML declaration may not be processed correctly by `srcml2src`
and by other libxml2-based tools (e.g., xmllint). Use the LE or BE
version of the encoding, e.g., UTF-32BE, UTF-32LE, instead.

SEE ALSO
========

`srcml2src`(1)

AUTHOR
======

Written by Michael L. Collard Original author and Huzefa Kagdi Original
author
