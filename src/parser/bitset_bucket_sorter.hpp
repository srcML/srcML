/**
 * @file bitset_bucket_sorter.hpp
 * 
 * @copyright Copyright (C) 2014  srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 *
 * The srcML translator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML translator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML translator; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef BITSET_BUCKET_SORTER
#define BITSET_BUCKET_SORTER

template <typename T>
constexpr unsigned long convert(const T /* base */) {
    return 0;
}

template<typename T, typename... Args>
constexpr unsigned long convert(const int pos, T value, Args... args) {
    return ((value < 32 * (pos + 1)) ? (1 << (value - 32 * pos)) : 0UL) | convert(pos, args...);
}

#define token_set(...) (const unsigned long[]){ convert(0, __VA_ARGS__), convert(1, __VA_ARGS__), convert(2, __VA_ARGS__), convert(3, __VA_ARGS__), convert(4, __VA_ARGS__), convert(5, __VA_ARGS__), convert(6, __VA_ARGS__), convert(7, __VA_ARGS__), \
convert(8, __VA_ARGS__), convert(9, __VA_ARGS__), convert(10, __VA_ARGS__), convert(11, __VA_ARGS__), convert(12, __VA_ARGS__), convert(13, __VA_ARGS__), convert(14, __VA_ARGS__), convert(15, __VA_ARGS__) }, 10

#endif
