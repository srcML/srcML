/*
  srcml.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  The srcml program to transform to/from the srcML format, plus provides a variety of
  querying and transformation features.

  Replaces the src2srcml and srcml2src of the original srcML toolkit.
*/

#include "libsrcml/srcml.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <string>
#include <map>
#include <archive.h>
#include <archive_entry.h>
//#include <curl/curl.h>
#include <pthread.h>

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

//CLI Option Functions

struct srcml_request_t {
  int command;
  int markup_options;
  std::string filename;
  std::string output;
  std::string src_encoding;
  std::string encoding;
  std::string files_from;
  std::string language;
  std::vector<std::string> register_ext;
  int tabs;
  std::string directory;
  std::string src_versions;
  std::string prefix;
  std::string xmlns_uri;
  std::vector<std::string> xmlns_prefix;
  std::string relaxng;
  std::string xpath;
  std::vector<std::string> xpathparam;
  std::string xslt;
  int unit;
  std::vector<std::string> positional_args;
};

srcml_request_t srcml_request = { 0 };

/* These are internal to srcml */
const int SRCML_COMMAND_LONGINFO = 1<<0;
const int SRCML_COMMAND_INFO = 1<<1;
const int SRCML_COMMAND_INFO_FILENAME = 1<<2;
const int SRCML_COMMAND_OPERATOR = 1<<3;
const int SRCML_COMMAND_CPP_TEXT_IF0 = 1<<4;
const int SRCML_COMMAND_CPP_MARKUP_ELSE = 1<<5;
const int SRCML_COMMAND_QUIET = 1<<6;
const int SRCML_COMMAND_VERBOSE = 1<<7;
const int SRCML_COMMAND_VERSION = 1<<8;
const int SRCML_COMMAND_DEBUG = 1<<9;
const int SRCML_COMMAND_EXPRESSION = 1<<10;
const int SRCML_COMMAND_INTERACTIVE = 1<<11;
const int SRCML_COMMAND_XML = 1<<12;
const int SRCML_COMMAND_LIST = 1<<13;
const int SRCML_COMMAND_UNITS = 1<<14;
const int SRCML_COMMAND_INFO_DIRECTORY = 1<<15;
const int SRCML_COMMAND_INFO_ENCODING = 1<<16;
const int SRCML_COMMAND_INFO_LANGUAGE = 1<<17;
const int SRCML_COMMAND_INFO_SRC_VERSION = 1<<18;
const int SRCML_COMMAND_TO_DIRECTORY = 1<<19;

// Define Program Options
prog_opts::options_description general("General Options");
prog_opts::options_description src2srcml_options("src2srcml Options");
prog_opts::options_description srcml2src_options("srcml2src Options");
prog_opts::options_description cpp_markup("CPP Markup Options");
prog_opts::options_description line_col("Line/Column Position");
prog_opts::options_description markup("Markup Extensions");
prog_opts::options_description src2srcml_metadata("Metadata Options");
prog_opts::options_description srcml2src_metadata("Metadata Options");
prog_opts::options_description prefix("Prefix Options");
prog_opts::options_description query_transform("Query and Transform Options");
prog_opts::options_description srcml_archive_options("srcML Archive Options");
prog_opts::options_description src2srcml("src2srcml");
prog_opts::options_description srcml2src("srcml2src");
prog_opts::options_description positional_options("positional");
prog_opts::options_description all("All Options");

// Positional Args
prog_opts::positional_options_description input_file;

/* DREW:  Most of the no parameter options could be recorded this way */
template <int option>
void option_markup(bool opt) {
  if (opt)
    srcml_request.markup_options |= option;
}

template <int command>
void option_command(bool opt) {
  if (opt)
    srcml_request.command |= command;
}

void option_filename(const std::string& value) { srcml_request.filename = value; }
void option_output(const std::string& value) {srcml_request.output = value;}
void option_src_encoding(const std::string& value) {srcml_request.src_encoding = value;}
void option_encoding(const std::string& value) {srcml_request.encoding = value;}
void option_files_from(const std::string& value) {srcml_request.files_from = value;}
void option_language(const std::string& value) {srcml_request.language = value;}
void option_register_ext(const std::vector<std::string>& values) {srcml_request.register_ext = values;}
void option_tabs(const int value) {srcml_request.tabs = value;}
void option_directory(const std::string& value) {srcml_request.directory = value;}
void option_src_versions(const std::string& value) {srcml_request.src_versions = value;}
void option_prefix(const std::string& value) {srcml_request.prefix = value;}
void option_xmlns_uri(const std::string& value) {srcml_request.xmlns_uri = value;}
void option_xmlns_prefix(const std::vector<std::string>& values) {srcml_request.xmlns_prefix = values;}
void option_relaxng(const std::string& value) {srcml_request.relaxng = value;}
void option_xpath(const std::string& value) {srcml_request.xpath = value;}
void option_xpathparam(const std::vector<std::string>& values) {srcml_request.xpathparam = values;}
void option_xslt(const std::string& value) {srcml_request.xslt = value;}
void option_unit(const int value) {srcml_request.unit = value;}
void positional_args(const std::vector<std::string>& value) {srcml_request.positional_args = value;}

void option_help(const std::string& help_opt) {
  if (help_opt == "") {
  //MIGHT NEED A NEW HEADER AND FOOTER FOR THE GENERAL OPTION
    std::cout << SRCML_HEADER << "\n";
    std::cout << general << "\n";
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
      << help_opt << "' in the --help-module option\n";
    exit(1);
  }
}

//OPTIONS DEFINED BY srcml.h
//void option_literal(bool opt) {};
//void option_modifier(bool opt) {};
//void option_archive(bool opt) {}
//void option_position(bool opt) {}
//void option_compress(bool opt) {}
//void option_no_xml_dexlaration(bool opt) {}
//void option_no_namespace_decl(bool opt) {}
//void option_cpp(bool opt) {}
//void option_cpp_text_else(bool opt) {}
//void option_cpp_markup_if0(bool opt) {}
//void option_apply_root(bool opt) {}

//INTERNAL SRCML COMMANDS
//void option_longinfo(bool opt) {}
//void option_info(bool opt) {}
//void option_show_filename(bool opt) {} //INFO_FILENAME?
//void option_operator(bool opt) {}
//void option_cpp_text_if0(bool opt) {}
//void option_cpp_markup_else(bool opt) {}
//void option_quiet(bool opt) {}
//void option_verbose(bool opt) {}
//void option_version(bool opt) {}
//void option_debug(bool opt) {}
//void option_expression(bool opt) {}
//void option_interactive(bool opt) {}
//void option_xml(bool opt) {}
//void option_list(bool opt) {}
//void option_units(bool opt) {}
//void option_show_directory(bool opt) {}
//void option_show_encoding(bool opt) {}
//void option_show_language(bool opt) {}
//void option_show_src_version(bool opt) {}
//void option_to_dir(bool opt) {}

/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. (FROM BOOST LIBRARY EXAMPLES)*/
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2);

// Custom Parser Definition
std::pair<std::string, std::string> custom_parser(const std::string& s);

int main(int argc, char * argv[]) {
  try {
    general.add_options()
      ("compress,z", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_COMPRESS>), "output in gzip format")
      ("help,h", prog_opts::value<std::string>()->implicit_value("")->notifier(&option_help),"display this help and exit. USAGE: help or help [module name]. MODULES: src2srcml, srcml2src")
      ("no-namespace-decl", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_NO_NAMESPACE_DECL>), "do not output any namespace declarations")
      ("no-xml-declaration", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_NO_XML_DECL>), "do not output the XML declaration")
      ("output,o", prog_opts::value<std::string>()->notifier(&option_output), "write result ouput to arg which is a FILE or URI")
      ("quiet,q", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_QUIET>), "suppresses status messages")
      ("src-encoding,t", prog_opts::value<std::string>()->notifier(&option_src_encoding), "set the input source encoding to arg (default:  ISO-8859-1)")
      ("verbose,v", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_VERBOSE>), "conversion and status information to stderr")    
      ("version,V", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_VERSION>), "display version number and exit")
      ;

    src2srcml_options.add_options()
      ("archive,r", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_ARCHIVE>), "store output in a srcML archive, default for multiple input files")
      ("debug,g", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_DEBUG>), "markup translation errors, namespace http://www.sdml.info/srcML/srcerr")
      ("encoding,x", prog_opts::value<std::string>()->notifier(&option_encoding),"set the output XML encoding to ENC (default:  UTF-8)")
      ("expression,e", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_EXPRESSION>), "expression mode for translating a single expression not in a statement")
      ("files-from", prog_opts::value<std::string>()->notifier(&option_files_from), "read list of source file names, either FILE or URI, from arg to form a srcML archive")
      ("interactive,c", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INTERACTIVE>), "immediate output while parsing, default for keyboard input")
      ("language,l", prog_opts::value<std::string>()->notifier(&option_language), "set the language to C, C++, or Java")
      ("register-ext", prog_opts::value< std::vector<std::string> >()->notifier(&option_register_ext), "register file extension EXT for source-code language LANG. arg format EXT=LANG")
      ;

    srcml2src_options.add_options()
      ("xml,X", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_XML>), "output in XML instead of text")
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
      ("tabs", prog_opts::value<int>()->notifier(&option_tabs)->default_value(8), "set tabs arg characters apart.  Default is 8")
      ;

    markup.add_options()
      ("literal", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_LITERAL>), "markup literal values, namespace 'http://www.sdml.info/srcML/literal'")
      ("modifier", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_MODIFIER>), "markup type modifiers, namespace 'http://www.sdml.info/srcML/modifier'")
      ("operator", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_OPERATOR>), "markup operators, namespace 'http://www.sdml.info/srcML/operator'")
      ;
      
    src2srcml_metadata.add_options()
      ("directory,d", prog_opts::value<std::string>()->notifier(&option_directory), "set the arg directory attribute")
      ("filename,f", prog_opts::value<std::string>()->notifier(&option_filename), "set the arg filename attribute")
      ("src-version,s", prog_opts::value<std::string>()->notifier(&option_src_versions), "set the arg version attribute")
      ;

    srcml2src_metadata.add_options()
      ("info,i", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO>), "display most metadata except file count (individual units) and exit")
      ("list", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_LIST>), "list all the files in the srcML archive and exit")
      ("longinfo,L", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_LONGINFO>), "display all metadata including file count (individual units) and exit")
      ("prefix,p", prog_opts::value<std::string>()->notifier(&option_prefix), "display prefix of namespace given by URI arg and exit")
      ("units,n", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_UNITS>), "display number of srcML files and exit")
      ("show-directory", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO_DIRECTORY>), "display source directory name and exit")
      ("show-encoding", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO_ENCODING>), "display xml encoding and exit")
      ("show-filename", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO_FILENAME>), "display source filename and exit")
      ("show-language", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO_LANGUAGE>), "display source language and exit")
      ("show-src-version", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_INFO_SRC_VERSION>), "display source version and exit")
      ;

    prefix.add_options()
      ("xmlns", prog_opts::value<std::string>()->notifier(&option_xmlns_uri), "set the default namespace to arg")
      ("xmlns:", prog_opts::value< std::vector<std::string> >()->notifier(&option_xmlns_prefix), "set the namespace arg format PREFIX=URI")
      ;

    query_transform.add_options()
      ("apply-root", prog_opts::bool_switch()->notifier(&option_markup<SRCML_OPTION_APPLY_ROOT>), "apply an xslt program or xpath query to the root element")
      ("relaxng", prog_opts::value<std::string>()->notifier(&option_relaxng), "output individual units that match RELAXNG_FILE (FILE or URI) arg")
      ("xpath", prog_opts::value<std::string>()->notifier(&option_xpath), "apply XPATH expression arg to each individual unit")
      ("xpathparam", prog_opts::value< std::vector<std::string> >()->notifier(&option_xpathparam), "passes a parameter NAME and VAL arg to the XSLT program. arg format NAME=VAL")
      ("xslt", prog_opts::value<std::string>()->notifier(&option_xslt), "apply XSLT_FILE (FILE or URI) arg transformation to each individual unit")
      ;

    srcml_archive_options.add_options()
      ("to-dir", prog_opts::bool_switch()->notifier(&option_command<SRCML_COMMAND_TO_DIRECTORY>), "extract all files from srcML and create them in the filesystem")
      ("unit,U", prog_opts::value<int>()->notifier(&option_unit), "extract individual unit number arg from srcML")
      ;
    
    positional_options.add_options()
      ("input-files", prog_opts::value< std::vector<std::string> >()->notifier(&positional_args), "input files")
      ;

    //Group src2srcml Options
    src2srcml.add(general).add(src2srcml_options).add(cpp_markup).add(line_col).add(markup).add(src2srcml_metadata).add(prefix);

    //Group srcml2src Options
    srcml2src.add(general).add(srcml2src_options).add(src2srcml_metadata).add(query_transform).add(srcml_archive_options);

    //Group all Options
    all.add(general).add(src2srcml_options).add(srcml2src_options).
      add(cpp_markup).add(line_col).add(markup).add(src2srcml_metadata).
      add(srcml2src_metadata).add(prefix).add(query_transform).add(srcml_archive_options).
      add(positional_options);

    //Positional Args
    input_file.add("input-files", -1);

    //ASSIGN THE CLI ARGS TO MAP
    prog_opts::variables_map cli_map;
    prog_opts::store(prog_opts::command_line_parser(argc, argv).options(all).
      positional(input_file).extra_parser(custom_parser).run(), cli_map);
    prog_opts::notify(cli_map);

    //CHECK OPTION CONFLICTS
    conflicting_options(cli_map, "quiet", "verbose"); 

  }
  catch(std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  
  

  if (!srcml_request.positional_args.empty()) {

    struct srcml_archive* archive;
    struct srcml_unit* unit;

    /* create a new srcml archive structure */
    archive = srcml_create_archive();

    /* open a srcML archive for output */
    srcml_write_open_filename(archive, "project.xml");

    /* add all the files to the archive */
    for(int i = 0; i < srcml_request.positional_args.size(); ++i) {
      std::cout << "ARG #" << i+1 << " " << srcml_request.positional_args[i] << "\n";
      unit = srcml_create_unit(archive);

      srcml_unit_set_filename(unit, srcml_request.positional_args[i].c_str());

      /* Translate to srcml and append to the archive */
      srcml_parse_unit_filename(unit, srcml_request.positional_args[i].c_str());

      /* Translate to srcml and append to the archive */
      srcml_write_unit(archive, unit);

      srcml_free_unit(unit);
    }

    /* close the srcML archive */
    srcml_close_archive(archive);

    /* free the srcML archive data */
    srcml_free_archive(archive);
  }

  //NEED THESE LATER
  //archive *arch = archive_read_new();
  //archive_entry *archEntry = archive_entry_new();
  //CURL *handle = curl_easy_init();

  return 0;
}

std::pair<std::string, std::string> custom_parser(const std::string& s) {
  if (s.find("--xmlns:") == 0) {
    return std::make_pair(std::string("xmlns:"), std::string(s.substr(s.find(":")+1)));
  }
  else {
    return std::make_pair(std::string(), std::string());
  }
}

void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2) {
  if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted()) {
    throw std::logic_error(std::string("Conflicting options '")
      + opt1 + "' and '" + opt2 + "'.");
  } 
}
