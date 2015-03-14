srcml2src
1
srcml2src 1.0
srcml2src
translates the XML source-code representation srcML into source code and
provides utility features for the srcML format
srcml2src
-nizcgv
LANGUAGE\_FLAG
FILENAME\_FLAG
DIRECTORY\_FLAG
ENCODING\_FLAG
SRC\_ENCODING\_FLAG
encoding
input-srcML-file
OUTPUT\_FLAG\_SHORT
output-source-code-file
DESCRIPTION
===========

The program `srcml2src` translates from the XML source-code
representation srcML into source code. The srcML format allows for XML
addressing, querying, and transformation of source code. The srcML
format for a source-code file is typically produced by `src2srcml`.

The combination of `src2srcml` and `srcml2src` allows for full
round-trip transformation of source-code files without any loss of
comments, white space, and preprocessing statements.

Conversion from the srcML format to source code is relatively
straightforward. All XML tags are removed and escaped characters are
unescaped. Special srcML elements for characters not permitted in XML,
e.g., formfeed, are converted back to their original characters.

In addition to conversion, `srcml2src` provides various srcML utilities
such as extracting metadata, extracting individual files in a srcML
archive (as either text or XML), and extracting metadata from individual
files in a srcML archive. It can also be used to extract all the
individual files in a srcML archive.

Further, `srcml2src` allows for querying srcML using XPath expressions
and RelaxNG, and transformation using XLST. With a srcML archive, a
query/transformation is applied individually to each file and the
results combined. This allows the XPath, XSLT, or RelaxNG to work
efficiently on large srcML documents that contain all of the source code
in a large project.

Using the character - in the place of an input srcML file filename uses
standard input, and in place of an output source-code file uses standard
output.

OPTIONS
=======

`HELP_FLAG_SHORT, HELP_FLAG`

:   Output the help and exit.

`VERSION_FLAG_SHORT, VERSION_FLAG`

:   Output the version of srcml2src and exit.

`SRC_ENCODING_FLAG_SHORT, SRC_ENCODING_FLAG=encoding`

:   Sets the encoding used for the output text source-code files to
    encoding. The default is DEFAULT\_TEXT\_ENCODING.

`UNIT_FLAG_SHORT, UNIT_FLAG=number`

:   Refers to a specific file in a srcML archive. The numbering starts
    at 1. If the number is outside of the range of nested units then
    nothing is returned. In combination with the attribute flags,
    outputs the attribute of that particular unit. In combination with
    the `` option outputs the unit in xml (not as text as is the
    default).

`TO_DIR_FLAG_SHORT, TO_DIR_FLAG`

:   Extracts all files from a srcML archive back into source-code files.
    The file path is based on the attributes `directory` and `filename`
    with necessary subdirectories created. Existing files with the same
    path are overwritten.

`XML_FLAG_SHORT, XML_FLAG`

:   Output is in srcML format. Used in combination with the `` option to
    output a particular nested unit in XML. The attributes on the output
    element `unit` are the attributes of the individual unit with any
    missing attributes taken from the root unit.

`COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG`

:   Output is in compressed gzip format.

``

:   No output of the default XML declaration for XML output options.
    Useful when the output is to be placed inside another XML document.

``

:   No output of namespace declarations for XML output options. Useful
    when the output is to be placed inside another XML document.

`VERBOSE_FLAG_SHORT, VERBOSE_FLAG`

:   Verbose output to standard error.

Metadata Options
================

Options to access to the metadata stored in the srcML document. Unless
otherwise noted, the metadata is quickly extracted from the start root
element or the XML declaration and does not traverse the entire
document. Output is to standard output.

LANGUAGE\_FLAG\_SHORT, LANGUAGE\_FLAG
Output the language of the srcML file as stored in the attribute
`language` in the element `unit`. The language is the value of the
attribute of the root element, or for a particular unit if specified
with the `` option.

DIRECTORY\_FLAG\_SHORT, DIRECTORY\_FLAG
Output the directory of the srcML file as stored in the attribute `dir`
in the element `unit`. The directory is the value of the attribute of
the root element, or for a particular unit if specified with the ``
option.

FILENAME\_FLAG\_SHORT, FILENAME\_FLAG
Output the filename of the srcML file as stored in the attribute
`filename` in the element `unit`. The filename is the value of the
attribute of the root element, or for a particular unit if specified
with the `` option.

SRCVERSION\_FLAG\_SHORT, SRCVERSION\_FLAG
Output the version of the srcML file as stored attribute `version` in
the element `unit`. The version is the value of the attribute of the
root element, or for a particular unit if specified with the `` option.

NAMESPACE\_FLAG\_SHORT, NAMESPACE\_FLAG=
uri
Output the prefix of the namespace given by uri. The name of the prefix
is output with a newline character. The default namespace is a blank
line. If the URI does not match a namespace declaration then no line is
output.

NESTED\_FLAG\_SHORT, NESTED\_FLAG
Output the number of nested units in a srcML archive. Result is 0 for a
srcML file that is not an archive. Requires traversal of the entire
document.

INFO\_FLAG\_SHORT, INFO\_FLAG
Output most of the metadata of the srcML file. Each individual piece of
data, i.e., namespaces, encoding, language, directory, filename, and
version, is on a separate line. Each metadata is the value of the
attribute of the root element, or for a particular unit if specified
with the `` option. The form of the information is that same as that of
the full flag option to src2srcml to form it, without the leading "--".

LONG\_INFO\_FLAG\_SHORT, LONG\_INFO\_FLAG
Output the metadata of the `` option along with the count of the nested
files in a srcML archive. Requires traversal of the entire srcML
document.

Querying and Transformation Options
===================================

Options for efficiently querying and transformation srcML. Standalone
XML tools that are DOM based (XPath, XSLT) have large memory
requirements with large srcML files. The `srcml2src` options work
efficiently with large srcML archives that contain large numbers of
files in a project, e.g., tens of thousands of files for a project in a
single 1 GB srcML archive 1 GB. All queries/transformations are
performed on a unit by unit basis inside the srcML archive.

XPATH\_FLAG=
xpath
Apply the XPath expression xpath to each individual unit in the srcML
archive and combine the results of the individual queries.

To refer to elements standard srcML prefixes (see `srcml2src`) are used.
One exception is that prefix "src" is used for srcML elements, e.g.,
`src:unit`. Note that these prefixes do not have to match the prefixes
used in the document.

If the result of the expression is anything other then a set of nodes
the individual unit results are combined. For numeric results, e.g.,
counts, the final result is the sum of the individual results. For
boolean values, the result is the logical `or` of the individual
results.

The output is a srcML archive with each XPath expression result in its
own individual element `unit`. If the XPath expression does not result
in a element `unit`, one is wrapped around the result. This format
allows for pipelines of queries and transformations.

XSLT\_FLAG=
xslt\_file
Apply the XSLT program xslt\_file to each individual unit in a srcML
archive and combine the results of the individual transformations.

The XSLT program can use any part of XSLT 1.0, and many EXSLT extension
functions. Since the XSLT program doesn't have access to some
information on the entire archive, the program is passed a series of
internally generated parameters. The parameters src:filename,
src:directory, src:version, and src:language are the corresponding
entries from the root element `unit`. The parameter src:position is the
position of an individual unit in the root unit, e.g., the position of
the file in the entire srcML archive. Parameters can also be passed to
the XSLT program from the `srcml2src` command line.

The output is a srcML document with the combined results of the XSLT
transformation wrapped in an outer element `unit` thereby providing an
output format that allows for repeated application.

PARAM\_FLAG
parameter\_name
parameter\_value
Pass the parameter parameter\_name and its corresponding
parameter\_value to the XSLT transformation. This is only valid with the
`` option. This is analagous to the --param; of xsltproc. The xsltproc
limit of 32 parameters applies here. Note that the internally generated
parameters src:filename, src:directory, src:version, src:language, and
src:position are automatically generated and are not part of the 32
parameter limit.

RELAXNG\_FLAG=
relaxng\_file
Apply the RelaxNG grammar relaxng\_file to each individual unit in the
srcML archive and combine the parts that match.

The RelaxNG grammar acts as a filter for individual units (files). If
the RelaxNG grammar is valid for a unit, then that unit is output. The
result is a srcML archive with only the files that are valid based on
the given RelaxNG grammar.

USAGE
=====

To translate the srcML file main.cpp.xml into the C++ source-code file
main.cpp:

`srcml2src` main.cpp.xml OUTPUT\_FLAG\_SHORT main.cpp

To extract the language of the srcML file main.cpp.xml:

`srcml2src` LANGUAGE\_FLAG main.cpp.xml

To extract the directory of the srcML file main.cpp.xml:

`srcml2src` DIRECTORY\_FLAG main.cpp.xml

To extract the filename of the srcML file main.cpp.xml:

`srcml2src` FILENAME\_FLAG main.cpp.xml

To extract the version of the srcML file main.cpp.xml:

`srcml2src` VERSION\_FLAG main.cpp.xml

To extract the xml encoding of the srcML file main.cpp.xml:

`srcml2src` ENCODING\_FLAG main.cpp.xml

RETURN STATUS
=============

STATUS\_SUCCESS: Normal

STATUS\_ERROR: Error

STATUS\_INPUTFILE\_PROBLEM: Problem with input file

STATUS\_UNKNOWN\_OPTION: Unknown option

STATUS\_UNKNOWN\_ENCODING: Unknown encoding

STATUS\_INVALID\_OPTION\_COMBINATION: Invalid combination of options

STATUS\_TERMINATED: Incomplete output due to termination

CAVEATS
=======

Line endings are normalized in XML and in the srcML format.

BUGS
====

There may be some parts of XML (e.g., comments) unused in the srcML
format (i.e., not generated by src2srcml) which are not preserved in the
output with the `` option.

SEE ALSO
========

`src2srcml`(1)

AUTHOR
======

Written by Michael L. Collard Original author and Huzefa Kagdi Original
author
