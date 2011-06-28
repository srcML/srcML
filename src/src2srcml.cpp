/*
  src2srcml.cpp

  Copyright (C) 2002-2011  SDML (www.sdml.info)

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
#ifdef __GNUG__
#include <sys/stat.h>
#endif
#include "srcmlapps.hpp"
#include "srcmlns.hpp"
#include "Options.hpp"
#include "project.hpp"
#include "Language.hpp"
#include "srcMLTranslator.hpp"
#include "URIStream.hpp"
#include <getopt.h>
#include <dirent.h>
#include <algorithm>

#define BASE_PROGRAM_NAME "src2srcml"

const char* PROGRAM_NAME = BASE_PROGRAM_NAME;

struct stringequal {
  const char *const lhs;

  stringequal(const char *l) : lhs(l) {}
};

bool operator==(const char* lhs, const stringequal& r) {
   return std::strcmp(lhs, r.lhs) == 0;
}

#include "libxml_archive_read.hpp"
#include "libxml_archive_write.hpp"

const char* const DEBUG_FLAG = "debug";
const char DEBUG_FLAG_SHORT = 'g';

const char* const LITERAL_FLAG = "literal";

const char* const OPERATOR_FLAG = "operator";

const char* const MODIFIER_FLAG = "modifier";

const char* const POSITION_FLAG = "position";
const char* const TABS_FLAG = "tabs";
const int TABS_FLAG_CODE = 256 + 0;
const char* const TABS_FLAG_FULL = "tabs=NUMBER";
const int DEFAULT_TABSIZE = 8;

const char* const INTERACTIVE_FLAG = "interactive";
const char INTERACTIVE_FLAG_SHORT = 'c';

const char* const CPP_MARKUP_ELSE_FLAG = "cpp-markup-else";
const int CPP_MARKUP_ELSE_FLAG_CODE = 256 + 1;

const char* const CPP_TEXTONLY_ELSE_FLAG = "cpp-text-else";
const int CPP_TEXTONLY_ELSE_FLAG_CODE = 256 + 2;

const char* const CPP_MARKUP_IF0_FLAG = "cpp-markup-if0";
const int CPP_MARKUP_IF0_FLAG_CODE = 256 + 3;

const char* const CPP_TEXTONLY_IF0_FLAG = "cpp-text-if0";
const int CPP_TEXTONLY_IF0_FLAG_CODE = 256 + 4;

const char* const EXPRESSION_MODE_FLAG = "expression";
const char EXPRESSION_MODE_FLAG_SHORT = 'e';

const char* const SELF_VERSION_FLAG = "self-version";

const char* const FILELIST_FLAG = "files-from";
const int FILELIST_FLAG_CODE = 256 + 5;
const char* const FILELIST_FLAG_FULL = "files-from=INPUT";

const char* const XMLNS_FLAG = "xmlns";
const int XMLNS_FLAG_CODE = 256 + 6;
const char* const XMLNS_DEFAULT_FLAG_FULL = "xmlns=URI";
const char* const XMLNS_FLAG_FULL = "xmlns:PREFIX=URI";

const char* const REGISTER_EXT_FLAG = "register-ext";
const int REGISTER_EXT_FLAG_CODE = 256 + 7;

const char* const OLD_FILENAME_FLAG = "old-filename";
const int OLD_FILENAME_FLAG_CODE = 256 + 8;

const char* const SKIP_DEFAULT_FLAG = "skip-default";
const int SKIP_DEFAULT_FLAG_CODE = 256 + 9;

const char* const RECURSIVE_FLAG = "recursive";
const int RECURSIVE_FLAG_CODE = 256 + 10;

const int DEFAULT_LANGUAGE = srcMLTranslator::LANGUAGE_CXX;

const char* const EXAMPLE_TEXT_FILENAME="foo.cpp";
const char* const EXAMPLE_XML_FILENAME="foo.cpp.xml";

const char FILELIST_COMMENT = '#';

struct uridata {
  char const * const uri;
  char const * const prefix;
  int option;
  const char* description;
};

uridata uris[] = {

  { SRCML_SRC_NS_URI,          SRCML_SRC_NS_PREFIX_DEFAULT, 0,               "primary srcML namespace" },
  { SRCML_CPP_NS_URI,          SRCML_CPP_NS_PREFIX_DEFAULT, OPTION_CPP,      "namespace for cpreprocessing elements" },
  { SRCML_ERR_NS_URI,          SRCML_ERR_NS_PREFIX_DEFAULT, OPTION_DEBUG,    "namespace for srcML debugging elements" },
  { SRCML_EXT_LITERAL_NS_URI,  SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT, OPTION_LITERAL,  "namespace for optional literal elements" },
  { SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT, OPTION_OPERATOR, "namespace for optional operator element"},
  { SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT, OPTION_MODIFIER, "namespace for optional modifier element"},
  { SRCML_EXT_POSITION_NS_URI, SRCML_EXT_POSITION_NS_PREFIX_DEFAULT, OPTION_POSITION, "namespace for optional position element and attributes" },
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

int option_error_status(int optopt);

// translate a file, maybe an archive
void src2srcml_file(srcMLTranslator& translator, const char* path, OPTION_TYPE& options, const char* dir, const char* filename, const char* version, int language, int tabsize, int& count, int & skipped, int & error, bool & showinput, bool shownumber = false);

using namespace LanguageName;

// output help
void output_help(const char* name) {
  printf( "Usage: %s [options] <src_infile>... [-o <srcML_outfile>]\n\n"

	  "Translates source-code files in C, C++, and Java into the XML source-code representation srcML.\n"
	  "Input can be from standard input, a disk file, or an archive file, e.g., tar, cpio, and zip.\n"
	  "Both single and multiple files can be stored in a srcML archive.\n\n"

	  "By default, output is to stdout.  You can specify a file for output using the --%s or -%c option.\n"
	  "When no filenames are given input is from stdin and output is to stdout.\n"
	  "An input filename of '-' also reads from stdin.\n\n"

    	  "Any input file can be a local filename (FILE) or a URI with the protocols http:, ftp:, or file:\n\n"

	  "Language is based on the file extension.  It can also be directly\n"
	  "set using the --language option.\n"
	  "\n"
	  "Options:\n", name, OUTPUT_FLAG, OUTPUT_FLAG_SHORT);

  printf("  -%c, --%-14s display this help and exit\n",      HELP_FLAG_SHORT, HELP_FLAG);
  printf("  -%c, --%-14s display version number and exit\n\n", VERSION_FLAG_SHORT, VERSION_FLAG);

  printf("  --%-18s read list of source file names from INPUT which is a FILE or URI,\n"
	 "                       to form a srcML archive\n",
	  FILELIST_FLAG_FULL);
  /*
  printf("  --%-18s ???\n\n", INPUT_FORMAT_FLAG);
  printf("  -%c, --%-14s write result to OUTPUT which is a FILE or URI\n", OUTPUT_FLAG_SHORT, OUTPUT_FLAG_FULL);
  */
  /*
  printf("  --%-18s ???\n\n", OUTPUT_FORMAT_FLAG);
  */
  printf("  -%c, --%-14s store output in a srcML archive, default for multiple input files\n",
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
	 "  " BASE_PROGRAM_NAME " (read from standard input, write to standard output)\n"
	 "  " BASE_PROGRAM_NAME " m.cpp          (read from file m.cpp, write to standard output)\n"
	 "  " BASE_PROGRAM_NAME " m.cpp -o m.cpp.xml (read from file m.cpp, write to file m.cpp.xml)\n"
	 "\n"
	 "  " BASE_PROGRAM_NAME " http://www.sdml.info/projects/srcml/ex/main.cpp (read from URI)\n"
	 "\n"
	 "  " BASE_PROGRAM_NAME " --directory=src --filename=m.cpp m.cpp -o m.cpp.xml "
	 "(element unit attributes dir \"src\", filename \"m.cpp\")\n"
	 "  " BASE_PROGRAM_NAME " --src-encoding=UTF-8 m.cpp m.cpp.xml         "
	 "(encoding of input text file is UTF-8)\n"
	 "  " BASE_PROGRAM_NAME " --xml-encoding=ISO-8859-1 m.cpp m.cpp.xml    "
	 "(set encoding of srcML file to ISO-8859-1)\n\n");

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
  const char* input_format;
  const char* output_format;
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

void process_dir(srcMLTranslator& translator, const char* dname, process_options& poptions, int& count, int & skipped, int & error, bool & showinput, bool shownumber);
void process_filelist(srcMLTranslator& translator, process_options& poptions, int& count, int & skipped, int & error, bool & showinput);

// setup options and collect info from arguments
int process_args(int argc, char* argv[], process_options & poptions);

int main(int argc, char* argv[]) {

  options |= OPTION_SKIP_DEFAULT;

  int exit_status = EXIT_SUCCESS;

  LIBXML_TEST_VERSION

  xmlGenericErrorFunc handler = (xmlGenericErrorFunc) libxml_error;
  initGenericErrorDefaultFunc(&handler);

  /* signal handling */

#if defined(__GNUG__) && !defined(__MINGW32__)
  // signal to toggle verbose flag
  pstd::signal(SIGUSR1, verbose_handler);
#endif

  Language::register_standard_file_extensions();

  process_options poptions =
    {
      0,
      STDIN,
      0,
      0,
      0,
      DEFAULT_TEXT_ENCODING,
      DEFAULT_XML_ENCODING,
      0,
      0,
      0,
      DEFAULT_TABSIZE,
      {	false, false, false, false, false, false }
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

  // if more than one input filename assume nested
  // a single input filename which is an archive is detected during archive processing
  if (input_arg_count > 1)
    options |= OPTION_NESTED;

#if defined(__GNUC__) && !defined(__MINGW32__)
  // verify that all input filenames exist and are nice and clean
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

    struct stat instat = { 0 };
    if (stat(argv[i], &instat) == -1) {
      perror(argv[i]);
      exit(STATUS_INPUTFILE_PROBLEM);
    }
  }

  // verify that only one input pipe is STDIN
  struct stat stdiostat = { 0 };
  fstat(STDIN_FILENO, &stdiostat);
  int stdiocount = 0;

   for (int i = input_arg_start; i <= input_arg_end; ++i) {

    // may not exist due to race condition, so check again
    struct stat instat = { 0 };
    if (stat(argv[i], &instat) == -1) {
      perror(argv[i]);
      exit(STATUS_INPUTFILE_PROBLEM);
    }

     if (instat.st_ino == stdiostat.st_ino)
      ++stdiocount;

    if (stdiocount > 1) {
      fprintf(stderr, "%s: Multiple input files are from standard input.\n", PROGRAM_NAME);
      exit(STATUS_INPUTFILE_PROBLEM);
    }
  }

  // verify that the output filename is not the same as any of the input filenames
  struct stat outstat = { 0 };
  stat(poptions.srcml_filename, &outstat);
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

    // may not exist due to race condition, so check again
    struct stat instat = { 0 };
    if (stat(argv[i], &instat) == -1) {
      perror(argv[i]);
      exit(STATUS_INPUTFILE_PROBLEM);
    }

    if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
      fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n",
	      PROGRAM_NAME, argv[i], poptions.srcml_filename);
      exit(STATUS_INPUTFILE_PROBLEM);
    }
  }
#endif
	
  // make sure user did not specify duplicate prefixes as an option
  for (int i = 0; i < num_prefixes - 1; ++i) {
    const char** presult = std::find(&urisprefix[i + 1], &urisprefix[num_prefixes], stringequal(urisprefix[i]));
    if (presult != (urisprefix + num_prefixes)) {

	fprintf(stderr, "%s: Namespace conflict for ", PROGRAM_NAME);
	if (urisprefix[i] == '\0') {
	  fprintf(stderr, "default prefix\n");
	} else {
	  fprintf(stderr, "prefix \'%s\'\n", urisprefix[i]);
	}
	fprintf(stderr, "Prefix URI conflicts:\n  %s\n  %s\n", uris[i].uri,
		uris[presult - &urisprefix[0]].uri);

	exit(STATUS_INVALID_OPTION_COMBINATION);
    }
  }

#if defined(__GNUG__) && !defined(__MINGW32__)
  // automatic interactive use from stdin (not on redirect or pipe)
  if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

    if (isatty(STDIN_FILENO))
      options |= OPTION_INTERACTIVE;
  }
#endif

  // all input is through libarchive
  if (!isoption(options, OPTION_FILELIST)) {
    if (xmlRegisterInputCallbacks(archiveReadMatch, archiveReadOpen, archiveRead, archiveReadClose) < 0) {
      fprintf(stderr, "%s: failed to register archive handler\n", PROGRAM_NAME);
      exit(1);
    }
  }

  try {

    // translator from input to output using determined language
    srcMLTranslator translator(poptions.language,
			       poptions.src_encoding,
			       poptions.xml_encoding,
			       poptions.srcml_filename,
			       options,
			       poptions.given_directory,
			       poptions.given_filename,
			       poptions.given_version,
			       urisprefix,
			       poptions.tabsize);


    bool showinput = false;
    bool shownumber = false;
    // output source encoding
    if (isoption(options, OPTION_VERBOSE)) {
      fprintf(stderr, "Source encoding:  %s\n", poptions.src_encoding);
      fprintf(stderr, "XML encoding:  %s\n", poptions.xml_encoding);
      showinput = false;
      shownumber = true;
    }

    // filecount
    int count = 0;

    // files skipped
    int skipped = 0;

    // file errors
    int error = 0;

#ifdef __GNUG__
    // setup so we can gracefully stop after a file at a time
    pstd::signal(SIGINT, terminate_handler);
#endif

    // translate input filenames from list in file
    if (isoption(options, OPTION_FILELIST)) {

      // if we don't have a filelist yet, get it from the first argument
      if (!poptions.fname && input_arg_count > 0)
        poptions.fname = argv[input_arg_start];

      // still no filelist? use stdin
      if (!poptions.fname)
        poptions.fname = STDIN;

      // so process the filelist
      process_filelist(translator, poptions, count, skipped, error, showinput);

    // translate from standard input
    } else if (input_arg_count == 0) {

      // translate from standard input using any directory, filename and version given on the command line
      src2srcml_file(translator, STDIN, options,
		     poptions.given_directory,
		     poptions.given_filename,
		     poptions.given_version,
		     poptions.language,
		     poptions.tabsize, count, skipped, error, showinput, shownumber);

    // translate filenames from the command line
    } else {

      // translate in batch the input files on the command line extracting the directory and filename attributes
      // from the full path
      for (int i = input_arg_start; i <= input_arg_end; ++i) {
	
	// process this command line argument
	src2srcml_file(translator, argv[i], options,
		       input_arg_count == 1 ? poptions.given_directory : 0,
		       input_arg_count == 1 ? poptions.given_filename : 0,
		       input_arg_count == 1 ? poptions.given_version : 0,
		       poptions.language,
		       poptions.tabsize,
		       count, skipped, error, showinput, shownumber);
      }
    }

    if (count == 0)
      exit(STATUS_INPUTFILE_PROBLEM);

    else if (showinput && isoption(options, OPTION_NESTED) && !isoption(options, OPTION_QUIET)) {
	fprintf(stderr, "\n"
		"Translated: %d\t"
		"Skipped: %d\t"
		"Error: %d\t"
		"Total: %d\n", count, skipped, error, count + skipped + error);
      }

  } catch (srcEncodingException) {
    fprintf(stderr, "%s: Translation encoding problem\n", PROGRAM_NAME);
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
    //    { INPUT_FORMAT_FLAG, required_argument, NULL, INPUT_FORMAT_FLAG_CODE },
    //    { OUTPUT_FORMAT_FLAG, required_argument, NULL, OUTPUT_FORMAT_FLAG_CODE },
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
      fprintf(stderr, "Try '%s --%s' for more information.\n", PROGRAM_NAME, HELP_FLAG);
      exit(option_error_status(optopt));
    }

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

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      poptions.srcml_filename = optarg;
      break;

    case FILELIST_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      //      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_FILELIST;

      // filelist mode is default nested mode
      options |= OPTION_NESTED;

      poptions.fname = optarg;
      break;

    case REGISTER_EXT_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      // check all the registered extensions.  There may be many (CSV)
      for (char* cursub = strtok(optarg, ","); cursub; cursub = strtok(NULL, ",")) {
        char* language = cursub;
        const char* extension = strsep(&language, "=");

	if (!Language::registerUserExt(extension, language)) {

	    fprintf(stderr, "%s: language \"%s\" is not supported.\n", PROGRAM_NAME, language);
	    fprintf(stderr, "Try '%s %s' for more information.\n", PROGRAM_NAME, HELP_FLAG);
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
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_XML_ENCODING;

      poptions.xml_encoding = optarg;

      // validate xml encoding
      if (!srcMLOutput::checkEncoding(poptions.xml_encoding)) {
	fprintf(stderr, "%s: xml encoding \"%s\" is not supported.\n", PROGRAM_NAME, poptions.xml_encoding);
	fprintf(stderr, "Try '%s %s' for more information.\n", PROGRAM_NAME, HELP_FLAG);
	exit(STATUS_UNKNOWN_ENCODING);
      }
      break;

    case TEXTENCODING_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_TEXT_ENCODING;

      poptions.src_encoding = optarg;

      // validate source encoding
      if (!srcMLOutput::checkEncoding(poptions.src_encoding)) {
	fprintf(stderr, "%s: text encoding \"%s\" is not supported.\n", PROGRAM_NAME, poptions.src_encoding);
	fprintf(stderr, "Try '%s %s' for more information.\n", PROGRAM_NAME, HELP_FLAG);
	exit(STATUS_UNKNOWN_ENCODING);
      }
      break;

    case XMLNS_FLAG_CODE: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      {
	// find the start of the embedded uri (if it is in there)
	char* ns_uri = argv[optind - 1];
        strsep(&ns_uri, "=");

	// now find the prefix in what is left
        char* ns_prefix = argv[optind - 1];
        strsep(&ns_prefix, ":");

	// if no uri, look in the next argument
	if (!ns_uri[0]) {
	  if (!(optind < argc && argv[optind][0] != '-')) {
	    fprintf(stderr, "%s: xmlns option selected but not specified.\n", PROGRAM_NAME);
	    exit(STATUS_LANGUAGE_MISSING);
	  }

	  ns_uri = argv[optind++];
	}

	// check uri to turn on specific option
	bool found = false;
	for (int i = 0; i < num_prefixes; ++i)
	  if (strcmp(ns_uri, uris[i].uri) == 0) {

	    options |= uris[i].option;

	    urisprefix[i] = ns_prefix ? ns_prefix : "";
	    poptions.prefixchange[i] = true;
	    found = true;
	    break;
	  }

	if (!found) {
	  fprintf(stderr, "%s: invalid namespace \"%s\"\n\n"
		  "Namespace URI must be on of the following:  \n", PROGRAM_NAME, ns_uri);
	  for (int i = 0; i < num_prefixes; ++i)
	    fprintf(stderr, "  %-35s %s\n", uris[i].uri, uris[i].description);

	  exit(STATUS_INVALID_LANGUAGE);
	}
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
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_LANGUAGE;

      // validate language selected
      poptions.language = Language::getLanguage(optarg);
      if (poptions.language == 0) {
	fprintf(stderr, "%s: invalid option -- Language flag must one of the following values:  "
		"%s %s %s %s\n", PROGRAM_NAME, LANGUAGE_C, LANGUAGE_CXX, LANGUAGE_JAVA, LANGUAGE_ASPECTJ);

	exit(STATUS_INVALID_LANGUAGE);
      }
      break;

    case DIRECTORY_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_DIRECTORY;

      poptions.given_directory = optarg;
      break;

    case FILENAME_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_FILENAME;

      poptions.given_filename = optarg;
      break;

    case SRCVERSION_FLAG_SHORT: 

      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);

      options |= OPTION_VERSION;

      poptions.given_version = optarg;
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
    case OLD_FILENAME_FLAG_CODE :
      options |= OPTION_OLD_FILENAME;
      break;

    case TABS_FLAG_CODE :
      /*
      // check for missing argument confused by an argument that looks like an option
      checkargisoption(PROGRAM_NAME, argv[lastoptind], optarg, optind, lastoptind);
      */

      options |= OPTION_POSITION;

      char * end;
      poptions.tabsize = pstd::strtol(optarg, &end, 10);
      
      // validate type of tabsize number
      if (errno == EINVAL || strlen(end) == strlen(optarg)) {
        fprintf(stderr, "%s: unit option value \"%s\" must be numeric.\n", PROGRAM_NAME, optarg);
        exit(STATUS_UNIT_INVALID);
      }

      // validate range of unit number
      if (poptions.tabsize <= 0) {
        fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", PROGRAM_NAME, poptions.tabsize);
        exit(STATUS_UNIT_INVALID);
      }

      break;

    case CPP_MARKUP_ELSE_FLAG_CODE: 
      if (cpp_else) {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		PROGRAM_NAME, CPP_MARKUP_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }

      options |= OPTION_CPP_MARKUP_ELSE;
      cpp_else = true;

      break;

    case CPP_TEXTONLY_ELSE_FLAG_CODE: 
      if (cpp_else) {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		PROGRAM_NAME, CPP_MARKUP_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }

      options &= ~OPTION_CPP_MARKUP_ELSE;
      cpp_else = true;

      break;

    case CPP_MARKUP_IF0_FLAG_CODE: 
      if (cpp_if0) {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		PROGRAM_NAME, CPP_MARKUP_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }

      options |= OPTION_CPP_MARKUP_IF0;
      cpp_if0 = true;

      break;

    case CPP_TEXTONLY_IF0_FLAG_CODE: 
      if (cpp_if0) {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		PROGRAM_NAME, CPP_MARKUP_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }

      options &= ~OPTION_CPP_MARKUP_IF0;
      cpp_if0 = true;

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
    /*
  case INPUT_FORMAT_FLAG_CODE:
    return STATUS_ERROR;
    break;
    */
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

void src2srcml_file(srcMLTranslator& translator, const char* path, OPTION_TYPE& options, const char* dir, const char* root_filename, const char* version, int language, int tabsize, int& count, int & skipped, int & error, bool & showinput, bool shownumber) {

  // handle local directories specially
  struct stat instat = { 0 };
  int stat_status = stat(path, &instat);
  if (!stat_status && S_ISDIR(instat.st_mode)) {
    process_dir(translator, path, *gpoptions, count, skipped, error, showinput, shownumber);
    return;
  }

  // single file archive (tar, zip, cpio, etc.) is listed as a single file
  // but is much, much more
  OPTION_TYPE save_options = options;

  // process the individual file (once), or an archive as many times as it takes
  void* context = 0;
  bool isarchive = false;
  do {

    // start with the original options
    options = save_options;
    std::string unit_filename;

    try {

      // open up the file
      unit_filename = path;
      context = translator.setInput(path);

      // check if file is bad
      if (!context || archiveReadStatus(context) < 0 ) {
        fprintf(stderr, "%s: Unable to open file %s\n", PROGRAM_NAME, path);

        ++error;

        return;
      }

      // so, do we have an archive?
      isarchive = isArchiveRead(context);

      // once any source archive is input, then we have to assume nested not just locally
      if (isarchive) {
        options |= OPTION_NESTED;
        save_options |= OPTION_NESTED;
        showinput = true;
        //        shownumber = true;
      }

      // output tracing information about the input file
      if (showinput && isArchiveFirst(context) && !isoption(options, OPTION_QUIET)) {

        // output the currently processed filename
        fprintf(stderr, "Path: %s", strcmp(path, STDIN) == 0 ? "standard input" : path);

        // output compression and format (if any)
        if (isarchive)
          fprintf(stderr, "\tFormat: %s", archiveReadFormat(context));

        if (archiveReadCompression(context) && strcmp(archiveReadCompression(context), "none"))
          fprintf(stderr, "\tCompression: %s", archiveReadCompression(context));

        fprintf(stderr, "\n");
      }

      // figure out the resulting filename
      bool foundfilename = true;
      if (archiveReadFilename(context))
        unit_filename = archiveReadFilename(context);
      else if (root_filename)
        unit_filename = root_filename;
      else if (strcmp(path, STDIN))
        unit_filename = path;
      else
        foundfilename = false;

      // special case:  skip directories (in archives)
      if (archiveIsDir(context)) {

        if (!isoption(options, OPTION_QUIET)) {
          if (!shownumber)
            fprintf(stderr, "Skipped '%s':  Is a directory.\n", unit_filename.c_str());
          else
            fprintf(stderr, "%5s %s\tSkipped: Is a directory.\n", "-", unit_filename.c_str());
        }

        ++skipped;

        // explicitly close, since we are skipping it
        archiveReadClose(context);

        continue;
      }

      // language (for this item in archive mode) based on extension, if not specified

      // 1) language may have been specified explicitly
      int reallanguage = language;

      // 2) try from the filename (basically the extension)
      if (!reallanguage)
        reallanguage = Language::getLanguageFromFilename(unit_filename.c_str());

      // 3) default language (if allowed)
      if (!reallanguage && !isoption(options, OPTION_SKIP_DEFAULT))
        reallanguage = DEFAULT_LANGUAGE;

      // error if can't find a language
      if (!reallanguage) {

        if (!isoption(options, OPTION_QUIET)) {
          if (!shownumber)
            fprintf(stderr, "Skipped '%s':  Unknown extension.\n", unit_filename.c_str() ? unit_filename.c_str() : "standard input");
          else
            fprintf(stderr, "%5s %s\tSkipped: Unknown extension.\n", "-", unit_filename.c_str() ? unit_filename.c_str() : "standard input");
	  
        }

        ++skipped;

        // close the file that we don't have a language for
        archiveReadClose(context);

        continue;
      }

      // turnon cpp namespace for non Java-based languages
      if (!(reallanguage == srcMLTranslator::LANGUAGE_JAVA || reallanguage == srcMLTranslator::LANGUAGE_ASPECTJ))
        options |= OPTION_CPP;

      // another file
      ++count;

      const char* c_filename = clean_filename(unit_filename.c_str());

      // output the currently processed filename
      if (!isoption(options, OPTION_QUIET) && shownumber)
        fprintf(stderr, "%5d %s\n", count, c_filename);

      // translate the file
      translator.translate(path, dir,
			   foundfilename ? c_filename : 0,
			   version, reallanguage);

    } catch (FileError) {

      // output tracing information about the input file
      if (showinput && !isoption(options, OPTION_QUIET)) {

        // output the currently processed filename
        fprintf(stderr, "Path: %s", strcmp(path, STDIN) == 0 ? "standard input" : path);
        fprintf(stderr, "\tError: Unable to open file.\n");

      } else {

      if (dir)
	fprintf(stderr, "%s: Unable to open file %s/%s\n", PROGRAM_NAME, dir, unit_filename.c_str());
      else
	fprintf(stderr, "%s: Unable to open file %s\n", PROGRAM_NAME, unit_filename.c_str());
      }

      ++error;

      return;
      //      exit(STATUS_INPUTFILE_PROBLEM);
    }

    // restore options for next time around
    options = save_options;

    // compound documents are interrupted gracefully
    if (isoption(options, OPTION_TERMINATE))
      return;
    //     return STATUS_TERMINATED;

  } while (isarchive && isAnythingOpen(context));
}

void process_dir(srcMLTranslator& translator, const char* directory, process_options& poptions, int& count, int & skipped, int & error, bool & showinput, bool shownumber) {

  // by default, all dirs are treated as an archive
  options |= OPTION_NESTED;

  // try to open the found directory
  DIR* dirp = opendir(directory);
  if (!dirp) {
    return;
  }

  showinput = true;

  // start of path from directory name
  std::string filename = directory;
  if (!filename.empty() && filename[filename.size() - 1] != '/')
    filename += "/";
  int basesize = filename.length();

  // record the stat info on the output file
  struct stat outstat = { 0 };
  stat(poptions.srcml_filename, &outstat);

  // process all non-directory files
  while (struct dirent* entry = readdir(dirp)) {

    // skip standard UNIX filenames, and . files
    if (entry->d_name[0] == '.')
      continue;

    // handle directories later after all the filenames
    struct stat fstat = { 0 };
    stat(entry->d_name, &fstat);
    if (S_ISDIR(fstat.st_mode)) {
      fprintf(stderr, "FILE IS DIR: %s\n", entry->d_name);
      continue;
    }

    // path with current filename
    filename.resize(basesize);
    filename += entry->d_name;

    // make sure that we are not processing the output file
    struct stat instat = { 0 };
    stat(filename.c_str(), &instat);
    if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
      if (!shownumber)
	fprintf(stderr, "Skipped '%s':  Output file.\n", poptions.srcml_filename);
      else
	fprintf(stderr, "%5s %s\tSkipped: Output file.\n", "-", poptions.srcml_filename);

      ++skipped;
      continue;
    }

    // translate the file listed in the input file using the directory and filename extracted from the path
    src2srcml_file(translator,
		   filename.c_str(),
		   options,
		   0,
		   0,
		   poptions.given_version,
		   poptions.language,
		   poptions.tabsize,
		   count, skipped, error, showinput, shownumber);
  }

  // no need to handle subdirectories, unless recursive
  if (!isoption(options, OPTION_RECURSIVE))
    return;

  // go back and process directories
  rewinddir(dirp);
  while (struct dirent* entry = readdir(dirp)) {

    // skip standard UNIX filenames, and . files
    // TODO:  Skip . and .. by default, but should we announce others?  E.g., .svn?
    if (entry->d_name[0] == '.')
      continue;

    // already handled other types of files
    struct stat fstat = { 0 };
    stat(entry->d_name, &fstat);
    if (!S_ISDIR(fstat.st_mode))
      continue;

    // path with current filename
    filename.resize(basesize);
    filename += entry->d_name;

    process_dir(translator, filename.c_str(), poptions, count, skipped, error, showinput, shownumber);
  }
}

void process_filelist(srcMLTranslator& translator, process_options& poptions, int& count, int & skipped, int & error, bool & showinput) {

  try {

    // translate all the filenames listed in the named file
    // Use libxml2 routines so that we can handle http:, file:, and gzipped files automagically
    URIStream uriinput(poptions.fname);
    char* line;
    if (xmlRegisterInputCallbacks(archiveReadMatch, archiveReadOpen, archiveRead, archiveReadClose) < 0) {
      fprintf(stderr, "%s: failed to register archive handler\n", PROGRAM_NAME);
      exit(1);
    }

    while ((line = uriinput.readline())) {

      // skip over whitespace
      // TODO:  Other types of whitespace?  backspace?
      line += strspn(line, " \t\f");

      // skip blank lines or comment lines
      if (line[0] == '\0' || line[0] == '\n' || line[0] == FILELIST_COMMENT)
        continue;

      showinput = true;

      // translate the file listed in the input file using the directory and filename extracted from the path
      src2srcml_file(translator,
                     line,
                     options,
                     0,
                     0,
                     poptions.given_version,
                     poptions.language,
                     poptions.tabsize,
                     count, skipped, error, showinput, true);
    }

  } catch (URIStreamFileError) {
    fprintf(stderr, "%s error: file/URI \'%s\' does not exist.\n", PROGRAM_NAME, poptions.fname);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
}
