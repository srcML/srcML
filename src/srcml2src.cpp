/*
  srcml2src.cpp

  Copyright (C) 2004-2011  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

#include "libxml_archive_read.hpp"
#include "libxml_archive_write.hpp"
#include "srcexfun.hpp"
#include "URIStream.hpp"

int option_error_status(int optopt);

#define BASE_PROGRAM_NAME "srcml2src"
const char* PROGRAM_NAME = BASE_PROGRAM_NAME;

char const * const EXPAND_FLAG = "to-dir";
char const EXPAND_FLAG_SHORT = 'a';

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

char const * const XSLT_ALL_FLAG = "apply-root";
const int XSLT_ALL_FLAG_CODE = 256 + 4;

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

void libxml_error(void *ctx, const char *msg, ...) {}

// output help message
void output_help(const char* name) {

  printf("Usage: %s [options] <srcML_infile>... [-o <src_outfile>]\n\n"
         "Translates from the the XML source-code representation srcML back to source-code.\n\n"
         "Extracts back to standard output, the disk, or to various general archive formats,\n"
	 "e.g., tar, cpio, zip, and with gzip, bzip2 compression.\n"
         "Provides access to metadata about the srcML document. For srcML archives\n"
	 "provides extraction of specific files, and efficient querying/transformation\n"
	 "using XPath, XSLT, and RelaxNG.\n\n"

	 "srcML archives contain multiple individual source code files,\n"
	 "e.g., an entire project or directory tree.\n\n"

	 "By default, output is to stdout.  You can specify a file for output using the --%s or -%c option.\n"
	 "When no filenames are given input is from stdin and output is to stdout.\n"
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
	  TEXTENCODING_FLAG_SHORT, TEXTENCODING_FLAG_FULL, DEFAULT_TEXT_ENCODING);

  printf("  -%c, --%-17s output text or XML in gzip format\n",
	  COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG);

  printf("  -%c, --%-17s conversion and status information to stderr\n\n",
	  VERBOSE_FLAG_SHORT, VERBOSE_FLAG);

  printf("  -%c, --%-17s output in XML instead of text\n",
	  XML_FLAG_SHORT, XML_FLAG);

  printf("  --%-21s do not output the XML declaration in XML output\n",
	  NO_XML_DECLARATION_FLAG);

  printf("  --%-21s do not output any namespace declarations in XML output\n\n",
	  NO_NAMESPACE_DECLARATION_FLAG);

  printf("Metadata Options:  \n\n"
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

  printf("  -%c, --%-17s display all metadata including file count (individual units)  and exit\n",
	  LONG_INFO_FLAG_SHORT, LONG_INFO_FLAG);

  printf("  --%-21s list all the files in the srcML archive and exit\n\n", LIST_FLAG);

  printf("srcML Archive Options:  \n\n"
	 "  -%c, --%-17s extract individual unit NUM from srcML\n",
	  UNIT_FLAG_SHORT, UNIT_FLAG_FULL);

  printf("  -%c, --%-17s extract all files from srcML and create them in the filesystem\n\n",
	  EXPAND_FLAG_SHORT, EXPAND_FLAG);

  printf("Query and Transformation Options:  \n\n"
	 "  --%-21s apply XPATH expression to each individual unit\n", XPATH_FLAG_FULL);
  printf("  --%-21s apply XSLT_FILE (FILE or URI) transformation to each individual unit\n", XSLT_FLAG_FULL);
  printf("  --%-21s passes a parameter NAME and VAL to an XSLT program\n", STRING_PARAM_FLAG_FULL);
  printf("  --%-21s passes a parameter NAME and XPATH to the XSLT program\n", PARAM_FLAG_FULL);
  printf("  --%-21s output individual units that match RELAXNG_FILE file (FILE or URI)\n", RELAXNG_FLAG_FULL);
  printf("  --%-21s apply an xslt program or xpath query to the root element\n\n", XSLT_ALL_FLAG);

  printf("Examples:  \n\n"
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
	 "%s\n", name, VERSION, COPYRIGHT);
}

void output_settings(const char * name)
{}

void output_features(const char * name)
{}

OPTION_TYPE options = 0;


#ifdef __GNUG__
extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);
#endif

int optionorder[5];
int optioncount = 0;

void output_info(srcMLUtility& su, int options, int optioncount, int optionorder[]);

typedef struct process_options
{
  // option values
  const char* ofilename;
  const char* input_format;
  const char* output_format;
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
} process_options;

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions);

// read and register xpath functions from a file
void register_xpath_functions_from_filename(const char * filename);

int main(int argc, char* argv[]) {

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
     { 0 }
  };

  // register default xpath extension functions
  xpathRegisterDefaultExtensionFunctions();

  // process command-line arguments
  int curarg = process_args(argc, argv, poptions);

  // first command line parameter is input filename
  const char* filename = "-";
  if (curarg < argc) {
    filename = argv[curarg];
    ++curarg;
  }

  /* Special checks for illegal combinations */

  // xml output and src-encoding (switch to encoding?)
  if (isoption(options, OPTION_XML) && isoption(options, OPTION_TEXT_ENCODING)) {

    fprintf(stderr, "%s: Options for xml output and specifying source encoding are incompatible.\n", PROGRAM_NAME);
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

#if defined(__GNUG__) && !defined(__MINGW32__)
  // verify that the output file is not the same as the input file
  struct stat instat = { 0 };
  if (stat(filename, &instat) == -1) {
    perror(PROGRAM_NAME);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  struct stat outstat = { 0 };
  if (stat(poptions.ofilename, &outstat) == -1) {
    perror(PROGRAM_NAME);
    exit(1);
  }

  if ((strcmp(poptions.ofilename, "-") != 0) && instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
    fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n", PROGRAM_NAME, filename, poptions.ofilename);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
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

  try {

    // setup for processing
    srcMLUtility su(filename, poptions.src_encoding, options);

    // list
    if (isoption(options, OPTION_LIST)) {

      su.list();

    // process get attribute options
    } else if (optioncount > 0) {

      // move to the appropriate unit and output the appropriate metadata
      su.move_to_unit(poptions.unit, su, options, optioncount, optionorder);

      // now finish longinfo
      if (isoption(options, OPTION_LONG_INFO)) {
	if (!isoption(options, OPTION_UNIT)) {
	  if (isatty(STDOUT_FILENO))
	    putchar('\r');
	  printf("units=\"%d\"\n", su.curunits());
	}
      }

    // namespace
    } else if (isoption(options, OPTION_NAMESPACE)) {

      su.move_to_unit(poptions.unit, su, options, optioncount, optionorder);

      for (int i = 0; i < poptions.nscount; ++i) {
	
	const char* prefix = su.namespace_ext(poptions.ns[i]);
	if (prefix) {
	  if (poptions.nscount == 1)
	    printf("%s\n", prefix);
	  else {
	    printf("xmlns");
	    if (prefix[0] != '\0')
	      printf(":%s", prefix);
	    printf("=\"%s\"\n", poptions.ns[i]);
	  }
	}
      }

    // process non-attribute options
    } else if (isoption(options, OPTION_NESTED)) {

#ifdef __GNUG__
      // gracefully finish current file in srcML archive mode
      pstd::signal(SIGINT, terminate_handler);
#endif
      // in verbose mode, the current counter is displayed
      // as it counts.  Need to overwrite this
      long count = su.unit_count();

      if (isatty(STDOUT_FILENO))
        putchar('\r');

      printf("%ld\n", count);

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
	exit_status = STATUS_TERMINATED;

    } else if (isoption(options, OPTION_EXPAND)) {

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

	//	su.expand(poptions.ofilename, poptions.output_format, poptions.to_directory);
      }
	*/

    } else if (isoption(options, OPTION_XML)) {

      su.extract_xml(poptions.ofilename, poptions.unit);

    } else if (isoption(options, OPTION_XPATH)) {

      if (poptions.transformcount == 0)
	su.extract_element(poptions.context, poptions.ofilename);
      else
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
	    "%d units\n", PROGRAM_NAME, poptions.unit, e.size);
    exit_status = STATUS_UNIT_INVALID;

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
    { ENCODING_FLAG, no_argument, NULL, ENCODING_FLAG_SHORT },
    { NESTED_FLAG, no_argument, NULL, NESTED_FLAG_SHORT },
    { INFO_FLAG, no_argument, NULL, INFO_FLAG_SHORT },
    { LONG_INFO_FLAG, no_argument, NULL, LONG_INFO_FLAG_SHORT },
    { EXPAND_FLAG, required_argument, NULL, EXPAND_FLAG_SHORT },
    { VERBOSE_FLAG, no_argument, NULL, VERBOSE_FLAG_SHORT },
    { XML_FLAG, no_argument, NULL, XML_FLAG_SHORT },
    { COMPRESSED_FLAG, no_argument, NULL, COMPRESSED_FLAG_SHORT },
    { UNIT_FLAG, required_argument, NULL, UNIT_FLAG_SHORT },
    { TEXTENCODING_FLAG, required_argument, NULL, TEXTENCODING_FLAG_SHORT },
    { NAMESPACE_FLAG, required_argument, NULL, NAMESPACE_FLAG_SHORT },
    { OMIT_FLAG, required_argument, NULL, OMIT_FLAG_SHORT },
    { QUIET_FLAG, no_argument, NULL, QUIET_FLAG_SHORT },
    { NO_XML_DECLARATION_FLAG, no_argument, &curoption, OPTION_XMLDECL | OPTION_XML },
    { NO_NAMESPACE_DECLARATION_FLAG, no_argument, &curoption, OPTION_NAMESPACEDECL | OPTION_XML },
    { SETTINGS_FLAG, no_argument, NULL, SETTINGS_FLAG_CODE },
    { FEATURES_FLAG, no_argument, NULL, FEATURES_FLAG_CODE },
    //    { INPUT_FORMAT_FLAG, required_argument, NULL, INPUT_FORMAT_FLAG_CODE },
    //    { OUTPUT_FORMAT_FLAG, required_argument, NULL, OUTPUT_FORMAT_FLAG_CODE },
    { LIST_FLAG, no_argument, NULL, LIST_FLAG_CODE },
    { REGISTER_EXTENSION_FLAG, required_argument, NULL, REGISTER_EXTENSION_FLAG_CODE },
    { REGISTER_EXTENSION_FILE_FLAG, required_argument, NULL, REGISTER_EXTENSION_FILE_FLAG_CODE },
    { REGISTER_EXTENSION_FUNCTION_FLAG, required_argument, NULL, REGISTER_EXTENSION_FUNCTION_FLAG_CODE },
    { REGISTER_EXTENSION_FUNCTION_FILE_FLAG, required_argument, NULL, REGISTER_EXTENSION_FUNCTION_FILE_FLAG_CODE },
    { EOL_FLAG, required_argument, NULL, EOL_FLAG_CODE },
    { XPATH_FLAG, required_argument, NULL, XPATH_FLAG_CODE },
    { XSLT_FLAG, required_argument, NULL, XSLT_FLAG_CODE },
    { PARAM_FLAG, required_argument, NULL, PARAM_FLAG_CODE },
    { STRING_PARAM_FLAG, required_argument, NULL, STRING_PARAM_FLAG_CODE },
    { RELAXNG_FLAG, required_argument, NULL, RELAXNG_FLAG_CODE },
    { XSLT_ALL_FLAG, no_argument, NULL, XSLT_ALL_FLAG_CODE },
    //    { CONTEXT_FLAG, required_argument, NULL, 'C' },
    { 0, 0, 0, 0 }
  };

  while (1) {
    int lastoptind = optind;
    curoption = 0;
    int option_index = 0;
    int c = getopt_long(argc, argv, "hVo:fdlsxniLavXzU:t:p:", cliargs, &option_index);
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
      break;

    case TEXTENCODING_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_TEXT_ENCODING;

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

    case NESTED_FLAG_SHORT:
      options |= OPTION_NESTED;
      break;

    case INFO_FLAG_SHORT:
      options |= OPTION_INFO;
      break;

    case LONG_INFO_FLAG_SHORT:
      options |= OPTION_LONG_INFO;
      break;

    case EXPAND_FLAG_SHORT:

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_EXPAND;

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
      poptions.unit = pstd::strtol(optarg, &end, 10);

      // validate type of unit number
      if (errno == EINVAL || strlen(end) == strlen(optarg)) {
	fprintf(stderr, "%s: unit option value \"%s\" must be numeric.\n", PROGRAM_NAME, optarg);
	exit(STATUS_UNIT_INVALID);
      }

      // validate range of unit number
      if (poptions.unit <= 0) {
	fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", PROGRAM_NAME, poptions.unit);
	exit(STATUS_UNIT_INVALID);
      }

      break;

    case SETTINGS_FLAG_CODE :
      output_settings(PROGRAM_NAME);
      exit(STATUS_SUCCESS);
      break;

    case FEATURES_FLAG_CODE :
      output_features(PROGRAM_NAME);
      exit(STATUS_SUCCESS);
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
      poptions.registerext[poptions.registerextcount] = (char*) malloc(strlen(end) + 1);
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

      xpathRegisterExtensionFunction(optarg, end);

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
      poptions.params[poptions.paramcount] = (char*) malloc(strlen(end) + 1 + 2);
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

    case XSLT_ALL_FLAG_CODE:

      options |= OPTION_XSLT_ALL;
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
    return STATUS_XMLENCODING_MISSING;
    break;

  case TEXTENCODING_FLAG_SHORT:
    return STATUS_SRCENCODING_MISSING;
    break;

  case UNIT_FLAG_SHORT:
    return STATUS_UNIT_MISSING;
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

void output_info(srcMLUtility& su, int options, int optioncount, int optionorder[]) {

      // output all the namespaces
      if (isoption(options, OPTION_INFO) || isoption(options, OPTION_LONG_INFO)) {

	for (int i = 0; i < MAXNS; ++i) {
	  if (su.nsv[i].first == "")
	    break;

	  printf("%s=\"%s\"\n", su.nsv[i].second.c_str(), su.nsv[i].first.c_str());
	  
	}
      }

      // output attributes in order specified by the options on the command line
      for (int i = 0; i < optioncount; ++i) {

	// find attribute name from option
	const char* attribute_name = "";
	const char* attribute_title = "";
	int option = optionorder[i];

	switch (option) {
	case OPTION_XML_ENCODING:
	  attribute_name = ".encoding";
	  attribute_title = "encoding";
	  break;
	case OPTION_LANGUAGE:
	  attribute_name = UNIT_ATTRIBUTE_LANGUAGE;
	  attribute_title = attribute_name;
	  break;
	case OPTION_DIRECTORY:
	  attribute_name = UNIT_ATTRIBUTE_DIRECTORY;
	  attribute_title = "directory";
	  break;
	case OPTION_FILENAME:
	  attribute_name = UNIT_ATTRIBUTE_FILENAME;
	  attribute_title = attribute_name;
	  break;
	case OPTION_VERSION:
	  attribute_name = UNIT_ATTRIBUTE_VERSION;
	  attribute_title = "src-version";
	  break;
	};

	// output the option
	const char* l = su.attribute(attribute_name);
	if (l) {
	  if (optioncount == 1)
	    printf("%s\n", l);
	  else
	    printf("%s=\"%s\"\n", attribute_title, l);
	}
      }

      if (isoption(options, OPTION_LONG_INFO) && !isoption(options, OPTION_UNIT) && isatty(STDOUT_FILENO))
	    printf("units=\"%d", 1);
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
      xpathRegisterExtensionFunction(extension_function, end);
    }
  }
}
