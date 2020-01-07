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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_SRCML_UTILTIES_HPP
#define INCLUDED_SRCML_UTILTIES_HPP

#include <archive.h>
#include <archive_entry.h>
#include <srcml.h>
#include <memory>
#include <OpenFileLimiter.hpp>

// std::shared_ptr deleter for srcml archive
// some compilers will not use the default_delete<srcml_archive> for std::shared_ptr
inline void srcml_archive_deleter(srcml_archive* arch) {
    srcml_archive_close(arch);
    srcml_archive_free(arch);

    OpenFileLimiter::close();
}

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

            OpenFileLimiter::close();
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
