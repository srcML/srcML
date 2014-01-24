/*
  srcml2src.cpp

  Copyright (C) 2004-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

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
*/

/*
  Main program to translate from srcML to source code
*/

#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include "srcmlapps.hpp"
#include "project.hpp"
#include "srcMLUtility.hpp"
#include "Options.hpp"
#include "srcmlns.hpp"
#include <getopt.h>
#include <unistd.h>
#include "archive.h"
#include "libxml_archive_read.hpp"
#include "libxml_archive_write.hpp"
#include "srcexfun.hpp"
#include "URIStream.hpp"
#include <srcml_wrapper.hpp>
// #include <archive.h>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#else
#include <libxslt/xslt.h>
#include <libxslt/xsltconfig.h>
#include <libexslt/exslt.h>
#endif

int option_error_status(int optopt);

const char* diff_version;

#define BASE_PROGRAM_NAME "srcml2src"
const char* PROGRAM_NAME = BASE_PROGRAM_NAME;

char const * const TO_DIR_FLAG = "to-dir";
char const TO_DIR_FLAG_SHORT = 'a';

char const * const UNIT_FLAG = "unit";
char const UNIT_FLAG_SHORT = 'U';
char const * const UNIT_FLAG_FULL = "unit=NUM";

char const * const XML_FLAG = "xml";
char const XML_FLAG_SHORT = 'X';

char const * const INFO_FLAG = "info";
char const INFO_FLAG_SHORT = 'i';

char const * const LONG_INFO_FLAG = "longinfo";
char const LONG_INFO_FLAG_SHORT = 'L';

char const * const NAMESPACE_FLAG = "prefix";
char const NAMESPACE_FLAG_SHORT = 'p';
char const * const NAMESPACE_FLAG_FULL = "prefix=URI";

char const * const OMIT_FLAG = "omit";
char const OMIT_FLAG_SHORT = 'O';
char const * const OMIT_FLAG_FULL = "omit=NAMESPACE";

char const * const XPATH_FLAG = "xpath";
const int XPATH_FLAG_CODE = 256 + 0;
char const * const XPATH_FLAG_FULL = "xpath=XPATH";

char const * const XSLT_FLAG = "xslt";
const int XSLT_FLAG_CODE = 256 + 1;
char const * const XSLT_FLAG_FULL = "xslt=XSLT_FILE";

char const * const PARAM_FLAG = "xpathparam";
const int PARAM_FLAG_CODE = 256 + 2;
char const * const PARAM_FLAG_FULL = "xpathparam NAME=VAL";

char const * const STRING_PARAM_FLAG = "param";
const int STRING_PARAM_FLAG_CODE = 256 + 3;
char const * const STRING_PARAM_FLAG_FULL = "param NAME=VAL";

char const * const APPLY_ROOT_FLAG = "apply-root";
const int APPLY_ROOT_FLAG_CODE = 256 + 4;

char const * const RELAXNG_FLAG = "relaxng";
const int RELAXNG_FLAG_CODE = 256 + 5;
char const * const RELAXNG_FLAG_FULL = "relaxng=RELAXNG_FILE";

char const * const CONTEXT_FLAG = "context";
//char const CONTEXT_FLAG_SHORT = '';
char const * const CONTEXT_FLAG_FULL = "context=CONTEXT";

const char* const LIST_FLAG = "list";
const int LIST_FLAG_CODE = 256 + 6;

const char* const REGISTER_EXTENSION_FLAG = "register-ext";
const int REGISTER_EXTENSION_FLAG_CODE = 256 + 7;

const char* const REGISTER_EXTENSION_FILE_FLAG = "register-ext-file";
const int REGISTER_EXTENSION_FILE_FLAG_CODE = 256 + 8;

const char* const REGISTER_EXTENSION_FUNCTION_FLAG = "register-xpath-func";
const int REGISTER_EXTENSION_FUNCTION_FLAG_CODE = 256 + 9;

const char* const REGISTER_EXTENSION_FUNCTION_FILE_FLAG = "register-xpath-func-file";
const int REGISTER_EXTENSION_FUNCTION_FILE_FLAG_CODE = 256 + 10;

const char* const EOL_FLAG = "eol";
const int EOL_FLAG_CODE = 256 + 11;

const char* const DIFF_FLAG_LONG = "revision";
const int DIFF_FLAG_CODE = 256 + 12;

const char* const PRESERVE_FLAG_LONG = "preserve";
const int PRESERVE_FLAG_CODE = 256 + 13;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void libxml_error(void *ctx, const char *msg, ...) {}

#pragma GCC diagnostic pop

// output help message
void output_help(const char* name) {

  printf("Usage: %s [options] <srcML_infile>... [-o <src_outfile>]\n\n"
         "Translates from the the XML source-code representation srcML back to source-code.\n\n"
         "Extracts back to standard output, the disk, or to traditional archive formats,\n"
         "e.g., tar, cpio, zip, and with optional gzip, bzip2 compression.\n"
         "Provides access to metadata about the srcML document. For srcML archives\n"
         "provides extraction of specific files, and efficient querying/transformation\n"
         "using XPath, XSLT, and RelaxNG.\n\n"

         "srcML archives contain multiple individual source code files,\n"
         "e.g., an entire project or directory tree.\n\n"

         "By default, output is to stdout.  You can specify a file for output using the --%s or -%c option.\n"
         "When no filenames are given, input is from stdin and output is to stdout.\n"
         "An input filename of '-' also reads from stdin.\n\n"

         "Any input file, including XSLT and RelaxNG files, can be a local\n"
         "filename (FILE) or a URI with the protocols http:, ftp:, or file:\n\n"

         "The srcML files can be in xml, or compressed with gzip or bzip2 (detected automatically).\n\n"

         "Options:\n", name, OUTPUT_FLAG, OUTPUT_FLAG_SHORT);

  printf("  -%c, --%-17s display this help and exit\n", HELP_FLAG_SHORT, HELP_FLAG);
  printf("  -%c, --%-17s display version number and exit\n\n", VERSION_FLAG_SHORT, VERSION_FLAG);
  /*
    printf("  --%-21s ???\n\n", INPUT_FORMAT_FLAG);
  */
  printf("  -%c, --%-17s write result to OUTPUT which is a FILE or URI\n", OUTPUT_FLAG_SHORT, OUTPUT_FLAG_FULL);
  /*
    printf("  --%-21s ???\n\n", OUTPUT_FORMAT_FLAG);
  */

  printf("  -%c, --%-17s set the output source encoding to ENC (default:  %s) \n",
         SRC_ENCODING_FLAG_SHORT, SRC_ENCODING_FLAG_FULL, DEFAULT_TEXT_ENCODING);

  printf("  -%c, --%-17s output text or XML in gzip format\n",
         COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG);

  printf("  -%c, --%-17s conversion and status information to stderr\n",
         VERBOSE_FLAG_SHORT, VERBOSE_FLAG);

  printf("  -%c, --%-17s suppresses status messages\n\n",
         QUIET_FLAG_SHORT, QUIET_FLAG);

  printf("  -%c, --%-17s output in XML instead of text\n",
         XML_FLAG_SHORT, XML_FLAG);

  printf("  --%-21s do not output the XML declaration in XML output\n",
         NO_XML_DECLARATION_FLAG);

  printf("  --%-21s do not output any namespace declarations in XML output\n\n",
         NO_NAMESPACE_DECLARATION_FLAG);

  printf("Metadata Options:\n"
         "  -%c, --%-17s display source language and exit\n", LANGUAGE_FLAG_SHORT, LANGUAGE_FLAG);

  printf("  -%c, --%-17s display source directory name and exit\n", DIRECTORY_FLAG_SHORT, DIRECTORY_FLAG);

  printf("  -%c, --%-17s display source filename and exit\n", FILENAME_FLAG_SHORT, FILENAME_FLAG   );

  printf("  -%c, --%-17s display source version and exit\n", SRCVERSION_FLAG_SHORT, SRCVERSION_FLAG);

  printf("  -%c, --%-17s display xml encoding and exit\n", ENCODING_FLAG_SHORT, ENCODING_FLAG);

  printf("  -%c, --%-17s display prefix of namespace given by URI and exit\n",
         NAMESPACE_FLAG_SHORT, NAMESPACE_FLAG_FULL);

  printf("  -%c, --%-17s display number of srcML files and exit\n\n", NESTED_FLAG_SHORT, NESTED_FLAG);

  printf("  -%c, --%-17s display most metadata except file count (individual units) and exit\n",
         INFO_FLAG_SHORT, INFO_FLAG);

  printf("  -%c, --%-17s display all metadata including file count (individual units) and exit\n",
         LONG_INFO_FLAG_SHORT, LONG_INFO_FLAG);

  printf("  --%-21s list all the files in the srcML archive and exit\n\n", LIST_FLAG);

  printf("srcML Archive Options:\n"
         "  -%c, --%-17s extract individual unit NUM from srcML\n",
         UNIT_FLAG_SHORT, UNIT_FLAG_FULL);

  printf("  -%c, --%-17s extract all files from srcML and create them in the filesystem\n\n",
         TO_DIR_FLAG_SHORT, TO_DIR_FLAG);

  printf("Query and Transformation Options:\n"
         "  --%-21s apply XPATH expression to each individual unit\n", XPATH_FLAG_FULL);
  printf("  --%-21s apply XSLT_FILE (FILE or URI) transformation to each individual unit\n", XSLT_FLAG_FULL);
  //  printf("  --%-21s passes a parameter NAME and VAL to an XSLT program\n", STRING_PARAM_FLAG_FULL);
  printf("  --%-21s passes a parameter NAME and VAL to the XSLT program\n", PARAM_FLAG_FULL);
  printf("  --%-21s output individual units that match RELAXNG_FILE (FILE or URI)\n", RELAXNG_FLAG_FULL);
  printf("  --%-21s apply an xslt program or xpath query to the root element\n\n", APPLY_ROOT_FLAG);

  printf("Examples:\n"
         "  Read from file main.cpp.xml, write to file main.cpp:\n\n"
         "  " BASE_PROGRAM_NAME " main.cpp.xml -o main.cpp\n\n"

         "  Read from URI, write to file main.cpp:\n\n"
         "  " BASE_PROGRAM_NAME " http://www.sdml.info/projects/srcml/ex/main.cpp.xml main.cpp\n\n"

         "  Read from file main.cpp.xml, output language attribute to stdout:\n\n"
         "  " BASE_PROGRAM_NAME " main.cpp.xml --language\n\n");

  printf("www.sdml.info\n"
         "Report bugs to %s\n", EMAIL_ADDRESS);
}

// output version message
void output_version(const char* name) {

  printf("%s Version %s\n"
         "%s\n", name, "srcml2src Version Trunk 19106 Fri Jan 24 10:55:0 2014 -0500", COPYRIGHT);

  printf("Using: ");
  if(atoi(xmlParserVersion) == LIBXML_VERSION)
    printf("libxml %d, ", LIBXML_VERSION);
  else
    printf("libxml %s (Compiled %d), ", xmlParserVersion, LIBXML_VERSION);

#ifdef LIBXSLT_VERSION
  if(xsltLibxsltVersion == LIBXSLT_VERSION)
    printf("libxslt %d, ", LIBXSLT_VERSION);
  else
    printf("libxslt %d (Compiled %d), ", xsltLibxsltVersion, LIBXSLT_VERSION);

  if(xsltLibxmlVersion != atoi(xmlParserVersion))
    printf("libxslt(libxml) %d, ", xsltLibxsltVersion);

  if(exsltLibexsltVersion == LIBEXSLT_VERSION)
    printf("libexslt %d, ", LIBEXSLT_VERSION);
  else
    printf("libexslt %d (Compiled %d), ", exsltLibexsltVersion, LIBEXSLT_VERSION);
#else

  void* handle = dlopen("libxslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libxslt.so.1", RTLD_LAZY);
    if (!handle) {
      handle = dlopen("libxslt.dylib", RTLD_LAZY);
    }
  }

  int* xsltLibxsltVersion = 0;
  if (handle) {
    dlerror();
    xsltLibxsltVersion = (int*)dlsym(handle, "xsltLibxsltVersion");
    char* error;
    if ((error = dlerror()) != NULL) {
      dlclose(handle);
      handle = 0;
    } else
      printf("libxslt %d, ", *xsltLibxsltVersion);
  }

  if (handle) {
    dlerror();
    int& xsltLibxmlVersion = *(int*)dlsym(handle, "xsltLibxmlVersion");
    char* error;
    if ((error = dlerror()) != NULL) {
      dlclose(handle);
      handle = 0;
    } else if (xsltLibxmlVersion != LIBXML_VERSION)
      printf("libxslt(libxml) %d, ", xsltLibxmlVersion);
  }

  // done with libxslt handle
  if (handle) {
    dlclose(handle);
    handle = 0;
  }

  // now to libexslt
  handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle) {
    // now to libexslt
    handle = dlopen("libexslt.so.0", RTLD_LAZY);
    if (!handle) {
      handle = dlopen("libexslt.dylib", RTLD_LAZY);
    }
  }

  if (handle) {
    dlerror();
    int& exsltLibexsltVersion = *(int*)dlsym(handle, "exsltLibexsltVersion");
    char* error;
    if ((error = dlerror()) != NULL) {
      dlclose(handle);
      handle = 0;
    } else
      printf("libexslt %d, ", exsltLibexsltVersion);
  }

  if (handle) {
    dlerror();
    int& exsltLibxsltVersion = *(int*)dlsym(handle, "exsltLibxsltVersion");
    char* error;
    if ((error = dlerror()) != NULL) {
      dlclose(handle);
      handle = 0;
    } else
      printf("libexslt(libxslt) %d, ", exsltLibxsltVersion);
  }

  if (handle) {
    dlerror();
    int& exsltLibxmlVersion = *(int*)dlsym(handle, "exsltLibxmlVersion");
    char* error;
    if ((error = dlerror()) != NULL) {
      dlclose(handle);
      handle = 0;
    } else if (exsltLibxmlVersion != LIBXML_VERSION)
      printf("libexslt(libxml) %d, ", exsltLibxmlVersion);
  }

  if(handle) dlclose(handle);
  handle = 0;
#endif
#ifndef LIBSRCML_COMPILER_IS_MSVC
  if(archive_version_number() == ARCHIVE_VERSION_NUMBER)
    printf("libarchive %d\n", ARCHIVE_VERSION_NUMBER);
  else
    printf("libarchive %d (Compiled %d)\n", archive_version_number(), ARCHIVE_VERSION_NUMBER);
#endif
}

OPTION_TYPE options = OPTION_XMLDECL | OPTION_NAMESPACEDECL;


#ifdef __GNUG__
extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);
#endif

int optionorder[5];
int optioncount = 0;

void output_info(srcMLUtility& su, int options, int optioncount, int optionorder[]);

FILE* output;

typedef struct process_options
{
  // option values
  const char* ofilename;
  const char* input_format;
  const char* output_format;
  const char* xml_encoding;
  const char* src_encoding;
  const char* to_directory;
  int unit;
  const char* context;
  int nscount;
  const char* ns[MAXNS + 1];
  int omitnscount;
  const char * omitns[MAXNS + 1];
  int paramcount;
  const char* params[MAXPARAMS * 2 + 1];
  int transformcount;
  const char* transforms[MAXTRANSFORMS + 1];
  int registerextcount;
  const char* registerext[MAXPARAMS * 2 + 1];
  int registerfilescount;
  const char* registerfiles[MAXPARAMS + 1];
  const char* diff_version;
  const char* preserve_uri;

} process_options;

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions);

// read and register xpath functions from a file
void register_xpath_functions_from_filename(const char * filename);

void exit_cleanup() {

  xmlCleanupCharEncodingHandlers();
  xmlCleanupGlobals();
  xmlDictCleanup();
  xmlCleanupParser();

}

#if defined(__GNUC__) && !defined(__MINGW32__)
// stat initializer                                                                                                                     
struct stat init_stat;
#endif

int main(int argc, char* argv[]) {

  atexit(exit_cleanup);

  xmlGenericErrorFunc handler = (xmlGenericErrorFunc) libxml_error;
  initGenericErrorDefaultFunc(&handler);

  xmlRegisterDefaultInputCallbacks();

  if (xmlRegisterInputCallbacks(archiveReadMatch, archiveReadOpen, archiveRead, archiveReadClose) < 0) {
    fprintf(stderr, "%s: failed to register archive handler\n", PROGRAM_NAME);
    exit(1);
  }
  /*
    if (xmlRegisterOutputCallbacks(archiveWriteDiskMatch, archiveWriteDiskOpen, archiveWriteDisk, archiveWriteDiskClose) < 0) {
    fprintf(stderr, "%s: failed to register archive handler\n", PROGRAM_NAME);
    exit(1);
    }
  */

  /* signal handling */

#if defined(__GNUG__) && !defined(__MINGW32__)
  // signal to toggle verbose flag
  pstd::signal(SIGUSR1, verbose_handler);
#endif

  int exit_status = EXIT_SUCCESS;

  process_options poptions =
    {
      "-",
      0,
      0,
      0,
      DEFAULT_TEXT_ENCODING,
      0,
      0,
      "src:unit",
      0,
      { 0 },
      0,
      { 0 },
      0,
      { 0 },
      0,
      { 0 },
      0,
      { 0 },
      0,
      { 0 },
      0,
      0
    };

  // process command-line arguments
  int curarg = process_args(argc, argv, poptions);

  // first command line parameter is input filename
  const char* filename = "-";

  bool is_multi_input = (argc - curarg) > 1;
  bool is_multi_op = options & (OPTION_INFO | OPTION_LONG_INFO | OPTION_LIST | OPTION_UNIT | OPTION_NAMESPACE | OPTION_ARCHIVE);

  do {

  if (curarg < argc) {
    filename = argv[curarg];
    ++curarg;
  }

  /* Special checks for illegal combinations */
  // xml output and src-encoding (switch to encoding?)
  if (isoption(options, OPTION_XML) && isoption(options, OPTION_SRC_ENCODING)) {

    fprintf(stderr, "%s: Options for xml output and specifying source encoding are incompatible.\n", PROGRAM_NAME);
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

#if defined(__GNUG__) && !defined(__MINGW32__)
  // verify that a non-stdio direct output file is not the same as the input file
  if (strcmp(filename, "-") != 0 && strcmp(poptions.ofilename, "-") != 0) {

    // input file
    struct stat instat = init_stat;
    if (stat(filename, &instat) == -1) {
      goto done;
    }

    struct stat outstat = init_stat;
    if (stat(poptions.ofilename, &outstat) == -1) {
      goto done;
    }

    if ((strcmp(poptions.ofilename, "-") != 0) && instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
      fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n", PROGRAM_NAME, filename, poptions.ofilename);
      exit(STATUS_INPUTFILE_PROBLEM);
    }
  }
 done:
#endif

  // info options are convenience functions for multiple options
  if (isoption(options, OPTION_INFO) || isoption(options, OPTION_LONG_INFO)) {
    optionorder[0] = OPTION_XML_ENCODING;
    optionorder[1] = OPTION_LANGUAGE;
    optionorder[2] = OPTION_DIRECTORY;
    optionorder[3] = OPTION_FILENAME;
    optionorder[4] = OPTION_VERSION;
    optioncount = 5;

    options |= OPTION_NAMESPACE;
  }

  if (isoption(options, OPTION_NULL) && !isoption(options, OPTION_XML))
    options |= OPTION_TO_DIR;

  // register default xpath and xslt extension functions if needed
  if (isoption(options, OPTION_XPATH) || isoption(options, OPTION_XSLT)) {
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "statement", "/src:unit//node()[self::src:while or self::src:if or self::src:return or self::src:for]");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "statement_node", "[self::src:while or self::src:if or self::src:return or self::src:for]");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "if", "/src:unit//src:if");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "while", "/src:unit//src:while");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "nestedwhile", ".//src:while//src:while");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "returntype", "/src:unit//src:function/src:type");

    // srcdiff containing functions
    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hascommon",
                                   "self::*[not(descendant::diff:*) or descendant::diff:common]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hasinsert",
                                   "descendant::diff:insert[1]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hasdelete",
                                   "descendant::diff:delete[1]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hasdifference",
                                   "descendant::diff:*[self::diff:insert or self::diff:delete]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "haschange",
                                   "descendant::diff:insert[following-sibling::node()[1][self::diff:delete] or preceding-sibling::node()[1][self::diff:delete]][1]");

    // srcdiff includes functions
    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "common",
                                   "not(ancestor::diff:*[1][self::diff:insert or self::diff:delete])");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "inserted",
                                   "ancestor::diff:*[1][self::diff:insert]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "deleted",
                                   "ancestor::diff:*[1][self::diff:delete]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "difference",
                                   "ancestor::diff:*[1][self::diff:insert or self::diff:delete]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "changed",
                                   "ancestor::diff:*[1][self::diff:insert[following-sibling::node()[1][self::diff:delete] or preceding-sibling::node()[1][self::diff:delete]] or self::diff:delete[following-sibling::node()[1][self::diff:insert] or preceding-sibling::node()[1][self::diff:insert]]]");

  }

  if (isoption(options, OPTION_DIFF))
    diff_version = poptions.diff_version;

  if (isoption(options, OPTION_DIFF) && isoption(options, OPTION_XML) && poptions.unit == 0)
    poptions.unit = -1;

  try {

    // setup for processing
    srcMLUtility su(filename, poptions.src_encoding, options, poptions.diff_version);

    // for options, there may be an output file
    output = stdout;
    if (strcmp(poptions.ofilename, "") != 0 && (strcmp(poptions.ofilename, "-") != 0)) {
      output = fopen(poptions.ofilename, "w");
    }

    // list
    if (isoption(options, OPTION_LIST)) {

      su.list();

      // process get attribute options
    } else if (optioncount > 0) {

      if (isoption(options, OPTION_LONG_INFO))
        poptions.unit = -1;

      // move to the appropriate unit and output the appropriate metadata
      su.move_to_unit(poptions.unit, su, options, optioncount, optionorder, output);

      // now finish longinfo
      if (isoption(options, OPTION_LONG_INFO)) {
        if (!isoption(options, OPTION_UNIT)) {
          if (isatty(fileno(output)))
            fprintf(output, "\r");

          fprintf(output, "units=\"%d\"\n", su.curunits());
        }
      }

      // namespace
    } else if (isoption(options, OPTION_NAMESPACE)) {

      su.move_to_unit(poptions.unit, su, options, optioncount, optionorder, output);

      for (int i = 0; i < poptions.nscount; ++i) {

        const char* prefix = su.namespace_ext(poptions.ns[i]);
        if (prefix) {
          if (poptions.nscount == 1)
            fprintf(output, "%s\n", prefix);
          else {
            fprintf(output, "xmlns");
            if (prefix[0] != '\0')
              fprintf(output, ":%s", prefix);
            fprintf(output, "=\"%s\"\n", poptions.ns[i]);
          }
        }
      }

      // process non-attribute options
    } else if (isoption(options, OPTION_ARCHIVE)) {

#ifdef __GNUG__
      // gracefully finish current file in srcML archive mode
      pstd::signal(SIGINT, terminate_handler);
#endif
      // in verbose mode, the current counter is displayed
      // as it counts.  Need to overwrite this
      long count = su.unit_count(output);

      if (!isatty(fileno(output)))
	if(is_multi_input)
	  fprintf(output, "%ld\t%s\n", count, filename);
	else
	  fprintf(output, "%ld\n", count);
      else
	if(is_multi_input)
	  fprintf(output, "\t%s\n", filename);
	else
	  fprintf(output, "\n");

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
        exit_status = STATUS_TERMINATED;

    } else if (isoption(options, OPTION_TO_DIR)) {

#ifdef __GNUG__
      // gracefully finish current file in srcML archive mode
      pstd::signal(SIGINT, terminate_handler);
#endif

      su.expand(poptions.ofilename, poptions.output_format, poptions.to_directory);

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
        exit_status = STATUS_TERMINATED;
      /*
        while (curarg < argc) {
        fprintf(stderr, "ANOTHER: %s\n", argv[curarg]);
        su.set_input_filename(argv[curarg]);
        ++curarg;

        //      su.expand(poptions.ofilename, poptions.output_format, poptions.to_directory);
        }
      */

    } else if (isoption(options, OPTION_PRESERVE)) {

      su.extract_xml_uri(poptions.ofilename, poptions.unit, poptions.preserve_uri);

    } else if (isoption(options, OPTION_XML)) {

      su.extract_xml(poptions.ofilename, poptions.unit);

    } else if (isoption(options, OPTION_XPATH)) {

      su.xpath(poptions.ofilename, poptions.context, poptions.transforms);

    } else if (isoption(options, OPTION_XSLT)) {

      su.xslt(poptions.context, poptions.ofilename, poptions.transforms, poptions.params, poptions.paramcount);

    } else if (isoption(options, OPTION_RELAXNG)) {

      su.relaxng(poptions.ofilename, poptions.transforms);

    } else {

      std::string cfilename = poptions.ofilename;
      if (poptions.output_format) {
        cfilename = ".";
        cfilename.append(poptions.output_format);
      }

      if (!archiveWriteMatch_src2srcml(cfilename.c_str()))

        su.extract_text(0 /* null to_directory */, poptions.ofilename, poptions.unit);

      else {

#ifdef __GNUG__
        // gracefully finish current file in srcML archive mode
        pstd::signal(SIGINT, terminate_handler);
#endif

        su.expand(poptions.ofilename, poptions.output_format, "" /* blank to_directory */);

        // if we terminated early, output the correct status
        if (isoption(options, OPTION_TERMINATE))
          exit_status = STATUS_TERMINATED;

      }
    }

  } catch (const OutOfRangeUnitError& e) {

    fprintf(stderr, "%s: unit %d  was selected from srcML that only contains "
            "%d units\n", PROGRAM_NAME, poptions.unit, (int)e.size);
    exit_status = STATUS_INVALID_ARGUMENT;

    return exit_status;
    /*
      } catch (LibXMLError) {
      exit_status = STATUS_UNIT_INVALID;
      throw "XML error";
    */
  } catch (TerminateLibXMLError error) {

    fprintf(stderr, "%s:  Program terminated by user\n", PROGRAM_NAME);
    exit_status = STATUS_TERMINATED;

    return exit_status;

  } catch (LibXMLError error) {

    if (error.getErrorNum() == 0) {
      exit_status = STATUS_INPUTFILE_PROBLEM;
      fprintf(stderr, "%s: Unable to open input file as XML\n", PROGRAM_NAME);
    }

  } catch (const char* s) {

    fprintf(stderr, "%s: %s\n", PROGRAM_NAME, s);
    if (!exit_status)
      exit_status = STATUS_ERROR;
  } catch (...) {
    if (!exit_status)
      exit_status = STATUS_ERROR;
  }

  if (strcmp(poptions.ofilename, "") != 0 && (strcmp(poptions.ofilename, "-") != 0)) {
    fclose(output);
  }

  
  if(!is_multi_op || (exit_status && !(exit_status & STATUS_INPUTFILE_PROBLEM))) break;

  } while(curarg < argc);

  return exit_status;
}

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions)
{
  int curoption = 0;
  struct option cliargs[] = {
    { HELP_FLAG, no_argument, NULL, HELP_FLAG_SHORT },
    { VERSION_FLAG, no_argument, NULL, VERSION_FLAG_SHORT },
    { OUTPUT_FLAG, required_argument, NULL, OUTPUT_FLAG_SHORT },
    { FILENAME_FLAG, no_argument, NULL, FILENAME_FLAG_SHORT },
    { DIRECTORY_FLAG, no_argument, NULL, DIRECTORY_FLAG_SHORT },
    { LANGUAGE_FLAG, no_argument, NULL, LANGUAGE_FLAG_SHORT },
    { SRCVERSION_FLAG, no_argument, NULL, SRCVERSION_FLAG_SHORT },
    { ENCODING_FLAG, optional_argument, NULL, ENCODING_FLAG_SHORT },
    { NESTED_FLAG, no_argument, NULL, NESTED_FLAG_SHORT },
    { INFO_FLAG, no_argument, NULL, INFO_FLAG_SHORT },
    { LONG_INFO_FLAG, no_argument, NULL, LONG_INFO_FLAG_SHORT },
    { TO_DIR_FLAG, required_argument, NULL, TO_DIR_FLAG_SHORT },
    { VERBOSE_FLAG, no_argument, NULL, VERBOSE_FLAG_SHORT },
    { XML_FLAG, no_argument, NULL, XML_FLAG_SHORT },
    { COMPRESSED_FLAG, no_argument, NULL, COMPRESSED_FLAG_SHORT },
    { UNIT_FLAG, required_argument, NULL, UNIT_FLAG_SHORT },
    { SRC_ENCODING_FLAG, required_argument, NULL, SRC_ENCODING_FLAG_SHORT },
    { NAMESPACE_FLAG, required_argument, NULL, NAMESPACE_FLAG_SHORT },
    { OMIT_FLAG, required_argument, NULL, OMIT_FLAG_SHORT },
    { QUIET_FLAG, no_argument, NULL, QUIET_FLAG_SHORT },
    { NULL_FLAG, no_argument, NULL, NULL_FLAG_SHORT },
    { NO_XML_DECLARATION_FLAG, no_argument, NULL, NO_XML_DECLARATION_FLAG_CODE },
    { NO_NAMESPACE_DECLARATION_FLAG, no_argument, NULL, NO_NAMESPACE_DECLARATION_FLAG_CODE },
    //    { INPUT_FORMAT_FLAG, required_argument, NULL, INPUT_FORMAT_FLAG_CODE },
    //    { OUTPUT_FORMAT_FLAG, required_argument, NULL, OUTPUT_FORMAT_FLAG_CODE },
    { LIST_FLAG, no_argument, NULL, LIST_FLAG_CODE },
    { REGISTER_EXTENSION_FLAG, required_argument, NULL, REGISTER_EXTENSION_FLAG_CODE },
    { REGISTER_EXTENSION_FILE_FLAG, required_argument, NULL, REGISTER_EXTENSION_FILE_FLAG_CODE },
    { REGISTER_EXTENSION_FUNCTION_FLAG, required_argument, NULL, REGISTER_EXTENSION_FUNCTION_FLAG_CODE },
    { REGISTER_EXTENSION_FUNCTION_FILE_FLAG, required_argument, NULL, REGISTER_EXTENSION_FUNCTION_FILE_FLAG_CODE },
    { EOL_FLAG, required_argument, NULL, EOL_FLAG_CODE },
    { DIFF_FLAG_LONG, required_argument, NULL, DIFF_FLAG_CODE },
    { PRESERVE_FLAG_LONG, required_argument, NULL, PRESERVE_FLAG_CODE },

    { XPATH_FLAG, required_argument, NULL, XPATH_FLAG_CODE },
    { XSLT_FLAG, required_argument, NULL, XSLT_FLAG_CODE },
    { PARAM_FLAG, required_argument, NULL, PARAM_FLAG_CODE },
    { STRING_PARAM_FLAG, required_argument, NULL, STRING_PARAM_FLAG_CODE },
    { RELAXNG_FLAG, required_argument, NULL, RELAXNG_FLAG_CODE },
    { APPLY_ROOT_FLAG, no_argument, NULL, APPLY_ROOT_FLAG_CODE },
    //    { CONTEXT_FLAG, required_argument, NULL, 'C' },
    { 0, 0, 0, 0 }
  };

  while (1) {
    int lastoptind = optind;
    curoption = 0;
    int option_index = 0;
    int c = getopt_long(argc, argv, "hVo:fdlsxniLa:vXzU:t:p:Oq0", cliargs, &option_index);
    if (c == -1)
      break;

    if (curoption) {
      options |= curoption;
      continue;
    }

    // missing or extra option argument
    if (c == '?') {
      fprintf(stderr, "Try '%s --%s' for more information.\n", PROGRAM_NAME, HELP_FLAG);
      exit(option_error_status(optopt));
    }

    char* end = 0;
    switch(c) {

    case HELP_FLAG_SHORT:
      output_help(PROGRAM_NAME);
      exit(STATUS_SUCCESS);
      break;

    case VERSION_FLAG_SHORT:
      output_version(PROGRAM_NAME);
      exit(STATUS_SUCCESS);
      break;

    case OUTPUT_FLAG_SHORT:
      poptions.ofilename = optarg;
      break;

    case FILENAME_FLAG_SHORT:
      options |= OPTION_FILENAME;
      optionorder[optioncount++] = OPTION_FILENAME;
      break;

    case DIRECTORY_FLAG_SHORT:
      options |= OPTION_DIRECTORY;
      optionorder[optioncount++] = OPTION_DIRECTORY;
      break;

    case SRCVERSION_FLAG_SHORT:
      options |= OPTION_VERSION;
      optionorder[optioncount++] = OPTION_VERSION;
      break;

    case LANGUAGE_FLAG_SHORT:
      options |= OPTION_LANGUAGE;
      optionorder[optioncount++] = OPTION_LANGUAGE;
      break;

    case ENCODING_FLAG_SHORT:
      options |= OPTION_XML_ENCODING;
      optionorder[optioncount++] = OPTION_XML_ENCODING;

      poptions.xml_encoding = optarg;
      break;

    case SRC_ENCODING_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_SRC_ENCODING;

      poptions.src_encoding = optarg;

      // validate source encoding
      if (!srcMLUtility::checkEncoding(poptions.src_encoding)) {
        fprintf(stderr, "%s: text encoding \"%s\" is not supported.\n", PROGRAM_NAME, poptions.src_encoding);
        exit(STATUS_UNKNOWN_ENCODING);
      }
      break;

    case NAMESPACE_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_NAMESPACE;

      poptions.ns[poptions.nscount++] = optarg;
      break;

    case OMIT_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_OMIT;

      poptions.omitns[poptions.omitnscount++] = optarg;
      break;

    case QUIET_FLAG_SHORT:
      options |= OPTION_QUIET;
      break;

    case NULL_FLAG_SHORT:
      options |= OPTION_NULL;
      break;

    case NESTED_FLAG_SHORT:
      options |= OPTION_ARCHIVE;
      break;

    case INFO_FLAG_SHORT:
      options |= OPTION_INFO;
      break;

    case LONG_INFO_FLAG_SHORT:
      options |= OPTION_LONG_INFO;
      break;

    case TO_DIR_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_TO_DIR;

      poptions.to_directory = optarg;
      break;

    case VERBOSE_FLAG_SHORT:
      options |= OPTION_VERBOSE;
      break;

    case XML_FLAG_SHORT:
      options |= OPTION_XML;
      break;

    case COMPRESSED_FLAG_SHORT:
      options |= OPTION_COMPRESSED;
      break;

    case UNIT_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_UNIT;

      // try to convert to number
      poptions.unit = (int)pstd::strtol(optarg, &end, 10);

      // validate type of unit number
      if (errno == EINVAL || strlen(end) == strlen(optarg)) {
        fprintf(stderr, "%s: unit option value \"%s\" must be numeric.\n", PROGRAM_NAME, optarg);
        exit(STATUS_INVALID_ARGUMENT);
      }

      // validate range of unit number
      if (poptions.unit <= 0) {
        fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", PROGRAM_NAME, poptions.unit);
        exit(STATUS_INVALID_ARGUMENT);
      }

      break;

      /*
        case INPUT_FORMAT_FLAG_CODE:

        // check for missing argument confused by an argument that looks like an option
        checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

        options |= OPTION_INPUT_FORMAT;
        poptions.input_format = optarg;
        break;
      */
      /*
        case OUTPUT_FORMAT_FLAG_CODE:

        // check for missing argument confused by an argument that looks like an option
        checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

        options |= OPTION_OUTPUT_FORMAT;
        poptions.output_format = optarg;
        break;
      */
    case LIST_FLAG_CODE :
      options |= OPTION_LIST;
      break;

    case REGISTER_EXTENSION_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // register extension name
      poptions.registerext[poptions.registerextcount++] = optarg;

      // must be both name and value, but value could be empty
      if (!strchr(optarg, '=')) {
        fprintf(stderr, "%s: Register extension name and value must be given.\n", PROGRAM_NAME);
        exit(1);
      }

      // registerext value
      end = optarg;
      strsep(&end, "=");
      poptions.registerext[poptions.registerextcount] = (char*) srcml_malloc(strlen(end) + 1);
      strcpy((char *) poptions.registerext[poptions.registerextcount], end);
      poptions.registerextcount++;
      break;

    case REGISTER_EXTENSION_FILE_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // register files name
      poptions.registerfiles[poptions.registerfilescount++] = optarg;
      break;

    case REGISTER_EXTENSION_FUNCTION_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // must be both name and value, but value could be empty
      if (!strchr(optarg, '=')) {
        fprintf(stderr, "%s: Register extension function name and value must be given.\n", PROGRAM_NAME);
        exit(1);
      }

      // register xpath extension function
      end = optarg;
      strsep(&end, "=");

      // TODO:  Extract the extension prefix
      xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, optarg, end);

      break;

    case REGISTER_EXTENSION_FUNCTION_FILE_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // register files name
      register_xpath_functions_from_filename(optarg);
      break;

    case EOL_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // TODO: Handle end of line parameter
      break;

    case DIFF_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_DIFF;
      poptions.diff_version = optarg;

      break;

    case PRESERVE_FLAG_CODE :

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_PRESERVE;
      poptions.preserve_uri = optarg;

      break;

    case XPATH_FLAG_CODE:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // check for blank argument
      checkargisnonempty(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      if(poptions.transformcount != 0)
        {
          fprintf(stderr, "%s: Only one xpath expression, xslt file, or relaxng file may be used.\n", PROGRAM_NAME);
          exit(STATUS_ERROR);
        }

      options |= OPTION_XPATH;
      poptions.transforms[poptions.transformcount++] = optarg;
      break;

    case XSLT_FLAG_CODE:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // check for blank argument
      checkargisnonempty(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      if(poptions.transformcount != 0)
        {
          fprintf(stderr, "%s: Only one xpath expression, xslt file, or relaxng file may be used.\n", PROGRAM_NAME);
          exit(STATUS_ERROR);
        }

      options |= OPTION_XSLT;
      poptions.transforms[poptions.transformcount++] = optarg;
      break;

    case STRING_PARAM_FLAG_CODE:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // param name
      poptions.params[poptions.paramcount++] = optarg;

      // must be both name and value, but value could be empty
      if (!strchr(optarg, '=')) {
        fprintf(stderr, "%s: Param name and value must be given.\n", PROGRAM_NAME);
        exit(1);
      }

      // param value
      end = optarg;
      strsep(&end, "=");
      poptions.params[poptions.paramcount] = (char*) srcml_malloc(strlen(end) + 1 + 2);
      strcpy((char *) poptions.params[poptions.paramcount], "\'");
      strcat((char *) poptions.params[poptions.paramcount], end);
      strcat((char *) poptions.params[poptions.paramcount], "\'");
      poptions.paramcount++;
      break;

    case PARAM_FLAG_CODE:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // must be both name and value, but value could be empty
      if (!strchr(optarg, '=')) {
        fprintf(stderr, "%s: Param name and value must be given.\n", PROGRAM_NAME);
        exit(1);
      }

      // param name
      poptions.params[poptions.paramcount++] = optarg;

      // param value
      {
        char* place = optarg;
        strsep(&place, "=");
        poptions.params[poptions.paramcount++] = place;
      }
      break;

    case RELAXNG_FLAG_CODE:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      if(poptions.transformcount != 0)
        {
          fprintf(stderr, "%s: Only one xpath expression, xslt file, or relaxng file may be used.\n", PROGRAM_NAME);
          exit(STATUS_ERROR);
        }

      options |= OPTION_RELAXNG;
      poptions.transforms[poptions.transformcount++] = optarg;
      break;

    case NO_XML_DECLARATION_FLAG_CODE:
      options |= OPTION_XML;
      options &= ~OPTION_XMLDECL;
      break;

    case NO_NAMESPACE_DECLARATION_FLAG_CODE:
      options |= OPTION_XML;
      options &= ~OPTION_NAMESPACEDECL;
      break;

    case APPLY_ROOT_FLAG_CODE:

      options |= OPTION_APPLY_ROOT;
      break;

    default:
      // TODO:  Fix error message
      fprintf(stderr, "WHAT: %d\n", c);
      break;
    };
  }

  return  optind;
}
#ifdef __GNUG__
extern "C" void verbose_handler(int) {

  if ((options &= OPTION_VERBOSE) == 0)
    options |= OPTION_VERBOSE;
  else
    options &= ~OPTION_VERBOSE;
}

extern "C" void terminate_handler(int) {

  // setup to terminate after current file
  options |= OPTION_TERMINATE;

  // turn off handler for this signal
  pstd::signal(SIGINT, SIG_DFL);
}
#endif

int option_error_status(int optopt) {

  switch (optopt) {

  case ENCODING_FLAG_SHORT:
    return STATUS_INVALID_ARGUMENT;
    break;

  case SRC_ENCODING_FLAG_SHORT:
    return STATUS_INVALID_ARGUMENT;
    break;

  case UNIT_FLAG_SHORT:
    return STATUS_INVALID_ARGUMENT;
    break;

  case INPUT_FORMAT_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case OUTPUT_FORMAT_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case XPATH_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case XSLT_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case PARAM_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case STRING_PARAM_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case REGISTER_EXTENSION_FILE_FLAG_CODE :
    return STATUS_ERROR;
    break;

  case REGISTER_EXTENSION_FLAG_CODE :
    return STATUS_ERROR;
    break;

  case REGISTER_EXTENSION_FUNCTION_FILE_FLAG_CODE :
    return STATUS_ERROR;
    break;

  case REGISTER_EXTENSION_FUNCTION_FLAG_CODE :
    return STATUS_ERROR;
    break;

  case '\0':
    return STATUS_UNKNOWN_OPTION;
    break;
  };

  return 0;
}

void register_xpath_functions_from_filename(const char * filename) {

  URIStream xpath_extension_file(filename);

  char * extension_function;
  while((extension_function = xpath_extension_file.readline())) {

    if(strcmp(extension_function, "")) {

      // must be both name and value, but value could be empty
      if (!strchr(extension_function, '=')) {
        fprintf(stderr, "%s: Register extension function name and value must be given.\n", PROGRAM_NAME);
        exit(1);
      }

      // register xpath extension function
      const char * end = extension_function;
      strsep((char **)&end, "=");
      xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, extension_function, end);
    }
  }
}
