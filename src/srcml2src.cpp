/*
  srcml2src.cpp

  Copyright (C) 2004-2010  SDML (www.sdml.info)

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
#include "version.h"
#include "srcmlapps.h"
#include "project.h"
#include "srcMLUtility.h"
#include "Options.h"
#include <list>
#include "srcmlns.h"
#include <getopt.h>

int option_error_status(int optopt);

char const * const NAME = "srcml2src";

char const * const EXPAND_FLAG = "extract-all";
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

char const * const XPATH_FLAG = "xpath";
//char const XPATH_FLAG_SHORT = '';
char const * const XPATH_FLAG_FULL = "xpath=XPATH";

char const * const XSLT_FLAG = "xslt";
//char const XSLT_FLAG_SHORT = '';
char const * const XSLT_FLAG_FULL = "xslt=XSLT_FILE";

char const * const PARAM_FLAG = "param";
char const * const PARAM_FLAG_FULL = "param NAME VALUE";
//char const PARAM_FLAG_SHORT = '';

char const * const STRING_PARAM_FLAG = "stringparam";
char const * const STRING_PARAM_FLAG_FULL = "stringparam NAME VALUE";
//char const PARAM_FLAG_SHORT = '';

char const * const XSLT_ALL_FLAG = "xslt-all";
//char const XSLT_ALL_FLAG_SHORT = '';

char const * const RELAXNG_FLAG = "relaxng";
//char const RELAXNG_FLAG_SHORT = '';
char const * const RELAXNG_FLAG_FULL = "relaxng=RELAXNG_FILE";

char const * const CONTEXT_FLAG = "context";
//char const CONTEXT_FLAG_SHORT = '';
char const * const CONTEXT_FLAG_FULL = "context=CONTEXT";

// output help message
void output_help(const char* name) {

  printf("Usage: %s [options] <srcML_infile>... [-o <src_outfile>]\n\n"
         "Translates from the the XML source-code representation srcML to source-code text files.\n"
         "The srcML files can be in xml or gzip compressed xml (detected automatically).\n"
         "Also provides for access to metadata about the srcML document, extracting specific\n"
         "parts of compound srcML documents, querying using XPath and RelaxNG, and\n"
	 "and transformation with XSLT.\n\n"

	 "By default, output is to stdout.  You can specify a file for output using the --%s or -%c option.\n"
	 "When no filenames are given input is from stdin and output is to stdout.\n"
	 "An input filename of '-' also reads from stdin.\n\n"

	 "Any input file, including XSLT or RelaxNG file, can be a local\n"
	 "filename (FILE) or a URI with the protocols http:, ftp:, or file:\n\n"

	 "Options:\n", name, OUTPUT_FLAG, OUTPUT_FLAG_SHORT);

  printf("  -%c, --%-17s display this help and exit\n", HELP_FLAG_SHORT, HELP_FLAG);
  printf("  -%c, --%-17s display version number and exit\n\n", VERSION_FLAG_SHORT, VERSION_FLAG);

  printf("  -%c, --%-17s write result to OUTPUT which is a FILE or URI\n", OUTPUT_FLAG_SHORT, OUTPUT_FLAG_FULL);

  printf("  -%c, --%-17s set the output source encoding to ENC (default:  %s) \n",
	  TEXTENCODING_FLAG_SHORT, TEXTENCODING_FLAG_FULL, DEFAULT_TEXT_ENCODING);

  printf("  -%c, --%-17s output text or XML in gzip format\n",
	  COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG);

  printf("  -%c, --%-17s conversion and status information to stderr\n\n",
	  VERBOSE_FLAG_SHORT, VERBOSE_FLAG);

  printf("  -%c, --%-17s output in XML instead of text\n",
	  XML_FLAG_SHORT, XML_FLAG);

  printf("  --%-21s do not output the default XML declaration in XML output\n",
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

  printf("  -%c, --%-17s display number of nested units and exit\n\n", NESTED_FLAG_SHORT, NESTED_FLAG);

  printf("  -%c, --%-17s display most metadata (except nested unit count) and exit\n",
	  INFO_FLAG_SHORT, INFO_FLAG);

  printf("  -%c, --%-17s display all metadata (including nested unit count) and exit\n\n",
	  LONG_INFO_FLAG_SHORT, LONG_INFO_FLAG);

  printf("Compound Document Options:  \n\n"
	 "  -%c, --%-17s extract nested unit NUM from a compound srcML document\n",
	  UNIT_FLAG_SHORT, UNIT_FLAG_FULL);

  printf("  -%c, --%-17s extract all files from a compound srcML document\n\n",
	  EXPAND_FLAG_SHORT, EXPAND_FLAG);

  printf("Query and Transformation Options:  \n\n"
	 "  --%-20s apply XPATH expression to each nested unit\n", XPATH_FLAG_FULL);
  printf("  --%-20s apply XSLT_FILE (FILE or URI) transformation to each nested unit\n", XSLT_FLAG_FULL);
  printf("  --%-20s passes a parameter NAME and VALUE to XSLT\n", PARAM_FLAG_FULL);
  printf("  --%-20s passes a string parameter NAME and VALUE to XSLT\n", STRING_PARAM_FLAG_FULL);
  printf("  --%-20s output nested units that match RELAXNG_FILE file (FILE or URI) grammar\n\n", RELAXNG_FLAG_FULL);

  printf("Examples:  \n"
	    "  %1$s                          (read from standard input, write to standard output)\n"
	    "  %1$s main.cpp.xml             (read from file main.cpp.xml, write to standard output)\n"
	    "  %1$s main.cpp.xml -o main.cpp (read from file main.cpp.xml, write to file main.cpp)\n"
	    "\n"
	    "  %1$s http://www.sdml.info/projects/srcml/ex/main.cpp.xml (read from URI, write to standard output)\n"
	    "\n"
	    "  %1$s --language               (read from standard input, output language attribute)\n"
	    "  %1$s --directory              (read from standard input, output directory attribute)\n"
	    "  %1$s --filename               (read from standard input, output filename attribute)\n"
	    "  %1$s --src-version            (read from standard input, output version attribute)\n\n", name);

  printf("www.sdml.info\n"
         "Report bugs to %s\n", EMAIL_ADDRESS);
}

// output version message
void output_version(const char* name) {

  printf("%s Version %s\n"
	 "%s\n", name, VERSION, COPYRIGHT);
}

int options = 0;


#ifdef __GNUG__
extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);
#endif

int optionorder[5];
int optioncount = 0;

const int MAXPARAMS = 32;

const int MAXXSLT = 32;

const int MAXXPATH = 32;

//const char* ofilename = "-";

typedef struct process_options
{
  // option values
  const char* ofilename;
  const char* src_encoding;
  int unit;
  const char* context;
  std::list<const char*> ns;
  int paramcount;
  const char* params[MAXPARAMS * 2 + 1];
  int xsltcount;
  const char* xsltfiles[MAXXSLT + 1];
  int xpathcount;
  const char* xpathexpr[MAXXPATH + 1];
  std::list<const char*> xpathexprlist;
} process_options;

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions);

// option values
//const char* src_encoding = DEFAULT_TEXT_ENCODING;
//int unit = 0;
//const char* context = "src:unit";
//std::list<const char*> ns;

int main(int argc, char* argv[]) {

  /* signal handling */

#ifdef __GNUG__
  // toggling verbose flag
  pstd::signal(SIGUSR1, verbose_handler);
#endif

  int exit_status = EXIT_SUCCESS;

  process_options poptions = 
    {
     "-",
     DEFAULT_TEXT_ENCODING,
     0,
     "src:unit",
     std::list<const char*>(),
     0,
     { 0 },
     0,
     { 0 },
     0,
     { 0 },
     std::list<const char*>()
  };

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

    fprintf(stderr, "%s: Options for xml output and specifying source encoding are incompatible.\n", argv[0]);
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

  /*
  // skip encoding and specifying encoding
  if (isoption(options, OPTION_SKIP_ENCODING) && isoption(options, OPTION_TEXT_ENCODING)) {

    std::cerr << NAME << ": Options for skipping encoding and specifying source encoding are incompatible.\n";
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

  // verify that the input file exists
  struct stat instat;
  int result = stat(filename, &instat);
  if ((strcmp(filename, "-") != 0) && result == -1) {
    fprintf(stderr, "%s: Problem with Input file '%s'\n", NAME, filename);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  // verify that the output file is not the same as the input file
#ifdef __GNUG__
  struct stat outstat;
  stat(ofilename, &outstat);
  if ((strcmp(ofilename, "-") != 0) && instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
    fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n", NAME, filename, ofilename);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
#endif
  */
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

    // process get attribute options
    if (optioncount > 0) {

      // move to the appropriate unit
      su.move_to_unit(poptions.unit);

      // output all the namespaces
      if (isoption(options, OPTION_INFO) || isoption(options, OPTION_LONG_INFO)) {

	const PROPERTIES_TYPE& ns = su.getNS();
	for (PROPERTIES_TYPE::const_iterator iter = ns.begin(); iter != ns.end(); ++iter)
	  printf("%s=\"%s\"\n", iter->second.c_str(), iter->first.c_str());
      }

      // output get attributes in order specified
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

      if (isoption(options, OPTION_LONG_INFO)) {
	if (!isoption(options, OPTION_UNIT))
	  printf("nested=\"%d\"\n", su.curunits());
      }

    // namespace
    } else if (isoption(options, OPTION_NAMESPACE)) {

      su.move_to_unit(poptions.unit);

      for (std::list<const char*>::const_iterator iter = poptions.ns.begin(); iter != poptions.ns.end(); ++iter) {

	const char* prefix = su.namespace_ext(*iter);
	if (prefix) {
	  if (poptions.ns.size() == 1)
	    printf("%s\n", prefix);
	  else {
	    printf("xmlns");
	    if (prefix[0] != '\0')
	      printf(":%s", prefix);
	    printf("=\"%s\"\n", *iter);
	  }
	}
      }

    // process non-attribute options
    } else if (isoption(options, OPTION_NESTED)) {

#ifdef __GNUG__
      // gracefully finish current file in compound document mode
      pstd::signal(SIGINT, terminate_handler);
#endif

      // in verbose mode, the current counter is displayed
      // as it counts.  Need to overwrite this
      if (isoption(options, OPTION_VERBOSE))
	puts("\r");

      printf("%d\n", su.unit_count());

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
	exit_status = STATUS_TERMINATED;

    } else if (isoption(options, OPTION_EXPAND)) {

#ifdef __GNUG__
      // gracefully finish current file in compound document mode
      pstd::signal(SIGINT, terminate_handler);
#endif

      su.expand();

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
	exit_status = STATUS_TERMINATED;

    } else if (isoption(options, OPTION_XML)) {

      su.extract_xml(poptions.ofilename, poptions.unit);

    } else if (isoption(options, OPTION_XPATH)) {

      if (poptions.xpathexprlist.empty())
	su.extract_element(poptions.context, poptions.ofilename);
      else
	su.xpath(poptions.ofilename, poptions.context, poptions.xpathexpr);

    } else if (isoption(options, OPTION_XSLT)) {

      su.xslt(poptions.context, poptions.ofilename, poptions.xsltfiles, poptions.params, poptions.paramcount);

    } else if (isoption(options, OPTION_RELAXNG)) {

      su.relaxng(poptions.ofilename, poptions.xpathexpr);

    } else {

      su.extract_text(poptions.ofilename, poptions.unit);

    }

  } catch (const OutOfRangeUnitError& e) {

    fprintf(stderr, "%s: unit %d  was selected from compound srcML document that contains "
	    "%d nested units\n", argv[0], poptions.unit, e.size);
    exit_status = STATUS_UNIT_INVALID;

    return exit_status;
    /*
  } catch (LibXMLError) {
    exit_status = STATUS_UNIT_INVALID;
    throw "XML error";
    */
  } catch (TerminateLibXMLError error) {

    exit_status = STATUS_TERMINATED;

    return exit_status;

  } catch (LibXMLError error) {

    if (error.getErrorNum() == 0) {
      exit_status = STATUS_INPUTFILE_PROBLEM;
      fprintf(stderr, "%s: Unable to open input file as XML\n", argv[0]);
    }

  } catch (const char* s) {
    
    fprintf(stderr, "%s: %s\n", argv[0], s);
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
    { EXPAND_FLAG, no_argument, NULL, EXPAND_FLAG_SHORT },
    { VERBOSE_FLAG, no_argument, NULL, VERBOSE_FLAG_SHORT },
    { XML_FLAG, no_argument, NULL, XML_FLAG_SHORT },
    { COMPRESSED_FLAG, no_argument, NULL, COMPRESSED_FLAG_SHORT },
    { UNIT_FLAG, required_argument, NULL, UNIT_FLAG_SHORT },
    { TEXTENCODING_FLAG, required_argument, NULL, TEXTENCODING_FLAG_SHORT },
    { NAMESPACE_FLAG, required_argument, NULL, NAMESPACE_FLAG_SHORT },
    { NO_XML_DECLARATION_FLAG, no_argument, &curoption, OPTION_XMLDECL | OPTION_XML },
    { NO_NAMESPACE_DECLARATION_FLAG, no_argument, &curoption, OPTION_NAMESPACEDECL | OPTION_XML },
    { XPATH_FLAG, required_argument, NULL, 'P' },
    { XSLT_FLAG, required_argument, NULL, 'S' },
    { PARAM_FLAG, required_argument, NULL, 'A' },
    { STRING_PARAM_FLAG, required_argument, NULL, 'B' },
    { RELAXNG_FLAG, required_argument, NULL, 'R' },
    //    { CONTEXT_FLAG, required_argument, NULL, 'C' },
    { 0, 0, 0, 0 }
  };

  while (1) {
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
      fprintf(stderr, "Try '%s %s' for more information.\n", argv[0], HELP_FLAG);
      exit(option_error_status(optopt));
    }

    char* end = 0;
    switch(c) {

    case 'h': 
      output_help(argv[0]);
      exit(STATUS_SUCCESS);
      break;

    case 'V': 
      output_version(argv[0]);
      exit(STATUS_SUCCESS);
      break;

    case 'o': 
      poptions.ofilename = optarg;
      break;

    case 'f':
      options |= OPTION_FILENAME;
      optionorder[optioncount++] = OPTION_FILENAME;
      break;

    case 'd':
      options |= OPTION_DIRECTORY;
      optionorder[optioncount++] = OPTION_DIRECTORY;
      break;

    case 's':
      options |= OPTION_VERSION;
      optionorder[optioncount++] = OPTION_VERSION;
      break;

    case 'l':
      options |= OPTION_LANGUAGE;
      optionorder[optioncount++] = OPTION_LANGUAGE;
      break;

    case 'x':
      options |= OPTION_XML_ENCODING;
      optionorder[optioncount++] = OPTION_XML_ENCODING;
      break;

    case 'n':
      options |= OPTION_NESTED;
      break;

    case 'i':
      options |= OPTION_INFO;
      break;

    case 'L':
      options |= OPTION_LONG_INFO;
      break;

    case 'a':
      options |= OPTION_EXPAND;
      break;

    case 'v':
      options |= OPTION_VERBOSE;
      break;

    case 'X':
      options |= OPTION_XML;
      break;

    case 'z':
      options |= OPTION_COMPRESSED;
      break;

    case 'U':
      options |= OPTION_UNIT;

      // try to convert to number
      poptions.unit = pstd::strtol(optarg, &end, 10);

      // validate type of unit number
      if (errno == EINVAL || strlen(end) == strlen(optarg)) {
	fprintf(stderr, "%s: unit option value \"%s\" must be numeric.\n", argv[0], optarg);
	exit(STATUS_UNIT_INVALID);
      }

      // validate range of unit number
      if (poptions.unit <= 0) {
	fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", argv[0], poptions.unit);
	exit(STATUS_UNIT_INVALID);
      }

      break;

    case 'P':
      options |= OPTION_XPATH;
      poptions.xpathexprlist.push_back(poptions.xpathexpr[poptions.xpathcount++] = optarg);
      break;

    case 'S':

      options |= OPTION_XSLT;
      poptions.xsltfiles[poptions.xsltcount++] = optarg;
      break;

    case 'A':

      // param name
      poptions.params[poptions.paramcount++] = optarg;
      
      // param value
      poptions.params[poptions.paramcount++] = argv[optind++];
      break;

    case 'B':

      // param name
      poptions.params[poptions.paramcount++] = optarg;
      
      // param value
      poptions.params[poptions.paramcount++] = argv[optind++];
      break;

    case 'R':
      options |= OPTION_RELAXNG;
      poptions.xpathexprlist.push_back(poptions.xpathexpr[poptions.xpathcount++] = optarg);
      break;

      /*
    case 'C':
      options |= OPTION_XPATH;
      poptions.context = optarg;
      break;
      */

    case 't':
      options |= OPTION_TEXT_ENCODING;

      poptions.src_encoding = optarg;

      // validate source encoding
      if (!srcMLUtility::checkEncoding(poptions.src_encoding)) {
	fprintf(stderr, "%s: text encoding \"%s\" is not supported.\n", argv[0], poptions.src_encoding);
	exit(STATUS_UNKNOWN_ENCODING);
      }
      break;

    case 'p':
      options |= OPTION_NAMESPACE;

      poptions.ns.push_back(optarg);
      break;

    default:
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

  case 'x':
    return STATUS_XMLENCODING_MISSING;
    break;

  case 't':
    return STATUS_SRCENCODING_MISSING;
    break;

  case 'U':
    return STATUS_UNIT_MISSING;
    break;

  case '\0':
    return STATUS_UNKNOWN_OPTION;
    break;
  };

  return 0;
}
