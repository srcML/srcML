// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file libxml2_utilities.hpp
 *
 * @copyright Copyright (C) 2018-2022 srcML, LLC. (www.srcML.org)
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
#define LIBXML2_DEFAULT_DELETE
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
