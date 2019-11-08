# srcML

srcML is an XML format for source code. The XML markup identifies 
elements of the abstract syntax for the language. The toolkit 
includes parsing supports conversion of C, C++, C#, and Java both 
to and from the srcML format. This allows leveraging XML tools to 
support the various tasks of source code exploration, analysis, 
and manipulation.

srcML toolkit includes:

* srcML client

Conversion to the srcML format, querying and transformation on 
srcML, and conversion of srcML back to source code using the 
command `srcml`

* libsrcml

A C interface for translation of source code to and from srcML, 
as well as, efficient manipulation and fact extraction (XPath, 
XSLT, and RelaxNG).  The srcML client `srcml` is built utilizing 
libsrcml.
