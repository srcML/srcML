/*
  src2srcml.cpp

  Copyright (C) 2002-2006  SDML (www.sdml.info)

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

#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include "version.h"
#include "srcmlapps.h"
#include "srcmlns.h"
#include "Options.h"
#include "project.h"
#include "Language.h"
#include "srcMLTranslator.h"

using namespace LanguageName;

const char* const NAME = "src2srcml";

const char* const DEBUG_FLAG = "--debug";
const char* const DEBUG_FLAG_SHORT = "-g";

const char* const LITERAL_FLAG = "--literal";

const char* const OPERATOR_FLAG = "--operator";

const char* const MODIFIER_FLAG = "--modifier";

const char* const INTERACTIVE_FLAG = "--interactive";
const char* const INTERACTIVE_FLAG_SHORT = "-c";

const char* const CPP_MARKUP_ELSE_FLAG = "--cpp_markup_else";
const char* const CPP_MARKUP_ELSE_FLAG_SHORT = "";

const char* const CPP_TEXTONLY_ELSE_FLAG = "--cpp_text_else";
const char* const CPP_TEXTONLY_ELSE_FLAG_SHORT = "";

const char* const CPP_MARKUP_IF0_FLAG = "--cpp_markup_if0";
const char* const CPP_MARKUP_IF0_FLAG_SHORT = "";

const char* const CPP_TEXTONLY_IF0_FLAG = "--cpp_text_if0";
const char* const CPP_TEXTONLY_IF0_FLAG_SHORT = "";

const char* const EXPRESSION_MODE_FLAG = "--expression";
const char* const EXPRESSION_MODE_FLAG_SHORT = "-e";

const char* const SELF_VERSION_FLAG = "--self-version";
const char* const SELF_VERSION_FLAG_SHORT = "";

const char* const DEFAULT_XML_ENCODING = "UTF-8";

const char* const FILELIST_FLAG = "--input-file";
const char* const FILELIST_FLAG_SHORT = "-i";

const char* const XMLNS_FLAG = "--xmlns";
const char* const XMLNS_DEFAULT_FLAG_FULL = "--xmlns=URI";
const char* const XMLNS_FLAG_FULL = "--xmlns:PREFIX=URI";

const int DEFAULT_LANGUAGE = srcMLTranslator::LANGUAGE_CXX;

const char* const EXAMPLE_TEXT_FILENAME="foo.cpp";
const char* const EXAMPLE_XML_FILENAME="foo.cpp.xml";

const char FILELIST_COMMENT = '#';

enum {
  SRCML_SRC_NS_URI_POS = 0, 
  SRCML_CPP_NS_URI_POS,
  SRCML_ERR_NS_URI_POS,
  SRCML_EXT_LITERAL_NS_URI_POS,
  SRCML_EXT_OPERATOR_NS_URI_POS,
  SRCML_EXT_MODIFIER_NS_URI_POS,
};

char const * const num2uri[] = {
  SRCML_SRC_NS_URI,
  SRCML_CPP_NS_URI,
  SRCML_ERR_NS_URI,
  SRCML_EXT_LITERAL_NS_URI,
  SRCML_EXT_OPERATOR_NS_URI,
  SRCML_EXT_MODIFIER_NS_URI,
};

// output help
void output_help(const char* name) {
  printf( "Usage: %s ( [options] <infile>... <outfile>\n\n"

	  "Translates source-code files into the XML source-code representation srcML\n"
	  "with support for creating compound srcML documents.\n\n"

	  "When no filenames are given read from standard input and write to standard output.\n"
	  "When only one filename is given write to standard output.\n"
	  "An input filename of '-' also reads from standard input.\n\n"

	  "Options:\n", name);

  printf("  %s, %s display this help and exit\n",      HELP_FLAG_SHORT, HELP_FLAG);
  printf("  %s, %s display version number and exit\n", VERSION_FLAG_SHORT, VERSION_FLAG);

  printf("  %s, %s translates a single, individual expression outside of any statement\n",
	  EXPRESSION_MODE_FLAG_SHORT, EXPRESSION_MODE_FLAG);

  printf("  %s, %s store all input source files in one compound srcML document\n",
	  NESTED_FLAG_SHORT, NESTED_FLAG);

  printf("  %s, %s treat input file as a list of source files for one compound srcML document\n\n",
	  FILELIST_FLAG_SHORT, FILELIST_FLAG);

  printf("  %s, %s set the output XML encoding to ENC (default:  %s)\n",
	  ENCODING_FLAG_SHORT, ENCODING_FLAG, DEFAULT_XML_ENCODING);

  printf("  %s, %s set the input source encoding to ENC (default:  %s)\n\n",
	  TEXTENCODING_FLAG_SHORT, TEXTENCODING_FLAG, DEFAULT_TEXT_ENCODING);

      /*
	      << "  " << SKIP_ENCODING_FLAG_SHORT        << ", " << setw(COL) <<  SKIP_ENCODING_FLAG
	      << "store the text without any text encoding changes" << "\n"
      */

  printf("  %s set the default namespace URI\n", XMLNS_DEFAULT_FLAG_FULL);
  printf("        (default:  xmlns=\"%s\")\n", SRCML_SRC_NS_URI);

  printf("  %s set the namespace PREFIX for the namespace URI\n", XMLNS_FLAG_FULL);
  printf("        (defaults:  xmlns:cpp=\"%s\")\n", SRCML_CPP_NS_URI);
  printf("        (defaults:  xmlns:srcerr=\"%s\")\n", SRCML_ERR_NS_URI);

  printf("  %s	do not output the default XML declaration\n", NO_XML_DECLARATION_FLAG);
  printf("  %s	do not output any namespace declarations\n\n", NO_NAMESPACE_DECLARATION_FLAG);

  printf("  %s, %s output in gzip format\n", COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG);

  printf("  %s, %s low-latency output\n", INTERACTIVE_FLAG_SHORT, INTERACTIVE_FLAG);

  printf("  %s, %s markup translation errors in namespace \"%s\"\n",
	  DEBUG_FLAG_SHORT, DEBUG_FLAG, SRCML_ERR_NS_URI);

  printf("  %s, %s verbose output to standard error\n\n", VERBOSE_FLAG_SHORT, VERBOSE_FLAG);

  printf("Metadata Options:\n\n"
	 "  %s, %s set the language to %s, %s (default), %s, or %s\n",
	 LANGUAGE_FLAG_SHORT, LANGUAGE_FLAG_FULL,
	 LANGUAGE_C, LANGUAGE_CXX, LANGUAGE_JAVA, LANGUAGE_ASPECTJ);

  printf("  %s, %s set the directory attribute to DIR\n",
	 DIRECTORY_FLAG_SHORT, DIRECTORY_FLAG_FULL);

  printf("  %s, %s set the filename attribute to FILE\n",
	 FILENAME_FLAG_SHORT, FILENAME_FLAG_FULL);

  printf("  %s, %s set the version attribute to VER\n\n",
	 SRCVERSION_FLAG_SHORT, SRCVERSION_FLAG_FULL);

      /*
	      << "  " << SELF_VERSION_FLAG <<  "  " << "        "
	      << "set the version attribute to the src2srcml version\n"
      */

  printf("Markup Extensions:\n\n");

  printf("  %s markup literal values in namespace \"%s\"\n",
	 LITERAL_FLAG, SRCML_EXT_LITERAL_NS_URI);

  printf("  %s markup operators in namespace \"%s\"\n",
	 OPERATOR_FLAG, SRCML_EXT_OPERATOR_NS_URI);

  printf("  %s markup type modifiers in namespace \"%s\"\n",
	 MODIFIER_FLAG, SRCML_EXT_MODIFIER_NS_URI);

  printf("\nCPP Markup Options:\n\n");

  printf("  %s  markup cpp #else regions (default)\n", CPP_MARKUP_ELSE_FLAG);
  printf("  %s  leave cpp #else regions as text\n\n", CPP_TEXTONLY_ELSE_FLAG);

  printf("  %s  markup cpp #if 0 regions\n", CPP_MARKUP_IF0_FLAG);
  printf("  %s  leave cpp #if 0 regions as text (default)\n\n", CPP_TEXTONLY_IF0_FLAG);

  printf("Examples:  \n"
	 "  %1$s                (read from standard input, write to standard output)\n"
	 "  %1$s m.cpp          (read from file m.cpp, write to standard output)\n"
	 "  %1$s m.cpp m.cpp.xml (read from file m.cpp, write to file m.cpp.xml)\n"
	 "  %1$s -              (read from standard input, write to standard output)\n"
	 "  %1$s - m.cpp.xml    (read from standard input, write to file m.cpp.xml)\n"
	 "  %1$s --directory=src --filename=m.cpp - m.cpp.xml "
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

int options = OPTION_CPP_MARKUP_ELSE | OPTION_CPP | OPTION_XMLDECL | OPTION_NAMESPACEDECL;
const char* src_encoding = DEFAULT_TEXT_ENCODING;
int language = 0;
const char* xml_encoding = DEFAULT_XML_ENCODING;
const char* given_directory = 0;
const char* given_filename = 0;
const char* given_version = 0;

const char* num2prefix[] = {

  SRCML_SRC_NS_PREFIX_DEFAULT,
  SRCML_CPP_NS_PREFIX_DEFAULT,
  SRCML_ERR_NS_PREFIX_DEFAULT,
  SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
  SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
  SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
};

bool prefixchange[] = {
  false,
  false,
  false,
  false,
  false,
  false,
};

// setup options and collect info from arguments
int process_args(int argc, char* argv[]);

#ifdef __GNUG__
extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);
#endif

int main(int argc, char* argv[]) {

  int exit_status = EXIT_SUCCESS;

  /* signal handling */

#ifdef __GNUG__
  // signal to toggle verbose flag
  pstd::signal(SIGUSR1, verbose_handler);
#endif

  // process command-line arguments
  int curarg = process_args(argc, argv);

  // help flag trumps all other options
  if (isoption(options, OPTION_HELP)) {
    output_help(NAME);
    exit(STATUS_SUCCESS);
  }

  // version flag trumps all other options except for help
  if (isoption(options, OPTION_PVERSION)) {
    output_version(NAME);
    exit(STATUS_SUCCESS);
  }

  /*
  // setup self-versioning
  if (isoption(options, OPTION_SELF_VERSION)) {
    //    given_version = version();
  }
  */

  /* Special checks for illegal combinations */

  /*
  // skip encoding and specifying encoding
  if (isoption(options, OPTION_SKIP_ENCODING) && isoption(options, OPTION_TEXT_ENCODING)) {

    fprintf(stderr, "%: Options for skipping encoding and specifying source encoding are incompatible.\n", NAME);
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }
  */

  /*
  */
  // eat optional option separator
  if (argc > (curarg) && strcmp(argv[curarg], OPTION_SEPARATOR) == 0)
      ++curarg;

  // output filename
  const char* srcml_filename = 0;

  // first command line parameter after options are the input filenames
  int input_arg_start = 0;
  int input_arg_end = -1;
  int input_arg_count = 0;
  if ((argc - (curarg - 1)) > 1) {

    // mark first input filename 
    input_arg_start = curarg;

    // mark last input filename assuming output srcml filename is last
    if ((argc - (curarg - 1)) > 2)
      input_arg_end = argc - 2;
    else
      input_arg_end = curarg;

    // calculate the total number of input files
    input_arg_count = input_arg_end - input_arg_start + 1;

    // if more than one input filename assume nested
    if (input_arg_count > 1)
      options |= OPTION_NESTED;

    // update the argument count with the input filenames
    curarg += input_arg_count;
  }

  // last command line parameter is output srcml filename
  if (!srcml_filename) {
    srcml_filename = "-";
    if ((argc - (curarg - 1)) > 1) {
      srcml_filename = argv[curarg];

      ++curarg;
    }
  }

  // verify that the output filename is not the same as any of the input filenames
#ifdef __GNUG__
  struct stat outstat;
  stat(srcml_filename, &outstat);
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

    struct stat instat;
    stat(argv[i], &instat);
    if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
      fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n",
	      NAME, argv[i], srcml_filename);
      exit(STATUS_INPUTFILE_PROBLEM);
    }
  }
#endif

  // make sure user did not specify duplicate prefixes as an option
  for (int i = 0; i < 6 - 1; ++i) {
    for (int j = i + 1; j < 6; ++j)
      if(strcmp(num2prefix[i], num2prefix[j]) == 0) {

	fprintf(stderr, "%s: Namespace conflict for ", NAME);
	if (num2prefix[i] == '\0') {
	  fprintf(stderr, "default prefix\n");
	} else {
	  fprintf(stderr, "prefix \'%s\'\n", num2prefix[i]);
	}
	fprintf(stderr, "Prefix URI conflicts:\n  %s\n  %s\n", num2uri[i], num2uri[j]);

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

  try {

    // convert from the map to an array to pass to the translator

    // for single file specified on command line, grab the language from the extension
    if (language == 0 && input_arg_count == 1) {

      char* path = argv[input_arg_start];
      char* ext = filename_ext(path);
      language = Language::getLanguageFromExtension(ext);
    }

    // translator from input to output using determined language
    if (language == 0)
	language = DEFAULT_LANGUAGE;
    srcMLTranslator translator(language, src_encoding, xml_encoding, srcml_filename, options, given_directory, given_filename, given_version, num2prefix);

  // output source encoding
  if (isoption(options, OPTION_VERBOSE)) {
    fprintf(stderr, "Source encoding:  %s\n", src_encoding);
    fprintf(stderr, "XML encoding:  %s\n", xml_encoding);
  }

  // translate input filenames from list in file
  if (isoption(options, OPTION_FILELIST)) {

    // assume file with list of filenames is from standard input
    std::istream* pinfilelist = &std::cin;

    // open the input file (if not standard input) that contains the list of filenames
    std::ifstream infile;
    if (input_arg_count > 0 && strcmp(argv[input_arg_start], "-")) {
      infile.open(argv[input_arg_start]);
      pinfilelist = &infile;
    }

#ifdef __GNUG__
    // setup so we can gracefully stop after a file at a time
    pstd::signal(SIGINT, terminate_handler);
#endif
      
    // translate all the filenames listed in the named file
    const int MAXFILENAME = 512;
    char line[MAXFILENAME];
    int count = 0;    // keep count for verbose mode
    while (pinfilelist->getline(line, MAXFILENAME)) {

      // skip blank lines or comment lines
      if (line == '\0' || line[0] == FILELIST_COMMENT)
	continue;

      // another file
      ++count;

      // in verbose mode output the currently processed filename
      if (isoption(options, OPTION_VERBOSE)) {

	fprintf(stderr, "%d\t%s", count, line);
      }

      // translate the file listed in the input file using the directory and filename extracted from the path
      char* dir = 0;
      char* filename = 0;
      try {
	std::istream* pin = translator.setupInput(line);
	filename_split(line, dir, filename);
	translator.translate(pin,
			     dir,
			     filename,
			     given_version,
			     language);
      } catch (FileError) {

	if (isoption(options, OPTION_VERBOSE))
	  fprintf(stderr, "\t\terror: file \'%s\%s\' does not exist.\n", dir, filename);
	else
	  fprintf(stderr, " error: file \'%s\%s\' does not exist.\n", dir, filename);
      }

      if (isoption(options, OPTION_VERBOSE)) {
	fprintf(stderr, "\n");
      }
      // compound documents are interrupted gracefully
      if (isoption(options, OPTION_TERMINATE))
	return STATUS_TERMINATED;
    }

  // translate from standard input
  } else if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

    // translate from standard input using any directory, filename and version given on the command line
    translator.translate(translator.setupInput(STDIN), given_directory, given_filename, given_version, 
			 language ? language : DEFAULT_LANGUAGE);

  // translate single input filename from command line
  }  else if (input_arg_count == 1) {

    // translate from path given on command line using directory given on the command line or extracted
    // from full path
    char* path = argv[input_arg_start];
    std::istream* pin = translator.setupInput(path);
    char* path_s = 0;
    char* filename_s = 0;
    filename_split(path, path_s, filename_s);

    // hack to fix where directory, but no filename
    if (path_s[0] && !filename_s[0]) {
      char* p = path_s;
      path_s = filename_s;
      filename_s = p;
    }

    try {
      translator.translate(pin,
			   isoption(options, OPTION_DIRECTORY) ? given_directory : path_s,
			   isoption(options, OPTION_FILENAME)  ? given_filename  : filename_s,
			   given_version,
			   language);

    } catch (FileError) {

      fprintf(stderr, "%s error: file \'%s\' does not exist.\n", NAME, path);
      exit(STATUS_INPUTFILE_PROBLEM);
    }

  // translate multiple input filenames on command line
  } else {

    int count = 0;    // keep count for verbose mode

#ifdef __GNUG__
    // setup so we can gracefully stop after a file at a time
    pstd::signal(SIGINT, terminate_handler);
#endif
      
    // translate in batch the input files on the command line extracting the directory and filename attributes
    // from the full path
    for (int i = input_arg_start; i <= input_arg_end; ++i) {

      char* path = argv[i];

      // another file
      ++count;

      // in verbose mode output the currently processed filename
      if (isoption(options, OPTION_VERBOSE)) {
	fprintf(stderr, "%d\t%s", count, path);
      }
      try {
	std::istream* pin = translator.setupInput(path);
	char* path_s = 0;
	char* filename_s = 0;
	filename_split(path, path_s, filename_s);
	translator.translate(pin, path_s, filename_s, 0, language);
      } catch (FileError) {
	fprintf(stderr, "%s error: file \'%s\' does not exist.\n", NAME, path);
      }

      if (isoption(options, OPTION_VERBOSE)) {
	fprintf(stderr, "\n");
      }

      // compound documents are interrupted gracefully
      if (isoption(options, OPTION_TERMINATE))
	return STATUS_TERMINATED;
    }
  }
  } catch (srcEncodingException) {
    fprintf(stderr, "Translation encoding problem\n");
    exit(STATUS_UNKNOWN_ENCODING);
  }

  return exit_status;
}

// setup options and collect info from arguments
int process_args(int argc, char* argv[]) {

  bool cpp_if0 = false;
  bool cpp_else = false;

  // process all command line options
  int position = 0;
  int curarg = 1;  // current argument
  while (argc > curarg && strlen(argv[curarg]) > 1 && argv[curarg][0] == '-' &&
	 strcmp(argv[curarg], OPTION_SEPARATOR) != 0) {

    // mark to detect changes
    int original_position = position;

    // help mode
    if (compare_flags(argv[curarg], HELP_FLAG, HELP_FLAG_SHORT, position)) {
      options |= OPTION_HELP;
      if (position == original_position) ++curarg;
    }

    // version mode
    else if (compare_flags(argv[curarg], VERSION_FLAG, VERSION_FLAG_SHORT, position)) {
      options |= OPTION_PVERSION;
      if (position == original_position) ++curarg;
    }

    /*
    // version mode
    else if (compare_flags(argv[curarg], SELF_VERSION_FLAG, SELF_VERSION_FLAG_SHORT, position)) {
      options |= OPTION_SELF_VERSION;
      if (position == original_position) ++curarg;
    }
    */

    // debug mode
    else if (compare_flags(argv[curarg], DEBUG_FLAG, DEBUG_FLAG_SHORT, position)) {
      options |= OPTION_DEBUG;
      if (position == original_position) ++curarg;
    }

    // nested mode
    else if (compare_flags(argv[curarg], NESTED_FLAG, NESTED_FLAG_SHORT, position)) {
      options |= OPTION_NESTED;
      if (position == original_position) ++curarg;
    }

    // literal mode
    else if (compare_flags(argv[curarg], LITERAL_FLAG, "", position)) {
      options |= OPTION_LITERAL;
      if (position == original_position) ++curarg;
    }

    // operator mode
    else if (compare_flags(argv[curarg], OPERATOR_FLAG, "", position)) {
      options |= OPTION_OPERATOR;
      if (position == original_position) ++curarg;
    }

    // modifier mode
    else if (compare_flags(argv[curarg], MODIFIER_FLAG, "", position)) {
      options |= OPTION_MODIFIER;
      if (position == original_position) ++curarg;
    }

    // interactive mode
    else if (compare_flags(argv[curarg], INTERACTIVE_FLAG, INTERACTIVE_FLAG_SHORT, position)) {
      options |= OPTION_INTERACTIVE;
      if (position == original_position) ++curarg;
    }

    // verbose mode
    else if (compare_flags(argv[curarg], VERBOSE_FLAG, VERBOSE_FLAG_SHORT, position)) {
      options |= OPTION_VERBOSE;
      if (position == original_position) ++curarg;
    }

    // compressed mode
    else if (compare_flags(argv[curarg], COMPRESSED_FLAG, COMPRESSED_FLAG_SHORT, position)) {

      options |= OPTION_COMPRESSED;
      if (position == original_position) ++curarg;
    }
    /*
    // skip encoding mode
    else if (compare_flags(argv[curarg], SKIP_ENCODING_FLAG, SKIP_ENCODING_FLAG_SHORT, position)) {
      options |= OPTION_SKIP_ENCODING;
      if (position == original_position) ++curarg;
    }
    */
    // markup of cpp #else mode
    else if (compare_flags(argv[curarg], CPP_MARKUP_ELSE_FLAG, CPP_MARKUP_ELSE_FLAG_SHORT, position)) {
      if (!cpp_else) {
	options |= OPTION_CPP_MARKUP_ELSE;
	if (position == original_position) ++curarg;

	cpp_else = true;

      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		NAME, CPP_MARKUP_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
    }

    // text-only cpp #else mode
    else if (compare_flags(argv[curarg], CPP_TEXTONLY_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG_SHORT, position)) {

      if (!cpp_else) {
	options &= ~OPTION_CPP_MARKUP_ELSE;
	if (position == original_position) ++curarg;

	cpp_else = true;
      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		NAME, CPP_MARKUP_ELSE_FLAG, CPP_TEXTONLY_ELSE_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
    }

    // markup of cpp #if 0 mode
    else if (compare_flags(argv[curarg], CPP_MARKUP_IF0_FLAG, CPP_MARKUP_IF0_FLAG_SHORT, position)) {
      if (!cpp_if0) {
	options |= OPTION_CPP_MARKUP_IF0;
	if (position == original_position) ++curarg;

	cpp_if0 = true;

      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		NAME, CPP_MARKUP_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
    }

    // text-only cpp #if 0 mode
    else if (compare_flags(argv[curarg], CPP_TEXTONLY_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG_SHORT, position)) {

      if (!cpp_if0) {
	// clear if previously marked
	options &= ~OPTION_CPP_MARKUP_IF0;
	if (position == original_position) ++curarg;

	cpp_if0 = true;
      } else {
	fprintf(stderr, "%s: Conflicting options %s and %s selected.\n",
		NAME, CPP_MARKUP_IF0_FLAG, CPP_TEXTONLY_IF0_FLAG);
	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
    }

    // filelist mode
    else if (compare_flags(argv[curarg], FILELIST_FLAG, FILELIST_FLAG_SHORT, position)) {
      options |= OPTION_FILELIST;
      if (position == original_position) ++curarg;

      // filelist mode is default nested mode
      options |= OPTION_NESTED;
    }

    // no xml declaration mode
    else if (compare_flags(argv[curarg], NO_XML_DECLARATION_FLAG, NO_XML_DECLARATION_FLAG, position)) {
      if (position == original_position) ++curarg;

      // turnoff default xml declaration
      options &= ~OPTION_XMLDECL;
    }

    // no namespace declaration mode
    else if (compare_flags(argv[curarg], NO_NAMESPACE_DECLARATION_FLAG, NO_NAMESPACE_DECLARATION_FLAG, position)) {
      if (position == original_position) ++curarg;

      // turnoff default xml declaration
      options &= ~OPTION_NAMESPACEDECL;
    }

    // language is based on parameter
    else if (compare_flags(argv[curarg], LANGUAGE_FLAG, LANGUAGE_FLAG_SHORT)) {

      options |= OPTION_LANGUAGE;

      const char* langparam = "";
      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	langparam = embedded + 1;
	++curarg;

      // check for language flag with missing language value
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: language option selected but not specified.\n", NAME);
	exit(STATUS_LANGUAGE_MISSING);
      } else {

	// extract parameter
	langparam = argv[(++curarg)++];
      }

      // validate language selected
      language = Language::getLanguage(langparam);
      if (language == 0) {
	fprintf(stderr, "%s: invalid option -- Language flag must one of the following values:  "
		"%s %s %s %s\n", NAME, LANGUAGE_C, LANGUAGE_CXX, LANGUAGE_JAVA, LANGUAGE_ASPECTJ);

	exit(STATUS_INVALID_LANGUAGE);
	break;
      }

      // turnoff default cpp reference for Java-based languages
      if (language == srcMLTranslator::LANGUAGE_JAVA || language == srcMLTranslator::LANGUAGE_ASPECTJ)
	options &= ~OPTION_CPP;

    // xml namespace specifications
    } else if (compare_flags(argv[curarg], XMLNS_FLAG, "") || strncmp(argv[curarg], XMLNS_FLAG, strlen(XMLNS_FLAG)) == 0) {

      options |= OPTION_XMLNS;

      char* embedded = extract_option(argv[curarg]);

      const char* ns_prefix;
      const char* ns_uri;

      // filename is embedded parameter
      if (embedded) {

	if (argv[curarg][strlen(XMLNS_FLAG)] != ':')
	  ns_prefix = "";
	else {
	  *embedded = '\0';
	  ns_prefix = argv[curarg] + strlen(XMLNS_FLAG) + 1;
	}

	ns_uri = embedded + 1;
	
      // check for language flag with missing language value
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: xmlns option selected but not specified.\n", NAME);
	exit(STATUS_LANGUAGE_MISSING);
      } else {

	// extract prefix
	if (strlen(argv[curarg]) == strlen(XMLNS_FLAG))
	  ns_prefix = "";
	else
	  ns_prefix = argv[curarg] + strlen(XMLNS_FLAG) + 1;

	// uri is next argument
	ns_uri = argv[++curarg];
      }

      ++curarg;

      // update the uri's
      // check for standard namespaces, store them, and update any flags
      if (strcmp(ns_uri, SRCML_SRC_NS_URI) == 0) {

	num2prefix[SRCML_SRC_NS_URI_POS] = ns_prefix;
	prefixchange[SRCML_SRC_NS_URI_POS] = true;

      } else if (strcmp(ns_uri, SRCML_CPP_NS_URI) == 0) {

	// specifying the cpp prefix automatically turns on preprocessor
	options |= OPTION_CPP;

	num2prefix[SRCML_CPP_NS_URI_POS] = ns_prefix;
	prefixchange[SRCML_CPP_NS_URI_POS] = true;

      } else if (strcmp(ns_uri, SRCML_ERR_NS_URI) == 0) {

	// specifying the error prefix automatically turns on debugging
	options |= OPTION_DEBUG;

	num2prefix[SRCML_ERR_NS_URI_POS] = ns_prefix;
	prefixchange[SRCML_ERR_NS_URI_POS] = true;

      } else if (strcmp(ns_uri, SRCML_EXT_LITERAL_NS_URI) == 0) {

	// specifying the literal prefix automatically turns on literal markup
	options |= OPTION_LITERAL;

	num2prefix[SRCML_EXT_LITERAL_NS_URI_POS] = ns_prefix;
	prefixchange[SRCML_EXT_LITERAL_NS_URI_POS] = true;

      } else if (strcmp(ns_uri, SRCML_EXT_OPERATOR_NS_URI) == 0) {

	// specifying the operator prefix automatically turns on operator markup
	options |= OPTION_OPERATOR;

	num2prefix[SRCML_EXT_OPERATOR_NS_URI_POS] = ns_prefix;
	prefixchange[SRCML_EXT_OPERATOR_NS_URI_POS] = true;

      } else if (strcmp(ns_uri, SRCML_EXT_MODIFIER_NS_URI) == 0) {

	// specifying the operator prefix automatically turns on type modifier markup
	options |= OPTION_MODIFIER;

	num2prefix[SRCML_EXT_MODIFIER_NS_URI_POS] = ns_prefix;
	prefixchange[SRCML_EXT_MODIFIER_NS_URI_POS] = true;

      } else {
	fprintf(stderr, "%s: invalid namespace \"%s\"\n\n"
		"Namespace URI must be on of the following:  \n"
		"  %-35s primary srcML namespace\n"
		"  %-35s namespace for cpreprocessing elements\n"
		"  %-35s namespace for srcML debugging elements\n"
		"  %-35s namespace for optional literal elements\n"
		"  %-35s namespace for optional operator element\n"
		"  %-35s namespace for optional modifier element\n",
		NAME, ns_uri,
		SRCML_SRC_NS_URI, SRCML_CPP_NS_URI, SRCML_ERR_NS_URI,
		SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_MODIFIER_NS_URI
		);
	exit(STATUS_INVALID_LANGUAGE);
      }
    }
    
    // xml_encoding
    else if (compare_flags(argv[curarg], ENCODING_FLAG, ENCODING_FLAG_SHORT)) {

      options |= OPTION_XML_ENCODING;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	xml_encoding = embedded + 1;
	++curarg;

      // check for encoding flag with missing encoding
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: encoding selected but not specified.\n", NAME);
	exit(STATUS_XMLENCODING_MISSING);
      } else {

	// extract parameter
	xml_encoding = argv[(++curarg)++];
      }

      // validate xml encoding
      if (!srcMLOutput::checkEncoding(xml_encoding)) {
	fprintf(stderr, "%s: xml encoding \"%s\" is not supported.\n", NAME, xml_encoding);
	exit(STATUS_UNKNOWN_ENCODING);
      }
    }

    // text encoding
    else if (compare_flags(argv[curarg], TEXTENCODING_FLAG, TEXTENCODING_FLAG_SHORT)) {

      options |= OPTION_TEXT_ENCODING;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	src_encoding = embedded + 1;
	++curarg;

      // check for text encoding flag with missing text encoding
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: text encoding selected but not specified.\n", NAME);
	exit(STATUS_SRCENCODING_MISSING);
      } else {

	// extract parameter
	src_encoding = argv[(++curarg)++];
      }

      // validate source encoding
      if (!srcMLOutput::checkEncoding(src_encoding)) {
	fprintf(stderr, "%s: text encoding \"%s\" is not supported.\n", NAME, src_encoding);
	exit(STATUS_UNKNOWN_ENCODING);
      }
    }

    // optional directory specification
    else if (compare_flags(argv[curarg], DIRECTORY_FLAG, DIRECTORY_FLAG_SHORT)) {

      options |= OPTION_DIRECTORY;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	given_directory = embedded + 1;
	++curarg;

      // validate that the filename is given and is not another flag
      } else if (argc <= curarg + 1 || argv[curarg + 1][0] == '-') {
	fprintf(stderr, "%s: invalid option -- Directory must be specified.\n", NAME);
	exit(STATUS_DIRECTORY_MISSING);
      } else {

	// extract parameter
	given_directory = argv[(++curarg)++];
      }
    }

    // optional filename
    else if (compare_flags(argv[curarg], FILENAME_FLAG, FILENAME_FLAG_SHORT)) {

      options |= OPTION_FILENAME;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	given_filename = embedded + 1;
	++curarg;

      // validate that the filename is given and is not another flag
      } else if (argc <= curarg + 1 || argv[curarg + 1][0] == '-') {
	fprintf(stderr, "%s: invalid option -- Filename must be specified.\n", NAME);
	exit(STATUS_FILENAME_MISSING);

      // filename is separate parameter
      } else {

	// extract parameter
	given_filename = argv[(++curarg)++];
      }
    }

    // optional version attribute
    else if (compare_flags(argv[curarg], SRCVERSION_FLAG, SRCVERSION_FLAG_SHORT)) {

      options |= OPTION_VERSION;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	given_version = embedded + 1;
	++curarg;

      // validate that the version is given and is not another flag
      } else if (argc <= curarg + 1 || argv[curarg + 1][0] == '-') {
	fprintf(stderr, "%s: invalid option -- Version must be specified.\n", NAME);
	exit(STATUS_VERSION_MISSING);

      // filename is separate parameter
      } else {

	// extract parameter
	given_version = argv[(++curarg)++];
      }
    }

    // expression mode
    else if (compare_flags(argv[curarg], EXPRESSION_MODE_FLAG, EXPRESSION_MODE_FLAG_SHORT, position)) {
      options |= OPTION_EXPRESSION;
      if (position == original_position) ++curarg;
    }

    // reached the end of a multi-short form option
    else if (position > 0 && argv[curarg][position + 1] == '\0') {

      ++curarg;

    // invalid option
    } else {

      fprintf(stderr, "%s: unrecognized option '%s'\n", NAME, argv[curarg]);
      fprintf(stderr, "try '%s %s' for more information.\n", NAME, HELP_FLAG);
      exit(STATUS_UNKNOWN_OPTION);
    }
  }

  return curarg;
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
