/**
 * @file srcml_cli.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcml_cli.hpp>
#include <src_prefix.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <stdlib.h>
#include <srcml_logger.hpp>

namespace prog_opts = boost::program_options;

const char* SRCML_HEADER = "";

const char* SRCML_FOOTER = "\n\
  Have a question or need to report a bug?\n\
  Contact us at http://www.srcml.org/support.html\n\
  www.srcML.org";

const char* SRC2SRCML_HEADER = "Usage: srcml [options] <src_infile>... [-o <srcML_outfile>]\
  \n\n\
  Translates C, C++, and Java source code into the XML\n\
  source-code representation srcML. Input can be from standard input, a file,\n\
  a directory, or an archive file, i.e., tar, cpio, and zip. Multiple files\n\
  are stored in a srcML archive.\
  \n\n\
  The source-code language is based on the file extension. Additional extensions\n\
  for a language can be registered, and can be directly set using the --language\n\
  option.\
  \n\n\
  By default, output is to stdout.You can specify a file for output using the\n\
  --output or -o option. When no filenames are given, input is from stdin and\n\
  output is to stdout. An input filename of '-' also reads from stdin.\
  \n\n\
  Any input file can be a local filename or a URI with the protocols http:,\n\
  ftp:, or file: \n\n";

const char* SRC2SRCML_FOOTER = "Examples:\
  \n\n\
  Read from standard input, write to standard output:\n\
  srcml\n\
  \n\
  Read from file m.cpp, write to standard output:\n\
  srcml m.cpp\n\
  \n\
  Read from file m.cpp, write to file m.cpp.xml:\n\
  srcml m.cpp -o m.cpp.xml\n\
  \n\
  Read from URI, write to standard output:\n\
  srcml http://www.srcML.org/projects/srcml/ex/main.cpp\n\
  \n\
  Element unit attribute filename \"m.cpp\":\n\
  srcml --filename=m.cpp m.cpp -o m.cpp.xml\n\
  \n\
  Set encoding of input text file to UTF-8:\n\
  srcml --src-encoding=UTF-8 m.cpp m.cpp.xml\n\
  \n\
  Set encoding of srcML file to ISO-8859-1:\n\
  srcml --xml-encoding=ISO-8859-1 m.cpp m.cpp.xml\n\
  \n\
  Have a question or need to report a bug?\n\
  Contact us at http://www.srcml.org/support.html\n\
  www.srcML.org";

const char* SRCML2SRC_HEADER = "Usage: srcml [options] <srcML_infile>... [-o <src_outfile>]\
  \n\n\
  Translates from the the XML source-code representation srcML back to\n\
  source-code.\
  \n\n\
  Extracts back to standard output or disk. Provides access to metadata about\n\
  the srcML document. For srcML archives provides extraction of specific\n\
  files, and efficient querying/transformation using XPath, XSLT, and RelaxNG.\
  \n\n\
  srcML archives contain multiple individual source code files, e.g., an\n\
  entire project or directory tree.\
  \n\n\
  By default, output is to stdout.You can specify a file for output using the\n\
  --output or -o option. When no filenames are given, input is from stdin and\n\
  output is to stdout. An input filename of '-' also reads from stdin.\
  \n\n\
  Any input file, including XSLT and RelaxNG files, can be a local filename\n\
  or a URI with the protocols http:, ftp:, or file:\
  \n\n\
  The srcML files can be in xml, or compressed with gzip or bzip2 (detected\n\
  automatically).\n\n";

const char* SRCML2SRC_FOOTER = "Examples:\
  \n\n\
  Read from file m.cpp.xml, write to file m.cpp:\n\
  srcml m.cpp.xml -o m.cpp\n\
  \n\
  Read from URI, write to file m.cpp:\n\
  srcml http://www.example.org/m.cpp.xml m.cpp\n\
  \n\
  Read from file m.cpp.xml, output language attribute to stdout:\n\
  srcml m.cpp.xml --show-language\n\
  \n\
  Have a question or need to report a bug?\n\
  Contact us at http://www.srcml.org/support.html\n\
  www.srcML.org";

srcml_request_t srcml_request;

// Define Program Options
prog_opts::options_description general("GENERAL OPTIONS");
prog_opts::options_description src2srcml_options("CREATING SRCML");
prog_opts::options_description srcml2src_options("EXTRACTING SOURCE CODE");
prog_opts::options_description query_transform("TRANSFORMATIONS");
prog_opts::options_description positional_options("POSITIONAL");
prog_opts::options_description deprecated_options("DEPRECATED OPTIONS");
prog_opts::options_description debug_options("DEBUG OPTIONS");
prog_opts::options_description experimental_options("EXPERIMENTAL OPTIONS");
prog_opts::options_description implicit_value_handlers("IMPLICIT VALUE HANDLER");
prog_opts::options_description all("ALL OPTIONS");

prog_opts::options_description markup_options("MARKUP OPTIONS");
prog_opts::options_description xml_form("XML FORM");
prog_opts::options_description metadata_options("METADATA OPTIONS");

prog_opts::options_description src2srcml("");
prog_opts::options_description srcml2src("");


// Positional Args
prog_opts::positional_options_description input_file;

// tranformation check on input
bool is_transformation(const srcml_input_src& input);

/* Most of the no parameter options could be recorded this way */
template <int option>
void option_markup(bool opt) {
    /*
      If we have markup options the NULL optional arguement needs to
      first be initializied before the bitwise work can be done.
    */
    if (!srcml_request.markup_options)
        srcml_request.markup_options = 0;

    if (opt)
        *srcml_request.markup_options |= option;
}

template <int command>
void option_command(bool opt) {
    if (opt)
      srcml_request.command |= command;
}

// deprecated option command
// (This is required as non-deprecated options may use same values)
template <int command>
void option_command_deprecated(bool opt) {
    if (opt) {
      srcml_request.command |= command;

      // Notify user of deprecated options
      if (command == SRCML_COMMAND_UNITS)
        SRCMLLogger::log(SRCMLLogger::WARNING_MSG, "srcml: use of option --units or -n is deprecated");
      if (command == SRCML_COMMAND_EXPRESSION)
        SRCMLLogger::log(SRCMLLogger::WARNING_MSG, "srcml: use of option --expression or -e is deprecated");
    }
}

// Generic fields
template <boost::optional<std::string> srcml_request_t::*pfield>
void option_field(const std::string& value) { srcml_request.*pfield = value; }

template <std::vector<std::string> srcml_request_t::*pfield>
void option_field(const std::vector<std::string>& value) { srcml_request.*pfield = value; }

template <int srcml_request_t::*pfield>
void option_field(int value) { srcml_request.*pfield = value; }

template <boost::optional<size_t> srcml_request_t::*pfield>
void option_field(size_t value) { srcml_request.*pfield = value; }

// option files_from
template <>
void option_field<&srcml_request_t::files_from>(const std::vector<std::string>& value) {

    srcml_request.files_from = value;
    BOOST_FOREACH(const std::string& inputFile, value) {
        srcml_request.input_sources.push_back(src_prefix_add_uri("filelist", inputFile));
    }
}

// option src encoding
template <>
void option_field<&srcml_request_t::src_encoding>(const std::string& value) {

    if (value.empty() || srcml_check_encoding(value.c_str()) == 0) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: invalid src encoding \"" + value + "\"");
        exit(CLI_ERROR_INVALID_ARGUMENT);
    }
    srcml_request.src_encoding = value;
}

// option xml encoding attribute
template <>
void option_field<&srcml_request_t::att_xml_encoding>(const std::string& value) {

    if (value.empty() || srcml_check_encoding(value.c_str()) == 0) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: invalid xml encoding \"" + value + "\"");
        exit(CLI_ERROR_INVALID_ARGUMENT);
    }
    srcml_request.att_xml_encoding = value;
}

// option language attribute
template <>
void option_field<&srcml_request_t::att_language>(const std::string& value) {

    // check language
    if (value.empty() || srcml_check_language(value.c_str()) == 0) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: invalid language \"" + value + "\"");
        exit(6); //ERROR CODE TBD
    }
    srcml_request.att_language = value;
}

// option tabs
template <>
void option_field<&srcml_request_t::tabs>(int value) {

    // check tabstop
    if (value < 1) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: " + std::to_string(value) + " is an invalid tab stop. Tab stops must be 1 or higher.");
        exit(1); //ERROR CODE TBD
    }

    srcml_request.tabs = value;
    *srcml_request.markup_options |= SRCML_OPTION_POSITION;
}

void option_output_filename(const std::string& value) {
  srcml_request.output_filename = srcml_output_dest(value == "-" ? "stdout://-" : value);

  if (srcml_request.output_filename.protocol == "file")  {
    if (srcml_request.output_filename.isdirectory || (srcml_request.output_filename.extension == ""
        && srcml_request.output_filename.filename[srcml_request.output_filename.filename.length() - 1] == '/')) {

      srcml_request.command |= SRCML_COMMAND_TO_DIRECTORY;
      srcml_request.command |= SRCML_COMMAND_NOARCHIVE;
    }
  }
}

void option_xmlns_uri(const std::string& value) {
    srcml_request.xmlns_namespaces[""] = value;
    srcml_request.xmlns_namespace_uris[value] = "";
}

void option_xmlns_prefix(const std::vector<std::string>& values) {
    BOOST_FOREACH( std::string value, values )
    {
      std::size_t delim = value.find("=");
      if (delim == std::string::npos) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: xmlns format missing \"=\"");
        exit(1); //ERROR CODE TBD
      }

      srcml_request.xmlns_namespaces[value.substr(0, delim)] = value.substr(delim + 1);
      srcml_request.xmlns_namespace_uris[value.substr(delim + 1)] = value.substr(0, delim);
    }
}

// option output to directory
void option_to_dir(const std::string& value) {
    srcml_request.output_filename = srcml_output_dest(value);
    srcml_request.command |= SRCML_COMMAND_TO_DIRECTORY;
    srcml_request.command |= SRCML_COMMAND_NOARCHIVE;
}

void positional_args(const std::vector<std::string>& value) {
    srcml_request.input_sources.reserve(srcml_request.input_sources.size() + value.size());

    BOOST_FOREACH(const std::string& iname, value) {

        // record the position of stdin
        if (iname == "-" || iname == "stdin://-")
            srcml_request.stdindex = (int) srcml_request.input_sources.size();

        srcml_input_src input(iname);

        if (!(is_transformation(input))) {
          srcml_request.input_sources.push_back(input);
        }
    }
}

void raw_text_args(const std::vector<std::string>& value) {
  BOOST_FOREACH(const std::string& raw_text, value) {
    srcml_request.input_sources.push_back(src_prefix_add_uri("text",raw_text));
  }
}

void option_help(const std::string& help_opt) {
    if (help_opt.empty()) {
        // TODO: A new header and footer for the general option
        std::cout << SRCML_HEADER << "\n";
        std::cout << general << "\n";

        std::cout << src2srcml << "\n";
        std::cout << srcml2src << "\n";

        std::cout << SRCML_FOOTER << "\n";
    }
    else if (help_opt == "src2srcml") {
        std::cout << SRC2SRCML_HEADER << "\n";
        std::cout << src2srcml << "\n";
        std::cout << SRC2SRCML_FOOTER << "\n";
    }
    else if (help_opt == "srcml2src") {
        std::cout << SRCML2SRC_HEADER << "\n";
        std::cout << srcml2src << "\n";
        std::cout << SRCML2SRC_FOOTER << "\n";
    }
    else {
        std::cout << "Unknown module '"
                  << help_opt << "' in --help\n";
        exit(1);
    }
    exit(0);
}

/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. (FROM BOOST LIBRARY EXAMPLES)*/
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2);

// Determine dependent options
void option_dependency(const prog_opts::variables_map& vm, const char* option, const char* dependent_option);

// Custom Parser Definition
std::pair<std::string, std::string> custom_parser(const std::string& s);

// Debug
void debug_cli_opts(const struct srcml_request_t srcml_request);

// Sanitize element input
element clean_element_input(const std::basic_string< char >& element_input);

// Sanitize attribute input
attribute clean_attribute_input(const std::basic_string< char >& attribute_input);

// Interpretation of CLI options
srcml_request_t parseCLI(int argc, char* argv[]) {
    try {
        general.add_options()
            ("help,h", prog_opts::value<std::string>()->notifier(&option_help),"display this help and exit. USAGE: help or help [module name]. MODULES: src2srcml, srcml2src")
            ("version,V", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_VERSION>), "display version number and exit")
            ("verbose,v", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_VERBOSE>), "conversion and status information to stderr")
            ("quiet,q", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_QUIET>), "suppress status messages")
            ("list", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_LIST>), "list all files in the srcML archive and exit")
            ("info,i", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO>), "display most metadata except srcML file count and exit")
            ("longinfo,L", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_LONGINFO>), "display all metadata including srcML file count and exit")
            ("jobs,j", prog_opts::value<int>()->notifier(&option_field<&srcml_request_t::max_threads>)->default_value(4), "set the maximum number of threads srcml can spawn")
            ("output,o", prog_opts::value<std::string>()->notifier(&option_output_filename)->default_value("stdout://-"), "write ouput to a file")
            ;

        src2srcml_options.add_options()
            ("language,l", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_language>), "set the language to C, C++, or Java")
            ("register-ext", prog_opts::value< std::vector<std::string> >()->notifier(&option_field<&srcml_request_t::language_ext>), "register file extension EXT for source-code language LANG. arg format EXT=LANG")
            ("src-encoding", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::src_encoding>), "set the input source encoding")
            ("files-from", prog_opts::value<std::vector<std::string> >()->notifier(&option_field<&srcml_request_t::files_from>), "read list of source file names to form a srcML archive")
            ("output-xml,X", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_XML>), "output in XML instead of text")
            ("archive,r", prog_opts::bool_switch()->notifier(&option_markup<SRCML_ARCHIVE>), "store output in a srcML archive, default for multiple input files")
            ("in-order", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_OUTPUT_ORDERED>), "enable strict output ordering")
            ("text,t", prog_opts::value< std::vector<std::string> >()->notifier(&raw_text_args), "raw string text to be processed")
            ;

        markup_options.add_options()
            ("position", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_POSITION>), "include line/column attributes, namespace 'http://www.srcML.org/srcML/position'")
            ("tabs", prog_opts::value<int>()->notifier(&option_field<&srcml_request_t::tabs>), "set tabs arg characters apart.  Default is 8")
            ("cpp", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_CPP>), "enable preprocessor parsing and markup for Java and non-C/C++ languages")
            ("cpp-markup-if0", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_CPP_MARKUP_IF0>), "markup cpp #if 0 regions")
            ("cpp-nomarkup-else", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_CPP_TEXT_ELSE>), "leave cpp #else regions as text")
            ;

        xml_form.add_options()
            ("xml-encoding,x", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_xml_encoding>)->default_value("UTF-8"),"set output XML encoding. Default is UTF-8")
            ("no-xml-declaration", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_XML_DECL>), "do not output the XML declaration")
            ("xmlns", prog_opts::value<std::string>()->notifier(&option_xmlns_uri), "set the default namespace to arg")
            ("xmlns:", prog_opts::value< std::vector<std::string> >()->notifier(&option_xmlns_prefix), "set the namespace. arg format PREFIX=URI")
            ;

        metadata_options.add_options()
            ("filename,f", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_filename>), "set the filename attribute")
            ("url", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_url>), "set the url attribute")
            ("src-version,s", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_version>), "set the version attribute")
            ("hash", prog_opts::bool_switch()->notifier(&option_markup<SRCML_HASH>), "add hash to srcml output")
            ("timestamp", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_TIMESTAMP>), "add timestamp to srcml output")
            ("prefix,p", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::xmlns_prefix_query>), "display prefix of namespace given by URI arg and exit")
            ("show-language", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE>), "display source language and exit")
            ("show-url", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_URL>), "display source url name and exit")
            ("show-filename", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_FILENAME>), "display source filename and exit")
            ("show-src-version", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION>), "display source version and exit")
            ("show-timestamp", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP>), "display timestamp and exit")
            ("show-hash", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_HASH>), "display hash and exit")
            ("show-encoding", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_ENCODING>), "display xml encoding and exit")
            ("show-unit-count", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_UNITS>), "display number of srcML files and exit")
            ;

        srcml2src_options.add_options()
            ("output-src,S", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_SRC>), "output in text instead of XML")
            ("to-dir", prog_opts::value<std::string>()->notifier(&option_to_dir), "extract all files from srcML and create them in the filesystem")
            ;

        query_transform.add_options()
            ("relaxng", prog_opts::value< std::vector<std::string> >(), "output individual units that match RELAXNG file or URI")
            ("xpath", prog_opts::value< std::vector<std::string> >(), "apply XPATH expression to each individual unit")
            ("xslt", prog_opts::value< std::vector<std::string> >(), "apply XSLT file or URI transformation to each individual unit")
            ("attribute", prog_opts::value< std::vector<std::string> >(), "add attribute to xpath query")
            ("element", prog_opts::value< std::vector<std::string> >(), "add element to xpath query")
            ("unit,U", prog_opts::value<int>()->notifier(&option_field<&srcml_request_t::unit>), "extract individual unit number from srcML")
            ("revision", prog_opts::value<size_t>()->notifier(&option_field<&srcml_request_t::revision>), "extract the given revision (0 = original, 1 = modified)")
            ;

        positional_options.add_options()
            ("input-files", prog_opts::value< std::vector<std::string> >()->notifier(&positional_args), "input files")
            ;
        
        // Work arounds for dealing with implicit option values properly
        implicit_value_handlers.add_options()
            ("text-equals-null", prog_opts::value< std::vector<std::string> >()->implicit_value(std::vector<std::string>(), "")->notifier(&raw_text_args), "work around for null text")
            ;

        deprecated_options.add_options()
            ("units,n", prog_opts::bool_switch()->notifier(&option_command_deprecated<SRCML_COMMAND_UNITS>), "display number of srcML files and exit")
            ("expression,e", prog_opts::bool_switch()->notifier(&option_command_deprecated<SRCML_COMMAND_EXPRESSION>), "expression mode for translating a single expression not in a statement")
            ;

        debug_options.add_options()
            ("dev", prog_opts::bool_switch()->notifier(&option_command<SRCML_DEBUG_MODE>), "Enable developer debug mode.")
            ;
        experimental_options.add_options()
            ("update", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_UPDATE>), "output and update existing srcml")
            ("interactive,c", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INTERACTIVE>), "immediate output while parsing, default for keyboard input")
            ("xml-processing", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::xml_processing>), "add XML processing instruction")
            ("xpathparam", prog_opts::value< std::vector<std::string> >(), "passes a parameter NAME and VAL arg to the XSLT program. arg format NAME=VAL")
            ("pretty", prog_opts::value<std::string>()->implicit_value("")->notifier(&option_field<&srcml_request_t::pretty_format>), "custom formatting for output")
            ("external", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::external>), "run a user defined external script or application on srcml client output")
            ("line-ending", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::line_ending>), "set the line endings for a desired environment \"Windows\" or \"Unix\"")
            ;

        // Group src2srcml Options
        //src2srcml.add(src2srcml_options).add(markup_options).add(xml_form).add(metadata_options);
        src2srcml.add(src2srcml_options).add(markup_options).add(xml_form).add(metadata_options);

        // Group srcml2src Options
        //srcml2src.add(srcml2src_options).add(query_transform);
        srcml2src.add(srcml2src_options).add(query_transform);

        // Group all Options
        all.add(general).add(src2srcml_options).add(markup_options).add(xml_form).add(metadata_options).add(srcml2src_options).
            add(query_transform).add(positional_options).add(implicit_value_handlers).add(deprecated_options).add(debug_options).add(experimental_options);

        // Positional Args
        input_file.add("input-files", -1);

        // Assign the CLI args to the map
        prog_opts::variables_map cli_map;

        const prog_opts::basic_parsed_options< char >& cliopts = prog_opts::command_line_parser(argc, argv).options(all).
                         positional(input_file).extra_parser(custom_parser).run();

        std::vector< prog_opts::basic_option< char > > parsedOptions = cliopts.options;

        // loop the cli options in the order they were processed/received
        BOOST_FOREACH(const prog_opts::basic_option< char >& option, parsedOptions) {
          if (option.string_key == "relaxng" || option.string_key == "xpath" || option.string_key == "xslt" || option.string_key == "xpathparam"
             || option.string_key == "element" || option.string_key == "attribute") {

            if (option.string_key == "xpath")
              srcml_request.xpath_query_support.push_back(std::make_pair(boost::none,boost::none));

            BOOST_FOREACH(const std::basic_string< char >& vals, option.value) {
              if (option.string_key == "element" && srcml_request.xpath_query_support.size() < 1) {

                SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: element option must follow an --xpath option");
                exit(SRCML_STATUS_INVALID_ARGUMENT);
              }
              if (option.string_key == "attribute" && srcml_request.xpath_query_support.size() < 1) {

                SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: attribute option must follow an --xpath option");
                exit(SRCML_STATUS_INVALID_ARGUMENT);
              }

              if (option.string_key == "element") {
                srcml_request.xpath_query_support.at(srcml_request.xpath_query_support.size() - 1).first = clean_element_input(vals);
              }
              else if (option.string_key == "attribute") {
                srcml_request.xpath_query_support.at(srcml_request.xpath_query_support.size() - 1).second = clean_attribute_input(vals);
              }
              else {
                srcml_request.transformations.push_back(src_prefix_add_uri(option.string_key, vals));
              }
            }
          }
        }

        prog_opts::store(cliopts , cli_map);
        prog_opts::notify(cli_map);

        // Check option conflicts
        conflicting_options(cli_map, "quiet", "verbose");
        conflicting_options(cli_map, "output", "to-dir");
        conflicting_options(cli_map, "cpp-text-else", "cpp-markup-else");
        conflicting_options(cli_map, "cpp-text-if0", "cpp-markup-if0");
        conflicting_options(cli_map, "output-src", "output-xml");

        // Check dependent options
        // Format: option_dependency(cli_map, [option], [option]);
        option_dependency(cli_map, "text", "language");

        // If input was from stdin, then artificially put a "-" into the list of input files
        if (srcml_request.input_sources.empty())
          positional_args(std::vector<std::string>(1, "stdin://-"));

        if (srcml_request.input_sources.size() == 1 && srcml_request.input_sources[0].isdirectory) {
          std::string url = srcml_request.input_sources[0].resource;
          while (url.length() > 0 && (url.at(0) == '.' || url.at(0) == '/')) {
            url.erase(0,1);
          }
          srcml_request.att_url = url;
        }

        // If position option is used without tabs...set default tab of 8
        if ((*srcml_request.markup_options & SRCML_OPTION_POSITION && srcml_request.tabs == 0) || srcml_request.tabs == 0)
          srcml_request.tabs = 8;

#if defined(__GNUG__) && !defined(__MINGW32__)
        // automatic interactive use from stdin (not on redirect or pipe)
        if (isatty(STDIN_FILENO))
            option_command<SRCML_COMMAND_INTERACTIVE>(true);
#endif

        // Ideally get this from libsrcml
        const std::vector<std::string> reserved_namespaces = {
        "=http://www.srcML.org/srcML/src",
        "cpp=http://www.srcML.org/srcML/cpp",
        "java=http://www.srcML.org/srcML/java",};

        BOOST_FOREACH(const std::string& ns, reserved_namespaces) {
          size_t delim = ns.find('=');
          std::string prefix = ns.substr(0, delim);
          std::string uri = ns.substr(delim + 1);

          // A reserved prefix wasn't used or it was set to the same uri
          if (srcml_request.xmlns_namespaces.find(prefix) == srcml_request.xmlns_namespaces.end() || srcml_request.xmlns_namespaces[prefix] == uri) {
            continue;
          }

          // A reserved uri is set to a different prefix
          if (srcml_request.xmlns_namespace_uris.find(uri) != srcml_request.xmlns_namespaces.end() && srcml_request.xmlns_namespace_uris[uri] != prefix) {
            continue;
          }

          SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: prefix \"" + prefix + "\" assigned multiple URIs \"" + uri + "\", \"" + srcml_request.xmlns_namespaces[prefix] + "\"");
          exit(1); // TODO Need a real error code
        }

    }
    // Unknown Option
    catch(boost::program_options::unknown_option& e) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: " + std::string(e.what()));
        exit(3);
    }
    // Missing Option Value
    catch(boost::program_options::error_with_option_name& e) {
        std::string error_msg(e.what());
        
        /*
            This allows for --help with no value (currently a work around for implicit issues)
            We check the error message for a section to identify when --help is used without a value
            and call the function manually to print the appropriate help message. 
            Calls to option_help automatically exit the cli.
        */
        if (error_msg.find("'--help' is missing") != std::string::npos) {
                option_help("");
        }

        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: " + std::string(error_msg));
        exit(7);
    }
    // Catch all other issues with generic error
    catch(std::exception& e) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: " + std::string(e.what()));
        exit(1);
    }

    return srcml_request;
}

// Custom parser for xmlns: option
std::pair<std::string, std::string> custom_parser(const std::string& s) {
    if (s.find("--xmlns:") == 0)
        return std::make_pair(std::string("xmlns:"), std::string(s.substr(s.find(":")+1)));
    
    // Divert --text="" to a hidden option that allows empty args
    if (s.find("--text=") == 0) {
        std::string val = s.substr(s.find("=") + 1);
        if (val == "")
            return std::make_pair(std::string("text-equals-null"), std::string());
        else
            return std::make_pair(std::string("text"), std::string(val));
    }

    return std::make_pair(std::string(), std::string());
}

// Set to detect option conflicts
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2) {
    if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted()) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: Conflicting options '" + std::string(opt1) + "' and '" + std::string(opt2) + "'.");
        exit(15);
    }
}

// Check for dependent options
void option_dependency(const prog_opts::variables_map& vm,
                        const char* option, const char* dependent_option)
{
    if (vm.count(option) && !vm[option].defaulted()) {
        if (vm.count(dependent_option) == 0) {
            throw std::logic_error(std::string("Option '") + option
                                    + "' requires option '" + dependent_option + "'.");
        }
    }
}

bool is_transformation(const srcml_input_src& input) {
  std::string ext = input.extension;

  if (ext == ".rng") {
    srcml_request.transformations.push_back(src_prefix_add_uri("relaxng", input.filename));
    return true;
  }

  if (ext == ".xsl") {
    srcml_request.transformations.push_back(src_prefix_add_uri("xslt", input.filename));
    return true;
  }

  return false;
}

element clean_element_input(const std::basic_string< char >& element_input) {
  std::string vals = element_input;
  size_t elemn_index = vals.find(":");

  // Element requires a prefix
  if (elemn_index == std::string::npos) {
    exit(1);
  }

  element elem;
  elem.prefix = vals.substr(0, elemn_index);
  elem.name = vals.substr(elemn_index + 1);
  return elem;
}

attribute clean_attribute_input(const std::basic_string< char >& attribute_input) {
  std::string vals = attribute_input;
  size_t attrib_colon = vals.find(":");
  size_t attrib_equals = vals.find("=");

  // Attribute must have a value
  if (attrib_equals == std::string::npos) {
    SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: the attribute " + vals + "is missing a value");
    exit(SRCML_STATUS_INVALID_ARGUMENT);
  }

  // Missing prefix requires an element with a prefix
  if (attrib_colon == std::string::npos && !(srcml_request.xpath_query_support.at(srcml_request.xpath_query_support.size() - 1).first)) {
    SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, "srcml: the attribute " + vals + " is missing a prefix or an element with a prefix");
    exit(SRCML_STATUS_INVALID_ARGUMENT);
  }

  attribute attrib;

  if (attrib_colon != std::string::npos) {
    attrib.prefix = vals.substr(0, attrib_colon);
    attrib.name = vals.substr(attrib_colon + 1, attrib_equals - attrib_colon - 1);
  }
  else {
    attrib.prefix = srcml_request.xpath_query_support.at(srcml_request.xpath_query_support.size() - 1).first->prefix;
    attrib.name = vals.substr(0, attrib_equals);
  }

  size_t attrib_value_start = attrib_equals + 1;

  // value may be wrapped with quotes that need to be removed
  if (vals[attrib_value_start] == '\'' || vals[attrib_value_start] == '"')
    ++attrib_value_start;

  size_t attrib_value_size = vals.size() - attrib_value_start;

  if (vals[attrib_value_start + attrib_value_size - 1] == '\'' || vals[attrib_value_start + attrib_value_size - 1] == '"')
    --attrib_value_size;

  attrib.value = vals.substr(attrib_value_start, attrib_value_size);

  return attrib;
}
