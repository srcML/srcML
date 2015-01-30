# What is srcML?

In short, srcML is an XML format for source code. Specifically, the parsing technology supports C/C++, C#, Java. The XML markup identifies elements of the abstract syntax for the language. This allows us to leverage XML tools to support the various tasks of exploration, analysis, and manipulation.

srcML toolkit includes:

* srcML client

  	Both conversion to the srcML format, operations on srcML, as well as, conversion of srcML to source code is accomplished using the command `srcml`.

* libsrcml

  	The library provides a C interface for translation of source code to and from srcML, as well as, efficient manipulation and fact extraction (XPath, XSLT, and RelaxNG).  The srcML client `srcml` is built utilizing libsrcml.
