/**
 * @file srcml_utilities.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_SRCML_UTILTIES_HPP
#define INCLUDED_SRCML_UTILTIES_HPP

#include <archive.h>
#include <archive_entry.h>
#include <memory>

// std::unique_ptr deleter functions for srcml
// usage: std::unique<srcml_archive> p(srcml_archive_create());
// Call p.get() for original pointer
// Will deallocate automatically at end of std::unique_ptr lifetime
namespace std {
    template<>
    struct default_delete<srcml_archive> {
        void operator()(srcml_archive* arch) { 
            srcml_archive_close(arch);
            srcml_archive_free(arch);
        }
    };

    template<>
    struct default_delete<srcml_unit> {
        void operator()(srcml_unit* unit) { 
            srcml_unit_free(unit);
        }
    };
}

#endif
