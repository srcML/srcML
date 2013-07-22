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
// g++ -o srcml ../srcml.cpp -lboost_program_options

/*
	Arguement conflicts

	-l --language
	-x --encoding
	-d --directory
	-f --filename
	-s --src-version


*/

namespace prog_opts = boost::program_options;

int main(int argc, char * argv[]) {

	prog_opts::options_description general("Options");
	general.add_options()
		("help,h", "display this help and exit")
		("version,V", "display version number and exit")
		("archive,n", "store output in a srcML archive, default for multiple input files")
		("compress,z", "output in gzip format")
		("debug,g", "markup translation errors, namespace http://www.sdml.info/srcML/srcerr")
		("expression,e", "expression mode for translating a single expression not in a statement")
		("interactive,c", "immediate output while parsing, default for keyboard input")
		("no-namespace-decl", "do not output any namespace declarations")
		("no-xml-dexlaration", "do not output the XML declaration")
		("output,o", prog_opts::value<std::string>(), "write result ouput to arg which is a FILE or URI")
		("files-from", prog_opts::value<std::string>(), "read list of source file names, either FILE or URI, from arg to form a srcML archive")
		("register-ext", prog_opts::value<std::string>(), "register file extension EXT for source-code language LANG. arg format EXT=LANG")
		("src-encoding", prog_opts::value<std::string>(), "set the input source encoding to arg (default:  ISO-8859-1)")
		("verbose,v", "conversion and status information to stderr")
		("quiet,q", "suppresses status messages")
		("xml,X", "output in XML instead of text")
		;

	prog_opts::options_description cpp_markup("CPP Markup Options");
	cpp_markup.add_options()
		("cpp", "preprocessor parsing and markup for Java and non-C/C++ languages")
		("cpp-markup-else", "markup cpp #else regions (default)")
		("cpp-markup-if0", "markup cpp #if 0 regions")
		("cpp-text-else", "leave cpp #else regions as text")
		("cpp-text-if0", "leave cpp #if 0 regions as text (default)")
		;

	prog_opts::options_description line_col("Line/Column Position");
	line_col.add_options()
		("position", "include line/column attributes, namespace 'http://www.sdml.info/srcML/position'")
		("tabs", prog_opts::value<int>(), "set tabs arg characters apart.  Default is 8")
		;

	prog_opts::options_description markup("Markup Extensions");
	markup.add_options()
		("literal", "markup literal values, namespace 'http://www.sdml.info/srcML/literal'")
		("modifier", "markup type modifiers, namespace 'http://www.sdml.info/srcML/modifier'")
		("operator", "markup operators, namespace 'http://www.sdml.info/srcML/operator'")
		;

	prog_opts::options_description metadata("Metadata Options");
	metadata.add_options()
		("info,i", "display most metadata except file count (individual units) and exit")
		("list", "list all the files in the srcML archive and exit")
		("longinfo,L", "display all metadata including file count (individual units) and exit")
		("prefix,p", prog_opts::value<std::string>(), "display prefix of namespace given by URI arg and exit")
		("units,n", "display number of srcML files and exit")
		;

	prog_opts::options_description prefix("Prefix Options");
	prefix.add_options()
		("xmlns=", prog_opts::value<std::string>(), "set the default namespace to arg")
		("xmlns:", prog_opts::value<std::string>(), "set the namespace arg format PREFIX=URI")
		;

	prog_opts::options_description query_transform("Query and Transform Options");
	query_transform.add_options()
		("apply-root", "apply an xslt program or xpath query to the root element")
		("relaxng=", prog_opts::value<std::string>(), "output individual units that match RELAXNG_FILE (FILE or URI) arg")
		("xpath=", prog_opts::value<std::string>(), "apply XPATH expression arg to each individual unit")
		("xpathparam", prog_opts::value<std::string>(), "passes a parameter NAME and VAL arg to the XSLT program. arg format NAME=VAL")
		("xslt=", prog_opts::value<std::string>(), "apply XSLT_FILE (FILE or URI) arg transformation to each individual unit")
		;

	prog_opts::options_description srcml_archive("srcML Archive Options");
	srcml_archive.add_options()
		("to-dir", "extract all files from srcML and create them in the filesystem")
		("unit=,U", prog_opts::value<int>(), "extract individual unit number arg from srcML")
		;

	//ASSIGN THE CLI ARGS TO MAP
	prog_opts::variables_map cli_map;
	prog_opts::store(prog_opts::parse_command_line(argc, argv, general), cli_map);
	prog_opts::notify(cli_map);    
	
	//MERGE ALL OPTIONS
	prog_opts::options_description all("srcML");
  all.add(general).add(cpp_markup).add(line_col).add(markup).add(metadata).add(prefix).add(query_transform).add(srcml_archive);

  //DISPLAY HELP
	if (cli_map.count("help")) {  
		
		std::cout << "THIS IS WHERE THE HELP HEADER GOES\n\n";
	 	std::cout << all << "\n";
	 	std::cout << "THIS IS WHERE THE HELP FOOTER GOES\n";

	 	return 0;
	}

  return 0;
}

