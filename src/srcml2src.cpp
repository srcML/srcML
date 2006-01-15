/*
  srcml2src.cpp

  Copyright (C) 2004, 2005  SDML (www.sdml.info)

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

const char* ATTRIBUTE_FILENAME_FLAG = "--attributefilename";
const char* ATTRIBUTE_FILENAME_FLAG_SHORT = "-a";

using std::setw;

// output help message
void output_help(const char* name) {

     std::cout << "Usage: " << name << " [options] <infile> <outfile>" << '\n'
	       << "Translates program files from the the XML source-code representation srcML to text files." << '\n'
	       << "The srcML files can be in plain text, or compressed with gzip (detected automatically)." << '\n'
	       << "Also provides various utilities for working with srcML documents." << '\n'
               << '\n'
	       << "When no filenames are given read from standard input." << '\n'
	       << "When only one filename is given write to standard output."  << '\n'
	       << "An input filename of '-' also reads from standard input." << '\n'
               << '\n'
	       << "Options:"  << '\n'
               << std::left
	       << "  " << HELP_FLAG_SHORT       << ", " << setw(COL) << HELP_FLAG       << "display this help and exit\n"
	       << "  " << VERSION_FLAG_SHORT    << ", " << setw(COL) << VERSION_FLAG    << "display version number and exit\n"
	       << '\n'
	       << "  " << LANGUAGE_FLAG_SHORT   << ", " << setw(COL) << LANGUAGE_FLAG
	       << "display source language (e.g., " << LANGUAGE_C << ", " << LANGUAGE_CXX << ", " << LANGUAGE_JAVA << ") and exit\n"
	       << "  " << DIRECTORY_FLAG_SHORT  << ", " << setw(COL) << DIRECTORY_FLAG  << "display source directory name and exit\n"
	       << "  " << FILENAME_FLAG_SHORT   << ", " << setw(COL) << FILENAME_FLAG   << "display source filename and exit\n"
	       << "  " << SRCVERSION_FLAG_SHORT << ", " << setw(COL) << SRCVERSION_FLAG << "display source version and exit\n"
	       << "  " << ENCODING_FLAG_SHORT   << ", " << setw(COL) << ENCODING_FLAG   << "display xml encoding and exit\n"
	       << '\n'
	       << "  " << NESTED_FLAG_SHORT     << ", " << setw(COL) << NESTED_FLAG     << "display number of nested units and exit\n"
	       << "  " << UNIT_FLAG_SHORT       << ", " << setw(COL) << UNIT_FLAG_FULL  << "process nested unit NUM from a compound srcML document\n"
	       << "  " << XML_FLAG_SHORT        << ", " << setw(COL) << XML_FLAG        << "output is in XML instead of text\n"
	       << "  " << EXPAND_FLAG_SHORT     << ", " << setw(COL) << EXPAND_FLAG     << "extract all files from a compound srcML document\n"
	       << '\n'
	       << "  " << TEXTENCODING_FLAG_SHORT << ", " << setw(COL) <<  TEXTENCODING_FLAG_FULL  << "set the output source encoding to ENC (default:  "
	      << "based on locale" << ") \n"
	       << '\n'
               << "  " << VERBOSE_FLAG_SHORT    << ", " << setw(COL) << VERBOSE_FLAG    << "verbose output\n"
	       << '\n'
	       << "Examples:  " << '\n'
	       << "  " << name << "                       " << "(read from standard input, write to standard output)" << '\n'
	       << "  " << name << " main.cpp.xml          " << "(read from file main.cpp.xml, write to standard output)" << '\n'
	       << "  " << name << " main.cpp.xml main.cpp " << "(read from file main.cpp.xml, write to file main.cpp)" << '\n'
	       << "  " << name << " -                     " << "(read from standard input, write to standard output)" << '\n'
	       << "  " << name << " - main.cpp            " << "(read from standard input, write to file main.cpp)" << '\n'
	       << "  " << name << " --filename            " << "(read from standard input, output filename)" << '\n'
	       << "  " << name << " --directory           " << "(read from standard input, output directory)" << '\n'
	       << "  " << name << " --language            " << "(read from standard input, output language)" << '\n'
       //	       << "  " << name << " " << ATTRIBUTE_FILENAME_FLAG_SHORT << "                  "
       //	       << "(read from standard input, write to filename given in srcML attributes)" << '\n'
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
  const char* encoding = DEFAULT_TEXT_ENCODING;
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

    // extract unit flag
    else if (compare_flags(argv[curarg], UNIT_FLAG, UNIT_FLAG_SHORT)) {
      options |= OPTION_UNIT;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	unit = pstd::atoi(embedded + 1);
	++curarg;

      // check for unit flag with missing unit number
      } else if (argc <= curarg + 2 || strcmp(argv[curarg], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": unit option selected but no number specified." << '\n';
	exit(0);
      } else {
	++curarg;
	unit = pstd::atoi(argv[curarg]);
	++curarg;
      }

      // validate unit number
      if (unit < 0) {
	std::cerr << NAME << ": unit \"" << unit << "\" is not valid." << '\n';
	exit(0);
      }
    }

    // encoding
    else if (compare_flags(argv[curarg], ENCODING_FLAG, ENCODING_FLAG_SHORT)) {

      options |= OPTION_TEXT_ENCODING;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	encoding = embedded + 1;
	++curarg;

      // check for encoding flag with missing encoding
      } else if (argc <= curarg + 2 || strcmp(argv[curarg], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": encoding selected but not specified." << '\n';
	exit(0);
      } else {
	++curarg;
	encoding = argv[curarg];
	++curarg;
      }

      // validate xml encoding
      if (!srcMLUtility::checkEncoding(encoding)) {
	std::cerr << NAME << ": encoding \"" << encoding << "\" is not supported." << '\n';
	exit(0);
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
	std::cerr << NAME << ": text encoding selected but not specified." << '\n';
	exit(0);
      } else {
	// move to parameter
	++curarg;

	// extract parameter
	src_encoding = argv[curarg];

	// move over parameter
	++curarg;
      }

      // validate source encoding
      if (!srcMLUtility::checkEncoding(src_encoding)) {
	std::cerr << NAME << ": text encoding \"" << src_encoding << "\" is not supported." << '\n';
	exit(0);
      }

    // reached the end of a multi-short form option
    } else if (position > 0 && argv[curarg][position + 1] == '\0') {

      ++curarg;

    // invalid option
    } else {

      std::cerr << NAME << ": unrecognized option '" << argv[curarg] << "'\n";
      std::cerr << "try '" << NAME << " " << HELP_FLAG << "' for more information." << "\n";
      exit(0);
    }
  }

  // eat optional option separator
  if (argc > curarg && strcmp(argv[curarg], OPTION_SEPARATOR) == 0)
      ++curarg;

  // assume stdin and stdout unless filenames on command line
  std::istream* pin = &std::cin;
  std::ostream* pout = &std::cout;

  // first command line parameter is input filename
  std::ifstream fin;
  char* filename = 0;
  if (argc > curarg && !compare_flags(argv[curarg], STDIN, STDIN)) {
    filename = argv[curarg];
    fin.open(argv[curarg]);

    if (!fin) {
      std::cerr << "Error:  Input srcML file \"" << argv[curarg] << "\" does not exist.\n";
      exit(0);
    }

    pin = &fin;

    ++curarg;
  }

  // second command line parameter is output filename
  std::ofstream fout;
  char* ofilename = "-";
  if (argc == curarg + 1) {
    ofilename = argv[curarg];

    // open the file if it is not a src code or xml output
    if (isoption(options, OPTION_FILENAME) 
    || isoption(options, OPTION_DIRECTORY)
    || isoption(options, OPTION_LANGUAGE)
    || isoption(options, OPTION_VERSION)
    || isoption(options, OPTION_NESTED)
    || isoption(options, OPTION_XML_ENCODING)) {

      fout.open(ofilename);
      pout = &fout;
    }

    ++curarg;

  } else if (argc > curarg + 1) {

      std::cerr << NAME << ": More than one output file specified.\n";
      std::cerr << "try '" << NAME << " --help' for more information." << "\n";
      exit(0);
  }

  // help flag
  if (isoption(options, OPTION_HELP)) {
    output_help(NAME);
    exit(0);
  }

  // version flag
  if (isoption(options, OPTION_PVERSION)) {
    output_version(NAME);
    exit(0);
  }

  try {

    // setup for processing
    srcMLUtility su(filename, src_encoding, options);

    // process the option
    if (isoption(options, OPTION_FILENAME) && isoption(options, OPTION_UNIT)) {

      std::string l = su.unit_attribute(unit, "filename");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_FILENAME)) {

      std::string l = su.attribute("filename");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_DIRECTORY) && isoption(options, OPTION_UNIT)) {

      std::string l = su.unit_attribute(unit, "dir");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_DIRECTORY)) {

      std::string l = su.attribute("dir");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_LANGUAGE) && isoption(options, OPTION_UNIT)) {

      std::string l = su.unit_attribute(unit, "language");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_LANGUAGE)) {

      std::string l = su.attribute("language");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_VERSION) && isoption(options, OPTION_UNIT)) {

      std::string l = su.unit_attribute(unit, "version");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_VERSION)) {

      std::string l = su.attribute("version");
      if (l != "")
	*pout << l << '\n';

    } else if (isoption(options, OPTION_NESTED)) {

      (*pout) << su.unit_count() << '\n';

    } else if (isoption(options, OPTION_XML_ENCODING)) {

      (*pout) << su.getencoding() << '\n';

    } else if (isoption(options, OPTION_UNIT) && isoption(options, OPTION_XML)) {

      su.extract_xml(ofilename, unit);

    } else if (isoption(options, OPTION_UNIT)) {

      su.extract_text(ofilename, unit);

    } else if (isoption(options, OPTION_EXPAND)) {

      su.expand();
      /*
    } else if (isoption(options, OPTION_XML)) {

      su.translate_xml(ofilename);

      */
    } else {

      su.translate(ofilename);
    }
  } catch (const char* s) {
    
    std::cerr << NAME << ": " << s << "\n";

  } catch (...) {

  }

  return exit_status;
}
