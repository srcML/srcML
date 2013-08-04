/*
  srcml.cpp

  Copyright (C) 2013  SDML (www.sdml.info)

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
*/

#include <iostream>
#include <boost/program_options.hpp>
#include <string>

//Compiled with:
// g++ -o srcml ./srcml.cpp -lboost_program_options

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
void option_compress(const bool& opt);
void option_help(const std::string& opt);
void option_no_namespace_decl(const bool& opt);
void option_no_xml_dexlaration(const bool& opt);
void option_output(const std::string& opt);
void option_quiet(const bool& opt);
void option_src_encoding(const std::string& opt);
void option_verbose(const bool& opt);
void option_version(const bool& opt);

void option_archive(const bool& opt);
void option_debug(const bool& opt);
void option_encoding(const std::string& opt);
void option_expression(const bool& opt);
void option_files_from(const std::string& opt);
void option_interactive(const bool& opt);
void option_language(const std::string& opt);
void option_register_ext(const std::string& opt);

void option_xml(const bool& opt);

void option_cpp(const bool& opt);
void option_cpp_markup_else(const bool& opt);
void option_cpp_markup_if0(const bool& opt);
void option_cpp_text_else(const bool& opt);
void option_cpp_text_if0(const bool& opt);

void option_position(const bool& opt);
void option_tabs(const int& opt);

void option_literal(const bool& opt);
void option_modifier(const bool& opt);
void option_operator(const bool& opt);

void option_directory(const std::string& opt);
void option_filename(const std::string& opt);
void option_src_versions(const std::string& opt);

void option_info(const bool& opt);
void option_list(const bool& opt);
void option_longinfo(const bool& opt);
void option_prefix(const std::string& opt);
void option_units(const bool& opt);
void option_show_directory(const bool& opt);
void option_show_encoding(const bool& opt);
void option_show_filename(const bool& opt);
void option_show_language(const bool& opt);
void option_show_src_version(const bool& opt);

void option_xmlns_uri(const std::string& opt);
void option_xmlns_prefix(const std::string& opt);

void option_apply_root(const bool& opt);
void option_relaxng(const std::string& opt);
void option_xpath(const std::string& opt);
void option_xpathparam(const std::string& opt);
void option_xslt(const std::string& opt);

void option_to_dir(const bool& opt);
void option_unit(const int& opt);

void positional_args(const std::string& opt);

/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. (FROM BOOST LIBRARY EXAMPLES)*/
void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2);

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
prog_opts::options_description srcml_archive("srcML Archive Options");
prog_opts::options_description src2srcml("src2srcml");
prog_opts::options_description srcml2src("srcml2src");
prog_opts::options_description positional_options("positional");
prog_opts::options_description all("All Options");

// Positional Args
prog_opts::positional_options_description input_file;

int main(int argc, char * argv[]) {
	try {
		general.add_options()
			("compress,z", prog_opts::bool_switch()->notifier(&option_compress), "output in gzip format")
			("help,h", prog_opts::value<std::string>()->implicit_value("")->notifier(&option_help),"display this help and exit. USAGE: help or help [module name]. MODULES: src2srcml, srcml2src")
			("no-namespace-decl", prog_opts::bool_switch()->notifier(&option_no_namespace_decl), "do not output any namespace declarations")
			("no-xml-dexlaration", prog_opts::bool_switch()->notifier(&option_no_xml_dexlaration), "do not output the XML declaration")
			("output=,o", prog_opts::value<std::string>()->notifier(&option_output), "write result ouput to arg which is a FILE or URI")
			("quiet,q", prog_opts::bool_switch()->notifier(&option_quiet), "suppresses status messages")
			("src-encoding=,t", prog_opts::value<std::string>()->notifier(&option_src_encoding), "set the input source encoding to arg (default:  ISO-8859-1)")
			("verbose,v", prog_opts::bool_switch()->notifier(&option_verbose), "conversion and status information to stderr")		
			("version,V", prog_opts::bool_switch()->notifier(&option_version), "display version number and exit")
			;

		src2srcml_options.add_options()
			("archive,n", prog_opts::bool_switch()->notifier(&option_archive), "store output in a srcML archive, default for multiple input files")
			("debug,g", prog_opts::bool_switch()->notifier(&option_debug), "markup translation errors, namespace http://www.sdml.info/srcML/srcerr")
			("encoding=,x", prog_opts::value<std::string>()->notifier(&option_encoding),"set the output XML encoding to ENC (default:  UTF-8)")
			("expression,e", prog_opts::bool_switch()->notifier(&option_expression), "expression mode for translating a single expression not in a statement")
			("files-from", prog_opts::value<std::string>()->notifier(&option_files_from), "read list of source file names, either FILE or URI, from arg to form a srcML archive")
			("interactive,c", prog_opts::bool_switch()->notifier(&option_interactive), "immediate output while parsing, default for keyboard input")
			("language=,l", prog_opts::value<std::string>()->notifier(&option_language), "set the language to C, C++, or Java")
			("register-ext", prog_opts::value<std::string>()->notifier(&option_register_ext), "register file extension EXT for source-code language LANG. arg format EXT=LANG")
			;

		srcml2src_options.add_options()
			("xml,X", prog_opts::bool_switch()->notifier(&option_xml), "output in XML instead of text")
			;

		cpp_markup.add_options()
			("cpp", prog_opts::bool_switch()->notifier(&option_cpp), "preprocessor parsing and markup for Java and non-C/C++ languages")
			("cpp-markup-else", prog_opts::bool_switch()->notifier(&option_cpp_markup_else), "markup cpp #else regions (default)")
			("cpp-markup-if0", prog_opts::bool_switch()->notifier(&option_cpp_markup_if0), "markup cpp #if 0 regions")
			("cpp-text-else", prog_opts::bool_switch()->notifier(&option_cpp_text_else), "leave cpp #else regions as text")
			("cpp-text-if0", prog_opts::bool_switch()->notifier(&option_cpp_text_if0), "leave cpp #if 0 regions as text (default)")
			;

		line_col.add_options()
			("position", prog_opts::bool_switch()->notifier(&option_position), "include line/column attributes, namespace 'http://www.sdml.info/srcML/position'")
			("tabs=", prog_opts::value<int>()->notifier(&option_tabs), "set tabs arg characters apart.  Default is 8")
			;

		markup.add_options()
			("literal", prog_opts::bool_switch()->notifier(&option_literal), "markup literal values, namespace 'http://www.sdml.info/srcML/literal'")
			("modifier", prog_opts::bool_switch()->notifier(&option_modifier), "markup type modifiers, namespace 'http://www.sdml.info/srcML/modifier'")
			("operator", prog_opts::bool_switch()->notifier(&option_operator), "markup operators, namespace 'http://www.sdml.info/srcML/operator'")
			;

		src2srcml_metadata.add_options()
			("directory=,d", prog_opts::value<std::string>()->notifier(&option_directory), "set the arg directory attribute")
			("filename=,f", prog_opts::value<std::string>()->notifier(&option_filename), "set the arg filename attribute")
			("src-version=,s", prog_opts::value<std::string>()->notifier(&option_src_versions), "set the arg version attribute")
			;

		srcml2src_metadata.add_options()
			("info,i", prog_opts::bool_switch()->notifier(&option_info), "display most metadata except file count (individual units) and exit")
			("list", prog_opts::bool_switch()->notifier(&option_list), "list all the files in the srcML archive and exit")
			("longinfo,L", prog_opts::bool_switch()->notifier(&option_longinfo), "display all metadata including file count (individual units) and exit")
			("prefix=,p", prog_opts::value<std::string>()->notifier(&option_prefix), "display prefix of namespace given by URI arg and exit")
			("units,n", prog_opts::bool_switch()->notifier(&option_units), "display number of srcML files and exit")
			("show-directory", prog_opts::bool_switch()->notifier(&option_show_directory), "display source directory name and exit")
			("show-encoding", prog_opts::bool_switch()->notifier(&option_show_encoding), "display xml encoding and exit")
			("show-filename", prog_opts::bool_switch()->notifier(&option_show_filename), "display source filename and exit")
			("show-language", prog_opts::bool_switch()->notifier(&option_show_language), "display source language and exit")
			("show-src-version", prog_opts::bool_switch()->notifier(&option_show_src_version), "display source version and exit")
			;

		prefix.add_options()
			("xmlns=", prog_opts::value<std::string>()->notifier(&option_xmlns_uri), "set the default namespace to arg")
			("xmlns:", prog_opts::value<std::string>()->notifier(&option_xmlns_prefix), "set the namespace arg format PREFIX=URI")
			;

		query_transform.add_options()
			("apply-root", prog_opts::bool_switch()->notifier(&option_apply_root), "apply an xslt program or xpath query to the root element")
			("relaxng=", prog_opts::value<std::string>()->notifier(&option_relaxng), "output individual units that match RELAXNG_FILE (FILE or URI) arg")
			("xpath=", prog_opts::value<std::string>()->notifier(&option_xpath), "apply XPATH expression arg to each individual unit")
			("xpathparam", prog_opts::value<std::string>()->notifier(&option_xpathparam), "passes a parameter NAME and VAL arg to the XSLT program. arg format NAME=VAL")
			("xslt=", prog_opts::value<std::string>()->notifier(&option_xslt), "apply XSLT_FILE (FILE or URI) arg transformation to each individual unit")
			;

		srcml_archive.add_options()
			("to-dir", prog_opts::bool_switch()->notifier(&option_to_dir), "extract all files from srcML and create them in the filesystem")
			("unit=,U", prog_opts::value<int>()->notifier(&option_unit), "extract individual unit number arg from srcML")
			;    

		positional_options.add_options()
			("input-file", prog_opts::value<std::string>()->notifier(&positional_args), "input file")
			;

		//Group src2srcml Options
		src2srcml.add(general).add(src2srcml_options).add(cpp_markup).add(line_col).add(markup).add(src2srcml_metadata).add(prefix);

		//Group srcml2src Options
		srcml2src.add(general).add(srcml2src_options).add(src2srcml_metadata).add(query_transform).add(srcml_archive);

		//Group all Options
		all.add(general).add(src2srcml_options).add(srcml2src_options).
			add(cpp_markup).add(line_col).add(markup).add(src2srcml_metadata).
			add(srcml2src_metadata).add(prefix).add(query_transform).add(srcml_archive).
			add(positional_options);

		//Positional Args
		input_file.add("input-file", 1);

		//ASSIGN THE CLI ARGS TO MAP
		prog_opts::variables_map cli_map;
		prog_opts::store(prog_opts::command_line_parser(argc, argv).options(all).positional(input_file).run(), cli_map);
		prog_opts::notify(cli_map);

		//CHECK OPTION CONFLICTS
		conflicting_options(cli_map, "quiet", "verbose");	
	
	}
	catch(std::exception& e) {
  	std::cerr << e.what() << "\n";
  	return 1;
  }

  return 0;
}

void conflicting_options(const prog_opts::variables_map& vm, const char* opt1, const char* opt2) {
	if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted()) {
		throw std::logic_error(std::string("Conflicting options '")
			+ opt1 + "' and '" + opt2 + "'.");
	} 
}

void option_compress(const bool& opt) {
	
}

void option_help(const std::string& help_opt) {
	if (help_opt == ""){
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

void option_no_namespace_decl(const bool& opt){

}

void option_no_xml_dexlaration(const bool& opt){

}

void option_output(const std::string& opt){

}

void option_quiet(const bool& opt){

}

void option_src_encoding(const std::string& opt){

}

void option_verbose(const bool& opt){

}

void option_version(const bool& opt){

}

void option_archive(const bool& opt){

}

void option_debug(const bool& opt){

}

void option_encoding(const std::string& opt){

}

void option_expression(const bool& opt){

}

void option_files_from(const std::string& opt){

}

void option_interactive(const bool& opt){

}

void option_language(const std::string& opt){

}

void option_register_ext(const std::string& opt){

}

void option_xml(const bool& opt){

}

void option_cpp(const bool& opt){

}

void option_cpp_markup_else(const bool& opt){

}

void option_cpp_markup_if0(const bool& opt){

}

void option_cpp_text_else(const bool& opt){

}

void option_cpp_text_if0(const bool& opt){

}

void option_position(const bool& opt){

}

void option_tabs(const int& opt){

}

void option_literal(const bool& opt){

}

void option_modifier(const bool& opt){

}

void option_operator(const bool& opt){

}

void option_directory(const std::string& opt){

}

void option_filename(const std::string& opt){

}

void option_src_versions(const std::string& opt){

}

void option_info(const bool& opt){

}

void option_list(const bool& opt){

}

void option_longinfo(const bool& opt){

}

void option_prefix(const std::string& opt){

}

void option_units(const bool& opt){

}

void option_show_directory(const bool& opt){

}

void option_show_encoding(const bool& opt){

}

void option_show_filename(const bool& opt){

}

void option_show_language(const bool& opt){

}

void option_show_src_version(const bool& opt){

}

void option_xmlns_uri(const std::string& opt){

}

void option_xmlns_prefix(const std::string& opt){

}

void option_apply_root(const bool& opt){

}

void option_relaxng(const std::string& opt){

}

void option_xpath(const std::string& opt){

}

void option_xpathparam(const std::string& opt){

}

void option_xslt(const std::string& opt){

}

void option_to_dir(const bool& opt){

}

void option_unit(const int& opt){

}

void positional_args(const std::string& opt){

}
