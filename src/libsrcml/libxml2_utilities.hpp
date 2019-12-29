/**
 * @file libxml2_utilities.hpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_LIBXML2_UTILITIES_HPP
#define INCLUDED_LIBXML2_UTILITIES_HPP

#include <libxml/xpath.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>
#include <libxml/relaxng.h>
#include <algorithm>
#include <memory>

// std::unique_ptr deleter functions for libxml2
// usage: std::unique<xmlDoc> p(xmlReadMemory);
// Call p.get() for original pointer
// Will deallocate automatically at end of std::unique_ptr lifetime
namespace std {
    template<>
    struct default_delete<xmlDoc> {
        void operator()(xmlDoc* doc) { xmlFreeDoc(doc); }
    };

    template<>
    struct default_delete<xmlNodeSet> {
        void operator()(xmlNodeSet* nodeSet) { xmlXPathFreeNodeSet(nodeSet); }
    };

    template<>
    struct default_delete<xmlBuffer> {
        void operator()(xmlBuffer* buffer) { xmlBufferFree(buffer); }
    };

    template<>
    struct default_delete<xmlOutputBuffer> {
        void operator()(xmlOutputBuffer* buffer) { xmlOutputBufferClose(buffer); }
    };

    template<>
    struct default_delete<xmlParserInputBuffer> {
        void operator()(xmlParserInputBuffer* buffer) { xmlFreeParserInputBuffer(buffer); }
    };

    template<>
    struct default_delete<xmlXPathObject> {
        void operator()(xmlXPathObject* xpath) { xmlXPathFreeObject(xpath); }
    };

    template<>
    struct default_delete<xmlXPathContext> {
        void operator()(xmlXPathContext* context) { xmlXPathFreeContext(context); }
    };

    template<>
    struct default_delete<xmlRelaxNGValidCtxt> {
        void operator()(xmlRelaxNGValidCtxt* rngctx) { xmlRelaxNGFreeValidCtxt(rngctx); }
    };

    template<>
    struct default_delete<xmlRelaxNGParserCtxt> {
        void operator()(xmlRelaxNGParserCtxt* ctxt) { xmlRelaxNGFreeParserCtxt(ctxt); }
    };

    template<>
    struct default_delete<xmlRelaxNG> {
        void operator()(xmlRelaxNG* rng) { xmlRelaxNGFree(rng); }
    };
}

#endif
