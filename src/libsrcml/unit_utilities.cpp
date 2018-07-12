/**
 * @file unit_utilities.cpp
 *
 * @copyright Copyright (C) 2018 srcML, LLC. (www.srcML.org)
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

#include <unit_utilities.hpp>

// Update unit attributes with xml parsed attributes
void unit_update_attributes(srcml_unit* unit, int num_attributes, const xmlChar** attributes) {

    // collect attributes
    for (int pos = 0; pos < num_attributes; ++pos) {

        std::string attribute = (const char*) attributes[pos * 5];
        std::string value((const char *)attributes[pos * 5 + 3], attributes[pos * 5 + 4] - attributes[pos * 5 + 3]);

        // @todo Fix for multiple attributes
        //value = attribute_revision(value);
        
        if (attribute == "timestamp")
            srcml_unit_set_timestamp(unit, value.c_str());
        else if (attribute == "hash")
            srcml_unit_set_hash(unit, value.c_str());
        else if (attribute == "language")
            srcml_unit_set_language(unit, value.c_str());
        else if (attribute == "revision")
            unit->revision = value;
        else if (attribute == "filename")
            srcml_unit_set_filename(unit, value.c_str());
        else if (attribute == "version")
            srcml_unit_set_version(unit, value.c_str());
        else if (attribute == "tabs" || attribute == "options" || attribute == "hash")
            ;
        //else if (attribute == "src-encoding")
            //archive->options |= SRCML_OPTION_STORE_ENCODING, srcml_unit_set_src_encoding(unit, value.c_str());
        else {

            unit->attributes.push_back(attribute);
            unit->attributes.push_back(value);
        }
    }
}

