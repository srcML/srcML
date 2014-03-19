/**
 * @file src_input_file.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <src_input_file.hpp>

// Convert input to a ParseRequest and assign request to the processing queue
void src_input_file(ParseQueue& queue,
                    srcml_archive* srcml_arch,
                    const std::string& input_file,
                    const boost::optional<std::string>& option_language,
                    const boost::optional<std::string>& option_filename,
                    const boost::optional<std::string>& option_directory,
                    const boost::optional<std::string>& option_version)
{
    // form the parsing request
    ParseRequest request;
    if (option_filename)
        request.filename = *option_filename;
    else if (input_file != "_")
        request.filename = input_file;
    request.directory = option_directory;
    request.version = option_version;
    request.srcml_arch = srcml_arch;
    request.language = option_language ? *option_language : "";
    request.disk_filename = input_file;

    // Hand request off to the processing queue
    queue.push(request);
}
