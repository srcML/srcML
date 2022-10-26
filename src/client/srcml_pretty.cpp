// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_pretty.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml_pretty.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <SRCMLStatus.hpp>
#include <srcml_utilities.hpp>

using namespace ::std::literals::string_view_literals;

std::string expand_namespace(std::string_view separator, size_t ns_size) {
    std::string ns = "";
    for (size_t i = 0; i < ns_size; ++i) {
        ns += "%s";
        if ((i + 1) < ns_size) {
            ns += separator;
        }
    }
    return ns;
}

void show_carret_error(size_t pos) {
    std::string spacing(pos, ' ');
    SRCMLstatus(WARNING_MSG, spacing + "^");
}

void pretty_print(std::string_view format, const std::vector<std::string>& args) {

    std::vector<std::string> thisargs(args.rbegin(), args.rend());

    // replace the first argument in the format with the value
    // note: Ignoring the format type
    std::ostringstream format_string;
    const char* s = format.data();
    while (s && *s) {
        if (*s == '%' && *++s != '%' && !thisargs.empty()) {
            auto value = thisargs.back();
            thisargs.pop_back();
            format_string << value;
            ++s;
            continue;
        }

        format_string << *s++;
    }

    std::cout << format_string.str();
}

pretty_template_t split_template_sections(std::string_view pretty_input) {
    std::string input_template(pretty_input);

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
        SRCMLstatus(WARNING_MSG, "srcml: Pretty format is missing \"{\"");
        output_template.error_location = (size_t)0;
        return output_template;
    }
    if (header_pos != std::string::npos && footer_pos == std::string::npos) {
        SRCMLstatus(WARNING_MSG, "srcml: Pretty format is missing \"}\"");
        output_template.error_location = (size_t)input_template.length();
        return output_template;
    }

    // Split up the header, body, and footer template strings
    if (header_pos != std::string::npos && footer_pos != std::string::npos) {
        if (header_pos > 0) {
            output_template.header = input_template.substr(0, header_pos - 1);
        }

        if (header_pos + 2 <= footer_pos) {
            output_template.body = input_template.substr(header_pos + 2, footer_pos - header_pos - 3);
        }

        if (!(footer_pos + 2 > input_template.length())) {
            output_template.footer = input_template.substr(footer_pos + 2);
        }
    }
    else {
        output_template.body = input_template;
    }

    return output_template;
}

std::optional<size_t> parse_templates(std::string& template_string, std::vector<std::string>& section_args, std::string_view allowed_args, size_t ns_size) {

    size_t found = 0;
    bool first = true;

    while (true) {
        std::string template_arg = "";
        std::string replace_arg = "%s";

        if (first)
            found = template_string.find("%", 0);
        else
            found = template_string.find("%", found + 1);
        first = false;

        if (found == std::string::npos) {
            break;
        }

        // For %% case - just ignore it
        if (((found + 1) < template_string.length()) && (template_string[found + 1] == '%' || template_string[found + 1] == 's')) {
            continue;
        }

        if ((found + 1) < template_string.length()) {
            size_t valid_arg = allowed_args.find(template_string[found + 1]);

            // Error
            if (valid_arg == std::string::npos) {
                return found + 1;
            }

            template_arg += template_string[found + 1];

            if (template_arg == "N"sv) {
                replace_arg = expand_namespace(" ", ns_size);
                if ((found + 3) < template_string.length()) {
                    if (template_string[found + 2] == ':' && template_string[found + 3] == 'u'){
                        template_arg += ":u";
                        template_string.erase(found + 2, 2);
                    }

                    if (template_string[found + 2] == ':' && template_string[found + 3] == 'p'){
                        template_arg += ":p";
                        template_string.erase(found + 2, 2);
                    }
                }
            }

            template_string.erase(found, 2);
            section_args.push_back(template_arg);
            template_string.insert(found, replace_arg);
        }
    }

    return std::nullopt;
}

const char* acquire_metadata(srcml_archive* srcml_arch, srcml_unit* srcml_unit, std::string_view arg) {

        if (arg == "f"sv)           // %f: file name attribute on the unit
            return srcml_unit_get_filename(srcml_unit);

        if (arg == "h"sv)           // %h: hash attribute on the unit
            return srcml_unit_get_hash(srcml_unit);

        if (arg == "l"sv)           // %l: unit language
            return srcml_unit_get_language(srcml_unit);

        if (arg == "S"sv)           // %S: source encoding attribute on the archive
            return srcml_archive_get_src_encoding(srcml_arch);

        if (arg == "s"sv)           // %s: source encoding attribute on the unit
            return srcml_unit_get_src_encoding(srcml_unit);

        if (arg == "t"sv)           // %t: timestamp on the unit
            return srcml_unit_get_timestamp(srcml_unit);

        if (arg == "U"sv)           // %U: url attribute on the archive
            return srcml_archive_get_url(srcml_arch);

        if (arg == "V"sv)           // %V: version attribute on the archive
            return srcml_archive_get_version(srcml_arch);

        if (arg == "v"sv)           // %v: version attribute on the unit
            return srcml_unit_get_version(srcml_unit);

        if (arg == "X"sv)           // %X: XML encoding on the archive
            return srcml_archive_get_xml_encoding(srcml_arch);

    return "???";
}

void display_template(srcml_archive* srcml_arch, pretty_template_t& output_template, size_t ns_size, int unit_num, bool xml) {

    // C and c are available here as they are not metadata marked up in the file.
    int unit_count = 0;

    std::vector<std::string> header_params;
    std::vector<std::string> body_params;
    std::vector<std::string> footer_params;

    if (output_template.header) {
        for (const auto& arg : output_template.header_args) {

            const char* param = acquire_metadata(srcml_arch, nullptr, arg);
            if (param) {
                header_params.emplace_back(param);
            }
            else {
                if (output_template.header_args.size() > 1)
                    header_params.push_back("");
            }
        }

        if (header_params.size() != 0)
            pretty_print(*output_template.header, header_params);
    }

    std::unique_ptr<srcml_unit> unit;

    if (unit_num > 0 && !(xml)) {
        while (unit_count != unit_num) {
            srcml_archive_skip_unit(srcml_arch);
            ++unit_count;
        }
    }
    else {
        unit.reset(srcml_archive_read_unit(srcml_arch));
    }

    if (output_template.body) {
        while (unit) {
            for (const auto& arg : output_template.body_args) {
                if (arg == "i"sv) {
                    body_params.push_back(std::to_string(unit_count));
                }
                else if (arg == "N"sv) {
                    for (size_t i = 0; i < ns_size; ++i) {
                        if (srcml_archive_get_namespace_uri(srcml_arch, i)) {
                            if (srcml_archive_get_namespace_prefix(srcml_arch, i)[0] == '\0') {
                                body_params.push_back("xmlns=" + std::string(srcml_archive_get_namespace_uri(srcml_arch, i)));
                            }
                            else{
                                body_params.push_back("xmlns:" + std::string(srcml_archive_get_namespace_prefix(srcml_arch, i)) + "=" + srcml_archive_get_namespace_uri(srcml_arch, i));
                            }
                        }
                    }
                }
                else if (arg == "N:u"sv) {
                    for (size_t i = 0; i < ns_size; ++i) {
                        if (srcml_archive_get_namespace_uri(srcml_arch, i)) {
                            body_params.emplace_back(srcml_archive_get_namespace_uri(srcml_arch, i));
                        }
                    }
                }
                else if (arg == "N:p"sv) {
                    for (size_t i = 0; i < ns_size; ++i) {
                        if (srcml_archive_get_namespace_uri(srcml_arch, i)) {
                            if (srcml_archive_get_namespace_prefix(srcml_arch, i)[0] == '\0') {
                                body_params.emplace_back("");
                            }
                            else{
                                body_params.emplace_back(srcml_archive_get_namespace_prefix(srcml_arch, i));
                            }
                        }
                    }
                }
                else {
                    const char* param = acquire_metadata(srcml_arch, unit.get(), arg);
                    if (param) {
                        body_params.emplace_back(param);
                    }
                    else {
                        if (output_template.body_args.size() > 1)
                            body_params.emplace_back("");
                    }
                }
            }

            if (body_params.size() != 0)
                pretty_print(*output_template.body, body_params);

            body_params.clear();
            unit.reset(srcml_archive_read_unit(srcml_arch));

            // When you want to print only the information from a specific unit
            if (unit_num > 0 && unit_num == unit_count && !(xml)) {
                break;
            }

            if (unit)
                ++unit_count;
        }
    }

    if (output_template.footer) {
        for (const auto& arg : output_template.footer_args) {
            if (arg == "C"sv) {
                footer_params.push_back(std::to_string(unit_count + 1));
            }
            else {
                const char* param = acquire_metadata(srcml_arch, nullptr, arg);
                if (param) {
                    footer_params.emplace_back(param);
                }
                else {
                    if (output_template.footer_args.size() > 1)
                        footer_params.emplace_back("");
                }
            }
        }

        if (footer_params.size() != 0)
            pretty_print(*output_template.footer, footer_params);
    }
}

int srcml_pretty(srcml_archive* srcml_arch, std::string_view pretty_input, const srcml_request_t& srcml_request) {
    int unit_num = srcml_request.unit;
    pretty_template_t output_template = split_template_sections(pretty_input);
    size_t ns_size = srcml_archive_get_namespace_size(srcml_arch);

    if (output_template.header) {
        output_template.error_location = parse_templates(*output_template.header, output_template.header_args, valid_header_args, ns_size);
        if (output_template.error_location) {
            SRCMLstatus(WARNING_MSG, "srcml: header format error:\n" +
                             *output_template.header);
            show_carret_error(*output_template.error_location);
            return -1;
        }
    }

    if (output_template.body) {
        output_template.error_location = parse_templates(*output_template.body, output_template.body_args, valid_body_args, ns_size);
        if (output_template.error_location) {
            SRCMLstatus(WARNING_MSG, "srcml: body format error:\n" +
                             *output_template.body);
            show_carret_error(*output_template.error_location);
            return -1;
        }
    }

    if (output_template.footer) {
        output_template.error_location = parse_templates(*output_template.footer, output_template.footer_args, valid_footer_args, ns_size);
        if (output_template.error_location) {
            SRCMLstatus(WARNING_MSG, "srcml: footer format error:\n" +
                             *output_template.footer);
            show_carret_error(*output_template.error_location);
            return -1;
        }
    }

    display_template(srcml_arch, output_template, ns_size, unit_num, option(SRCML_COMMAND_XML));

    return 0;
}
