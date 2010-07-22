/*
  src2srcml.cpp

  Copyright (C) 2002-2010  SDML (www.sdml.info)

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

  Main program to run the srcML translator.
*/

#include <cstring>
#include <sys/stat.h>
#include "version.h"
#include "srcmlapps.h"
#include "srcmlns.h"
#include "Options.h"
#include "project.h"
#include "Language.h"
#include "srcMLTranslator.h"
#include "URIStream.h"
#include <getopt.h>
#include <dirent.h>

const char* PROGRAM_NAME = "";

#ifdef CURL
#include "libxml_curl_io.h"
#endif

#ifdef LIBARCHIVE
#include "libxml_archive_read.h"
#include "libxml_archive_write.h"
#endif

int option_error_status(int optopt);

void src2srcml_file(srcMLTranslator& translator, const char* path, OPTION_TYPE& options, const char* dir, const char* filename, const char* version, int language, int tabsize, int& count);

using namespace LanguageName;

const char* const DEBUG_FLAG = "debug";
const char DEBUG_FLAG_SHORT = 'g';

const char* const LITERAL_FLAG = "literal";

const char* const OPERATOR_FLAG = "operator";

const char* const MODIFIER_FLAG = "modifier";

const char* const POSITION_FLAG = "position";
const char* const TABS_FLAG = "tabs";
const char TABS_FLAG_CODE = 'T';
const char* const TABS_FLAG_FULL = "tabs=NUMBER";

const char* const INTERACTIVE_FLAG = "interactive";
const char INTERACTIVE_FLAG_SHORT = 'c';

const char* const CPP_MARKUP_ELSE_FLAG = "cpp-markup-else";
const char CPP_MARKUP_ELSE_FLAG_CODE = 'm';

const char* const CPP_TEXTONLY_ELSE_FLAG = "cpp-text-else";
const char CPP_TEXTONLY_ELSE_FLAG_CODE = 'E';

const char* const CPP_MARKUP_IF0_FLAG = "cpp-markup-if0";
const char CPP_MARKUP_IF0_FLAG_CODE = '0';

const char* const CPP_TEXTONLY_IF0_FLAG = "cpp-text-if0";
const char CPP_TEXTONLY_IF0_FLAG_CODE = 'p';

const char* const EXPRESSION_MODE_FLAG = "expression";
const char EXPRESSION_MODE_FLAG_SHORT = 'e';

const char* const SELF_VERSION_FLAG = "self-version";

const char* const DEFAULT_XML_ENCODING = "UTF-8";

const char* const FILELIST_FLAG = "files-from";
const char FILELIST_FLAG_CODE = 'F';
const char* const FILELIST_FLAG_FULL = "files-from=INPUT";

const char* const XMLNS_FLAG = "xmlns";
const char XMLNS_FLAG_CODE = 'X';
const char* const XMLNS_DEFAULT_FLAG_FULL = "xmlns=URI";
const char* const XMLNS_FLAG_FULL = "xmlns:PREFIX=URI";

const char* const REGISTER_EXT_FLAG = "register-ext";
const char REGISTER_EXT_FLAG_CODE = 'R';

const char* const OLD_FILENAME_FLAG = "old-filename";
const char OLD_FILENAME_FLAG_CODE = 'O';

const char* const SKIP_DEFAULT_FLAG = "skip-default";
const char SKIP_DEFAULT_FLAG_CODE = 'S';

const char* const RECURSIVE_FLAG = "recursive";
const char RECURSIVE_FLAG_CODE = 'r';

const char* const QUIET_FLAG = "quiet";
const char QUIET_FLAG_SHORT = 'q';

const int DEFAULT_LANGUAGE = srcMLTranslator::LANGUAGE_CXX;

const char* const EXAMPLE_TEXT_FILENAME="foo.cpp";
const char* const EXAMPLE_XML_FILENAME="foo.cpp.xml";

const char FILELIST_COMMENT = '#';

bool process;

struct uridata {
  char const * const uri;
  int option;
};

uridata uris[] = {

  { SRCML_SRC_NS_URI,          0 },
  { SRCML_CPP_NS_URI,          OPTION_CPP },
  { SRCML_ERR_NS_URI,          OPTION_DEBUG },
  { SRCML_EXT_LITERAL_NS_URI,  OPTION_LITERAL },
  { SRCML_EXT_OPERATOR_NS_URI, OPTION_OPERATOR },
  { SRCML_EXT_MODIFIER_NS_URI, OPTION_MODIFIER },
  { SRCML_EXT_POSITION_NS_URI, OPTION_POSITION },
};

const char* urisprefix[] = {

  SRCML_SRC_NS_PREFIX_DEFAULT,
  SRCML_CPP_NS_PREFIX_DEFAULT,
  SRCML_ERR_NS_PREFIX_DEFAULT,
  SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
  SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
  SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
  SRCML_EXT_POSITION_NS_PREFIX_DEFAULT,
};

const int num_prefixes = sizeof(uris) / sizeof(uris[0]);

void libxml_error(void *ctx, const char *msg, ...) {}

// output help
void output_help(const char* name) {
  printf( "Usage: %s [options] <src_infile>... [-o <srcML_outfile>]\n\n"

	  "Translates source-code files in C, C++, and Java into the XML source-code representation srcML.\n"
	  "Both single and multiple files can be stored in a single srcML document.\n\n"

	  "By default, output is to stdout.  You can specify a file for output using the --%s or -%c option.\n"
	  "When no filenames are given input is from stdin and output is to stdout.\n"
	  "An input filename of '-' also reads from stdin.\n\n"

    	  "Any input file can be a local filename (FILE) or a URI with the protocols http:, ftp:, or file:\n\n"

	  "Language is based on the file extension, with a default of C++.  It can also be directly\n"
	  "set using the --language option.\n"
	  "\n"
	  "Options:\n", name, OUTPUT_FLAG, OUTPUT_FLAG_SHORT);

  printf("  -%c, --%-14s display this help and exit\n",      HELP_FLAG_SHORT, HELP_FLAG);
  printf("  -%c, --%-14s display version number and exit\n\n", VERSION_FLAG_SHORT, VERSION_FLAG);

  printf("  --%-18s read list of source file names from INPUT which is a FILE or URI,\n"
	 "                       to form a compound srcML document\n",
	  FILELIST_FLAG_FULL);
  printf("  --%-18s ???\n\n", INPUT_FORMAT_FLAG);
  printf("  -%c, --%-14s write result to OUTPUT which is a FILE or URI\n", OUTPUT_FLAG_SHORT, OUTPUT_FLAG_FULL);
  printf("  --%-18s ???\n\n", OUTPUT_FORMAT_FLAG);

  printf("  -%c, --%-14s store output in a compound srcML document, default for multiple input files\n",
	  COMPOUND_FLAG_SHORT, COMPOUND_FLAG);

  printf("  -%c, --%-14s expression mode for translating a single expression not in a statement\n",
	  EXPRESSION_MODE_FLAG_SHORT, EXPRESSION_MODE_FLAG);

  printf("  -%c, --%-14s set the output XML encoding to ENC (default:  %s)\n",
	  ENCODING_FLAG_SHORT, ENCODING_FLAG, DEFAULT_XML_ENCODING);

  printf("  -%c, --%-14s set the input source encoding to ENC (default:  %s)\n\n",
	  TEXTENCODING_FLAG_SHORT, TEXTENCODING_FLAG, DEFAULT_TEXT_ENCODING);

  printf("  -%c, --%-14s output in gzip format\n", COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG);

  printf("  -%c, --%-14s immediate output while parsing, default for keyboard input\n", INTERACTIVE_FLAG_SHORT, INTERACTIVE_FLAG);

  printf("  -%c, --%-14s markup translation errors in namespace %s\n",
	  DEBUG_FLAG_SHORT, DEBUG_FLAG, SRCML_ERR_NS_URI);

  printf("  -%c, --%-14s conversion and status information to stderr\n\n",
	  VERBOSE_FLAG_SHORT, VERBOSE_FLAG);

  printf("  --%-18s do not output the default XML declaration\n", NO_XML_DECLARATION_FLAG);
  printf("  --%-18s do not output any namespace declarations\n\n", NO_NAMESPACE_DECLARATION_FLAG);

  printf("Metadata Options:\n\n"
	 "  -%c, --%-14s set the language to %s, %s (default), %s, or %s\n",
	 LANGUAGE_FLAG_SHORT, LANGUAGE_FLAG_FULL,
	 LANGUAGE_C, LANGUAGE_CXX, LANGUAGE_JAVA, LANGUAGE_ASPECTJ);

  printf("  -%c, --%-14s set the directory attribute to DIR\n",
	 DIRECTORY_FLAG_SHORT, DIRECTORY_FLAG_FULL);

  printf("  -%c, --%-14s set the filename attribute to FILE\n",
	 FILENAME_FLAG_SHORT, FILENAME_FLAG_FULL);

  printf("  -%c, --%-14s set the version attribute to VER\n\n",
	 SRCVERSION_FLAG_SHORT, SRCVERSION_FLAG_FULL);

  printf("Markup Extensions:\n\n");

  printf("  --%-18s markup literal values in namespace \"%s\"\n",
	 LITERAL_FLAG, SRCML_EXT_LITERAL_NS_URI);

  printf("  --%-18s markup operators in namespace \"%s\"\n",
	 OPERATOR_FLAG, SRCML_EXT_OPERATOR_NS_URI);

  printf("  --%-18s markup type modifiers in namespace \"%s\"\n",
	 MODIFIER_FLAG, SRCML_EXT_MODIFIER_NS_URI);

  printf("\nLine/Column Position:\n\n");

  printf("  --%-18s markup literal values in namespace \"%s\"\n",
	 POSITION_FLAG, SRCML_EXT_POSITION_NS_URI);

  printf("  --%-18s have tabs NUMBER characters apart.  Default is %d\n",
	 TABS_FLAG_FULL, 8);

  printf("\nPrefix Options:\n\n");

  printf("  --%-18s set the default namespace URI\n", XMLNS_DEFAULT_FLAG_FULL);

  printf("  --%-18s set the namespace PREFIX for the namespace URI\n\n", XMLNS_FLAG_FULL);

  printf("                       Predefined URIs and Prefixes:\n");
  printf("                         xmlns=\"%s\"\n", SRCML_SRC_NS_URI);
  printf("                         xmlns:%s=\"%s\"\n", SRCML_CPP_NS_PREFIX_DEFAULT, SRCML_CPP_NS_URI);
  printf("                         xmlns:%s=\"%s\"\n", SRCML_ERR_NS_PREFIX_DEFAULT, SRCML_ERR_NS_URI);

  printf("\nCPP Markup Options:\n\n");

  printf("  --%-18s markup cpp #else regions (default)\n", CPP_MARKUP_ELSE_FLAG);
  printf("  --%-18s leave cpp #else regions as text\n\n", CPP_TEXTONLY_ELSE_FLAG);

  printf("  --%-18s markup cpp #if 0 regions\n", CPP_MARKUP_IF0_FLAG);
  printf("  --%-18s leave cpp #if 0 regions as text (default)\n\n", CPP_TEXTONLY_IF0_FLAG);

  printf("Examples:  \n"
	 "  %1$s                (read from standard input, write to standard output)\n"
	 "  %1$s m.cpp          (read from file m.cpp, write to standard output)\n"
	 "  %1$s m.cpp -o m.cpp.xml (read from file m.cpp, write to file m.cpp.xml)\n"
	 "\n"
	 "  %1$s http://www.sdml.info/projects/srcml/ex/main.cpp (read from URI)\n"
	 "\n"
	 "  %1$s --directory=src --filename=m.cpp m.cpp -o m.cpp.xml "
	 "(element unit attributes dir \"src\", filename \"m.cpp\")\n"
	 "  %1$s --src-encoding=UTF-8 m.cpp m.cpp.xml         "
	 "(encoding of input text file is UTF-8)\n"
	 "  %1$s --xml-encoding=ISO-8859-1 m.cpp m.cpp.xml    "
	 "(set encoding of srcML file to ISO-8859-1)\n\n", name);

  printf("www.sdml.info\n"
	 "Report bugs to %s\n", EMAIL_ADDRESS);
}

// output version message
void output_version(const char* name) {
  printf("%s Version %s\n%s\n", name, VERSION,COPYRIGHT);
}

void output_settings(const char * name)
{}

void output_features(const char * name)
{}

OPTION_TYPE options = OPTION_CPP_MARKUP_ELSE;

#ifdef __GNUG__
extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);
#endif

struct process_options
{
  // options
  // output filename
  const char* srcml_filename;
  const char* fname;
  const char * input_format;
  const char * output_format;
  int language;
  const char* src_encoding;
  const char* xml_encoding;
  const char* given_directory;
  const char* given_filename;
  const char* given_version;
  int tabsize;
  bool prefixchange[num_prefixes];
};

process_options* gpoptions = 0;

void process_dir(srcMLTranslator& translator, const char* dname, process_options& poptions, int& count);
void process_filelist(srcMLTranslator& translator, process_options& poptions, int& count);

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions);

// register standard file extensions
void register_standard_file_extensions();

int main(int argc, char* argv[]) {

  options |= OPTION_SKIP_DEFAULT;

  PROGRAM_NAME = argv[0];

  int exit_status = EXIT_SUCCESS;

  LIBXML_TEST_VERSION

  xmlGenericErrorFunc handler = (xmlGenericErrorFunc) libxml_error;
  initGenericErrorDefaultFunc(&handler);

#ifdef CURL
  if (xmlRegisterInputCallbacks(curlMatch, curlOpen, curlRead, curlClose) < 0) {
    fprintf(stderr, "failed to register curl handler\n");
    exit(1);
  }
#endif

  /* signal handling */

#ifdef __GNUG__
  // signal to toggle verbose flag
  pstd::signal(SIGUSR1, verbose_handler);
#endif

  register_standard_file_extensions();

  process_options poptions =
    {
      0,
      "-",
      0,
      0,
      0,
      DEFAULT_TEXT_ENCODING,
      DEFAULT_XML_ENCODING,
      0,
      0,
      0,
      8,
      {
	false,
	false,
	false,
	false,
	false,
	false,
      }
    };

  gpoptions = &poptions;

  // process command-line arguments
  int curarg = process_args(argc, argv, poptions);

  /* Special checks for illegal combinations */

  // first command line parameter after options are the input filenames
  int input_arg_start = curarg;
  int input_arg_end = argc - 1;
  int input_arg_count = input_arg_end - input_arg_start + 1;

  // no output specified, so use stdout
  if (!poptions.srcml_filename)
    poptions.srcml_filename = "-";

  // special check for standard input (only) and no specified language
  if (!isoption(options, OPTION_FILELIST) && ((input_arg_count == 0) ||
       (input_arg_count == 1 && strcmp(argv[input_arg_start], "-") == 0)) &&
      (poptions.language == 0)) {

    fprintf(stderr, "%s:  Language must be specified for standard input.\n", PROGRAM_NAME);
    exit(1);
  }

  // if more than one input filename assume nested
  if (input_arg_count > 1)
    options |= OPTION_NESTED;

  // verify that the output filename is not the same as any of the input filenames
#ifdef __GNUG__
  struct stat outstat;
  stat(poptions.srcml_filename, &outstat);
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

    struct stat instat;
    stat(argv[i], &instat);
    if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
      fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n",
	      argv[0], argv[i], poptions.srcml_filename);
      exit(STATUS_INPUTFILE_PROBLEM);
    }
  }

  // verify that only one input pipe is STDIN
  struct stat stdiostat;
  fstat(STDIN_FILENO, &stdiostat);
  int stdiocount = 0;
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

	struct stat instat;
	stat(argv[i], &instat);
	if (instat.st_ino == stdiostat.st_ino)
	  ++stdiocount;

	if (stdiocount > 1) {
	  fprintf(stderr, "%s: Multiple input files are from standard input.\n",
		  PROGRAM_NAME);
	  exit(STATUS_INPUTFILE_PROBLEM);
	}
  }
#endif
	
  // make sure user did not specify duplicate prefixes as an option
  for (int i = 0; i < num_prefixes - 1; ++i) {
    for (int j = i + 1; j < num_prefixes; ++j)
      if(strcmp(urisprefix[i], urisprefix[j]) == 0) {

	fprintf(stderr, "%s: Namespace conflict for ", PROGRAM_NAME);
	if (urisprefix[i] == '\0') {
	  fprintf(stderr, "default prefix\n");
	} else {
	  fprintf(stderr, "prefix \'%s\'\n", urisprefix[i]);
	}
	fprintf(stderr, "Prefix URI conflicts:\n  %s\n  %s\n", uris[i].uri, uris[j].uri);

	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
  }

  // automatic interactive use from stdin (not on redirect or pipe)
#ifdef __GNUG__
  if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

    if (isatty(STDIN_FILENO))
      options |= OPTION_INTERACTIVE;
  }
#endif

#ifdef LIBARCHIVE

  if (!isoption(options, OPTION_FILELIST)) {
  if (xmlRegisterInputCallbacks(archiveReadMatch, archiveReadOpen, archiveRead, archiveReadClose) < 0) {
    fprintf(stderr, "%s: failed to register archive handler\n", argv[0]);
    exit(1);
  }
  }
  /*
  if (xmlRegisterOutputCallbacks(archiveWriteMatch_src2srcml, archiveWriteOpen, archiveWrite, archiveWriteClose) < 0) {
    fprintf(stderr, "%s: failed to register archive handler\n", argv[0]);
    exit(1);
  }
  */
#endif

  try {

    // for single file specified on command line, grab the language from the extension
    if (poptions.language == 0 && input_arg_count == 1) {

      char* path = argv[input_arg_start];
      poptions.language = Language::getLanguageFromFilename(path);
      /*
      if (!poptions.language) {
	  fprintf(stderr, "Skipping '%s'.  No language can be determined.\n", path);
	  exit(1);
      }
      */
    }

    // turnon cpp namespace for non Java-based languages
    if (!(poptions.language == srcMLTranslator::LANGUAGE_JAVA || poptions.language == srcMLTranslator::LANGUAGE_ASPECTJ))
      options |= OPTION_CPP;

    // translator from input to output using determined language
    srcMLTranslator translator(poptions.language == 0 ? DEFAULT_LANGUAGE : poptions.language, poptions.src_encoding, poptions.xml_encoding, poptions.srcml_filename, options, poptions.given_directory, poptions.given_filename, poptions.given_version, urisprefix, poptions.tabsize);

    // output source encoding
    if (isoption(options, OPTION_VERBOSE)) {
      fprintf(stderr, "Source encoding:  %s\n", poptions.src_encoding);
      fprintf(stderr, "XML encoding:  %s\n", poptions.xml_encoding);
    }

    // filecount for verbose mode
    int count = 0;

#ifdef __GNUG__
    // setup so we can gracefully stop after a file at a time
    pstd::signal(SIGINT, terminate_handler);
#endif

    // translate input filenames from list in file
    if (isoption(options, OPTION_FILELIST)) {

      if (!poptions.fname && input_arg_count > 0)
        poptions.fname = argv[input_arg_start];
      if (!poptions.fname)
        poptions.fname = "-";

      process_filelist(translator, poptions, count);

    // translate from standard input
    } else if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

      options &= ~OPTION_SKIP_DEFAULT;

      // translate from standard input using any directory, filename and version given on the command line
      src2srcml_file(translator, "-", options,
		     poptions.given_directory,
		     poptions.given_filename,
		     poptions.given_version,
		     poptions.language,
		     poptions.tabsize, count);

    // translate filenames from the command line
    } else {

      // translate in batch the input files on the command line extracting the directory and filename attributes
      // from the full path
      for (int i = input_arg_start; i <= input_arg_end; ++i) {
	
	// in verbose mode output the currently processed filename
	if (isoption(options, OPTION_VERBOSE))
	  fprintf(stderr, "Input:\t%s\n", strcmp(argv[i], "-") == 0 ? "" : argv[i]);

	src2srcml_file(translator, argv[i], options,
		       input_arg_count == 1 ? poptions.given_directory : 0,
		       input_arg_count == 1 ? poptions.given_filename : 0,
		       input_arg_count == 1 ? poptions.given_version : 0,
		       poptions.language,
		       poptions.tabsize,
		       count);
      }
    }

  } catch (srcEncodingException) {
    fprintf(stderr, "Translation encoding problem\n");
    exit(STATUS_UNKNOWN_ENCODING);
  }

  return exit_status;
}

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions) {

  bool cpp_if0 = false;
  bool cpp_else = false;

  int curoption = 0;
  struct option cliargs[] = {
    { HELP_FLAG, no_argument, NULL, HELP_FLAG_SHORT },
    { VERSION_FLAG, no_argument, NULL, VERSION_FLAG_SHORT },
    { OUTPUT_FLAG, required_argument, NULL, OUTPUT_FLAG_SHORT },
    { COMPOUND_FLAG, no_argument, NULL, COMPOUND_FLAG_SHORT },
    { EXPRESSION_MODE_FLAG, no_argument, NULL, EXPRESSION_MODE_FLAG_SHORT },
    { ENCODING_FLAG, required_argument, NULL, ENCODING_FLAG_SHORT },
    { TEXTENCODING_FLAG, required_argument, NULL, TEXTENCODING_FLAG_SHORT },
    { COMPRESSED_FLAG, no_argument, NULL, COMPRESSED_FLAG_SHORT },
    { INTERACTIVE_FLAG, no_argument, NULL, INTERACTIVE_FLAG_SHORT },
    { DEBUG_FLAG, no_argument, NULL, DEBUG_FLAG_SHORT },
    { VERBOSE_FLAG, no_argument, NULL, VERBOSE_FLAG_SHORT },
    { LANGUAGE_FLAG, required_argument, NULL, LANGUAGE_FLAG_SHORT },
    { DIRECTORY_FLAG, required_argument, NULL, DIRECTORY_FLAG_SHORT },
    { FILENAME_FLAG, required_argument, NULL, FILENAME_FLAG_SHORT },
    { SRCVERSION_FLAG, required_argument, NULL, SRCVERSION_FLAG_SHORT },
    { SETTINGS_FLAG, no_argument, NULL, SETTINGS_FLAG_CODE },
    { FEATURES_FLAG, no_argument, NULL, FEATURES_FLAG_CODE },
    { INPUT_FORMAT_FLAG, required_argument, NULL, INPUT_FORMAT_FLAG_CODE },
    { OUTPUT_FORMAT_FLAG, required_argument, NULL, OUTPUT_FORMAT_FLAG_CODE },
    { FILELIST_FLAG, required_argument, NULL, FILELIST_FLAG_CODE },
    //    { FILELIST_FLAG, optional_argument, NULL, FILELIST_FLAG_CODE },
    { REGISTER_EXT_FLAG, required_argument, NULL, REGISTER_EXT_FLAG_CODE },
    { XMLNS_FLAG, required_argument, NULL, XMLNS_FLAG_CODE },
    { SKIP_DEFAULT_FLAG, no_argument, NULL, SKIP_DEFAULT_FLAG_CODE },
    { RECURSIVE_FLAG, no_argument, NULL, RECURSIVE_FLAG_CODE },
    { QUIET_FLAG, no_argument, NULL, QUIET_FLAG_SHORT },
    { NO_XML_DECLARATION_FLAG, no_argument, &curoption, OPTION_XMLDECL | OPTION_XML },
    { NO_NAMESPACE_DECLARATION_FLAG, no_argument, &curoption, OPTION_NAMESPACEDECL | OPTION_XML },
    { OLD_FILENAME_FLAG, no_argument, NULL, OLD_FILENAME_FLAG_CODE },
    { TABS_FLAG, required_argument, NULL, TABS_FLAG_CODE },
    { POSITION_FLAG, no_argument, &curoption, OPTION_POSITION },
    { LITERAL_FLAG, no_argument, &curoption, OPTION_LITERAL },
    { OPERATOR_FLAG, no_argument, &curoption, OPTION_OPERATOR },
    { MODIFIER_FLAG, no_argument, &curoption, OPTION_MODIFIER },
    { CPP_MARKUP_ELSE_FLAG, no_argument, NULL, CPP_MARKUP_ELSE_FLAG_CODE },
    { CPP_TEXTONLY_ELSE_FLAG, no_argument, NULL, CPP_TEXTONLY_ELSE_FLAG_CODE },
    { CPP_MARKUP_IF0_FLAG, no_argument, NULL, CPP_MARKUP_IF0_FLAG_CODE },
    { CPP_TEXTONLY_IF0_FLAG, no_argument, NULL, CPP_TEXTONLY_IF0_FLAG_CODE },
    { 0, 0, 0, 0 }
  };

  // process all command line options
  char* embedded;
  const char* ns_prefix;
  const char* ns_uri;

  while (1) {
    int lastoptind = optind;
    curoption = 0;
    int option_index = 0;
    bool special = optind < argc && !strncmp(argv[optind], "--xmlns:", 8);
    opterr = !special ? 1 : 0;
    int c = getopt_long(argc, argv, "hVo:nex:t:zcgvl:d:f:s:q", cliargs, &option_index);

    if (c == -1)
      break;

    if (curoption) {
      options |= curoption;
      continue;
    }

    // treat --xmlns:prefix=url as --xmlns=url for processing
    if (special && c == '?') {
      c = XMLNS_FLAG_CODE;
    }

    // missing or extra option argument
    if (c == '?') {
      fprintf(stderr, "Try '%s %s' for more information.\n", argv[0], HELP_FLAG);
      exit(option_error_status(optopt));
    }

    char* end = 0;
    switch(c) {

    case HELP_FLAG_SHORT: 
      output_help(argv[0]);
      exit(STATUS_SUCCESS);

      break;

    case VERSION_FLAG_SHORT: 
      output_version(argv[0]);
      exit(STATUS_SUCCESS);
      break;

    case OUTPUT_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      poptions.srcml_filename = optarg;
      break;

    case FILELIST_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      //      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_FILELIST;

      // filelist mode is default nested mode
      options |= OPTION_NESTED;

      poptions.fname = optarg;
      break;

    case REGISTER_EXT_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      // check all the registered extensions.  There may be many (CSV)
      for (char* cursub = strtok(optarg, ","); cursub; cursub = strtok(NULL, ",")) {
	const char* extension = cursub;
	const char* language = asg_split(cursub);

	if(!Language::registerUserExt(extension, language))
	  {
	    fprintf(stderr, "%s: language \"%s\" is not supported.\n", argv[0], language);
	    fprintf(stderr, "Try '%s %s' for more information.\n", argv[0], HELP_FLAG);
	    exit(STATUS_ERROR);
	  }
      }

      break;

    case NESTED_FLAG_SHORT: 
      options |= OPTION_NESTED;
      break;

    case EXPRESSION_MODE_FLAG_SHORT: 
      options |= OPTION_EXPRESSION;
      break;

    case ENCODING_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_XML_ENCODING;

      poptions.xml_encoding = optarg;

      // validate xml encoding
      if (!srcMLOutput::checkEncoding(poptions.xml_encoding)) {
	fprintf(stderr, "%s: xml encoding \"%s\" is not supported.\n", argv[0], poptions.xml_encoding);
	fprintf(stderr, "Try '%s %s' for more information.\n", argv[0], HELP_FLAG);
	exit(STATUS_UNKNOWN_ENCODING);
      }
      break;

    case TEXTENCODING_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_TEXT_ENCODING;

      poptions.src_encoding = optarg;

      // validate source encoding
      if (!srcMLOutput::checkEncoding(poptions.src_encoding)) {
	fprintf(stderr, "%s: text encoding \"%s\" is not supported.\n", argv[0], poptions.src_encoding);
	fprintf(stderr, "Try '%s %s' for more information.\n", argv[0], HELP_FLAG);
	exit(STATUS_UNKNOWN_ENCODING);
      }
      break;

    case XMLNS_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      ns_prefix = 0;
      ns_uri = 0;

      // find the ':' or '=' or end of string
      for (embedded = argv[optind - 1]; *embedded; ++embedded)
	if (*embedded == ':' || *embedded == '=')
	  break;

      // found prefix for sure
      if (*embedded == '=') {

	ns_prefix = "";
	ns_uri = embedded + 1;

      } else if (*embedded == ':') {

	ns_prefix = embedded + 1;

      } else {

	ns_prefix = "";
      }

      // look for uri in rest of this argument
      if (!ns_uri)
	for (; *embedded; ++embedded)
	  if (*embedded == '=') {
	    ns_uri = embedded + 1;
	    *embedded = '\0';
	    break;
	  }

      // look for uri in next argument
      if (!ns_uri) {
	if (!(optind < argc && argv[optind][0] != '-')) {
	  fprintf(stderr, "%s: xmlns option selected but not specified.\n", PROGRAM_NAME);
	  exit(STATUS_LANGUAGE_MISSING);
	}

	ns_uri = argv[optind++];
      }

      // check uri to turn on specific option
      process = false;
      for (int i = 0; i < num_prefixes; ++i)
	if (strcmp(ns_uri, uris[i].uri) == 0) {

	  options |= uris[i].option;

	  urisprefix[i] = ns_prefix;
	  poptions.prefixchange[i] = true;
	  process = true;
	  break;
	}

      if (!process) {
	fprintf(stderr, "%s: invalid namespace \"%s\"\n\n"
		"Namespace URI must be on of the following:  \n"
		"  %-35s primary srcML namespace\n"
		"  %-35s namespace for cpreprocessing elements\n"
		"  %-35s namespace for srcML debugging elements\n"
		"  %-35s namespace for optional literal elements\n"
		"  %-35s namespace for optional operator element\n"
		"  %-35s namespace for optional modifier element\n"
		"  %-35s namespace for optional position element\n",
		argv[0], ns_uri,
		SRCML_SRC_NS_URI, SRCML_CPP_NS_URI, SRCML_ERR_NS_URI,
		SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_MODIFIER_NS_URI,
		SRCML_EXT_POSITION_NS_URI
		);
	exit(STATUS_INVALID_LANGUAGE);
      }
      break;

    case SKIP_DEFAULT_FLAG_CODE: 
      options |= OPTION_SKIP_DEFAULT;
      break;

    case RECURSIVE_FLAG_CODE: 
      options |= OPTION_RECURSIVE;
      break;

    case QUIET_FLAG_SHORT: 
      options |= OPTION_QUIET;
      break;

    case COMPRESSED_FLAG_SHORT: 
      options |= OPTION_COMPRESSED;
      break;

    case INTERACTIVE_FLAG_SHORT: 
      options |= OPTION_INTERACTIVE;
      break;

    case DEBUG_FLAG_SHORT: 
      options |= OPTION_DEBUG;
      break;

    case VERBOSE_FLAG_SHORT: 
      options |= OPTION_VERBOSE;
      break;

    case LANGUAGE_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_LANGUAGE;

      // validate language selected
      poptions.language = Language::getLanguage(optarg);
      if (poptions.language == 0) {
	fprintf(stderr, "%s: invalid option -- Language flag must one of the following values:  "
		"%s %s %s %s\n", argv[0], LANGUAGE_C, LANGUAGE_CXX, LANGUAGE_JAVA, LANGUAGE_ASPECTJ);

	exit(STATUS_INVALID_LANGUAGE);
      }
      break;

    case DIRECTORY_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_DIRECTORY;

      poptions.given_directory = optarg;
      break;

    case FILENAME_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_FILENAME;

      poptions.given_filename = optarg;
      break;

    case SRCVERSION_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_VERSION;

      poptions.given_version = optarg;
      break;

    case SETTINGS_FLAG_CODE :
      output_settings(argv[0]);
      exit(STATUS_SUCCESS);
      break;

    case FEATURES_FLAG_CODE :
      output_features(argv[0]);
      exit(STATUS_SUCCESS);
      break;

    case INPUT_FORMAT_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_INPUT_FORMAT;

      poptions.input_format = optarg;
      break;

    case OUTPUT_FORMAT_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_OUTPUT_FORMAT;

      poptions.output_format = optarg;
      break;

    case OLD_FILENAME_FLAG_CODE :
      options |= OPTION_OLD_FILENAME;
      break;

    case TABS_FLAG_CODE :
      /*
      // check for missing argument confused by an argument that looks like an option
      checkargisoption(argv[0], argv[lastoptind], optarg, optind, lastoptind);
      */

      options |= OPTION_POSITION;

      char * end;
      poptions.tabsize = pstd::strtol(optarg, &end, 10);
      
      // validate type of tabsize number
      if (errno == EINVAL || strlen(end) == strlen(optarg)) {
        fprintf(stderr, "%s: unit option value \"%s\" must be numeric.\n", argv[0], optarg);
        exit(STATUS_UNIT_INVALID);
      }

      // validate range of unit number
      if (poptions.tabsize <= 0) {
        fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", argv[0], poptions.tabsize);
        exit(STATUS_UNIT_INVALID);
      }

      break;


    case CPP_MARKUP_ELSE_FLAG_CODE: 
      if (!cpp_else) {
	options |= OPTION_CPP_MARKUP_ELSE;

	cpp_else = true;

      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		argv[0], CPP_MARKUP_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
      break;

    case CPP_TEXTONLY_ELSE_FLAG_CODE: 
      if (!cpp_else) {
	options &= ~OPTION_CPP_MARKUP_ELSE;

	cpp_else = true;
      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		argv[0], CPP_MARKUP_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
      break;

    case CPP_MARKUP_IF0_FLAG_CODE: 
      if (!cpp_if0) {
	options |= OPTION_CPP_MARKUP_IF0;

	cpp_if0 = true;

      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		argv[0], CPP_MARKUP_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
      break;

    case CPP_TEXTONLY_IF0_FLAG_CODE: 
      if (!cpp_if0) {
	// clear if previously marked
	options &= ~OPTION_CPP_MARKUP_IF0;

	cpp_if0 = true;

      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		argv[0], CPP_MARKUP_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
      break;

    default:
      exit(STATUS_UNKNOWN_OPTION);
      break;
    };
  }

  return optind;
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

  case FILENAME_FLAG_SHORT:
    return STATUS_FILENAME_MISSING;
    break;

  case LANGUAGE_FLAG_SHORT:
    return STATUS_LANGUAGE_MISSING;
    break;

  case DIRECTORY_FLAG_SHORT:
    return STATUS_DIRECTORY_MISSING;
    break;

  case SRCVERSION_FLAG_SHORT:
    return STATUS_VERSION_MISSING;
    break;

  case ENCODING_FLAG_SHORT:
    return STATUS_XMLENCODING_MISSING;
    break;

  case TEXTENCODING_FLAG_SHORT:
    return STATUS_SRCENCODING_MISSING;
    break;

  case INPUT_FORMAT_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case OUTPUT_FORMAT_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case TABS_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case REGISTER_EXT_FLAG_CODE:
    return STATUS_ERROR;
    break;

  case '\0':
    return STATUS_UNKNOWN_OPTION;
    break;
  };

  return 0;
}

void register_standard_file_extensions()
{
  Language::registerUserExt( "c", LANGUAGE_C );
  Language::registerUserExt( "h", LANGUAGE_C );
  Language::registerUserExt( "C", LANGUAGE_CXX );
  Language::registerUserExt( "cpp", LANGUAGE_CXX );
  Language::registerUserExt( "cc", LANGUAGE_CXX );
  Language::registerUserExt( "cxx", LANGUAGE_CXX );
  Language::registerUserExt( "c++", LANGUAGE_CXX );
  Language::registerUserExt( "H", LANGUAGE_CXX );
  Language::registerUserExt( "hpp", LANGUAGE_CXX );
  Language::registerUserExt( "hh", LANGUAGE_CXX );
  Language::registerUserExt( "hxx", LANGUAGE_CXX );
  Language::registerUserExt( "h++", LANGUAGE_CXX );
  Language::registerUserExt( "java", LANGUAGE_JAVA );
  Language::registerUserExt( "aj", LANGUAGE_ASPECTJ );

/*
Language::registerUserExt( LanguageName::LANGUAGE_CXX_0X, LANGUAGE_CXX_0X },
*/

}

void src2srcml_file(srcMLTranslator& translator, const char* path, OPTION_TYPE& options, const char* dir, const char* filename, const char* version, int language, int tabsize, int& count) {

  // handle directories specially
  struct stat instat;
  stat(path, &instat);
  if (S_ISDIR(instat.st_mode)) {
    process_dir(translator, path, *gpoptions, count);
    return;
  }

  options |= OPTION_SKIP_DEFAULT;

  int reallanguage = 0;
  char* afilename = 0;

  bool isarchive = false;
#ifdef LIBARCHIVE

  // single file archive (tar, zip, cpio, etc.) is listed as a single file
  // but is much, much more
  OPTION_TYPE save_options = options;

  // process the individual file (once), or an archive as many times as it takes
  bool first = true;
  while (first || !archiveReadStatus()) {

    // start with the original options
    //    options = save_options;

    // if using libarchive, then get the filename (which starts the open)
    if (archiveReadMatch(path)) {
      const char* result = archiveReadFilename(path);

      afilename = result ? strdup(result) : 0;
    }

    if (archiveReadStatus() < 0 ) {
      fprintf(stderr, "%s: Unable to open file %s\n", PROGRAM_NAME, path);
      if (first)
	return;

    } else {

    if (isArchiveRead()) {
      isarchive = true;
      options |= OPTION_NESTED;
    }
  
    // in verbose mode output the currently processed filename
    if (first && archiveReadMatch(path) && isoption(options, OPTION_VERBOSE)
	&& (strcmp(archiveReadCompression(), "none")))
      fprintf(stderr, "Compression:\t%s\n", archiveReadCompression());

    // in verbose mode output the currently processed filename
    if (first && isArchiveRead() && isoption(options, OPTION_VERBOSE))
      fprintf(stderr, "Format:\t%s\n", archiveReadFormat());

    first = false;

#endif

    // find the separate dir and filename
    const char* ndir = dir;
    const char* nfilename = filename;
    if (strcmp(path, "-") && !nfilename)
	  nfilename = path;
    if (afilename)
      nfilename = afilename;

    // language (for this item in archive mode) based on extension, if not specified
    reallanguage = language;
    if (reallanguage == 0 && nfilename)
      reallanguage = Language::getLanguageFromFilename(nfilename);
    if (reallanguage == 0 && !isoption(options, OPTION_SKIP_DEFAULT))
      reallanguage = DEFAULT_LANGUAGE;
    if (!reallanguage) {
    //    if (!archiveReadMatch(nfilename) && !reallanguage) {

      if (!isoption(options, OPTION_QUIET)) {
	if (isoption(options, OPTION_VERBOSE))
	  fprintf(stderr, "Skipping '%s'.  No language can be determined.\n", nfilename);
	else
	  fprintf(stderr, "%s:  Skipping '%s'.  No language can be determined.\n", PROGRAM_NAME, nfilename);
      }

    } else {

    // now that we have the language, turnon cpp namespace for non Java-based languages
    if (!(reallanguage == srcMLTranslator::LANGUAGE_JAVA || reallanguage == srcMLTranslator::LANGUAGE_ASPECTJ))
      options |= OPTION_CPP;

    // another file
    ++count;

    // in verbose mode output the currently processed filename
    if (isoption(options, OPTION_VERBOSE))
      fprintf(stderr, "%d\t%s", count, !isarchive ? path : afilename);

    try {
      translator.translate(path, ndir, nfilename, version, reallanguage, tabsize);

    } catch (FileError) {

      if (dir)
	fprintf(stderr, "%s error: file \'%s/%s\' does not exist.\n", PROGRAM_NAME, dir, nfilename);
      else
	fprintf(stderr, "%s error: file \'%s\' does not exist.\n", PROGRAM_NAME, nfilename);

      exit(STATUS_INPUTFILE_PROBLEM);
    }
    }

    // in verbose mode output end info about this file
    if (isoption(options, OPTION_VERBOSE))
      fprintf(stderr, "\n");
    }

    // compound documents are interrupted gracefully
    if (isoption(options, OPTION_TERMINATE))
      return;
    //     return STATUS_TERMINATED;

#ifdef LIBARCHIVE
    // done if not an archive
    if (!isarchive)
      break;
  }
#endif
}

void process_dir(srcMLTranslator& translator, const char* dname, process_options& poptions, int& count) {

  // by default, all dirs are treated as an archive
  options |= OPTION_NESTED;

  // try to open the found directory
  DIR* dir = opendir(dname);
  if (!dir)
    return;

  // process all non-directory files
  std::string sline = dname;
  sline += "/";
  int basesize = sline.length();
  while (struct dirent* entry = readdir(dir)) {

    if (entry->d_type == DT_DIR)
      continue;

    // skip standard UNIX filenames, and . files
    if (entry->d_name[0] == '.')
      continue;

    sline.resize(basesize);
    sline += entry->d_name;

    // translate the file listed in the input file using the directory and filename extracted from the path
    src2srcml_file(translator,
		   sline.c_str(),
		   options,
		   0,
		   0,
		   poptions.given_version,
		   poptions.language,
		   poptions.tabsize,
		   count);
  }

  if (!isoption(options, OPTION_RECURSIVE))
    return;

  // now process directories
  rewinddir(dir);
  while (struct dirent* entry = readdir(dir)) {

    if (entry->d_type != DT_DIR)
      continue;

    // skip standard UNIX filenames, and . files
    if (entry->d_name[0] == '.')
      continue;

    sline.resize(basesize);
    sline += entry->d_name;

    process_dir(translator, sline.c_str(), poptions, count);
  }
}

void process_filelist(srcMLTranslator& translator, process_options& poptions, int& count) {
  try {

    // translate all the filenames listed in the named file
    // Use libxml2 routines so that we can handle http:, file:, and gzipped files automagically
    URIStream uriinput(poptions.fname);
    char* line;
    while ((line = uriinput.getline())) {

      // skip blank lines or comment lines
      if (line[0] == '\0' || line[0] == '\n' || line[0] == FILELIST_COMMENT)
        continue;

      // in verbose mode output the currently processed filename
      if (isoption(options, OPTION_VERBOSE))
        fprintf(stderr, "Input:\t%s\n", strcmp(line, "-") == 0 ? "" : line);

      // translate the file listed in the input file using the directory and filename extracted from the path
      src2srcml_file(translator,
                     line,
                     options,
                     0,
                     0,
                     poptions.given_version,
                     poptions.language,
                     poptions.tabsize,
                     count);
    }

  } catch (URIStreamFileError) {
    fprintf(stderr, "%s error: file/URI \'%s\' does not exist.\n", PROGRAM_NAME, poptions.fname);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
}
