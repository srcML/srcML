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
#include <cstring>
#include <sys/stat.h>
#include "version.h"
#include "srcmlapps.h"
#include "project.h"
#include "srcMLUtility.h"
#include "Options.h"
#include <list>
#include "srcmlns.h"

char const * const NAME = "srcml2src";

char const * const EXPAND_FLAG = "--extract-all";
char const * const EXPAND_FLAG_SHORT = "-a";

char const * const UNIT_FLAG = "--unit";
char const * const UNIT_FLAG_SHORT = "-U";
char const * const UNIT_FLAG_FULL = "--unit=NUM";

char const * const XML_FLAG = "--xml";
char const * const XML_FLAG_SHORT = "-X";

char const * const INFO_FLAG = "--info";
char const * const INFO_FLAG_SHORT = "-i";

char const * const LONG_INFO_FLAG = "--longinfo";
char const * const LONG_INFO_FLAG_SHORT = "-l";

char const * const NAMESPACE_FLAG = "--prefix";
char const * const NAMESPACE_FLAG_SHORT = "-p";
char const * const NAMESPACE_FLAG_FULL = "--prefix=URI";

using std::setw;

// output help message
void output_help(const char* name) {

     std::cout << "Usage: " << name << " [options] <infile> <outfile>\n\n"
	          "Translates from the the XML source-code representation srcML to source-code text files.\n"
                  "The srcML files can be in xml or gzip compressed xml (detected automatically).\n"
	          "Also provides various utilities for accessing metadata about the srcML document, \n"
	          "and extracting parts of compound srcML documents.\n\n"

	          "When no filenames are given read from standard input and write to standard output.\n"
	          "When only one filename is given write to standard output.\n"
	          "An input filename of '-' also reads from standard input.\n\n"

	          "Options:\n"
               << std::left
	       << "  " << HELP_FLAG_SHORT       << ", " << setw(COL) << HELP_FLAG       
	       << "display this help and exit\n"
	       << "  " << VERSION_FLAG_SHORT    << ", " << setw(COL) << VERSION_FLAG    
	       << "display version number and exit\n\n"

#ifdef LIBXML_ENABLED
	       << "  " << TEXTENCODING_FLAG_SHORT << ", " << setw(COL) <<  TEXTENCODING_FLAG_FULL  
	       << "set the output source encoding to ENC (default:  " << DEFAULT_TEXT_ENCODING << ") \n"
       /*
 	       << "  " << SKIP_ENCODING_FLAG_SHORT        << ", " << setw(COL) <<  SKIP_ENCODING_FLAG
	       << "skip any text encoding transformation" << "\n"
       */
	       << '\n'
#endif
	       << "  " << UNIT_FLAG_SHORT       << ", " << setw(COL) << UNIT_FLAG_FULL  
	       << "extract nested unit NUM from a compound srcML document\n"
	       << "  " << EXPAND_FLAG_SHORT     << ", " << setw(COL) << EXPAND_FLAG     
	       << "extract all files from a compound srcML document\n\n"
	       << "  " << XML_FLAG_SHORT        << ", " << setw(COL) << XML_FLAG        
	       << "output in XML instead of text\n"
#ifdef LIBXML_ENABLED
	      << "  " << COMPRESSED_FLAG_SHORT  << ", " << setw(COL) <<  COMPRESSED_FLAG
	      << "output XML in gzip format\n"
#endif
	       << '\n'
	      << "  " << NO_XML_DECLARATION_FLAG  << "    " 
	      << "do not output the default XML declaration in XML output\n"
	      << "  " << NO_NAMESPACE_DECLARATION_FLAG  << "     " 
	      << "do not output any namespace declarations in XML output\n\n"
               << "  " << VERBOSE_FLAG_SHORT    << ", " << setw(COL) << VERBOSE_FLAG    << "verbose output\n\n"
	       << "Metadata Options:  \n\n"
	       << "  " << LANGUAGE_FLAG_SHORT   << ", " << setw(COL) << LANGUAGE_FLAG
	       << "display source language and exit\n"
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
	       << "  " << NESTED_FLAG_SHORT     << ", " << setw(COL) << NESTED_FLAG     
	       << "display number of nested units and exit\n\n"
	       << "  " << INFO_FLAG_SHORT  << ", " << setw(COL) << INFO_FLAG
	       << "display most metadata (except nested unit count) and exit\n"
	       << "  " << LONG_INFO_FLAG_SHORT  << ", " << setw(COL) << LONG_INFO_FLAG
	       << "display all metadata (including nested unit count) and exit\n\n"
	       << "Examples:  \n";

     printf("  %1$s                       (read from standard input, write to standard output)\n"
	    "  %1$s main.cpp.xml          (read from file main.cpp.xml, write to standard output)\n"
	    "  %1$s main.cpp.xml main.cpp (read from file main.cpp.xml, write to file main.cpp)\n"
	    "  %1$s -                     (read from standard input, write to standard output)\n"
	    "  %1$s - main.cpp            (read from standard input, write to file main.cpp)\n"
	    "  %1$s --language            (read from standard input, output language attribute)\n"
	    "  %1$s --directory           (read from standard input, output directory attribute)\n"
	    "  %1$s --filename            (read from standard input, output filename attribute)\n"
	    "  %1$s --src-version         (read from standard input, output version attribute)\n\n", name);

     printf("www.sdml.info\n"
            "Report bugs to %s\n", EMAIL_ADDRESS);
}

// output version message
void output_version(const char* name) {

    std::cout << name << " Version " << VERSION << '\n'
	      << COPYRIGHT << '\n';
}

int options = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

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
  std::list<const char*> ns;

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
    /*
    // skip encoding mode
    else if (compare_flags(argv[curarg], SKIP_ENCODING_FLAG, SKIP_ENCODING_FLAG_SHORT, position)) {
      options |= OPTION_SKIP_ENCODING;
      if (position == original_position) ++curarg;
    }
    */
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
	std::cerr << NAME << ": namespace option selected but no namespace specified.\n";
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
      char* value = 0;
      if (embedded) {

	value = embedded + 1;
	++curarg;

      // check for unit flag with missing, separate unit number
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	std::cerr << NAME << ": unit option selected but no number specified.\n";
	exit(STATUS_UNIT_MISSING);
      } else {
	value = argv[(++curarg)++];
      }

      char *end;
      unit = pstd::strtol(value, &end, 10);

      // validate type of unit number
      if (errno == EINVAL || strlen(end) == strlen(value)) {
	std::cerr << NAME << ": unit option value \"" <<  value << "\" must be numeric.\n";
	exit(STATUS_UNIT_INVALID);
      }

      // validate range of unit number
      if (unit <= 0) {
	fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", NAME, unit);
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
	std::cerr << NAME << ": text encoding selected but not specified.\n";
	exit(STATUS_SRCENCODING_MISSING);
      } else {

	// extract parameter
	src_encoding = argv[(++curarg)++];
      }

      // validate source encoding
      if (!srcMLUtility::checkEncoding(src_encoding)) {
	std::cerr << NAME << ": text encoding \"" << src_encoding << "\" is not supported.\n";
	exit(STATUS_UNKNOWN_ENCODING);
      }
#else
      std::cerr << NAME << ": The source encoding option, i.e., " << TEXTENCODING_FLAG << ", is only supported in the libxml version.\n";
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
  const char* filename = "-";
  if (argc > curarg && !compare_flags(argv[curarg], STDIN, STDIN)) {
    filename = argv[curarg];
    ++curarg;
  }

  // second command line parameter is output filename
  const char* ofilename = "-";
  if (argc == curarg + 1) {
    ofilename = argv[curarg];
    ++curarg;

  } else if (argc > curarg + 1) {

      std::cerr << NAME << ": More than one output file specified.\n"
		<< "try '" << NAME << " --help' for more information." << "\n";
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

  /*
  // skip encoding and specifying encoding
  if (isoption(options, OPTION_SKIP_ENCODING) && isoption(options, OPTION_TEXT_ENCODING)) {

    std::cerr << NAME << ": Options for skipping encoding and specifying source encoding are incompatible.\n";
    exit(STATUS_INVALID_OPTION_COMBINATION);
  }
  */

  // verify that the input file exists
  struct stat instat;
  int result = stat(filename, &instat);
  if (filename[0] != '-' && result == -1) {
    std::cerr << NAME << ": Problem with Input file '" << filename << "'\n";
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  // verify that the output file is not the same as the input file
  struct stat outstat;
  stat(ofilename, &outstat);
  if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
    std::cerr << NAME << ": Input file '" << filename << "'"
	      << " is the same as the output file '" << ofilename << "'\n";
    exit(STATUS_INPUTFILE_PROBLEM);
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

  try {

    // setup for processing
    srcMLUtility su(filename, src_encoding, options);

    // process get attribute options
    if (optioncount > 0) {

      // move to the appropriate unit
      su.move_to_unit(unit);

      // output all the namespaces
      if (isoption(options, OPTION_INFO) || isoption(options, OPTION_LONG_INFO)) {

	const PROPERTIES_TYPE& ns = su.getNS();
	for (PROPERTIES_TYPE::const_iterator iter = ns.begin(); iter != ns.end(); ++iter)
	  std::cout << iter->second << "=\"" << iter->first << "\"" << std::endl;
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
	  attribute_title = "encoding=";
	  break;
	case OPTION_LANGUAGE:
	  attribute_name = UNIT_ATTRIBUTE_LANGUAGE;
	  attribute_title = "language=";
	  break;
	case OPTION_DIRECTORY:
	  attribute_name = UNIT_ATTRIBUTE_DIRECTORY;
	  attribute_title = "directory=";
	  break;
	case OPTION_FILENAME:
	  attribute_name = UNIT_ATTRIBUTE_FILENAME;
	  attribute_title = "filename=";
	  break;
	case OPTION_VERSION:
	  attribute_name = UNIT_ATTRIBUTE_VERSION;
	  attribute_title = "src-version=";
	  break;
	};

	// output the option
	const char* l = su.attribute(attribute_name);
	if (l) {
	  if (optioncount == 1)
	    std::cout << l << '\n';
	  else
	    std::cout << attribute_title << "\"" << l << "\"\n";
	}
      }

      if (isoption(options, OPTION_LONG_INFO)) {
	if (!isoption(options, OPTION_UNIT))
	  std::cout << "nested=\"" << su.curunits() << "\"\n";
      }

    // namespace
    } else if (isoption(options, OPTION_NAMESPACE)) {

      su.move_to_unit(unit);

      for (std::list<const char*>::const_iterator iter = ns.begin(); iter != ns.end(); ++iter) {

	const char* prefix = su.namespace_ext(*iter);
	if (prefix) {
	  if (ns.size() == 1)
	    std::cout << prefix << '\n';
	  else {
	    std::cout << "xmlns";
	    if (prefix[0] != '\0')
	      std::cout << ":" << prefix;
	    std::cout << "=\"" << *iter << "\"\n";
	  }
	}
      }

    // process non-attribute options
    } else if (isoption(options, OPTION_NESTED)) {

      // gracefully finish current file in compound document mode
      pstd::signal(SIGINT, terminate_handler);

      std::cout << su.unit_count() << '\n';

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
	exit_status = STATUS_TERMINATED;

    } else if (isoption(options, OPTION_EXPAND)) {

      // gracefully finish current file in compound document mode
      pstd::signal(SIGINT, terminate_handler);

      su.expand();

      // if we terminated early, output the correct status
      if (isoption(options, OPTION_TERMINATE))
	exit_status = STATUS_TERMINATED;

    } else if (isoption(options, OPTION_XML)) {

      su.extract_xml(ofilename, unit);

    } else {

      su.extract_text(ofilename, unit);

    }

  } catch (const OutOfRangeUnitError& e) {

    std::cerr << NAME << ": " << "unit " << unit << " was selected from a compound srcML document that contains " << e.size
	      << " nested " << (e.size > 1 ? "units" : "unit") << '\n';
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
      std::cerr << NAME << ": " << "Unable to open input file as XML\n";
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
