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
#include <srcml.h>
#include <string>
#include <boost/foreach.hpp>

// transform srcml with query or transformation
void transform_srcml(const srcml_request_t& srcml_request,
                     const srcml_input_t& input_sources,
                     const srcml_output_dest& /* output*/) {

	BOOST_FOREACH(const srcml_input_src& input, input_sources) {
		srcml_archive* arch = srcml_create_archive();
		int status;
        if (contains<int>(input))
            status = srcml_read_open_fd(arch, input);
        else if (contains<FILE*>(input))
            status = srcml_read_open_FILE(arch, input);
        else
            status = srcml_read_open_filename(arch, input.c_str());
        if (status != SRCML_STATUS_OK)
            throw status;

		// iterate through all transformations added during cli parsing
		BOOST_FOREACH(const std::string& trans, srcml_request.transformations) {
			std::cerr << trans << "\n"; // output the transformations for debugging
		}

		srcml_free_archive(arch);
	}
}
