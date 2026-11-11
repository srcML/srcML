// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file BlockPool.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Per-thread free list of same-size memory blocks for reuse
 */

#ifndef BLOCK_POOL_HPP
#define BLOCK_POOL_HPP

#include <cstddef>
#include <new>

/**
 * BlockPool
 *
 * Per-thread free list of deleted blocks of one size, identified by Tag.
 * The list head is trivially destructible, so access is a plain thread-local
 * access without a guard, and the blocks are freed when the thread ends.
 */
template <typename Tag, std::size_t MAX_BLOCKS>
class BlockPool {
public:

    struct List {
        void* head;
        std::size_t count;
        bool registered;
        bool ended;
    };

    // blocks of this thread, for a caller to keep instead of finding them each time
    static List& local() {
        return list();
    }

    // a block for reuse, or null if none
    static void* take() {
        return take(list());
    }

    static void* take(List& l) {
        void* p = l.head;
        if (p) {
            l.head = *static_cast<void**>(p);
            --l.count;
        }
        return p;
    }

    // keep the block for reuse, returning false if not kept
    static bool give(void* p) {
        return give(list(), p);
    }

    static bool give(List& l, void* p) {
        if (l.count >= MAX_BLOCKS || l.ended)
            return false;

        // register freeing of the blocks at the end of the thread
        if (!l.registered) {
            cleanup();
            l.registered = true;
        }

        *static_cast<void**>(p) = l.head;
        l.head = p;
        ++l.count;
        return true;
    }

private:

    static List& list() {
        static thread_local List l = { nullptr, 0, false, false };
        return l;
    }

    struct Cleanup {
        ~Cleanup() {
            auto& l = list();
            while (l.head) {
                void* p = l.head;
                l.head = *static_cast<void**>(p);
                ::operator delete(p);
            }
            l.count = 0;
            l.ended = true;
        }
    };

    static void cleanup() {
        static thread_local Cleanup c;
        (void) c;
    }
};

#endif
