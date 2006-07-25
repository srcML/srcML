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
#include <sys/stat.h>
#include "version.h"
#include "srcmlapps.h"
#include "srcmlns.h"
#include "project.h"
#include "srcMLUtility.h"
#include "Options.h"

const char* NAME = "srcml2src";

const char* EXPAND_FLAG = "--extract-all";
const char* EXPAND_FLAG_SHORT = "-a";

const char* UNIT_FLAG = "--unit";
const char* UNIT_FLAG_SHORT = "-U";
const char* UNIT_FLAG_FULL = "--unit=NUM";

const char* XML_FLAG = "--xml";
const char* XML_FLAG_SHORT = "-X";

const char* INFO_FLAG = "--info";
const char* INFO_FLAG_SHORT = "-o";

const char* LONG_INFO_FLAG = "--longinfo";
const char* LONG_INFO_FLAG_SHORT = "-l";

const char* NAMESPACE_FLAG = "--prefix";
const char* NAMESPACE_FLAG_SHORT = "-p";
const char* NAMESPACE_FLAG_FULL = "--prefixURI";

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
	       << "  " << NAMESPACE_FLAG_SHORT  << ", " << setw(COL) << NAMESPACE_FLAG_FULL
	       << "display prefix of namespace given by URI and exit\n"
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
	       << "set the output source encoding to ENC (default:  " << DEFAULT_TEXT_ENCODING << ") \n"
 	       << "  " << SKIP_ENCODING_FLAG_SHORT        << ", " << setw(COL) <<  SKIP_ENCODING_FLAG
	       << "skip any text encoding transformation" << "\n"
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

int options = 0;

extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);

int optionorder[5];
int optioncount = 0;

int main(int argc, char* argv[]) {

  /* signal handling */

  // toggling verbose flag
  pstd::signal(SIGUSR1, verbose_handler);

  int exit_status = EXIT_SUCCESS;

  // option values
  const char* src_encoding = DEFAULT_TEXT_ENCODING;
  int unit = 0;
  std::vector<std::string> ns;

  // process all flags
  int position = 0;
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
      optionorder[optioncount++] = OPTION_FILENAME;
    }

    // dir flag
    else if (compare_flags(argv[curarg], DIRECTORY_FLAG, DIRECTORY_FLAG_SHORT, position)) {
      options |= OPTION_DIRECTORY;
      if (position == original_position) ++curarg;
      optionorder[optioncount++] = OPTION_DIRECTORY;
    }

    // language flag
    else if (compare_flags(argv[curarg], LANGUAGE_FLAG, LANGUAGE_FLAG_SHORT, position)) {
      options |= OPTION_LANGUAGE;
      if (position == original_position) ++curarg;
      optionorder[optioncount++] = OPTION_LANGUAGE;
    }

    // version flag
    else if (compare_flags(argv[curarg], SRCVERSION_FLAG, SRCVERSION_FLAG_SHORT, position)) {
      options |= OPTION_VERSION;
      if (position == original_position) ++curarg;
      optionorder[optioncount++] = OPTION_VERSION;
    }

    // encoding flag
    else if (compare_flags(argv[curarg], ENCODING_FLAG, ENCODING_FLAG_SHORT, position)) {
      options |= OPTION_XML_ENCODING;
      if (position == original_position) ++curarg;
      optionorder[optioncount++] = OPTION_XML_ENCODING;
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

    // nested flag
    else if (compare_flags(argv[curarg], NESTED_FLAG, NESTED_FLAG_SHORT, position)) {
      options |= OPTION_NESTED;
      if (position == original_position) ++curarg;
    }

    // skip encoding mode
    else if (compare_flags(argv[curarg], SKIP_ENCODING_FLAG, SKIP_ENCODING_FLAG_SHORT, position)) {
      options |= OPTION_SKIP_ENCODING;
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

    // namespace
    else if (compare_flags(argv[curarg], NAMESPACE_FLAG, NAMESPACE_FLAG_SHORT)) {
      options |= OPTION_NAMESPACE;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	ns.push_back(embedded + 1);
	++curarg;

      // check for namespace flag with missing namespace
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": namespace option selected but no namespace specified." << '\n';
	exit(STATUS_UNIT_MISSING);
      } else {
	ns.push_back(argv[(++curarg)++]);
      }
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

  // skip encoding and specifying encoding
  if (isoption(options, OPTION_SKIP_ENCODING) && isoption(options, OPTION_TEXT_ENCODING)) {

    std::cerr << NAME << ": Options for skipping encoding and specifying source encoding are incompatible.\n";
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }

  // verify that the output file is not the same as the input file
  struct stat instat;
  stat(filename, &instat);
  struct stat outstat;
  stat(ofilename, &outstat);
  if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
    std::cerr << NAME << ": Input file '" << filename << "'"
	      << " is the same as the output file '" << ofilename << "'\n";
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  try {

    // setup for processing
    srcMLUtility su(filename, src_encoding, options);

    // move to the appropriate unit
    if (isoption(options, OPTION_UNIT)) {

      try {

	su.move_to_unit(unit);

      } catch (LibXMLError) {
	exit_status = STATUS_UNIT_INVALID;
	throw "unit selected is out of range for this compound srcML document";
      }
    }

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

    // process get attribute options
    if (optioncount > 0) {

      const std::map<std::string, std::string> ns = su.getNS();

      for(std::map<std::string, std::string>::const_iterator iter = ns.begin(); iter != ns.end(); iter++) {
	std::string uri = (*iter).first;
	std::string prefix = (*iter).second;

	std::cout << prefix << "=\"" << uri << "\"" << std::endl;
      }

      // output get attributes in order
      for (int i = 0; i < optioncount; ++i) {

	// find attribute name from option
	char* attribute_name = "";
	char* attribute_title = "";
	int option = optionorder[i];
	if (option == OPTION_XML_ENCODING) {
	  attribute_name = "";
	  attribute_title = "Encoding:";
	} else if (option == OPTION_LANGUAGE) {
	  attribute_name = "language";
	  attribute_title = "Language:";
	} else if (option == OPTION_DIRECTORY) {
	  attribute_name = "dir";
	  attribute_title = "Directory:";
	} else if (option == OPTION_FILENAME) {
	  attribute_name = "filename";
	  attribute_title = "Filename:";
	} else if (option == OPTION_VERSION) {
	  attribute_name = "version";
	  attribute_title = "Version:";
	}

	// output the option
	bool nonnull = true;
	std::string l = option != OPTION_XML_ENCODING ? su.attribute(attribute_name, nonnull) : su.getencoding();
	if (nonnull) {
	  if (optioncount > 1)
	    std::cout << attribute_title << " ";
	  std::cout << l << '\n';
	}
      }

      if (isoption(options, OPTION_LONG_INFO)) {
	if (!isoption(options, OPTION_UNIT))
	  std::cout << "Nested: " << su.unit_count() << '\n';
      }

    // namespace
    } else if (isoption(options, OPTION_NAMESPACE)) {

	bool nonnull = true;
	std::string l = su.namespace_ext(ns[0], nonnull);
	if (nonnull) {
	  //	  if (optioncount > 1)
	  //	    std::cout << attribute_title << " ";
	  std::cout << l << '\n';
	}

    // process non-attribute options
    } else if (isoption(options, OPTION_NESTED)) {

      try {

	// gracefully finish current file in compound document mode
	pstd::signal(SIGINT, terminate_handler);

	std::cout << su.unit_count() << '\n';

	// if we terminated early, output the correct status
        if (isoption(options, OPTION_TERMINATE))

	  exit_status = STATUS_TERMINATED;

      } catch (LibXMLError error) {

	if (error.getErrorNum() != 0)
	  throw error;
      }

    } else if (isoption(options, OPTION_UNIT) && isoption(options, OPTION_XML)) {

      su.extract_xml(ofilename);

    } else if (isoption(options, OPTION_UNIT)) {

      su.extract_text(ofilename);

    } else if (isoption(options, OPTION_EXPAND)) {

      try {

	// gracefully finish current file in compound document mode
	pstd::signal(SIGINT, terminate_handler);

	su.expand();

      } catch (TerminateLibXMLError error) {

	exit_status = STATUS_TERMINATED;

      } catch (LibXMLError error) {

	if (error.getErrorNum() != 0)
	  throw error;
      }

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
