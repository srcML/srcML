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

#include <cstdio>
#include <fstream>
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

char const * const XPATH_FLAG = "--xpath";
char const * const XPATH_FLAG_SHORT = "";
char const * const XPATH_FLAG_FULL = "--xpath=XPATH";

char const * const CONTEXT_FLAG = "--context";
char const * const CONTEXT_FLAG_SHORT = "";
char const * const CONTEXT_FLAG_FULL = "--context=CONTEXT";

char const * const XSLT_FLAG = "--xslt";
char const * const XSLT_FLAG_SHORT = "";
char const * const XSLT_FLAG_FULL = "--xslt=XSLT_FILE";

char const * const XSLT_ALL_FLAG = "--xslt-all";
char const * const XSLT_ALL_FLAG_SHORT = "";

char const * const RELAXNG_FLAG = "--relaxng";
char const * const RELAXNG_FLAG_SHORT = "";
char const * const RELAXNG_FLAG_FULL = "--relaxng=RELAXNG_FILE";

char const * const PARAM_FLAG = "--param";
char const * const PARAM_FLAG_SHORT = "";

// output help message
void output_help(const char* name) {

  printf("Usage: %s [options] <infile> <outfile>\n\n"
         "Translates from the the XML source-code representation srcML to source-code text files.\n"
         "The srcML files can be in xml or gzip compressed xml (detected automatically).\n"
         "Also provides for access to metadata about the srcML document, extracting specific\n"
         "parts of compound srcML documents, querying using XPath and RelaxNG, and\n"
	 "and transformation with XSLT.\n\n"

         "When no filenames are given read from standard input and write to standard output.\n"
         "When only one filename is given write to standard output.\n"
	 "An input filename of '-' also reads from standard input.\n\n"

	 "Options:\n", name);

  printf("  %s, %-19s display this help and exit\n", HELP_FLAG_SHORT, HELP_FLAG);
  printf("  %s, %-19s display version number and exit\n", VERSION_FLAG_SHORT, VERSION_FLAG);

  printf("  %s, %-19s set the output source encoding to ENC (default:  %s) \n\n",
	  TEXTENCODING_FLAG_SHORT, TEXTENCODING_FLAG_FULL, DEFAULT_TEXT_ENCODING);

       /*
 	       << "  " << SKIP_ENCODING_FLAG_SHORT        << ", " << setw(COL) <<  SKIP_ENCODING_FLAG
	       << "skip any text encoding transformation" << "\n"
       */

  printf("  %s, %-19s extract nested unit NUM from a compound srcML document\n",
	  UNIT_FLAG_SHORT, UNIT_FLAG_FULL);

  printf("  %s, %-19s extract all files from a compound srcML document\n\n",
	  EXPAND_FLAG_SHORT, EXPAND_FLAG);


  printf("  %s, %-19s output in XML instead of text\n",
	  XML_FLAG_SHORT, XML_FLAG);

  printf("  %s, %-19s output XML in gzip format\n\n",
	  COMPRESSED_FLAG_SHORT, COMPRESSED_FLAG);

  printf("  %-23s do not output the default XML declaration in XML output\n",
	  NO_XML_DECLARATION_FLAG);

  printf("  %-23s do not output any namespace declarations in XML output\n\n",
	  NO_NAMESPACE_DECLARATION_FLAG);

  printf("  %s, %-19s verbose output\n\n",
	  VERBOSE_FLAG_SHORT, VERBOSE_FLAG);

  printf("Metadata Options:  \n\n"
	 "  %s, %-19s display source language and exit\n", LANGUAGE_FLAG_SHORT, LANGUAGE_FLAG);

  printf("  %s, %-19s display source directory name and exit\n", DIRECTORY_FLAG_SHORT, DIRECTORY_FLAG);

  printf("  %s, %-19s display source filename and exit\n", FILENAME_FLAG_SHORT, FILENAME_FLAG   );

  printf("  %s, %-19s display source version and exit\n", SRCVERSION_FLAG_SHORT, SRCVERSION_FLAG);

  printf("  %s, %-19s display xml encoding and exit\n", ENCODING_FLAG_SHORT, ENCODING_FLAG);

  printf("  %s, %-19s display prefix of namespace given by URI and exit\n",
	  NAMESPACE_FLAG_SHORT, NAMESPACE_FLAG_FULL);

  printf("  %s, %-19s display number of nested units and exit\n\n", NESTED_FLAG_SHORT, NESTED_FLAG);

  printf("  %s, %-19s display most metadata (except nested unit count) and exit\n",
	  INFO_FLAG_SHORT, INFO_FLAG);

  printf("  %s, %-19s display all metadata (including nested unit count) and exit\n\n",
	  LONG_INFO_FLAG_SHORT, LONG_INFO_FLAG);

  printf("Query and Tranformation Options:  \n\n"
	 "  %-23s apply XPATH expression to each nested unit\n", XPATH_FLAG_FULL);
  printf("  %-23s apply XSLT_FILE transformation to each nested unit\n", XSLT_FLAG_FULL);
  printf("  %-23s apply RELAXNG_FILE grammar file to each nested unit\n\n", RELAXNG_FLAG_FULL);

  printf("Examples:  \n"
	    "  %1$s                       (read from standard input, write to standard output)\n"
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

  printf("%s Version %s\n"
	 "%s\n", name, VERSION, COPYRIGHT);
}

int options = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

#ifdef __GNUG__
extern "C" void verbose_handler(int);

extern "C" void terminate_handler(int);
#endif

int optionorder[5];
int optioncount = 0;

const int MAXPARAMS = 32;
int paramcount = 0;
const char* params[MAXPARAMS * 2 + 1];

int main(int argc, char* argv[]) {

  /* signal handling */

#ifdef __GNUG__
  // toggling verbose flag
  pstd::signal(SIGUSR1, verbose_handler);
#endif

  int exit_status = EXIT_SUCCESS;

  // option values
  const char* src_encoding = DEFAULT_TEXT_ENCODING;
  int unit = 0;
  const char* xpath = ".";
  const char* context = "src:unit";
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

      options |= OPTION_COMPRESSED;
      if (position == original_position) ++curarg;
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
	fprintf(stderr, "%s: namespace option selected but no namespace specified.\n", NAME);
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
	fprintf(stderr, "%s: unit option selected but no number specified.\n", NAME);
	exit(STATUS_UNIT_MISSING);
      } else {
	value = argv[(++curarg)++];
      }

      char *end;
      unit = pstd::strtol(value, &end, 10);

      // validate type of unit number
      if (errno == EINVAL || strlen(end) == strlen(value)) {
	fprintf(stderr, "%s: unit option value \"%s\" must be numeric.\n", NAME, value);
	exit(STATUS_UNIT_INVALID);
      }

      // validate range of unit number
      if (unit <= 0) {
	fprintf(stderr, "%s: unit option value \"%d\" must be > 0.\n", NAME, unit);
	exit(STATUS_UNIT_INVALID);
      }
    }

    // parameters (for xslt) flag
    else if (compare_flags(argv[curarg], PARAM_FLAG, PARAM_FLAG_SHORT)) {

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      char* value = 0;
      if (embedded) {

	value = embedded + 1;
	++curarg;

      // check for param flag with missing, separate param name
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: param option selected but no property or value specified.\n", NAME);
	exit(STATUS_UNIT_MISSING);
      } else {
	params[paramcount] = argv[(++curarg)];
	++paramcount;
	params[paramcount] = argv[(++curarg)++];
	++paramcount;
      }
    }

    // xslt apply to entire unit
    else if (compare_flags(argv[curarg], XSLT_ALL_FLAG, XSLT_ALL_FLAG_SHORT, position)) {
      options |= OPTION_XSLT_ALL;
      if (position == original_position) ++curarg;
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
      if (!srcMLUtility::checkEncoding(src_encoding)) {
	fprintf(stderr, "%s: text encoding \"%s\" is not supported.\n", NAME, src_encoding);
	exit(STATUS_UNKNOWN_ENCODING);
      }

    // xpath
    } else if (compare_flags(argv[curarg], XPATH_FLAG, XPATH_FLAG_SHORT)) {
      options |= OPTION_XPATH;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	xpath = embedded + 1;
	++curarg;

      // check for namespace flag with missing namespace
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: xpath option selected but no xpath expression.\n", NAME);
	exit(STATUS_UNIT_MISSING); // FIX
      } else {
	xpath = argv[(++curarg)++];
      }

    // context
    } else if (compare_flags(argv[curarg], CONTEXT_FLAG, CONTEXT_FLAG_SHORT)) {
      options |= OPTION_XPATH;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	context = embedded + 1;
	++curarg;

      // check for namespace flag with missing namespace
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: context option selected but no context given.\n", NAME);
	exit(STATUS_UNIT_MISSING); // FIX
      } else {
	context = argv[(++curarg)++];
      }

    // xslt
    } else if (compare_flags(argv[curarg], XSLT_FLAG, XSLT_FLAG_SHORT)) {
      options |= OPTION_XSLT;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	xpath = embedded + 1;
	++curarg;

      // check for namespace flag with missing namespace
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: xpath option selected but no xpath expression.\n", NAME);
	exit(STATUS_UNIT_MISSING); // FIX
      } else {
	xpath = argv[(++curarg)++];
      }

    // relaxng
    } else if (compare_flags(argv[curarg], RELAXNG_FLAG, RELAXNG_FLAG_SHORT)) {
      options |= OPTION_RELAXNG;

      char* embedded = extract_option(argv[curarg]);

      // filename is embedded parameter
      if (embedded) {

	xpath = embedded + 1;
	++curarg;

      // check for namespace flag with missing namespace
      } else if (argc <= curarg + 1 || strcmp(argv[curarg + 1], OPTION_SEPARATOR) == 0) {
	fprintf(stderr, "%s: xpath option selected but no xpath expression.\n", NAME);
	exit(STATUS_UNIT_MISSING); // FIX
      } else {
	xpath = argv[(++curarg)++];
      }

    // reached the end of a multi-short form option
    } else if (position > 0 && argv[curarg][position + 1] == '\0') {

      ++curarg;

    // invalid option
    } else {

      fprintf(stderr, "%s: unrecognized option '%s'\n", NAME, argv[curarg]);
      fprintf(stderr, "try '%s %s' for more information.\n", NAME, HELP_FLAG);
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

      fprintf(stderr, "%1$s: More than one output file specified.\n"
	      "try '%1$s %2$s' for more information.\n", NAME, HELP_FLAG);
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

    fprintf(stderr, "%s: Options for xml output and specifying source encoding are incompatible.\n", NAME);
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
    fprintf(stderr, "%s: Problem with Input file '%s'\n", NAME, filename);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  // verify that the output file is not the same as the input file
#ifdef __GNUG__
  struct stat outstat;
  stat(ofilename, &outstat);
  if (instat.st_ino == outstat.st_ino && instat.st_dev == outstat.st_dev) {
    fprintf(stderr, "%s: Input file '%s' is the same as the output file '%s'\n", NAME, filename, ofilename);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
#endif

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

	printf("\r");
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

      su.move_to_unit(unit);

      for (std::list<const char*>::const_iterator iter = ns.begin(); iter != ns.end(); ++iter) {

	const char* prefix = su.namespace_ext(*iter);
	if (prefix) {
	  if (ns.size() == 1)
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

      su.extract_xml(ofilename, unit);

    } else if (isoption(options, OPTION_XPATH)) {

      if (strcmp(xpath, "."))
	su.xpath(ofilename, context, xpath);
      else
	su.extract_element(context, ofilename);

    } else if (isoption(options, OPTION_XSLT)) {

      params[paramcount + 1] = NULL;

      su.xslt(context, ofilename, xpath, params, paramcount);

    } else if (isoption(options, OPTION_RELAXNG)) {

      su.relaxng(ofilename, xpath);

    } else {

      su.extract_text(ofilename, unit);

    }

  } catch (const OutOfRangeUnitError& e) {

    fprintf(stderr, "%s: unit %d  was selected from compound srcML document that contains "
	    "%d nested units\n", NAME, unit, e.size);
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
      fprintf(stderr, "%s: Unable to open input file as XML\n", NAME);
    }

  } catch (const char* s) {
    
    fprintf(stderr, "%s: %s\n", NAME, s);
    if (!exit_status)
      exit_status = STATUS_ERROR;
  } catch (...) {
    if (!exit_status)
      exit_status = STATUS_ERROR;
  }

  return exit_status;
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
