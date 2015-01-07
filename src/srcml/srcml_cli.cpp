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

namespace prog_opts = boost::program_options;

const char* SRCML_HEADER = "";
const char* SRCML_FOOTER = "";

const char* SRC2SRCML_HEADER = "Usage: srcml [options] <src_infile>... [-o <srcML_outfile>]\
  \n\n\
  Translates C, C++, and Java source code into the XML source-code representation srcML.\n\
  Input can be from standard input, a file, a directory, or an archive file, i.e., tar, cpio, and zip.\n\
  Multiple files are stored in a srcML archive.\
  \n\n\
  The source-code language is based on the file extension.  Additional extensions for a language\n\
  can be registered, and can be directly set using the --language option.\
  \n\n\
  By default, output is to stdout.  You can specify a file for output using the --output or -o option.\n\
  When no filenames are given, input is from stdin and output is to stdout.\n\
  An input filename of '-' also reads from stdin.\
  \n\n\
  Any input file can be a local filename (FILE) or a URI with the protocols http:, ftp:, or file:\
  \n\n";

const char* SRC2SRCML_FOOTER = "Examples:\
  \n\n\
  src2srcml (read from standard input, write to standard output)\n\
  src2srcml m.cpp          (read from file m.cpp, write to standard output)\n\
  src2srcml m.cpp -o m.cpp.xml (read from file m.cpp, write to file m.cpp.xml)\n\
  \n\
  src2srcml http://www.sdml.info/projects/srcml/ex/main.cpp (read from URI)\n\
  \n\
  src2srcml --directory=src --filename=m.cpp m.cpp -o m.cpp.xml (element unit attributes dir \"src\", filename \"m.cpp\")\n\
  src2srcml --src-encoding=UTF-8 m.cpp m.cpp.xml         (encoding of input text file is UTF-8)\n\
  src2srcml --xml-encoding=ISO-8859-1 m.cpp m.cpp.xml    (set encoding of srcML file to ISO-8859-1)\n\
  \n\
  www.sdml.info\n\
  Report bugs to collard@uakron.edu";

const char* SRCML2SRC_HEADER = "Usage: srcml2src [options] <srcML_infile>... [-o <src_outfile>]\
  \n\n\
  Translates from the the XML source-code representation srcML back to source-code.\
  \n\n\
  Extracts back to standard output, the disk, or to traditional archive formats,\n\
  e.g., tar, cpio, zip, and with optional gzip, bzip2 compression.\n\
  Provides access to metadata about the srcML document. For srcML archives\n\
  provides extraction of specific files, and efficient querying/transformation\n\
  using XPath, XSLT, and RelaxNG.\
  \n\n\
  srcML archives contain multiple individual source code files,\n\
  e.g., an entire project or directory tree.\
  \n\n\
  By default, output is to stdout.  You can specify a file for output using the --output or -o option.\n\
  When no filenames are given, input is from stdin and output is to stdout.\n\
  An input filename of '-' also reads from stdin.\
  \n\n\
  Any input file, including XSLT and RelaxNG files, can be a local\n\
  filename (FILE) or a URI with the protocols http:, ftp:, or file:\
  \n\n\
  The srcML files can be in xml, or compressed with gzip or bzip2 (detected automatically).\
  \n\n";

const char* SRCML2SRC_FOOTER = "Examples:\
  \n\n\
  Read from file main.cpp.xml, write to file main.cpp:\
  \n\
  srcml main.cpp.xml -o main.cpp\
  \n\n\
  Read from URI, write to file main.cpp:\
  \n\
  srcml http://www.sdml.info/projects/srcml/ex/main.cpp.xml main.cpp\
  \n\n\
  Read from file main.cpp.xml, output language attribute to stdout:\
  \n\
  srcml main.cpp.xml --language\n\
  \n\
  www.sdml.info\n\
  Report bugs to collard@uakron.edu";

srcml_request_t srcml_request = { /* 0 */ };

// Define Program Options
prog_opts::options_description general("GENERAL OPTIONS");
prog_opts::options_description src2srcml_options("SRC2SRCML OPTIONS");
prog_opts::options_description srcml2src_options("SRCML2SRC OPTIONS");
prog_opts::options_description cpp_markup("CPP Markup Options");
prog_opts::options_description line_col("Line/Column Position");
prog_opts::options_description markup("Markup Extensions");
prog_opts::options_description src2srcml_metadata("src2srcml Metadata Options");
prog_opts::options_description srcml2src_metadata("srcml2src Metadata Options");
prog_opts::options_description prefix("Prefix Options");
prog_opts::options_description query_transform("Query and Transform Options");
prog_opts::options_description srcml_archive_options("srcML Archive Options");
prog_opts::options_description src2srcml("SRC2SRCML");
prog_opts::options_description srcml2src("SRCML2SRC");
prog_opts::options_description positional_options("positional");
prog_opts::options_description deprecated_options("Deprecated Options");
prog_opts::options_description debug_options("Debug Options");
prog_opts::options_description all("All Options");

// Positional Args
prog_opts::positional_options_description input_file;

/* DREW:  Most of the no parameter options could be recorded this way */
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

// Generic fields
template <boost::optional<std::string> srcml_request_t::*pfield>
void option_field(const std::string& value) { srcml_request.*pfield = value; }

template <std::vector<std::string> srcml_request_t::*pfield>
void option_field(const std::vector<std::string>& value) { srcml_request.*pfield = value; }

template <int srcml_request_t::*pfield>
void option_field(int value) { srcml_request.*pfield = value; }

// option files_from
template <>
void option_field<&srcml_request_t::files_from>(const std::vector<std::string>& value) {

    srcml_request.files_from = value;
    BOOST_FOREACH(const std::string& inputFile, value) {
        srcml_request.input.push_back(src_prefix_add_uri("filelist", inputFile));
    }
}

// option src encoding
template <>
void option_field<&srcml_request_t::src_encoding>(const std::string& value) {

    if (value.empty() || srcml_check_encoding(value.c_str()) == 0) {
        std::cerr << "srcml: invalid src encoding.\n";
        exit(4);
    }
    srcml_request.src_encoding = value;
}

// option xml encoding attribute
template <>
void option_field<&srcml_request_t::att_xml_encoding>(const std::string& value) {

    if (value.empty() || srcml_check_encoding(value.c_str()) == 0) {
        std::cerr << "srcml: invalid xml encoding.\n";
        exit(4);
    }
    srcml_request.att_xml_encoding = value;
}

// option language attribute
template <>
void option_field<&srcml_request_t::att_language>(const std::string& value) {

    // check language
    if (value.empty() || srcml_check_language(value.c_str()) == 0) {
        std::cerr << "srcml: invalid language.\n";
        exit(6); //ERROR CODE TBD
    }
    srcml_request.att_language = value;
}

// option tabs
template <>
void option_field<&srcml_request_t::tabs>(int value) {

    // check tabstop
    if (value < 1) {
        std::cerr << "srcml: " << value << " is an invalid tab stop. Tab stops must be 1 or higher.\n";
        exit(1); //ERROR CODE TBD
    }
    srcml_request.tabs = value;
}

template <>
void option_field<&srcml_request_t::output_filename>(const std::string& value) {
    srcml_request.output_filename = value == "-" ? "stdout://-" : value;
}

void option_xmlns_uri(const std::string& value) {
    srcml_request.xmlns_namespaces[""] = value;
}

void option_xmlns_prefix(const std::vector<std::string>& values) {
    BOOST_FOREACH( std::string value, values )
    {
      std::size_t delim = value.find("=");
      if (delim == std::string::npos)
        exit(1); //ERROR CODE TBD

      srcml_request.xmlns_namespaces[value.substr(0, delim)] = value.substr(delim + 1);
    }
}

// option output to directory
void option_to_dir(const std::string& value) {
    srcml_request.output_filename = value;
    srcml_request.command |= SRCML_COMMAND_TO_DIRECTORY;
}

void positional_args(const std::vector<std::string>& value) {
    srcml_request.input.reserve(srcml_request.input.size() + value.size());

    BOOST_FOREACH(const std::string& iname, value) {

        // record the position of stdin
        if (iname == "-" || iname == "stdin://-")
            srcml_request.stdindex = (int) srcml_request.input.size();

        srcml_request.input.push_back(src_prefix_add_uri(iname));
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
            ("compress,z", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_COMPRESS>), "output in gzip format")
            ("help,h", prog_opts::value<std::string>()->implicit_value("")->notifier(&option_help),"display this help and exit. USAGE: help or help [module name]. MODULES: src2srcml, srcml2src")
            ("no-archive", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_NOARCHIVE>), "output individual srcml units")
            ("no-namespace-decl", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_NAMESPACE_DECL>), "do not output any namespace declarations")
            ("no-xml-declaration", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_XML_DECL>), "do not output the XML declaration")
            ("output,o", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::output_filename>)->default_value("stdout://-"), "write result ouput to arg which is a FILE or URI")
            ("quiet,q", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_QUIET>), "suppresses status messages")
            ("src-encoding,t", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::src_encoding>)->default_value("ISO-8859-1"), "set the input source encoding to arg (default:  ISO-8859-1)")
            ("max-threads", prog_opts::value<int>()->notifier(&option_field<&srcml_request_t::max_threads>)->default_value(4), "set the maximum number of threads srcml can spawn")
            ("verbose,v", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_VERBOSE>), "conversion and status information to stderr")
            ("version,V", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_VERSION>), "display version number and exit")
            ("ordered", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_OUTPUT_ORDERED>), "enable strict output ordering")
            ("update", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_UPDATE>), "output and update existing srcml")
            ("line-ending", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::line_ending>), "set the line endings for a desired environment \"Windows\" or \"Unix\"")
            ("external", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::external>), "run a user defined external script or application on srcml client output")
            ;

        src2srcml_options.add_options()
            ("archive,r", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_ARCHIVE>), "store output in a srcML archive, default for multiple input files")
            ("debug,g", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_DEBUG>), "markup translation errors, namespace http://www.sdml.info/srcML/srcerr")
            ("xml-encoding,x", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_xml_encoding>)->default_value("UTF-8"),"set the output XML encoding to ENC (default:  UTF-8)")
            ("timestamp", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_TIMESTAMP>), "add timestamp to srcml output")
            ("hash", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_HASH>), "add hash to srcml output")
            ("expression,e", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_EXPRESSION>), "expression mode for translating a single expression not in a statement")
            ("files-from", prog_opts::value<std::vector<std::string> >()->notifier(&option_field<&srcml_request_t::files_from>), "read list of source file names, either FILE or URI, from arg to form a srcML archive")
            ("interactive,c", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INTERACTIVE>), "immediate output while parsing, default for keyboard input")
            ("language,l", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_language>), "set the language to C, C++, or Java")
            ("register-ext", prog_opts::value< std::vector<std::string> >()->notifier(&option_field<&srcml_request_t::language_ext>), "register file extension EXT for source-code language LANG. arg format EXT=LANG")
            ;

        srcml2src_options.add_options()
            ("get-language", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE>), "display source language and exit")
            ("get-directory", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY>), "display source directory name and exit")
            ("get-filename", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_FILENAME>), "display source filename and exit")
            ("get-src-version", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION>), "display source version and exit")
            ("get-timestamp", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP>), "display timestamp and exit")
            ("get-hash", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_HASH>), "display hash and exit")
            ("get-encoding", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DISPLAY_SRCML_ENCODING>), "display xml encoding and exit")
            ("output-xml,X", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_XML>), "output in XML instead of text")
            ("output-src,S", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_XML>), "output in text instead of XML")
            ;

        cpp_markup.add_options()
            ("cpp", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_CPP>), "preprocessor parsing and markup for Java and non-C/C++ languages")
            ("cpp-markup-else", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_CPP_MARKUP_ELSE>), "markup cpp #else regions (default)")
            ("cpp-markup-if0", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_CPP_MARKUP_IF0>), "markup cpp #if 0 regions")
            ("cpp-text-else", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_CPP_TEXT_ELSE>), "leave cpp #else regions as text")
            ("cpp-text-if0", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_CPP_TEXT_IF0>), "leave cpp #if 0 regions as text (default)")
            ;

        line_col.add_options()
            ("position", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_POSITION>), "include line/column attributes, namespace 'http://www.sdml.info/srcML/position'")
            ("tabs", prog_opts::value<int>()->notifier(&option_field<&srcml_request_t::tabs>)->default_value(8), "set tabs arg characters apart.  Default is 8")
            ;

        markup.add_options()
            ("literal", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_LITERAL>), "markup literal values, namespace 'http://www.sdml.info/srcML/literal'")
            ("modifier", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_MODIFIER>), "markup type modifiers, namespace 'http://www.sdml.info/srcML/modifier'")
            ("operator", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_OPERATOR>), "markup operators, namespace 'http://www.sdml.info/srcML/operator'")
            ("xml-processing", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::xml_processing>), "add XML processing instruction")
            ;

        src2srcml_metadata.add_options()
            ("directory,d", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_directory>), "set the arg directory attribute")
            ("filename,f", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_filename>), "set the arg filename attribute")
            ("src-version,s", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::att_version>), "set the arg version attribute")
            ;

        srcml2src_metadata.add_options()
            ("info,i", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO>), "display most metadata except file count (individual units) and exit")
            ("list", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_LIST>), "list all the files in the srcML archive and exit")
            ("longinfo,L", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_LONGINFO>), "display all metadata including file count (individual units) and exit")
            ("prefix,p", prog_opts::value<std::string>()->notifier(&option_field<&srcml_request_t::xmlns_prefix_query>), "display prefix of namespace given by URI arg and exit")
            ("count", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_UNITS>), "display number of srcML files and exit")
            ;

        prefix.add_options()
            ("xmlns", prog_opts::value<std::string>()->notifier(&option_xmlns_uri), "set the default namespace to arg")
            ("xmlns:", prog_opts::value< std::vector<std::string> >()->notifier(&option_xmlns_prefix), "set the namespace arg format PREFIX=URI")
            ;

        query_transform.add_options()
            ("apply-root", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_APPLY_ROOT>), "apply an xslt program or xpath query to the root element")
            ("relaxng", prog_opts::value< std::vector<std::string> >(), "output individual units that match RELAXNG_FILE (FILE or URI) arg")
            ("xpath", prog_opts::value< std::vector<std::string> >(), "apply XPATH expression arg to each individual unit")
            ("xpathparam", prog_opts::value< std::vector<std::string> >(), "passes a parameter NAME and VAL arg to the XSLT program. arg format NAME=VAL")
            ("xslt", prog_opts::value< std::vector<std::string> >(), "apply XSLT_FILE (FILE or URI) arg transformation to each individual unit")
            ("attribute", prog_opts::value< std::vector<std::string> >(), "add attribute to xpath query")
            ("element", prog_opts::value< std::vector<std::string> >(), "add element to xpath query")
            ;

        srcml_archive_options.add_options()
            ("to-dir", prog_opts::value<std::string>()->notifier(&option_to_dir), "extract all files from srcML and create them in the filesystem")
            ("unit,U", prog_opts::value<int>()->notifier(&option_field<&srcml_request_t::unit>), "extract individual unit number arg from srcML")
            ;

        positional_options.add_options()
            ("input-files", prog_opts::value< std::vector<std::string> >()->notifier(&positional_args), "input files")
            ;

        deprecated_options.add_options()
            ("units,n", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_UNITS>), "display number of srcML files and exit")
            ;

        debug_options.add_options()
            ("dev", prog_opts::bool_switch()->notifier(&option_command<SRCML_DEBUG_MODE>), "Enable developer debug mode.")
            ;

        // Group src2srcml Options
        src2srcml.add(src2srcml_options).add(cpp_markup).add(line_col).add(markup).add(src2srcml_metadata).add(prefix);

        // Group srcml2src Options
        srcml2src.add(srcml2src_options).add(srcml2src_metadata).add(query_transform).add(srcml_archive_options);

        // Group all Options
        all.add(general).add(src2srcml_options).add(srcml2src_options).
            add(cpp_markup).add(line_col).add(markup).add(src2srcml_metadata).
            add(srcml2src_metadata).add(prefix).add(query_transform).add(srcml_archive_options).
            add(positional_options).add(deprecated_options).add(debug_options);

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
             if (option.string_key == "element" && srcml_request.xpath_query_support.size() > 0) {
                srcml_request.xpath_query_support.at(srcml_request.xpath_query_support.size() - 1).first = clean_element_input(vals);
              }
              else if (option.string_key == "attribute" && srcml_request.xpath_query_support.size() > 0) {
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

        // Check dependent options
        option_dependency(cli_map, "no-archive", "to-dir");

        // If input was from stdin, then artificially put a "-" into the list of input files
        if (srcml_request.input.empty())
          positional_args(std::vector<std::string>(1, "stdin://-"));

#if defined(__GNUG__) && !defined(__MINGW32__)
        // automatic interactive use from stdin (not on redirect or pipe)
        if (isatty(STDIN_FILENO))
            option_command<SRCML_COMMAND_INTERACTIVE>(true);
#endif

    }
    // Unknown Option
    catch(boost::program_options::unknown_option& e) {
        std::cerr << "srcml: " << e.what() << "\n";
        exit(3);
    }
    // Missing Option Value
    catch(boost::program_options::error_with_option_name& e) {
        std::cerr << "srcml: " << e.what() << "\n";
        exit(7);
    }
    // Catch all other issues with generic error
    catch(std::exception& e) {
        std::cerr << "srcml: " << e.what() << "\n";
        exit(1);
    }
    
    return srcml_request;
}

// Custom parser for xmlns: option
std::pair<std::string, std::string> custom_parser(const std::string& s) {
    if (s.find("--xmlns:") == 0)
        return std::make_pair(std::string("xmlns:"), std::string(s.substr(s.find(":")+1)));
    else
        return std::make_pair(std::string(), std::string());
}

// Set to detect option conflicts
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2) {
    if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted()) {
        std::cerr << "srcml: " << "Conflicting options '" << opt1 << "' and '" << opt2 << "'.\n";
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
    exit(1);
  }

  // Missing prefix requires an element with a prefix
  if (attrib_colon == std::string::npos && !(srcml_request.xpath_query_support.at(srcml_request.xpath_query_support.size() - 1).first)) {
    exit(1);
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
    
  attrib.value = vals.substr(attrib_equals + 1);

  return attrib;
}
