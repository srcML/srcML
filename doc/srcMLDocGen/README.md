Generating srcML HTML Documentation
----

This is the user documentation which is generated and then moved to www.srcml.org.

To generate it, you will need [django](https://docs.djangoproject.com/en/1.9/topics/install/) and python. Once they are successfully installed, run the command:

```bash
./srcMLDocGen.py
```

This will generate the documentation in the form required for www.srcml.org, so that it can be easily dropped in. This means that the navigation bar will include links to files which aren't generated locally (e.g., the srcml home page, about page, style pages, etc.). This is done for convenience, but the main files that it generates includes:
  - c_srcML.html (and c_srcML.index.html)
  - cpp_srcML.html (and cpp_srcML.index.html)
  - cpreproc_srcML.html (and cpreproc_srcML.index.html)
  - cs_srcML.html (and cs_srcML.index.html)
  - java_srcML.html (and java_srcML.index.html)
  - srcMLGrammer.html
  - srcMLLangSupport.html
  - taglisting.txt
  - tagLocationDoc.txt
  - XPathExtFunc.html

To modify the language documentation (C, CPP, C Prepropcessor, C#, Java), find the DocConfig.xml file located in the corresponding language directory. For example, to change the C++ documentation, edit the file in srcMLDocGen/DocData/C/DocConfig.xml. This file is the input for srcMLDocGen.py which provides the following information:
  - Title of the element name
  - XPath queries and descriptions
  - Subelements
  - Links to the example files, in the same directory. These are written in the given language (C, C#, etc) and then srcml is run on the examples to produce the generated output.

The best examples are within the DocConfig.xml files themselves. See the DocConfig.xml file corresponding to the language documentation which needs updated for examples on how to use it. 
