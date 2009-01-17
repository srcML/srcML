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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
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

#ifdef LIBXML_ENABLED
const char* const DEFAULT_XML_ENCODING = "UTF-8";
#else
const char* const DEFAULT_XML_ENCODING = "ISO-8859-1";
#endif

const char* const FILELIST_FLAG = "--input-file";
const char* const FILELIST_FLAG_SHORT = "-i";

const char* const XMLNS_FLAG = "--xmlns";
const char* const XMLNS_DEFAULT_FLAG_FULL = "--xmlns=URI";
const char* const XMLNS_FLAG_FULL = "--xmlns:PREFIX=URI";

const int DEFAULT_LANGUAGE = srcMLTranslator::LANGUAGE_CXX;

const char* const EXAMPLE_TEXT_FILENAME="foo.cpp";
const char* const EXAMPLE_XML_FILENAME="foo.cpp.xml";

const char* const GAP = "                              ";

const char FILELIST_COMMENT = '#';

// split path into directory and filename
char* split_path(char* path);

using std::setw;

// output help
void output_help(const char* name) {
    std::cout << "Usage: " << name << " [options] <infile>... <outfile>" << '\n'
              << '\n'
	      << "Translates source-code files into the XML source-code representation srcML\n"
	      << "with support for creating compound srcML documents.\n"
              << '\n'
	      << "When no filenames are given read from standard input and write to standard output.\n"
	      << "When only one filename is given write to standard output."  << '\n'
	      << "An input filename of '-' also reads from standard input.\n"
              << '\n'
	      << "Options:"  << '\n'
              << std::left
	      << "  " << HELP_FLAG_SHORT        << ", " << setw(COL) <<  HELP_FLAG
	      << "display this help and exit\n"
	      << "  " << VERSION_FLAG_SHORT     << ", " << setw(COL) <<  VERSION_FLAG
	      << "display version number and exit\n"
              << '\n'
	      << "  " << EXPRESSION_MODE_FLAG_SHORT    << ", " << setw(COL) <<  EXPRESSION_MODE_FLAG
	      << "translates a single, individual expression outside of any statement\n"
              << '\n'
	      << "  " << NESTED_FLAG_SHORT      << ", " << setw(COL) <<  NESTED_FLAG
	      << "store all input source files in one compound srcML document\n"
	      << "  " << FILELIST_FLAG_SHORT    << ", " << setw(COL) <<  FILELIST_FLAG
	      << "treat input file as a list of source files for one compound srcML document\n"
              << '\n'
	      << "  " << ENCODING_FLAG_SHORT    << ", " << setw(COL) <<  ENCODING_FLAG_FULL
	      << "set the output XML encoding to ENC (default:  "
	      << DEFAULT_XML_ENCODING << ") \n"
#ifdef LIBXML_ENABLED
	      << "  " << TEXTENCODING_FLAG_SHORT        << ", " << setw(COL) <<  TEXTENCODING_FLAG_FULL
	      << "set the input source encoding to ENC (default:  " << DEFAULT_TEXT_ENCODING << ") \n"
      /*
	      << "  " << SKIP_ENCODING_FLAG_SHORT        << ", " << setw(COL) <<  SKIP_ENCODING_FLAG
	      << "store the text without any text encoding changes" << "\n"
      */
#endif
	      << '\n'
	      << "  " << setw(COL) <<  XMLNS_DEFAULT_FLAG_FULL << "    "
	      << "set the default namespace URI\n"
	      << "      " << setw(COL) << " " << "(default:  xmlns=\"" << SRCML_SRC_NS_URI << "\")\n"
	      << "  " << setw(COL) <<  XMLNS_FLAG_FULL << "    "
	      << "set the namespace PREFIX for the namespace URI\n"
	      << "      " << setw(COL) << " " << "(defaults:  xmlns:cpp=\"" << SRCML_CPP_NS_URI << "\",\n"
	      << "                          " << "            xmlns:srcerr=\"" << SRCML_ERR_NS_URI << "\")\n"
	      << '\n'
	      << "  " << NO_XML_DECLARATION_FLAG  << "    " 
	      << "do not output the default XML declaration\n"
	      << "  " << NO_NAMESPACE_DECLARATION_FLAG  << "     " 
	      << "do not output any namespace declarations\n"
              << '\n'
#ifdef LIBXML_ENABLED
	      << "  " << COMPRESSED_FLAG_SHORT  << ", " << setw(COL) <<  COMPRESSED_FLAG
	      << "output in gzip format\n"
#endif
	      << "  " << INTERACTIVE_FLAG_SHORT << ", " << setw(COL) <<  INTERACTIVE_FLAG
	      << "low-latency output\n"
	      << "  " << DEBUG_FLAG_SHORT       << ", " << setw(COL) <<  DEBUG_FLAG
	      << "markup translation errors in namespace \"" << SRCML_ERR_NS_URI << "\"\n"
              << "  " << VERBOSE_FLAG_SHORT     << ", " << setw(COL) << VERBOSE_FLAG
	      << "verbose output to standard error\n"
              << '\n'
	      << "Metadata Options:"  << '\n'
              << '\n'
	      << "  " << LANGUAGE_FLAG_SHORT    << ", " << setw(COL) << LANGUAGE_FLAG_FULL 
	      << "set the language to " << LANGUAGE_C << ", " << LANGUAGE_CXX << " (default), " << LANGUAGE_JAVA << ", or " << LANGUAGE_ASPECTJ << "\n"
	      << "  " << DIRECTORY_FLAG_SHORT   << ", " << setw(COL) <<  DIRECTORY_FLAG_FULL
	      << "set the directory attribute to DIR\n"
	      << "  " << FILENAME_FLAG_SHORT    << ", " << setw(COL) <<  FILENAME_FLAG_FULL
	      << "set the filename attribute to FILE\n"
	      << "  " << SRCVERSION_FLAG_SHORT  << ", " << setw(COL) <<  SRCVERSION_FLAG_FULL
	      << "set the version attribute to VER\n"
      /*
	      << "  " << SELF_VERSION_FLAG <<  "  " << "        "
	      << "set the version attribute to the src2srcml version\n"
      */
              << '\n'
	      << "Markup Extensions:"  << '\n'
              << '\n'
	      << "  " << LITERAL_FLAG   << "  " << /* setw(COL) <<*/ "             "
	      << "markup literal values in namespace \"" << SRCML_EXT_LITERAL_NS_URI << "\"\n"
	      << "  " << OPERATOR_FLAG  << " " << /* setw(COL) <<*/ "             "
	      << "markup operators in namespace \"" << SRCML_EXT_OPERATOR_NS_URI << "\"\n"
	      << "  " << MODIFIER_FLAG  << " " << /* setw(COL) <<*/ "             "
	      << "markup type modifiers in namespace \"" << SRCML_EXT_MODIFIER_NS_URI << "\"\n"
              << '\n'
	      << "CPP Markup Options:"  << '\n'
              << '\n'
	      << "  " << CPP_MARKUP_ELSE_FLAG   << "  " << /* setw(COL) <<*/ "     "
	      << "markup cpp #else regions (default)\n"
	      << "  " << CPP_TEXTONLY_ELSE_FLAG     << "  " << /* setw(COL) <<*/ "       "
	      << "leave cpp #else regions as text\n"
              << '\n'
	      << "  " << CPP_MARKUP_IF0_FLAG   << "  " << /* setw(COL) <<*/ "      "
	      << "markup cpp #if 0 regions\n"
	      << "  " << CPP_TEXTONLY_IF0_FLAG     << "  " << /* setw(COL) <<*/ "        "
	      << "leave cpp #if 0 regions as text (default)\n"
	      << '\n'
	      << "Examples:  \n"
	      << "  " << name << "                " << GAP
	      << "(read from standard input, write to standard output)\n"
	      << "  " << name << " m.cpp          " << GAP
	      << "(read from file m.cpp, write to standard output)\n"
	      << "  " << name << " m.cpp m.cpp.xml" << GAP
	      << "(read from file m.cpp, write to file m.cpp.xml)\n"
	      << "  " << name << " -              " << GAP
	      << "(read from standard input, write to standard output)\n"
	      << "  " << name << " - m.cpp.xml    " << GAP
	      << "(read from standard input, write to file m.cpp.xml)\n"
	      << "  " << name << " --directory=src --filename=m.cpp - m.cpp.xml "
	      << "(element unit attributes dir \"src\", filename \"m.cpp\")\n"
#ifdef LIBXML_ENABLED
	      << "  " << name << " --src-encoding=UTF-8 m.cpp m.cpp.xml         "
	      << "(encoding of input text file is UTF-8)\n"
#endif
	      << "  " << name << " --xml-encoding=ISO-8859-1 m.cpp m.cpp.xml    "
	      << "(set encoding of srcML file to ISO-8859-1)\n"

	      << '\n'
	      << "www.sdml.info\n"
	      << "Report bugs to " << EMAIL_ADDRESS << '\n';
}

// output version message
void output_version(const char* name) {
    std::cout << name << " Version " << VERSION << '\n'
	      << COPYRIGHT << '\n';
}

int options = OPTION_CPP_MARKUP_ELSE | OPTION_CPP | OPTION_XMLDECL | OPTION_NAMESPACEDECL;
const char* src_encoding = DEFAULT_TEXT_ENCODING;
int language = DEFAULT_LANGUAGE;
const char* xml_encoding = DEFAULT_XML_ENCODING;
const char* given_directory = 0;
const char* given_filename = 0;
const char* given_version = 0;

typedef std::map<std::string, std::string> URI_TYPE;

URI_TYPE option_uri;

// setup options and collect info from arguments
int process_args(int argc, char* argv[]);

extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);

int main(int argc, char* argv[]) {

  int exit_status = EXIT_SUCCESS;

  /* signal handling */

  // signal to toggle verbose flag
  pstd::signal(SIGUSR1, verbose_handler);

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

    std::cerr << NAME << ": Options for skipping encoding and specifying source encoding are incompatible.\n";
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }
  */

  /*
  */
  // eat optional option separator
  if (argc > (curarg) && strcmp(argv[curarg], OPTION_SEPARATOR) == 0)
      ++curarg;

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
  const char* srcml_filename = "-";
  if ((argc - (curarg - 1)) > 1) {
    srcml_filename = argv[curarg];

    ++curarg;
  }

  // verify that the output filename is not the same as any of the input filenames
  struct stat outstat;
  stat(srcml_filename, &outstat);
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

    struct stat instat;
    stat(argv[i], &instat);
    if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
	std::cerr << NAME << ": Input file '" << argv[i] << "'"
		  << " is the same as the output file '" << srcml_filename << "'\n";
	exit(STATUS_INPUTFILE_PROBLEM);
    }
  }

  // start with the user defined options
  URI_TYPE uri(option_uri.begin(), option_uri.end());

  // fill in any missing standard prefixes
  uri.insert(std::make_pair(SRCML_SRC_NS_URI, SRCML_SRC_NS_PREFIX_DEFAULT));
  uri.insert(std::make_pair(SRCML_CPP_NS_URI, SRCML_CPP_NS_PREFIX_DEFAULT));
  uri.insert(std::make_pair(SRCML_ERR_NS_URI, SRCML_ERR_NS_PREFIX_DEFAULT));
  uri.insert(std::make_pair(SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT));
  uri.insert(std::make_pair(SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT));
  uri.insert(std::make_pair(SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT));

  // make sure user did not specify duplicate prefixes as an option
  for (URI_TYPE::const_iterator po = uri.begin(); po != uri.end(); ++po) {
    URI_TYPE::const_iterator pi = po;
    ++pi;
    for ( ; pi != uri.end(); ++pi) {
      if (pi->second == po->second) {
	std::cerr << NAME << ": Namespace conflict for prefix \'" << po->second << "\' for the namespaces:\n";

        std::cerr << "\t" << pi->first;
	if (!option_uri.count(pi->first))
	  std::cerr << " (default)";
	std::cerr << '\n';

        std::cerr << "\t" << po->first;
	if (!option_uri.count(po->first))
	  std::cerr << " (default)";
	std::cerr << '\n';

	exit(STATUS_INVALID_OPTION_COMBINATION);
      }
    }
  }

  // automatic interactive use from stdin (not on redirect or pipe)
  if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

    struct stat instat;
    fstat(0, &instat);
    if (S_ISCHR(instat.st_mode))
      options |= OPTION_INTERACTIVE;
  }

  try {

    // convert from the map to an array to pass to the translator
    enum { SRCML_SRC_NS_URI_POS, 
       SRCML_CPP_NS_URI_POS,
       SRCML_ERR_NS_URI_POS,
       SRCML_EXT_LITERAL_NS_URI_POS,
       SRCML_EXT_OPERATOR_NS_URI_POS,
       SRCML_EXT_MODIFIER_NS_URI_POS,
    };

    const char* num2prefix[] = {

      uri[SRCML_SRC_NS_URI].c_str(),
      uri[SRCML_CPP_NS_URI].c_str(),
      uri[SRCML_ERR_NS_URI].c_str(),
      uri[SRCML_EXT_LITERAL_NS_URI].c_str(),
      uri[SRCML_EXT_OPERATOR_NS_URI].c_str(),
      uri[SRCML_EXT_MODIFIER_NS_URI].c_str(),
    };

    // translator from input to output using determined language
    srcMLTranslator translator(language, src_encoding, xml_encoding, srcml_filename, options, given_directory, given_filename, given_version, num2prefix);

  // output source encoding
  if (isoption(options, OPTION_VERBOSE)) {
    std::cerr << "Source encoding:  " << src_encoding << '\n';
    std::cerr << "XML encoding:  " << xml_encoding << '\n';
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

    // setup so we can gracefully stop after a file at a time
    pstd::signal(SIGINT, terminate_handler);
      
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
	std::cerr << count << '\t' << line;
      }

      // translate the file listed in the input file using the directory and filename extracted from the path
      const int MAXFILENAME = 512;
      char spath[MAXFILENAME];
      strncpy(spath, line, MAXFILENAME);
      char* path_filename = split_path(spath);
      char* path_directory = spath;
      try {
	translator.translate(line, path_directory, path_filename, given_version);

      } catch (FileError) {

	if (isoption(options, OPTION_VERBOSE))
	  std::cerr << "\t\terror: file \'" << line << "\' does not exist.";
	else
	  std::cerr << NAME << " error: file \'" << line << "\' does not exist." << "\n";
      }

      if (isoption(options, OPTION_VERBOSE)) {
	std::cerr << '\n';
      }
      // compound documents are interrupted gracefully
      if (isoption(options, OPTION_TERMINATE))
	return STATUS_TERMINATED;
    }

  // translate from standard input
  } else if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

    // translate from standard input using any directory, filename and version given on the command line
    translator.translate(STDIN, given_directory, given_filename, given_version);

  // translate single input filename from command line
  }  else if (input_arg_count == 1) {

    // translate from path given on command line using directory given on the command line or extracted
    // from full path
    char* path = argv[input_arg_start];
    const int MAXFILENAME = 512;
    char spath[MAXFILENAME];
    strncpy(spath, path, MAXFILENAME);
    char* path_filename = split_path(spath);
    char* path_directory = spath;

    try {
      translator.translate(path,
			   isoption(options, OPTION_DIRECTORY) ? given_directory : path_directory,
			   isoption(options, OPTION_FILENAME)  ? given_filename  : path_filename,
			   given_version);

    } catch (FileError) {

      std::cerr << NAME << " error: file \'" << path << "\' does not exist." << "\n";
      exit(STATUS_INPUTFILE_PROBLEM);
    }

  // translate multiple input filenames on command line
  } else {

    int count = 0;    // keep count for verbose mode

    // setup so we can gracefully stop after a file at a time
    pstd::signal(SIGINT, terminate_handler);
      
    // translate in batch the input files on the command line extracting the directory and filename attributes
    // from the full path
    for (int i = input_arg_start; i <= input_arg_end; ++i) {

      char* path = argv[i];
      const int MAXFILENAME = 512;
      char spath[MAXFILENAME];
      strncpy(spath, path, MAXFILENAME);
      char* path_filename = split_path(spath);
      char* path_directory = spath;

      // another file
      ++count;

      // in verbose mode output the currently processed filename
      if (isoption(options, OPTION_VERBOSE)) {
	std::cerr << count << '\t' << path;
      }
      try {
	translator.translate(path, path_directory, path_filename);
      } catch (FileError) {
	std::cerr << NAME << " error: file \'" << path << "\' does not exist." << "\n";
      }

      if (isoption(options, OPTION_VERBOSE)) {
	std::cerr << '\n';
      }

      // compound documents are interrupted gracefully
      if (isoption(options, OPTION_TERMINATE))
	return STATUS_TERMINATED;
    }
  }
  } catch (srcEncodingException) {
    std::cerr << "Translation encoding problem\n";
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
#ifdef LIBXML_ENABLED
      options |= OPTION_COMPRESSED;
      if (position == original_position) ++curarg;
#else
      std::cerr << NAME << ": The compression option, i.e., " << COMPRESSED_FLAG << ", is only supported in the libxml version.\n";
      exit(STATUS_LIBXML2_FEATURE);
#endif
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
	std::cerr << NAME << ": Conflicting options " << CPP_MARKUP_ELSE_FLAG << " and " 
		  << CPP_TEXTONLY_ELSE_FLAG << " selected.\n";
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
	std::cerr << NAME << ": Conflicting options " << CPP_MARKUP_ELSE_FLAG << " and " 
		  << CPP_TEXTONLY_ELSE_FLAG << " selected.\n";
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
	std::cerr << NAME << ": Conflicting options " << CPP_MARKUP_IF0_FLAG << " and " 
		  << CPP_TEXTONLY_IF0_FLAG << " selected.\n";
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
	std::cerr << NAME << ": Conflicting options " << CPP_MARKUP_IF0_FLAG << " and " 
		  << CPP_TEXTONLY_IF0_FLAG << " selected.\n";
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
	std::cerr << NAME << ": language option selected but not specified.\n";
	exit(STATUS_LANGUAGE_MISSING);
      } else {

	// extract parameter
	langparam = argv[(++curarg)++];
      }

      // validate language selected
      language = Language::getLanguage(langparam);
      if (language == 0) {
	std::cerr << NAME << ": invalid option -- Language flag must one of the following values:  "
		  << LANGUAGE_C << " " << LANGUAGE_CXX << " " << LANGUAGE_JAVA << " "
		  << LANGUAGE_ASPECTJ << '\n';
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

      std::string ns_prefix;
      std::string ns_uri;

      // filename is embedded parameter
      if (embedded) {

	if (argv[curarg][strlen(XMLNS_FLAG)] != ':')
	  ns_prefix = "";
	else
	  ns_prefix.assign(argv[curarg] + strlen(XMLNS_FLAG) + 1, embedded);

	ns_uri = embedded + 1;
	
      // check for language flag with missing language value
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": xmlns option selected but not specified.\n";
	exit(STATUS_LANGUAGE_MISSING);
      } else {

	// extract prefix
	if (strlen(argv[curarg]) == strlen(XMLNS_FLAG))
	  ns_prefix = "";
	else
	  ns_prefix.assign(argv[curarg] + strlen(XMLNS_FLAG) + 1);

	// uri is next argument
	ns_uri = argv[++curarg];
      }

      ++curarg;
     
      // check for existing namespaces
      if (option_uri.count(ns_uri) > 0) {
	  std::cerr << NAME << ": namespace listed more than once.\n";
	  exit(STATUS_INVALID_LANGUAGE);
	}

      // update the uri's
      // check for standard namespaces, store them, and update any flags
      option_uri[ns_uri] = ns_prefix;
      if (ns_uri == SRCML_SRC_NS_URI) {

	  // default

      } else if (ns_uri == SRCML_CPP_NS_URI) {

	// specifying the cpp prefix automatically turns on preprocessor
	options |= OPTION_CPP;

      } else if (ns_uri == SRCML_ERR_NS_URI) {

	// specifying the error prefix automatically turns on debugging
	options |= OPTION_DEBUG;

      } else if (ns_uri == SRCML_EXT_LITERAL_NS_URI) {

	// specifying the literal prefix automatically turns on literal markup
	options |= OPTION_LITERAL;

      } else if (ns_uri == SRCML_EXT_OPERATOR_NS_URI) {

	// specifying the operator prefix automatically turns on operator markup
	options |= OPTION_OPERATOR;

      } else if (ns_uri == SRCML_EXT_MODIFIER_NS_URI) {

	// specifying the operator prefix automatically turns on type modifier markup
	options |= OPTION_MODIFIER;

      } else {
	std::cerr << NAME << ": invalid namespace \"" << ns_uri << "\"\n" << '\n'
		  << "Namespace URI must be on of the following:  \n" << '\n'
		  << '\t' << SRCML_SRC_NS_URI << "\t\t" << "primary srcML namespace\n"
		  << '\t' << SRCML_CPP_NS_URI << "\t\t" << "namespace for cpreprocessing elements\n"
		  << '\t' << SRCML_ERR_NS_URI << '\t' << "namespace for srcML debugging elements\n" << '\n'
	          << "or an extension namespace:\n" << '\n'
		  << '\t' << SRCML_EXT_LITERAL_NS_URI << '\t' << "optional literal elements\n"
		  << '\t' << SRCML_EXT_OPERATOR_NS_URI << '\t' << "optional operator element\n"
		  << '\t' << SRCML_EXT_MODIFIER_NS_URI << '\t' << "optional modifier element\n" << '\n';
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
	std::cerr << NAME << ": encoding selected but not specified.\n";
	exit(STATUS_XMLENCODING_MISSING);
      } else {

	// extract parameter
	xml_encoding = argv[(++curarg)++];
      }

#ifdef LIBXML_ENABLED
      // validate xml encoding
      if (!srcMLOutput::checkEncoding(xml_encoding)) {
	std::cerr << NAME << ": xml encoding \"" << xml_encoding << "\" is not supported.\n";
	exit(STATUS_UNKNOWN_ENCODING);
      }
#endif
    }

    // text encoding
    else if (compare_flags(argv[curarg], TEXTENCODING_FLAG, TEXTENCODING_FLAG_SHORT)) {

#ifdef LIBXML_ENABLED
      options |= OPTION_TEXT_ENCODING;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	src_encoding = embedded + 1;
	++curarg;

      // check for text encoding flag with missing text encoding
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": text encoding selected but not specified.\n";
	exit(STATUS_SRCENCODING_MISSING);
      } else {

	// extract parameter
	src_encoding = argv[(++curarg)++];
      }

      // validate source encoding
      if (!srcMLOutput::checkEncoding(src_encoding)) {
	std::cerr << NAME << ": text encoding \"" << src_encoding << "\" is not supported.\n";
	exit(STATUS_UNKNOWN_ENCODING);
      }
#else
      std::cerr << NAME << ": The source encoding option, i.e., " << TEXTENCODING_FLAG << ", is only supported in the libxml version.\n";
      exit(STATUS_LIBXML2_FEATURE);
#endif
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
	std::cerr << NAME << ": invalid option -- Directory must be specified.\n";
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
	std::cerr << NAME << ": invalid option -- Filename must be specified.\n";
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
	std::cerr << NAME << ": invalid option -- Version must be specified.\n";
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

      std::cerr << NAME << ": unrecognized option '" << argv[curarg] << "'\n";
      std::cerr << "try '" << NAME << " " << HELP_FLAG << "' for more information." << "\n";
      exit(STATUS_UNKNOWN_OPTION);
    }
  }

  return curarg;
}

// filename part of path
char* split_path(char* path) {

  char* lastslash = 0;
  char* cur = path;
  while (*cur) {
    if (*cur == '/')
      lastslash = cur;
    ++cur;
  }

  if (lastslash) {
    *lastslash = '\0';
    return lastslash + 1;
  }

  return cur;
}

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
