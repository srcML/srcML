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
#include <cstring>
#include <stdlib.h>
#include <SRCMLStatus.hpp>

#include <boost/optional/optional_io.hpp>
#define CLI11_BOOST_OPTIONAL 1
#include <CLI11.hpp>

// Sanitize element input
element clean_element_input(const std::string& element_input);

// Sanitize attribute input
attribute clean_attribute_input(const std::string& attribute_input);

const char* SRCML_HEADER = R"(Usage: srcml [options] <src_infile>... [-o <srcML_outfile>]
       srcml [options] <srcML_infile>... [-o <src_outfile>]

Translates C, C++, C#, and Java source code to and from the XML
source-code representation srcML. Also supports querying and transformation of srcML.

Source-code input can be from text, standard input, a file,
a directory, or an archive file, i.e., tar, cpio, and zip. Multiple files
are stored in a srcML archive.
)";

const char* SRCML_FOOTER = R"(
Have a question or need to report a bug?
Contact us at http://www.srcml.org/support.html
www.srcML.org)";

const char* SRC2SRCML_HEADER = R"(Usage: srcml [options] <src_infile>... [-o <srcML_outfile>]

  Translates C, C++, C#, and Java source code into the XML
  source-code representation srcML. Input can be from standard input, a file,
  a directory, or an archive file, i.e., tar, cpio, and zip. Multiple files
  are stored in a srcML archive.
  
  The source-code language is based on the file extension. Additional extensions
  for a language can be registered, and can be directly set using the --language
  option.
  
  By default, output is to stdout. You can specify a file for output using the
  --output or -o option. When no filenames are given, input is from stdin and
  output is to stdout. An input filename of '-' also reads from stdin.

  Any input file can be a local filename or a URI with the protocols http:,
  ftp:, or file:
)";

const char* SRC2SRCML_FOOTER = R"(Examples:

  Read from standard input, write to standard output:
  srcml
  
  Read from file m.cpp, write to standard output:
  srcml m.cpp
  
  Read from file m.cpp, write to file m.cpp.xml:
  srcml m.cpp -o m.cpp.xml
  
  Read from URI, write to standard output:
  srcml http://www.srcML.org/projects/srcml/ex/main.cpp
  
  Element unit attribute filename "m.cpp":
  srcml --filename=m.cpp m.cpp -o m.cpp.xml
  
  Set encoding of input text file to UTF-8:
  srcml --src-encoding=UTF-8 m.cpp m.cpp.xml
  
  Set encoding of srcML file to ISO-8859-1:
  srcml --xml-encoding=ISO-8859-1 m.cpp m.cpp.xml
)";

const char* SRCML2SRC_HEADER = R"(Usage: srcml [options] <srcML_infile>... [-o <src_outfile>]
  
  Translates from the the XML source-code representation srcML back to
  source-code.
  
  Extracts back to standard output or disk. Provides access to metadata about
  the srcML document. For srcML archives provides extraction of specific
  files, and efficient querying/transformation using XPath, XSLT, and RelaxNG.
  
  srcML archives contain multiple individual source code files, e.g., an
  entire project or directory tree.
  
  By default, output is to stdout.You can specify a file for output using the
  --output or -o option. When no filenames are given, input is from stdin and
  output is to stdout. An input filename of '-' also reads from stdin.
  
  Any input file, including XSLT and RelaxNG files, can be a local filename
  or a URI with the protocols http:, ftp:, or file:
  
  The srcML files can be in xml, or compressed with gzip or bzip2 (detected
  automatically).
)";

const char* SRCML2SRC_FOOTER = R"(Examples:
  
  Read from file m.cpp.xml, write to file m.cpp:
  srcml m.cpp.xml -o m.cpp
  
  Read from URI, write to file m.cpp:
  srcml http://www.example.org/m.cpp.xml m.cpp
  
  Read from file m.cpp.xml, output language attribute to stdout:
  srcml m.cpp.xml --show-language
)";


srcml_request_t srcml_request;

void positional_args(srcml_request_t& srcml_request, const std::vector<std::string>& value);

// tranformation check on input
bool is_transformation(srcml_request_t& srcml_request, const srcml_input_src& input);

class srcMLApp : public CLI::App {
public:
    srcMLApp(std::string app_description, std::string app_name) 
        : CLI::App(app_description, app_name) {

        set_help_flag("-h,--help", "Output this help message and exit")->group("GENERAL OPTIONS");
        footer_ = SRCML_FOOTER;

        // custom error message
        failure_message_ = [](const CLI::App *app, const CLI::Error &e) {

            return std::string("srcml: ") + e.what() + "\n";
        };
    }
};

class srcMLFormatter : public CLI::Formatter {
public:
    std::string make_usage(const CLI::App *, std::string) const override { return ""; }

    inline std::string make_option_opts(const CLI::Option *opt) const override {
        std::stringstream out;

        if(opt->get_type_size() != 0) {
            if(!opt->get_type_name().empty())
                out << " " << get_label(opt->get_type_name());
        }

        return out.str();
    }
};

srcml_request_t parseCLI11(int argc, char* argv[]) {

    srcml_request_t srcml_request;

    // does this need to be an optional?
    srcml_request.markup_options = 0;

    // Cleanup the arguments for some special cases:
    //      xmlns prefix: --xmlns:pre="URL" -> --xmlns=pre="URL"
    //      empty strings on long options, e.g., --text="" -> --text ""
    std::vector<std::string> commandline;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.substr(0, 8) == "--xmlns:") {
            arg[7] = '=';
        }

        if (arg.back() == '=') {
            commandline.push_back(arg.substr(0, arg.size() - 1));
            commandline.push_back("");
        } else {
            commandline.push_back(arg);
        }
    }
    std::reverse(commandline.begin(), commandline.end());

    srcMLApp app{SRCML_HEADER, "srcml"};
    app.formatter(std::make_shared<srcMLFormatter>());
    app.get_formatter()->column_width(32);

    // positional arguments, i.e., input files
    app.add_option_function<std::vector<std::string>>("InputFiles", [&](const std::vector<std::string>&) {}, "")
        ->each([&](std::string filename) {

            // record the position of stdin
            if (filename == "-" || filename == "stdin://-")
                srcml_request.stdindex = (int) srcml_request.input_sources.size();

            srcml_input_src input(filename);

            if (!(is_transformation(srcml_request, input))) {
              srcml_request.input_sources.push_back(input);
            }
        })
        ->group("");

    // general 
    app.add_flag_callback("--version,-V", [&]() { srcml_request.command |= SRCML_COMMAND_VERSION; },
        "Output version number and exit")
        ->group("GENERAL OPTIONS");

    app.add_flag_callback("--verbose,-v", [&]() { srcml_request.command |= SRCML_COMMAND_VERBOSE; },
        "Conversion and status information to stderr")
        ->group("GENERAL OPTIONS");

    app.add_flag_callback("--quiet,-q",   [&]() { srcml_request.command |= SRCML_COMMAND_QUIET; },
        "Suppress status messages")
        ->group("GENERAL OPTIONS");

    app.add_option("-o,--output",
        "Write output to FILE")
        ->each([&](std::string value) {
            srcml_request.output_filename = srcml_output_dest(value);

            if (srcml_request.output_filename.isdirectory || (srcml_request.output_filename.extension.empty()
                && srcml_request.output_filename.filename.back() == '/')) {

                srcml_request.command |= SRCML_COMMAND_TO_DIRECTORY;
                srcml_request.command |= SRCML_COMMAND_NOARCHIVE;
            }
        })
        ->type_name("FILE")
        ->group("GENERAL OPTIONS");

    srcml_request.max_threads = 4;
    app.add_option("--jobs,-j", srcml_request.max_threads,
        "Allow up to NUM threads for source parsing")
        ->type_name("NUM")
        ->group("GENERAL OPTIONS");

    // src2srcml_options "CREATING SRCML"
    app.add_option("--files-from", 
        "Input source-code filenames from FILE")
        ->each([&](const std::string& value) {
            srcml_request.files_from.push_back(value);
            srcml_request.input_sources.push_back(src_prefix_add_uri("filelist", value));
        })
        ->type_name("FILE")
        ->type_size(-1)
        ->group("CREATING SRCML");

    auto language =
    app.add_option("--language,-l", srcml_request.att_language,
        "Set the source-code language to C, C++, C#, or Java. Required for --text option")
//        ->check(CLI::IsMember({"C","C++","C#","Java"}))
        ->expected(1)
        ->type_name("LANG")
        ->group("CREATING SRCML");

    app.add_option("--text,-t", 
        "Input source code from STRING, e.g., --text=\"int a;\"")
        ->each([&](std::string text) { 
            srcml_request.input_sources.push_back(src_prefix_add_uri("text", text)); })
        ->check([&](std::string value) {
            if (!value.empty() && value[0] == '-') {
                std::cerr << "srcml: --text: 1 required STRING missing";
                exit(7);
            }
            return "";
        })
        ->type_name("STRING")
        ->expected(1)
    //    ->needs(language)
    //    ->type_size(-1)
        ->group("CREATING SRCML");
    
    app.add_option("--register-ext", srcml_request.language_ext,
        "Register file extension EXT for source-code language LANG, e.g., --register-ext h=C++")
        ->type_name("EXT=LANG")
        ->group("CREATING SRCML");
    
    app.add_option("--src-encoding", srcml_request.src_encoding,
        "Set the input source-code encoding")->type_name("ENCODING")
        ->group("CREATING SRCML");
    
    app.add_flag_callback("--archive,-r",      [&]() { *srcml_request.markup_options |= SRCML_ARCHIVE; },
        "Create a srcML archive, default for multiple input files")
        ->group("CREATING SRCML");
    
    app.add_flag_callback("--output-xml,-X",   [&]() { srcml_request.command |= SRCML_COMMAND_XML; },
        "Output in XML instead of text")
        ->group("CREATING SRCML");
    
    app.add_flag_callback("--output-xml-outer",[&]() { 
        srcml_request.command |= SRCML_COMMAND_XML_FRAGMENT;
        srcml_request.command |= SRCML_COMMAND_XML;
    },
        "Output an inner unit from an XML archive")
        ->group("CREATING SRCML");
    
    app.add_flag_callback("--output-xml-inner",[&]() {
        srcml_request.command |= SRCML_COMMAND_XML_RAW;
        srcml_request.command |= SRCML_COMMAND_XML;
    },
        "Output contents of XML unit")
        ->group("CREATING SRCML");

    // markup options
    app.add_flag_callback("--position",        [&]() { *srcml_request.markup_options |= SRCML_OPTION_POSITION; },
        "Include start and end attributes with line/column of each element")
        ->group("MARKUP OPTIONS");

    srcml_request.tabs = 8;
    app.add_option("--tabs", srcml_request.tabs,
        "Set tab stop at every NUM characters, default of 8")
        ->check(CLI::Number)
        ->each([&](std::string){ *srcml_request.markup_options |= SRCML_OPTION_POSITION; })
        ->type_name("NUM")
        ->group("MARKUP OPTIONS");

#if 0
    // check tabstop
    if (value < 1) {
        SRCMLstatus(ERROR_MSG, "srcml: %d is an invalid tab stop. Tab stops must be 1 or higher.", value);
        exit(1); //ERROR CODE TBD
    }
#endif
    app.add_flag_callback("--cpp",              [&]() { *srcml_request.markup_options |= SRCML_OPTION_CPP; },
        "Enable preprocessor parsing and markup (default for C/C++/C#)")
        ->group("MARKUP OPTIONS");

    app.add_flag_callback("--cpp-markup-if0",   [&]() { *srcml_request.markup_options |= SRCML_OPTION_CPP_MARKUP_IF0; },
        "Markup preprocessor #if 0 regions")
        ->group("MARKUP OPTIONS");

    app.add_flag_callback("--cpp-nomarkup-else",[&]() { *srcml_request.markup_options |= SRCML_OPTION_CPP_TEXT_ELSE; },
        "Do not markup preprocessor #else/#elif regions")
        ->group("MARKUP OPTIONS");

    // xml_form
    // @todo Why not check in the client?
    srcml_request.att_xml_encoding = "UTF-8";
    app.add_option("--xml-encoding", srcml_request.att_xml_encoding,
        "Set output XML encoding. Default is UTF-8")
        ->check([&](const std::string &value) { 

            if (value.empty() || srcml_check_encoding(value.c_str()) == 0) {
                return std::string("invalid xml encoding \"") + value + "\"";
            }

            return std::string("");
        })
        ->type_name("ENCODING")
        ->group("ENCODING");

    app.add_flag_callback("--no-xml-declaration",[&]() { *srcml_request.markup_options |= SRCML_OPTION_XML_DECL; },
        "Do not output the XML declaration")
        ->type_name("XML")
        ->group("ENCODING");

    app.add_option("--xmlns", "Set the default namespace URI, or declare the PRE for namespace URI")
        ->each([&](const std::string& value) {  
            auto delim = value.find("=");
            if (delim == std::string::npos) {
                srcml_request.xmlns_namespaces[""] = value;
                srcml_request.xmlns_namespace_uris[value] = "";
            } else {
                srcml_request.xmlns_namespaces[value.substr(0, delim)] = value.substr(delim + 1);
                srcml_request.xmlns_namespace_uris[value.substr(delim + 1)] = value.substr(0, delim);
            }
        })
        ->type_name("URI, PRE=URI")
        ->expected(1)
        ->group("ENCODING");

    // metadata
    app.add_flag_callback("--list,-L",        [&]() { srcml_request.command |= SRCML_COMMAND_LIST; },
        "List all files in the srcML archive and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--info,-i",        [&]() { srcml_request.command |= SRCML_COMMAND_INFO; },
        "Output most metadata except srcML file count and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--full-info,-I",   [&]() { srcml_request.command |= SRCML_COMMAND_LONGINFO; },
        "Output all metadata including srcML file count and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-language",  [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE; },
        "Output source language and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-url",       [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_URL; },
        "Output source url and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-filename",  [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_FILENAME; },
        "Output source filename and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-src-version",   [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION; },
        "Output source version and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-timestamp", [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP; },
        "Output source timestamp and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-hash",      [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_HASH; },
        "Output source hash and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-encoding",  [&]() { srcml_request.command |= SRCML_COMMAND_DISPLAY_SRCML_ENCODING; },
        "Output xml encoding and exit")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--show-unit-count",[&]() { srcml_request.command |= SRCML_COMMAND_UNITS; },
        "Output number of srcML files and exit")
        ->group("METADATA OPTIONS");

    app.add_option("--show-prefix", srcml_request.xmlns_prefix_query, 
        "Output prefix of namespace URI and exit")
        ->type_name("URI")
        ->group("METADATA OPTIONS");

    app.add_option("--filename,-f", srcml_request.att_filename,
        "Set the filename attribute")
        ->type_name("FILENAME")
        ->group("METADATA OPTIONS");

    app.add_option("--url", srcml_request.att_url,
        "Set the url attribute")
        ->type_name("URL")
        ->group("METADATA OPTIONS");

    app.add_option("--src-version,-s", srcml_request.att_version, 
        "Set the version attribute")
        ->type_name("VERSION")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--hash",      [&]() { *srcml_request.markup_options |= SRCML_HASH; },
        "Include generated hash attribute")
        ->group("METADATA OPTIONS");

    app.add_flag_callback("--timestamp", [&]() { srcml_request.command |= SRCML_COMMAND_TIMESTAMP; },
        "Include generated timestamp attribute")
        ->group("METADATA OPTIONS");

    // srcml2src
    app.add_option("--unit,-U", srcml_request.unit, 
        "Extract the source code for an individual unit at position NUM in a srcML archive")
        ->type_name("NUM")
        ->group("EXTRACTING SOURCE CODE");

    app.add_flag_callback("--output-src,-S",  [&]() { srcml_request.command |= SRCML_COMMAND_SRC; },
        "Output source code instead of srcML")
        ->group("EXTRACTING SOURCE CODE");

    app.add_option_function<std::string>("--to-dir", [&](const std::string& value) {

        srcml_request.output_filename = srcml_output_dest(value);
        srcml_request.command |= SRCML_COMMAND_TO_DIRECTORY;
        srcml_request.command |= SRCML_COMMAND_NOARCHIVE;
        return true;
    },
        "Extract source-code files to a DIRECTORY")
        ->type_name("DIRECTORY")
        ->group("EXTRACTING SOURCE CODE");

    // query/transform
    app.add_option("--xpath", 
        "Apply XPATH expression to each individual srcML unit")
        ->each([&](std::string value) {
            srcml_request.transformations.push_back(src_prefix_add_uri("xpath", value));
            srcml_request.xpath_query_support.push_back(std::make_pair(boost::none,boost::none));
        })
        ->type_name("XPATH")
        ->group("QUERY & TRANSFORMATION");

    app.add_option("--attribute", 
        "Insert attribute PRE:NAME=\"VALUE\" into element results of XPath query in original unit")
        ->each([&](std::string value) { srcml_request.xpath_query_support.back().second = clean_attribute_input(value); })
        ->check([&](std::string) {
            if (srcml_request.xpath_query_support.empty()) {

                SRCMLstatus(ERROR_MSG, "srcml: attribute option must follow an --xpath option");
                exit(SRCML_STATUS_INVALID_ARGUMENT);
            }
            return "";
        })
        ->type_name("PRE:NAME=\"VALUE\"")
        ->group("QUERY & TRANSFORMATION");

    app.add_option("--element", 
        "Insert element PRE:NAME around each element result of XPath query in original unit")
        ->each([&](std::string value) { srcml_request.xpath_query_support.back().first = clean_element_input(value); })
        ->check([&](std::string) {
            if (srcml_request.xpath_query_support.empty()) {

                SRCMLstatus(ERROR_MSG, "srcml: element option must follow an --xpath option");
                exit(SRCML_STATUS_INVALID_ARGUMENT);
            }
            return "";
        })
        ->type_name("PRE:NAME")
        ->group("QUERY & TRANSFORMATION");

    app.add_option("--xslt", 
        "Apply the XSLT program FILE to each unit, where FILE can be a url")
        ->each([&](std::string value) {
            srcml_request.transformations.push_back(src_prefix_add_uri("xslt", value));
        })
        ->type_name("FILE")
        ->group("QUERY & TRANSFORMATION");

    app.add_option("--xslt-param", 
        "Passes the string parameter NAME with UTF-8 encoded string VALUE to the XSLT program")
        ->each([&](std::string value) {
            srcml_request.transformations.push_back(src_prefix_add_uri("xslt-param", value));
        })
        ->type_name("NAME=\"VALUE\"")
        ->group("QUERY & TRANSFORMATION");

    app.add_option("--relaxng", 
        "Output individual units that match the RelaxNG pattern FILE. FILE can be a url")
        ->each([&](std::string value) {
            srcml_request.transformations.push_back(src_prefix_add_uri("relaxng", value));
        })
        ->type_name("FILE")
        ->group("QUERY & TRANSFORMATION");

    // debug
    app.add_flag_callback("--dev",          [&]() { srcml_request.command |= SRCML_DEBUG_MODE; },
        "Enable developer debug mode")
        ->group("");

    app.add_flag_callback("--timing",       [&]() { srcml_request.command |= SRCML_TIMING_MODE; },
        "Enable developer timing mode")
        ->group("");

    // experimental_options
    app.add_flag_callback("--cat",          [&]() { srcml_request.command |= SRCML_COMMAND_CAT_XML; },
        "Cat all the XML units into a single unit")
        ->group("");

    app.add_flag("--revision", srcml_request.revision, 
        "Extract the given revision (0 = original, 1 = modified)")
        ->group("");

    app.add_flag_callback("--update",       [&]() { srcml_request.command |= SRCML_COMMAND_UPDATE; },
        "Output and update existing srcml")
        ->group("");

    app.add_flag("--xml-processing", srcml_request.xml_processing,
        "Add XML processing instruction")
        ->group("");
    
    app.add_flag("--pretty", srcml_request.pretty_format,
        "Custom formatting for output")
        ->group("");
    
    app.add_flag("--external", srcml_request.external,
        "Run a user defined external script or application on srcml client output")
        ->group("");
    
    app.add_flag("--line-ending", srcml_request.line_ending,
        "Set the line endings for a desired environment \"Windows\" or \"Unix\"")
        ->group("");
    
    app.add_flag_callback("--unstable-order",   [&]() { srcml_request.command |= SRCML_COMMAND_OUTPUT_UNSTABLE_ORDER; },
        "Enable non-strict output ordering")
        ->group("");
    
    app.add_flag_callback("--parser-test",      [&]() { srcml_request.command |= SRCML_COMMAND_PARSER_TEST; },
        "Run parser tests on the input files")
        ->group("");

    try {
        app.parse(commandline);
    } catch (const CLI::ParseError &e) {
        app.exit(e);
        exit(CLI_ERROR_INVALID_ARGUMENT);
    }

    if (srcml_request.output_filename == "")
        srcml_request.output_filename = "stdout://-";

    // if no input given, then artificially put a "-" into the list of input files
    if (srcml_request.input_sources.empty()) {
        srcml_request.stdindex = 0;
        srcml_request.input_sources.push_back(srcml_input_src("stdin://-"));
    }

    if (srcml_request.input_sources.size() == 1 && srcml_request.input_sources[0].isdirectory) {
        auto url = srcml_request.input_sources[0].resource;
        while (url.length() > 0 && (url[0] == '.' || url[0] == '/')) {
            url.erase(0,1);
        }
        srcml_request.att_url = url;
    }

    return srcml_request;
}
// deprecated option command
// (This is required as non-deprecated options may use same values)
template <int command>
void option_command_deprecated(bool opt) {
    if (opt) {
      srcml_request.command |= command;

      // Notify user of deprecated options
      if (command == SRCML_COMMAND_UNITS)
        SRCMLstatus(INFO_MSG, "srcml: use of option --units or -n is deprecated");
    }
}

#if 0
// option src encoding
template <>
void option_field<&srcml_request_t::src_encoding>(const std::string& value) {

    if (value.empty() /* || srcml_check_encoding(value.c_str()) == 0 */) {
        SRCMLstatus(ERROR_MSG, "srcml: invalid src encoding \"" + value + "\"");
        exit(CLI_ERROR_INVALID_ARGUMENT);
    }
    srcml_request.src_encoding = value;
}

// option xml encoding attribute
template <>
void option_field<&srcml_request_t::att_xml_encoding>(const std::string& value) {

    // required since an error occurs if checked in client
    if (value.empty() || srcml_check_encoding(value.c_str()) == 0) {
        SRCMLstatus(ERROR_MSG, "srcml: invalid xml encoding \"%s\"", value);
        exit(CLI_ERROR_INVALID_ARGUMENT);
    }
    srcml_request.att_xml_encoding = value;
}

// option language attribute
template <>
void option_field<&srcml_request_t::att_language>(const std::string& value) {

    // check language
    if (value.empty() || srcml_check_language(value.c_str()) == 0) {
        SRCMLstatus(ERROR_MSG, "srcml: invalid language \"%s\"", value);
        exit(6); //ERROR CODE TBD
    }
    srcml_request.att_language = value;
}
#endif

#if 0
void option_xmlns_uri(const std::string& value) {
    srcml_request.xmlns_namespaces[""] = value;
    srcml_request.xmlns_namespace_uris[value] = "";
}

void option_xmlns_prefix(const std::vector<std::string>& values) {
    for (const auto& value : values ) {

      std::size_t delim = value.find("=");
      if (delim == std::string::npos) {
        SRCMLstatus(ERROR_MSG, "srcml: xmlns format missing \"=\"");
        exit(1); //ERROR CODE TBD
      }

      srcml_request.xmlns_namespaces[value.substr(0, delim)] = value.substr(delim + 1);
      srcml_request.xmlns_namespace_uris[value.substr(delim + 1)] = value.substr(0, delim);
    }
}
#endif

void positional_args(srcml_request_t& srcml_request, const std::vector<std::string>& value) {
    srcml_request.input_sources.reserve(srcml_request.input_sources.size() + value.size());

    for (const auto& iname : value) {

        // record the position of stdin
        if (iname == "-" || iname == "stdin://-")
            srcml_request.stdindex = (int) srcml_request.input_sources.size();

        srcml_input_src input(iname);

        if (!(is_transformation(srcml_request, input))) {
          srcml_request.input_sources.push_back(input);
        }
    }
}

#if 0
/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. (FROM BOOST LIBRARY EXAMPLES)*/
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2);

// Determine dependent options
void option_dependency(const prog_opts::variables_map& vm, const char* option, const char* dependent_option);
#endif

#if 0

        input_file.add("input-files", -1);

        // Check option conflicts
        conflicting_options(cli_map, "quiet", "verbose");
        conflicting_options(cli_map, "output", "to-dir");
        conflicting_options(cli_map, "cpp-text-else", "cpp-markup-else");
        conflicting_options(cli_map, "cpp-text-if0", "cpp-markup-if0");
        conflicting_options(cli_map, "output-src", "output-xml");

        // Check dependent options
        // Format: option_dependency(cli_map, [option], [option]);
        option_dependency(cli_map, "text", "language");



        // If position option is used without tabs...set default tab of 8
        if ((*srcml_request.markup_options & SRCML_OPTION_POSITION && srcml_request.tabs == 0) || srcml_request.tabs == 0)
            srcml_request.tabs = 8;

        // check namespaces
        for (size_t i = 0; i < srcml_get_namespace_size(); ++i) {
            std::string prefix = srcml_get_namespace_prefix(i);
            std::string uri = srcml_get_namespace_uri(i);

            // A reserved prefix wasn't used or it was set to the same uri
            if (srcml_request.xmlns_namespaces.find(prefix) == srcml_request.xmlns_namespaces.end() || srcml_request.xmlns_namespaces[prefix] == uri) {
                continue;
            }

            // A reserved uri is set to a different prefix
            if (srcml_request.xmlns_namespace_uris.find(uri) != srcml_request.xmlns_namespaces.end() && srcml_request.xmlns_namespace_uris[uri] != prefix) {
                continue;
            }

            SRCMLstatus(ERROR_MSG, "srcml: prefix \"" + prefix + "\" assigned multiple URIs \"" + uri + "\", \"" + srcml_request.xmlns_namespaces[prefix] + "\"");
            exit(1); // TODO Need a real error code
        }



        if (srcml_request.command & SRCML_COMMAND_XML_RAW)
            option_command<SRCML_COMMAND_XML>(true);
    }
    // Unknown Option
    catch(boost::program_options::unknown_option& e) {
        SRCMLstatus(ERROR_MSG, "srcml: %s", e.what());
        exit(3);
    }
    // Missing Option Value
    catch(boost::program_options::error_with_option_name& e) {
        std::string error_msg(e.what());
        
        /* This allows for --help with no value (currently a work around for implicit issues)
            We check the error message for a section to identify when --help is used without a value
            and call the function manually to print the appropriate help message. 
            Calls to option_help automatically exit the cli. */
        if (error_msg.find("'--help' is missing") != std::string::npos) {
                option_help("");
        }

        SRCMLstatus(ERROR_MSG, "srcml: %s", error_msg);
        exit(7);
    }
    // Catch all other issues with generic error
    catch(std::exception& e) {
        SRCMLstatus(ERROR_MSG, "srcml: %s", e.what());
        exit(1);
    }

    return srcml_request;
}
    
// Set to detect option conflicts
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2) {
    if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted()) {
        SRCMLstatus(ERROR_MSG, "srcml: Conflicting options '%s' and '%s'.", opt1, opt2);
        exit(15);
    }
}

// Check for dependent options
void option_dependency(const prog_opts::variables_map& vm,
                        const char* option, const char* dependent_option) {

    if (vm.count(option) && !vm[option].defaulted()) {
        if (vm.count(dependent_option) == 0) {
            throw std::logic_error(std::string("Option '") + option
                                    + "' requires option '" + dependent_option + "'.");
        }
    }
}
#endif

bool is_transformation(srcml_request_t& srcml_request, const srcml_input_src& input) {

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

element clean_element_input(const std::string& element_input) {
    size_t elemn_index = element_input.find(":");
    if (elemn_index == std::string::npos) {
        // Element requires a prefix
        exit(1);
    }

    return element{ element_input.substr(0, elemn_index), element_input.substr(elemn_index + 1) };
}

attribute clean_attribute_input(const std::string& attribute_input) {
    std::string vals = attribute_input;
    size_t attrib_colon = vals.find(":");
    size_t attrib_equals = vals.find("=");

    // Attribute must have a value
    if (attrib_equals == std::string::npos) {
        SRCMLstatus(ERROR_MSG, "srcml: the attribute %s is missing a value", vals);
        exit(SRCML_STATUS_INVALID_ARGUMENT);
    }

    // Missing prefix requires an element with a prefix
    if (attrib_colon == std::string::npos && !(srcml_request.xpath_query_support.back().first)) {
        SRCMLstatus(ERROR_MSG, "srcml: the attribute %s is missing a prefix or an element with a prefix", vals);
        exit(SRCML_STATUS_INVALID_ARGUMENT);
    }

    attribute attrib;

    if (attrib_colon != std::string::npos) {
        attrib.prefix = vals.substr(0, attrib_colon);
        attrib.name = vals.substr(attrib_colon + 1, attrib_equals - attrib_colon - 1);
    } else {
        attrib.prefix = srcml_request.xpath_query_support.back().first->prefix;
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
