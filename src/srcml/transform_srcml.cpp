/**
 * @file transform_srcml.hpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <transform_srcml.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <string>
#include <boost/foreach.hpp>

// transform srcml with query or transformation
void transform_srcml(const srcml_request_t& srcml_request,
                     const srcml_input_t& input_sources,
                     const srcml_output_dest& output) {

	int status;

	// Convert output into srcml archive
	srcml_archive* out_arch = srcml_create_archive();
    if (contains<int>(output))
        status = srcml_write_open_fd(out_arch, output);
    else if (contains<FILE*>(output))
        status = srcml_write_open_FILE(out_arch, output);
    else
        status = srcml_write_open_filename(out_arch, output.c_str());
    if (status != SRCML_STATUS_OK)
        throw status;

    // Convert inputs into srcml archive
	BOOST_FOREACH(const srcml_input_src& input, input_sources) {
        srcml_archive* in_arch = srcml_create_archive();
        if (contains<int>(input))
            status = srcml_read_open_fd(in_arch, input);
        else if (contains<FILE*>(input))
            status = srcml_read_open_FILE(in_arch, input);
        else
            status = srcml_read_open_filename(in_arch, input.c_str());
        if (status != SRCML_STATUS_OK)
            throw status;

		// iterate through all transformations added during cli parsing
		BOOST_FOREACH(const std::string& trans, srcml_request.transformations) {
			std::string protocol;
			std::string resource;
			src_prefix_split_uri(trans, protocol, resource);
			if (protocol.compare("xpath") == 0) {
				srcml_append_transform_xpath(in_arch, resource.c_str());
			}
			else if (protocol.compare("relaxng") == 0) {
				std::cerr << protocol << " : " << resource << "\n"; //Stub
			}
			else if (protocol.compare("xslt") == 0) {
				std::cerr << protocol << " : " << resource << "\n"; // Stub
			}
			else if (protocol.compare("xpathparam") == 0) {
				std::cerr << protocol << " : " << resource << "\n"; // Stub
			}
		}
		srcml_apply_transforms(in_arch, out_arch);
		srcml_close_archive(in_arch);
		srcml_free_archive(in_arch);
	}

	srcml_close_archive(out_arch);
	srcml_free_archive(out_arch);
}
