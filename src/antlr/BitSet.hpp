// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file BitSet.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.collections.impl.BitSet. The number of words, and how it grows,
 * matches the original since it shows up in the generated code.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_BITSET_HPP
#define ANTLR2CPP_BITSET_HPP

#include "Util.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <initializer_list>

namespace antlrcpp {

class BitSet {
public:
    static constexpr int BITS = 64;
    static constexpr int LOG_BITS = 6;
    static constexpr int MOD_MASK = BITS - 1;

    std::vector<std::uint64_t> bits;

    BitSet() : BitSet(BITS) {}

    explicit BitSet(int nbits) : bits(static_cast<std::size_t>(((nbits - 1) >> LOG_BITS) + 1), 0) {}

    // Construct from Java long[] data
    static BitSet fromWords(const std::vector<std::int64_t>& data) {
        BitSet b(0);
        for (auto d : data)
            b.bits.push_back(static_cast<std::uint64_t>(d));
        return b;
    }

    static BitSet of(int el) {
        BitSet s(el + 1);
        s.add(el);
        return s;
    }

    static int wordNumber(int bit) { return bit >> LOG_BITS; }
    static std::uint64_t bitMask(int bitNumber) { return std::uint64_t(1) << (bitNumber & MOD_MASK); }
    static int numWordsToHold(int el) { return (el >> LOG_BITS) + 1; }

    int length() const { return static_cast<int>(bits.size()); }

    void add(int el) {
        int n = wordNumber(el);
        if (n >= length())
            growToInclude(el);
        bits[static_cast<std::size_t>(n)] |= bitMask(el);
    }

    BitSet and_(const BitSet& a) const {
        BitSet s = *this;
        s.andInPlace(a);
        return s;
    }

    void andInPlace(const BitSet& a) {
        int min = std::min(length(), a.length());
        for (int i = min - 1; i >= 0; i--)
            bits[static_cast<std::size_t>(i)] &= a.bits[static_cast<std::size_t>(i)];
        for (int i = min; i < length(); i++)
            bits[static_cast<std::size_t>(i)] = 0;
    }

    void clear() {
        std::fill(bits.begin(), bits.end(), 0);
    }

    void clear(int el) {
        int n = wordNumber(el);
        if (n >= length())
            growToInclude(el);
        bits[static_cast<std::size_t>(n)] &= ~bitMask(el);
    }

    int degree() const {
        int deg = 0;
        for (auto word : bits) {
            while (word) {
                word &= word - 1;
                ++deg;
            }
        }
        return deg;
    }

    bool equals(const BitSet& set) const {
        int n = std::min(length(), set.length());
        for (int i = n; i-- > 0;)
            if (bits[static_cast<std::size_t>(i)] != set.bits[static_cast<std::size_t>(i)])
                return false;
        if (length() > n) {
            for (int i = length(); i-- > n;)
                if (bits[static_cast<std::size_t>(i)] != 0)
                    return false;
        } else if (set.length() > n) {
            for (int i = set.length(); i-- > n;)
                if (set.bits[static_cast<std::size_t>(i)] != 0)
                    return false;
        }
        return true;
    }

    void growToInclude(int bit) {
        int newSize = std::max(length() << 1, numWordsToHold(bit));
        bits.resize(static_cast<std::size_t>(newSize), 0);
    }

    bool member(int el) const {
        if (el < 0)
            return false;
        int n = wordNumber(el);
        if (n >= length())
            return false;
        return (bits[static_cast<std::size_t>(n)] & bitMask(el)) != 0;
    }

    bool nil() const {
        for (auto word : bits)
            if (word != 0)
                return false;
        return true;
    }

    void notInPlace() {
        for (auto& word : bits)
            word = ~word;
    }

    void notInPlace(int minBit, int maxBit) {
        growToInclude(maxBit);
        for (int i = minBit; i <= maxBit; i++)
            bits[static_cast<std::size_t>(wordNumber(i))] ^= bitMask(i);
    }

    void orInPlace(const BitSet& a) {
        if (a.length() > length())
            bits.resize(a.bits.size(), 0);
        int min = std::min(length(), a.length());
        for (int i = min - 1; i >= 0; i--)
            bits[static_cast<std::size_t>(i)] |= a.bits[static_cast<std::size_t>(i)];
    }

    void remove(int el) {
        int n = wordNumber(el);
        if (n >= length())
            growToInclude(el);
        bits[static_cast<std::size_t>(n)] &= ~bitMask(el);
    }

    int size() const { return length() << LOG_BITS; }

    void subtractInPlace(const BitSet& a) {
        for (int i = 0; i < length() && i < a.length(); i++)
            bits[static_cast<std::size_t>(i)] &= ~a.bits[static_cast<std::size_t>(i)];
    }

    std::vector<int> toArray() const {
        std::vector<int> elems;
        for (int i = 0; i < (length() << LOG_BITS); i++)
            if (member(i))
                elems.push_back(i);
        return elems;
    }

    std::string toString(const std::string& separator = ",") const {
        std::string str;
        for (int i = 0; i < (length() << LOG_BITS); i++) {
            if (member(i)) {
                if (!str.empty())
                    str += separator;
                str += std::to_string(i);
            }
        }
        return str;
    }

    std::string toStringOfHalfWords() const {
        std::string s;
        for (int i = 0; i < length(); i++) {
            if (i != 0)
                s += ", ";
            std::uint64_t word = bits[static_cast<std::size_t>(i)];
            s += std::to_string(word & 0xFFFFFFFFULL) + "UL";
            s += ", ";
            s += std::to_string((word >> 32) & 0xFFFFFFFFULL) + "UL";
        }
        return s;
    }
};

} // namespace antlrcpp

#endif
