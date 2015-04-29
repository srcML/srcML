/**
 * @pretty.cpp
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

#include <srcml_pretty.hpp>
#include <iostream>
#include <vector>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

void show_carret_error(size_t pos) {
    for (size_t i = 0; i < pos; ++i) {
        std::cerr << " ";
    }
    std::cerr << "^\n";
}

void pretty_print(const std::string& format_string, const std::vector<std::string>& args)
{
    boost::format output_string(format_string);
    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        output_string % *it;
    }

    std::cerr << output_string.str();
}

pretty_template_t split_template_sections(const std::string& pretty_input) {
	std::string input_template = pretty_input;

	size_t header_pos = input_template.find("{");
	size_t footer_pos = input_template.find("}");

	pretty_template_t output_template;

	// For %{ %} cases - we'll just pretend we didn't see them.
    if ((footer_pos != 0) && input_template[footer_pos - 1] == '%') {
        input_template.erase(footer_pos - 1, 1);
        footer_pos = std::string::npos;
    }
    if ((header_pos != 0) && input_template[header_pos - 1] == '%') {
        input_template.erase(header_pos - 1, 1);
        header_pos = std::string::npos;
    }

	// Errors: you must have matching { and }.
	if (header_pos == std::string::npos && footer_pos != std::string::npos) { 
		std::cerr << "srcml: Pretty format is missing \"{\"" << "\n";
		output_template.error_location = (size_t)0;
		return output_template;
	}
	if (header_pos != std::string::npos && footer_pos == std::string::npos) {
		std::cerr << "srcml: Pretty format is missing \"}\"" << "\n";
		output_template.error_location = (size_t)input_template.length();
		return output_template;
	}

	// Split up the header, body, and footer template strings
	if (header_pos != std::string::npos && footer_pos != std::string::npos) {
		if (header_pos > 0)
			output_template.header = input_template.substr(0, header_pos - 1);
		
		if (header_pos + 2 <= footer_pos)
			output_template.body = input_template.substr(header_pos + 2, footer_pos - header_pos - 2);

		if (!(footer_pos + 2 > input_template.length()))
			output_template.footer = input_template.substr(footer_pos + 2);
	}
	else {
		output_template.body = input_template;
	}

	return output_template;
}

boost::optional<size_t> parse_templates(std::string& template_string, std::string& section_args, const std::string& allowed_args) {
    size_t found = -1;

    while (true) {
        found = template_string.find("%", found + 1);
        
        if (found == std::string::npos) {
            break;
        }

        // For %% case - jsut ignore it
        if (((found + 1) < template_string.length()) && template_string[found + 1] == '%') {
            continue;
        }

        if ((found + 1) < template_string.length()) {
            size_t loc = allowed_args.find(template_string[found + 1]);
            
            // Error
            if (loc == std::string::npos) {
                //std::cerr << "Invalid arg: " << template_string[found + 1] << "\n";
                return found + 1;
            }

            section_args += template_string[found + 1];
            template_string[found + 1] = 's';
        }
    }

    // Find and replace for common escapes
    boost::replace_all(template_string, "\\n", "\n");
    boost::replace_all(template_string, "\\t", "\t");
    boost::replace_all(template_string, "\\r", "\r");

    /* Debug for sanity...
    std::cerr << template_string << "\n";

    if (section_args) {
        for (size_t i = 0; i < section_args->length(); ++i) {
            std::cerr << section_args->at(i);
        }

        std::cerr << "\n";
    }
    */

    return boost::none;
}

const char* acquire_metadata(srcml_archive* srcml_arch, srcml_unit* srcml_unit, const char arg) {

    /*
    PRETTY Args
    - %C: total number of units (only for footer)
    - %c: unit number *** ADDED ***
    */

    std::cerr <<  "ARG: " << arg << "\n";
    switch (arg){
        case 'D':           // %D: directory attribute on the archive
            break;
        case 'd':           // %d: directory attribute on the unit
            break;
        case 'F':           // %F: file attribute on the archive
            break;
        case 'f':           // %f: file name attribute on the unit
            break;
        case 'h':           // %h: hash attribute on the unit
            break;
        case 'i':           // %i: index of the unit within the archive
            break;
        case 'S':           // %S: source encoding attribute on the archive
            break;
        case 's':           // %s: source encoding attribute on the unit
            break;
        case 'V':           // %V: version attribute on the archive
            break;
        case 'v':           // %v: version attribute on the unit
            break;
        case 'X':           // %X: XML encoding on the archive
            break;
        case 'x':           // %x: XML encoding attribute on the unit
            break;
        default:
            break;
    }

}

void display_template(srcml_archive* srcml_arch, pretty_template_t& output_template) {
    
    // C and c are available here as they are not metadata marked up in the file.
    int unit_number = 0;
    int unit_count = 0;

    std::vector<std::string> header_params;
    std::vector<std::string> body_params;
    std::vector<std::string> footer_params;

    if (output_template.header) {
        std::cerr << "HEADER\n";
        BOOST_FOREACH(const char arg, output_template.header_args) {
            const char* param = acquire_metadata(srcml_arch, NULL, arg);
        }
    }

    srcml_unit* unit = srcml_read_unit_header(srcml_arch);

    if (output_template.body) {
        std::cerr << "BODY\n";
        while (unit) {
            BOOST_FOREACH(const char arg, output_template.body_args) {
                const char* param = acquire_metadata(srcml_arch, unit, arg);
            }
            unit = srcml_read_unit_header(srcml_arch);
        }
    }

    if (output_template.footer) {
        std::cerr << "FOOTER\n";
        BOOST_FOREACH(const char arg, output_template.footer_args) {
            const char* param = acquire_metadata(srcml_arch, NULL, arg);
        }
    }
}

// TODO: RETURN REAL ERRORS
int srcml_pretty(srcml_archive* srcml_arch, const std::string& pretty_input) {

    bool unit_args = false;

	pretty_template_t output_template = split_template_sections(pretty_input);

	if (output_template.header) {
		//std::cerr << *output_template.header << "\n";
        output_template.error_location = parse_templates(*output_template.header, output_template.header_args, valid_header_args);
        if (output_template.error_location) {
            std::cerr << "srcml: header format error:\n";
            std::cerr << *output_template.header << "\n";
            show_carret_error(*output_template.error_location);
            return -1;
        }
	}

    if (output_template.body) {
        //std::cerr << *output_template.body << "\n";
        output_template.error_location = parse_templates(*output_template.body, output_template.body_args, valid_body_args);
        if (output_template.error_location) {
            std::cerr << "srcml: body format error:\n";
            std::cerr << *output_template.body << "\n";
            show_carret_error(*output_template.error_location);
            return -1;
        }
    }

	if (output_template.footer) {
		//std::cerr << *output_template.footer << "\n";
        output_template.error_location = parse_templates(*output_template.footer, output_template.footer_args, valid_footer_args);
        if (output_template.error_location) {
            std::cerr << "srcml: footer format error:\n";
            std::cerr << *output_template.footer << "\n";
            show_carret_error(*output_template.error_location);
            return -1;
        }
	}

    display_template(srcml_arch, output_template);
    
    //std::string helloString = "Hello %s and %s \nFun time!";
    //std::vector<std::string> args;
    //args.push_back("Alice");
    //args.push_back("Bob");
    //std::cout << format_range(helloString, args) << '\n';
}
