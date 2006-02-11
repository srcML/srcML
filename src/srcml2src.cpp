/*
  srcml2src.cpp

  Copyright (C) 2004-2006  SDML (www.sdml.info)

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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "version.h"
#include "srcmlapps.h"
#include "project.h"
#include "srcMLUtility.h"
#include "Options.h"

const char* NAME = "srcml2src";

const char* EXTRACT_FLAG = "--extract";
const char* EXTRACT_FLAG_SHORT = "-x";

const char* EXPAND_FLAG = "--extract-all";
const char* EXPAND_FLAG_SHORT = "-e";

const char* UNIT_FLAG = "--unit";
const char* UNIT_FLAG_SHORT = "-U";
const char* UNIT_FLAG_FULL = "--unit=NUM";

const char* XML_FLAG = "--xml";
const char* XML_FLAG_SHORT = "-X";

const char* INFO_FLAG = "--info";
const char* INFO_FLAG_SHORT = "-o";

const char* LONG_INFO_FLAG = "--longinfo";
const char* LONG_INFO_FLAG_SHORT = "-l";

using std::setw;

// output help message
void output_help(const char* name) {

     std::cout << "Usage: " << name << " [options] <infile> <outfile>" << '\n'
	       << "Translates program files from the the XML source-code representation srcML to text files." << '\n'
	       << "The srcML files can be in xml or gzip compressed xml (detected automatically)." << '\n'
	       << "Also provides various utilities for working with srcML documents." << '\n'
               << '\n'
	       << "When no filenames are given read from standard input and write to standard output." << '\n'
	       << "When only one filename is given write to standard output."  << '\n'
	       << "An input filename of '-' also reads from standard input." << '\n'
               << '\n'
	       << "Options:"  << '\n'
               << std::left
	       << "  " << HELP_FLAG_SHORT       << ", " << setw(COL) << HELP_FLAG       
	       << "display this help and exit\n"
	       << "  " << VERSION_FLAG_SHORT    << ", " << setw(COL) << VERSION_FLAG    
	       << "display version number and exit\n"
	       << '\n'
	       << "  " << LANGUAGE_FLAG_SHORT   << ", " << setw(COL) << LANGUAGE_FLAG
	       << "display source language (e.g., " << LANGUAGE_C << ", " << LANGUAGE_CXX << ", "
	       << LANGUAGE_JAVA << ") and exit\n"
	       << "  " << DIRECTORY_FLAG_SHORT  << ", " << setw(COL) << DIRECTORY_FLAG
	       << "display source directory name and exit\n"
	       << "  " << FILENAME_FLAG_SHORT   << ", " << setw(COL) << FILENAME_FLAG   
	       << "display source filename and exit\n"
	       << "  " << SRCVERSION_FLAG_SHORT << ", " << setw(COL) << SRCVERSION_FLAG
	       << "display source version and exit\n"
	       << "  " << ENCODING_FLAG_SHORT   << ", " << setw(COL) << ENCODING_FLAG
	       << "display xml encoding and exit\n"
	       << '\n'
	       << "  " << NESTED_FLAG_SHORT     << ", " << setw(COL) << NESTED_FLAG     
	       << "display number of nested units and exit\n"
	       << "  " << UNIT_FLAG_SHORT       << ", " << setw(COL) << UNIT_FLAG_FULL  
	       << "process nested unit NUM from a compound srcML document\n"
	       << "  " << XML_FLAG_SHORT        << ", " << setw(COL) << XML_FLAG        
	       << "output is in XML instead of text\n"
	       << "  " << EXPAND_FLAG_SHORT     << ", " << setw(COL) << EXPAND_FLAG     
	       << "extract all files from a compound srcML document\n"
	       << '\n'
#ifdef LIBXML_ENABLED
	       << "  " << TEXTENCODING_FLAG_SHORT << ", " << setw(COL) <<  TEXTENCODING_FLAG_FULL  
	       << "set the output source encoding to ENC (default:  " << "based on locale" << ") \n"
	       << '\n'
#endif
               << "  " << VERBOSE_FLAG_SHORT    << ", " << setw(COL) << VERBOSE_FLAG    << "verbose output\n"
	       << '\n'
	       << "Examples:  " << '\n'
	       << "  " << name << "                       "
	       << "(read from standard input, write to standard output)" << '\n'

	       << "  " << name << " main.cpp.xml          "
	       << "(read from file main.cpp.xml, write to standard output)" << '\n'

	       << "  " << name << " main.cpp.xml main.cpp "
	       << "(read from file main.cpp.xml, write to file main.cpp)" << '\n'

	       << "  " << name << " -                     "
	       << "(read from standard input, write to standard output)" << '\n'

	       << "  " << name << " - main.cpp            "
	       << "(read from standard input, write to file main.cpp)" << '\n'

	       << "  " << name << " --language            "
	       << "(read from standard input, output language attribute)" << '\n'

	       << "  " << name << " --directory           "
	       << "(read from standard input, output directory attribute)" << '\n'

	       << "  " << name << " --filename            "
	       << "(read from standard input, output filename attribute)" << '\n'

	       << "  " << name << " --src-version         "
	       << "(read from standard input, output version attribute)" << '\n'

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

  // option values
  const char* src_encoding = DEFAULT_TEXT_ENCODING;
  int unit = 0;

  // process all flags
  int position = 0;
  int options = 0;
  int curarg = 1;
  while (argc > curarg && strlen(argv[curarg]) > 1 && argv[curarg][0] == '-' && strcmp(argv[curarg], OPTION_SEPARATOR) != 0) {

    // mark to detect changes
    int original_position = position;

    // help flag
    if (compare_flags(argv[1], HELP_FLAG, HELP_FLAG_SHORT, position)) {
      options |= OPTION_HELP;
      if (position == original_position) ++curarg;
    }

    // version flag
    else if (compare_flags(argv[1], VERSION_FLAG, VERSION_FLAG_SHORT, position)) {
      options |= OPTION_PVERSION;
      if (position == original_position) ++curarg;
    }

    // filename flag
    else if (compare_flags(argv[curarg], FILENAME_FLAG, FILENAME_FLAG_SHORT, position)) {
      options |= OPTION_FILENAME;
      if (position == original_position) ++curarg;
    }

    // dir flag
    else if (compare_flags(argv[curarg], DIRECTORY_FLAG, DIRECTORY_FLAG_SHORT, position)) {
      options |= OPTION_DIRECTORY;
      if (position == original_position) ++curarg;
    }

    // language flag
    else if (compare_flags(argv[curarg], LANGUAGE_FLAG, LANGUAGE_FLAG_SHORT, position)) {
      options |= OPTION_LANGUAGE;
      if (position == original_position) ++curarg;
    }

    // version flag
    else if (compare_flags(argv[curarg], SRCVERSION_FLAG, SRCVERSION_FLAG_SHORT, position)) {
      options |= OPTION_VERSION;
      if (position == original_position) ++curarg;
    }

    // nested flag
    else if (compare_flags(argv[curarg], NESTED_FLAG, NESTED_FLAG_SHORT, position)) {
      options |= OPTION_NESTED;
      if (position == original_position) ++curarg;
    }

    // encoding flag
    else if (compare_flags(argv[curarg], ENCODING_FLAG, ENCODING_FLAG_SHORT, position)) {
      options |= OPTION_XML_ENCODING;
      if (position == original_position) ++curarg;
    }

    // expand flag
    else if (compare_flags(argv[curarg], EXPAND_FLAG, EXPAND_FLAG_SHORT, position)) {
      options |= OPTION_EXPAND;
      if (position == original_position) ++curarg;
    }

    // verbose flag
    else if (compare_flags(argv[curarg], VERBOSE_FLAG, VERBOSE_FLAG_SHORT, position)) {
      options |= OPTION_VERBOSE;
      if (position == original_position) ++curarg;
    }

    // xml output flag
    else if (compare_flags(argv[curarg], XML_FLAG, XML_FLAG_SHORT, position)) {
      options |= OPTION_XML;
      if (position == original_position) ++curarg;
    }

    // info flag
    else if (compare_flags(argv[curarg], INFO_FLAG, INFO_FLAG_SHORT, position)) {
      options |= OPTION_INFO;
      if (position == original_position) ++curarg;
    }

    // long info flag
    else if (compare_flags(argv[curarg], LONG_INFO_FLAG, LONG_INFO_FLAG_SHORT, position)) {
      options |= OPTION_LONG_INFO;
      if (position == original_position) ++curarg;
    }

    // extract unit flag
    else if (compare_flags(argv[curarg], UNIT_FLAG, UNIT_FLAG_SHORT)) {
      options |= OPTION_UNIT;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	unit = pstd::atoi(embedded + 1);
	++curarg;

      // check for unit flag with missing unit number
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": unit option selected but no number specified." << '\n';
	exit(STATUS_UNIT_MISSING);
      } else {
	unit = pstd::atoi(argv[(++curarg)++]);
      }

      // validate unit number
      if (unit < 0) {
	std::cerr << NAME << ": unit option value \"" << unit << "\" must be >= 0." << '\n';
	exit(STATUS_UNIT_INVALID);
      }
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
      if (!srcMLUtility::checkEncoding(src_encoding)) {
	std::cerr << NAME << ": text encoding \"" << src_encoding << "\" is not supported." << '\n';
	exit(STATUS_UNKNOWN_ENCODING);
      }
#else
      std::cerr << NAME << ": The source encoding option, i.e., " << TEXTENCODING_FLAG << ", is only supported in the libxml version." << '\n';
      exit(STATUS_LIBXML2_FEATURE);
#endif

    // reached the end of a multi-short form option
    } else if (position > 0 && argv[curarg][position + 1] == '\0') {

      ++curarg;

    // invalid option
    } else {

      std::cerr << NAME << ": unrecognized option '" << argv[curarg] << "'\n";
      std::cerr << "try '" << NAME << " " << HELP_FLAG << "' for more information." << "\n";
      exit(STATUS_UNKNOWN_OPTION);
    }
  }

  // eat optional option separator
  if (argc > curarg && strcmp(argv[curarg], OPTION_SEPARATOR) == 0)
      ++curarg;

  // first command line parameter is input filename
  char* filename = "-";
  if (argc > curarg && !compare_flags(argv[curarg], STDIN, STDIN)) {
    filename = argv[curarg];
    ++curarg;
  }

  // second command line parameter is output filename
  //  std::ofstream fout;
  char* ofilename = "-";
  if (argc == curarg + 1) {
    ofilename = argv[curarg];
    ++curarg;

  } else if (argc > curarg + 1) {

      std::cerr << NAME << ": More than one output file specified.\n";
      std::cerr << "try '" << NAME << " --help' for more information." << "\n";
      exit(0);
  }

  // help flag
  if (isoption(options, OPTION_HELP)) {
    output_help(NAME);
    exit(STATUS_SUCCESS);
  }

  // version flag
  if (isoption(options, OPTION_PVERSION)) {
    output_version(NAME);
    exit(STATUS_SUCCESS);
  }

  /* Special checks for illegal combinations */

  // xml output and src-encoding (switch to encoding?)
  if (isoption(options, OPTION_XML) && isoption(options, OPTION_TEXT_ENCODING)) {

    std::cerr << NAME << ": Options for xml output and specifying source encoding are incompatible.\n";
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

  try {

    // setup for processing
    srcMLUtility su(filename, src_encoding, options);

    // process the option
    if (isoption(options, OPTION_FILENAME) && isoption(options, OPTION_UNIT)) {

      try {
	bool nonnull;
	std::string l = su.unit_attribute(unit, "filename", nonnull);
	if (nonnull)
	  std::cout << l << '\n';

      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }

    } else if (isoption(options, OPTION_FILENAME)) {

      bool nonnull;
      std::string l = su.attribute("filename", nonnull);
      if (nonnull)
	std::cout << l << '\n';

    } else if (isoption(options, OPTION_DIRECTORY) && isoption(options, OPTION_UNIT)) {

      try {

	bool nonnull;
	std::string l = su.unit_attribute(unit, "dir", nonnull);
	if (nonnull)
	  std::cout << l << '\n';

      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }

    } else if (isoption(options, OPTION_DIRECTORY)) {

	bool nonnull;
	std::string l = su.attribute("dir", nonnull);
	if (nonnull)
	  std::cout << l << '\n';

    } else if (isoption(options, OPTION_LANGUAGE) && isoption(options, OPTION_UNIT)) {

      try {

	bool nonnull;
	std::string l = su.unit_attribute(unit, "language", nonnull);
	if (nonnull)
	  std::cout << l << '\n';

      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }

    } else if (isoption(options, OPTION_INFO) || isoption(options, OPTION_LONG_INFO)) {

      if (isoption(options, OPTION_UNIT)) {
      try {

	su.move_to_unit(unit);

      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }
      }

      bool nonnull;
      std::cout << "Encoding: " << su.getencoding() << '\n';

      std::string language = su.attribute("language", nonnull);
      if (nonnull)
	std::cout << "Language: " << language << '\n';

      std::string directory = su.attribute("dir", nonnull);
      if (nonnull)
	std::cout << "Directory: " << directory << '\n';

      std::string filename = su.attribute("filename", nonnull);
      if (nonnull)
	std::cout << "Filename: " << filename << '\n';

      std::string version = su.attribute("version", nonnull);
      if (nonnull)
	std::cout << "Version: " << version << '\n';

      if (isoption(options, OPTION_LONG_INFO)) {
	if (!isoption(options, OPTION_UNIT))
	  std::cout << "Nested: " << su.unit_count() << '\n';
      }

    } else if (isoption(options, OPTION_LANGUAGE)) {

      bool nonnull;
      std::string l = su.attribute("language", nonnull);
      if (nonnull)
	std::cout << l << '\n';

    } else if (isoption(options, OPTION_VERSION) && isoption(options, OPTION_UNIT)) {

      try {

	su.move_to_unit(unit);

	bool nonnull;
	std::string l = su.attribute("version", nonnull);
	if (nonnull)
	  std::cout << l << '\n';

      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }

    } else if (isoption(options, OPTION_VERSION)) {

      bool nonnull;
      std::string l = su.attribute("version", nonnull);
      if (nonnull)
	std::cout << l << '\n';

    } else if (isoption(options, OPTION_NESTED)) {

      std::cout << su.unit_count() << '\n';

    } else if (isoption(options, OPTION_XML_ENCODING)) {

      std::cout << su.getencoding() << '\n';

    } else if (isoption(options, OPTION_UNIT) && isoption(options, OPTION_XML)) {

      try {
	su.extract_xml(ofilename, unit);
      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }

    } else if (isoption(options, OPTION_UNIT)) {

      try {
	su.extract_text(ofilename, unit);
      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }

    } else if (isoption(options, OPTION_EXPAND)) {

      su.expand();

    } else {

      try {
	su.translate(ofilename);
      } catch (TranslateCompoundError) {
	throw "Attempting to translate entire compound srcML file without specifying nested unit";
      }

    }
  } catch (LibXMLError error) {

    if (error.getErrorNum() == 0) {
      exit_status = STATUS_INPUTFILE_PROBLEM;
      std::cerr << NAME << ": " << "Unable to open input file as XML" << "\n";
    }

  } catch (const char* s) {
    
    std::cerr << NAME << ": " << s << "\n";
    if (!exit_status)
      exit_status = STATUS_ERROR;
  } catch (...) {
    if (!exit_status)
      exit_status = STATUS_ERROR;
  }

  return exit_status;
}
