/**
 * @file srcml_sax2_utilities.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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

#include <srcml_sax2_utilities.hpp>
#include <srcml_sax2_reader.hpp>

#include <sstream>

#ifdef WITH_LIBXSLT
#include <libexslt/exslt.h>
#endif

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#define DLLOAD
#else
#undef DLLOAD
#endif

/**
 * dlopen_library
 *
 * Get a handle to libxslt dynamically loaded
 *
 * @returns Return handle on success and NULL on failure
 */
void* dlopen_library(std::vector<const char*> libnames) {
#ifdef DLLOAD
    for (auto libname : libnames) {
        void* handle = dlopen(libname, RTLD_LAZY);
        if (handle)
            return handle;
    }
#endif
    return 0;
}

/**
 * dlopen_libxslt
 *
 * Get a handle to libxslt dynamically loaded
 *
 * @returns Return handle on success and NULL on failure
 */
void* dlopen_libxslt() {

    return dlopen_library({ "libxslt.so", "libxslt.so.1", "libxslt.so.0", "libxslt.dylib" });
}

/**
 * dlopen_libexslt
 *
 * Get a handle to libexslt dynamically loaded
 *
 * @returns Return handle on success and NULL on failure
 */
void* dlopen_libexslt() {

    return dlopen_library({ "libexslt.so", "libexslt.so.0", "libexslt.dylib" });
}
