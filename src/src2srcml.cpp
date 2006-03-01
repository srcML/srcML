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
#include "version.h"
#include "srcmlapps.h"
#include "Options.h"

#include "srcMLTranslator.h"

const char* NAME = "src2srcml";

const char* DEBUG_FLAG = "--debug";
const char* DEBUG_FLAG_SHORT = "-g";

const char* EXTENDED_FLAG = "--extended";
const char* EXTENDED_FLAG_SHORT = "-t";

const char* COMPRESSED_FLAG = "--compressed";
const char* COMPRESSED_FLAG_SHORT = "-z";

const char* INTERACTIVE_FLAG = "--interactive";
const char* INTERACTIVE_FLAG_SHORT = "-c";

#ifdef LIBXML_ENABLED
const char* DEFAULT_XML_ENCODING = "UTF-8";
#else
const char* DEFAULT_XML_ENCODING = "ISO-8859-1";
#endif

const char* FILELIST_FLAG = "--input-file";
const char* FILELIST_FLAG_SHORT = "-i";

const int DEFAULT_LANGUAGE = srcMLTranslator::LANGUAGE_CXX;

const char* EXAMPLE_TEXT_FILENAME="foo.cpp";
const char* EXAMPLE_XML_FILENAME="foo.cpp.xml";

const char* GAP = "                                  ";

const char FILELIST_COMMENT = '#';

// filename part of path
std::string get_filename(const std::string& path);

// directory part of path
std::string get_directory(const std::string& path);

using std::setw;

// output help
void output_help(const char* name) {
    std::cout << "Usage: " << name << " [options] <infile>... <outfile>" << '\n'
              << '\n'
	      << "Translates source-code files into the XML source-code representation srcML." << '\n'
              << '\n'
	      << "When no filenames are given read from standard input and write to standard output." << '\n'
	      << "When only one filename is given write to standard output."  << '\n'
	      << "An input filename of '-' also reads from standard input." << '\n'
              << '\n'
	      << "Options:"  << '\n'
              << std::left
	      << "  " << HELP_FLAG_SHORT        << ", " << setw(COL) <<  HELP_FLAG
	      << "display this help and exit\n"
	      << "  " << VERSION_FLAG_SHORT     << ", " << setw(COL) <<  VERSION_FLAG
	      << "display version number and exit\n"
              << '\n'
	      << "  " << LANGUAGE_FLAG_SHORT    << ", " << setw(COL) << LANGUAGE_FLAG_FULL 
	      << "set the language to " << LANGUAGE_C << ", " << LANGUAGE_CXX << ", or " << LANGUAGE_JAVA << "\n"
	      << "  " << DIRECTORY_FLAG_SHORT   << ", " << setw(COL) <<  DIRECTORY_FLAG_FULL
	      << "set the directory attribute to DIR\n"
	      << "  " << FILENAME_FLAG_SHORT    << ", " << setw(COL) <<  FILENAME_FLAG_FULL
	      << "set the filename attribute to FILE\n"
	      << "  " << SRCVERSION_FLAG_SHORT  << ", " << setw(COL) <<  SRCVERSION_FLAG_FULL
	      << "set the version attribute to VER\n"
	      << '\n'
#ifdef LIBXML_ENABLED
	      << "  " << TEXTENCODING_FLAG_SHORT        << ", " << setw(COL) <<  TEXTENCODING_FLAG_FULL
	      << "set the input source encoding to ENC (default:  based on locale" << ") \n"
	      << "  " << SKIP_ENCODING_FLAG_SHORT        << ", " << setw(COL) <<  SKIP_ENCODING_FLAG
	      << "skip any text encoding transformation" << "\n"
#endif
	      << "  " << ENCODING_FLAG_SHORT    << ", " << setw(COL) <<  ENCODING_FLAG_FULL
	      << "set the output XML encoding to ENC (default:  "
	      << DEFAULT_XML_ENCODING << ") \n"
              << '\n'
	      << "  " << NESTED_FLAG_SHORT      << ", " << setw(COL) <<  NESTED_FLAG
	      << "store all input source files in one compound srcML document\n"
	      << "  " << FILELIST_FLAG_SHORT    << ", " << setw(COL) <<  FILELIST_FLAG
	      << "treat input file as a list of source files\n"
	      << '\n'
#ifdef LIBXML_ENABLED
	      << "  " << COMPRESSED_FLAG_SHORT  << ", " << setw(COL) <<  COMPRESSED_FLAG
	      << "output in gzip format\n"
#endif
	      << "  " << INTERACTIVE_FLAG_SHORT << ", " << setw(COL) <<  INTERACTIVE_FLAG
	      << "low-latency output\n"
	      << "  " << DEBUG_FLAG_SHORT       << ", " << setw(COL) <<  DEBUG_FLAG
	      << "debug translation errors with special namespace\n"
              << "  " << VERBOSE_FLAG_SHORT     << ", " << setw(COL) << VERBOSE_FLAG
	      << "verbose output to standard error\n"
	      << '\n'
	      << "Examples:  " << '\n'
	      << "  " << name << "                      " << GAP
	      << "(read from standard input, write to standard output)" << '\n'
	      << "  " << name << " main.cpp             " << GAP
	      << "(read from file main.cpp, write to standard output)" << '\n'
	      << "  " << name << " main.cpp main.cpp.xml" << GAP
	      << "(read from file main.cpp, write to file main.cpp.xml)" << '\n'
	      << "  " << name << " -                    " << GAP
	      << "(read from standard input, write to standard output)" << '\n'
	      << "  " << name << " - main.cpp.xml       " << GAP
	      << "(read from standard input, write to file main.cpp.xml)" << '\n'
	      << "  " << name << " --directory=src --filename=main.cpp - main.cpp.xml     "
	      << "(element unit attributes dir \"src\", filename \"main.cpp\")" << '\n'
#ifdef LIBXML_ENABLED
	      << "  " << name << " --src-encoding=UTF-8 main.cpp main.cpp.xml             "
	      << "(encoding of input text file is UTF-8)" << '\n'
#endif
	      << "  " << name << " --xml-encoding=ISO-8859-1 main.cpp main.cpp.xml        "
	      << "(set encoding of srcML file to ISO-8859-1)" << '\n'

	      << '\n'
	      << "www.sdml.info" << '\n'
	      << "Report bugs to " << EMAIL_ADDRESS << '\n';
}

// output version message
void output_version(const char* name) {
    std::cout << name << " Version " << VERSION << '\n'
	      << COPYRIGHT << '\n';
}

int main(int argc, char* argv[]) {

  int exit_status = EXIT_SUCCESS;

  const char* src_encoding = DEFAULT_TEXT_ENCODING;
  int language = DEFAULT_LANGUAGE;
  const char* xml_encoding = DEFAULT_XML_ENCODING;
  const char* given_directory = "";
  const char* given_filename = "";
  const char* given_version = "";

  // process all command line options
  int options = 0;
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

    // extended mode
    else if (compare_flags(argv[curarg], EXTENDED_FLAG, EXTENDED_FLAG_SHORT, position)) {
      options |= OPTION_EXTENDED;
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
      std::cerr << NAME << ": The compression option, i.e., " << COMPRESSED_FLAG << ", is only supported in the libxml version." << '\n';
      exit(STATUS_LIBXML2_FEATURE);
#endif
    }

    // skip encoding mode
    else if (compare_flags(argv[curarg], SKIP_ENCODING_FLAG, SKIP_ENCODING_FLAG_SHORT, position)) {
      options |= OPTION_SKIP_ENCODING;
      if (position == original_position) ++curarg;
    }

    // filelist mode
    else if (compare_flags(argv[curarg], FILELIST_FLAG, FILELIST_FLAG_SHORT, position)) {
      options |= OPTION_FILELIST;
      if (position == original_position) ++curarg;

      // filelist mode is default nested mode
      options |= OPTION_NESTED;
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
	std::cerr << NAME << ": language option selected but not specified." << '\n';
	exit(STATUS_LANGUAGE_MISSING);
      } else {

	// extract parameter
	langparam = argv[(++curarg)++];
      }

      // validate language selected
      if (strcmp(langparam, LANGUAGE_C) == 0)
	language = srcMLTranslator::LANGUAGE_C;
      else if (strcmp(langparam, LANGUAGE_CXX) == 0)
	language = srcMLTranslator::LANGUAGE_CXX;
      else if (strcmp(langparam, LANGUAGE_JAVA) == 0)
	language = srcMLTranslator::LANGUAGE_JAVA;
      else {
	std::cerr << NAME << ": invalid option -- Language flag must one of the following values:  "
		  << LANGUAGE_C << " " << LANGUAGE_CXX << " " << LANGUAGE_JAVA << '\n';
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
	std::cerr << NAME << ": encoding selected but not specified." << '\n';
	exit(STATUS_XMLENCODING_MISSING);
      } else {

	// extract parameter
	xml_encoding = argv[(++curarg)++];
      }

#ifdef LIBXML_ENABLED
      // validate xml encoding
      if (!srcMLOutput::checkEncoding(xml_encoding)) {
	std::cerr << NAME << ": xml encoding \"" << xml_encoding << "\" is not supported." << '\n';
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
	std::cerr << NAME << ": text encoding selected but not specified." << '\n';
	exit(STATUS_SRCENCODING_MISSING);
      } else {

	// extract parameter
	src_encoding = argv[(++curarg)++];
      }

      // validate source encoding
      if (!srcMLOutput::checkEncoding(src_encoding)) {
	std::cerr << NAME << ": text encoding \"" << src_encoding << "\" is not supported." << '\n';
	exit(STATUS_UNKNOWN_ENCODING);
      }
#else
      std::cerr << NAME << ": The source encoding option, i.e., " << TEXTENCODING_FLAG << ", is only supported in the libxml version." << '\n';
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

  /* Special checks for illegal combinations */

  // skip encoding and specifying encoding
  if (isoption(options, OPTION_SKIP_ENCODING) && isoption(options, OPTION_TEXT_ENCODING)) {

    std::cerr << NAME << ": Options for skipping encoding and specifying source encoding are incompatible.\n";
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

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
  char* srcml_filename = "-";
  if ((argc - (curarg - 1)) > 1) {
    srcml_filename = argv[curarg];

    ++curarg;
  }

  // verify that the output filename is not the same as any of the input filenames
  for (int i = input_arg_start; i <= input_arg_end; ++i) {

      if (strcmp(srcml_filename, argv[i]) == 0) {
	std::cerr << NAME << ": Input file '" << argv[i] << "'"
		  << " is the same as the output file '" << srcml_filename << "'\n";
	exit(STATUS_INPUTFILE_PROBLEM);
      }
  }

  try {

  // translator from input to output using determined language
  srcMLTranslator translator(language, src_encoding, xml_encoding, srcml_filename, options);

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
    if (input_arg_count > 0) {
      infile.open(argv[input_arg_start]);
      pinfilelist = &infile;
    }

    // translate all the filenames listed in the named file
    std::string line;
    int count = 0;    // keep count for verbose mode
    while (getline(*pinfilelist, line)) {

      // extract the filename from the line
      std::string infilename = line.substr(0, line.find_first_of(' '));

      // skip blank lines or comment lines
      if (infilename == "" || infilename[0] == FILELIST_COMMENT)
	continue;

      // in verbose mode output the currently processed filename
      if (isoption(options, OPTION_VERBOSE)) {
	++count;
	std::cerr << count << '\t' << infilename;
      }

      // translate the file listed in the input file using the directory and filename extracted from the path
      const char* path = infilename.c_str();
      try {
	translator.translate(path, get_directory(path).c_str(), get_filename(path).c_str(), given_version);

      } catch (FileError) {

	if (isoption(options, OPTION_VERBOSE))
	  std::cerr << "\t\terror: file \'" << path << "\' does not exist.";
	else
	  std::cerr << NAME << " error: file \'" << path << "\' does not exist." << "\n";
      }

      if (isoption(options, OPTION_VERBOSE)) {
	std::cerr << '\n';
      }
    }

  // translate from standard input
  } else if (input_arg_count == 0 || strcmp(argv[input_arg_start], STDIN) == 0) {

    // translate from standard input using any directory, filename and version given on the command line
    translator.translate(STDIN, given_directory, given_filename, given_version);

  // translate single input filename from command line
  }  else if (input_arg_count == 1) {

    // translate from path given on command line using directory given on the command line or extracted
    // from full path
    const char* path = argv[input_arg_start];

    std::string directory = isoption(options, OPTION_DIRECTORY) ? given_directory : get_directory(path);
    std::string filename  = isoption(options, OPTION_FILENAME)  ? given_filename  : get_filename(path);

    try {
      translator.translate(path, directory.c_str(), filename.c_str(), given_version);

    } catch (FileError) {

      std::cerr << NAME << " error: file \'" << path << "\' does not exist." << "\n";
      exit(STATUS_INPUTFILE_PROBLEM);
    }

  // translate multiple input filenames on command line
  } else {

    int count = 0;    // keep count for verbose mode

    // translate in batch the input files on the command line extracting the directory and filename attributes
    // from the full path
    for (int i = input_arg_start; i <= input_arg_end; ++i) {
      const char* path = argv[i];
      std::string sdirectory = get_directory(path);
      std::string sfilename = get_filename(path);

      // in verbose mode output the currently processed filename
      if (isoption(options, OPTION_VERBOSE)) {
	++count;
	std::cerr << count << '\t' << path;
      }
      try {
	translator.translate(path, sdirectory.c_str(), sfilename.c_str());
      } catch (FileError) {
	std::cerr << NAME << " error: file \'" << path << "\' does not exist." << "\n";
      }

      if (isoption(options, OPTION_VERBOSE)) {
	std::cerr << '\n';
      }
    }
  }
  } catch (srcEncodingException) {
    std::cerr << "Translation encoding problem" << '\n';
    exit(STATUS_UNKNOWN_ENCODING);
  }

  return exit_status;
}

// filename part of path
std::string get_filename(const std::string& path) {

  // filename is only part of the path
  size_t posslash = path.rfind('/');
  if (posslash != std::string::npos)
    return path.substr(posslash + 1);

  // filename is full path
  return path;
}

// directory part of path
std::string get_directory(const std::string& path) {

  // directory in path
  size_t posslash = path.rfind('/');
  if (posslash != std::string::npos)
    return path.substr(0, posslash);

  // no directory in path
  return "";
}
