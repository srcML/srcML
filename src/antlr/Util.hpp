// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Util.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * String helpers with the semantics of the Java library calls used by ANTLR 2.7.7.
 * Strings are byte strings, with each byte treated as a Latin-1 character.
 */

#ifndef ANTLR2CPP_UTIL_HPP
#define ANTLR2CPP_UTIL_HPP

#include <string>
#include <optional>
#include <vector>
#include <cstdint>

namespace antlrcpp {

// Java String that may be null
using OStr = std::optional<std::string>;

// java.lang.String.hashCode()
inline std::int32_t javaHashCode(const std::string& s) {
    std::uint32_t h = 0;
    for (unsigned char c : s)
        h = 31 * h + c;
    return static_cast<std::int32_t>(h);
}

inline bool startsWith(const std::string& s, const std::string& prefix) {
    return s.compare(0, prefix.size(), prefix) == 0;
}

inline bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// java.lang.String.indexOf(), returning -1 when not found
inline int indexOf(const std::string& s, char c, int from = 0) {
    auto pos = s.find(c, static_cast<std::size_t>(from));
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

inline int indexOf(const std::string& s, const std::string& t) {
    auto pos = s.find(t);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

inline int lastIndexOf(const std::string& s, char c) {
    auto pos = s.rfind(c);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

inline int lastIndexOf(const std::string& s, const std::string& t) {
    auto pos = s.rfind(t);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

// java.lang.String.substring()
inline std::string substring(const std::string& s, int begin, int end) {
    return s.substr(static_cast<std::size_t>(begin), static_cast<std::size_t>(end - begin));
}

inline std::string substring(const std::string& s, int begin) {
    return s.substr(static_cast<std::size_t>(begin));
}

// java.lang.String.trim(), which removes all characters <= ' '
inline std::string trim(const std::string& s) {
    std::size_t b = 0;
    std::size_t e = s.size();
    while (b < e && static_cast<unsigned char>(s[b]) <= ' ')
        ++b;
    while (e > b && static_cast<unsigned char>(s[e - 1]) <= ' ')
        --e;
    return s.substr(b, e - b);
}

// java.lang.Character predicates for Latin-1 characters
inline bool isJavaDigit(int c) {
    return c >= '0' && c <= '9';
}

inline bool isJavaLetter(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == 0xAA || c == 0xB5 || c == 0xBA ||
           (c >= 0xC0 && c <= 0xFF && c != 0xD7 && c != 0xF7);
}

inline bool isJavaLetterOrDigit(int c) {
    return isJavaLetter(c) || isJavaDigit(c);
}

inline bool isJavaSpaceChar(int c) {
    return c == ' ' || c == 0xA0;
}

inline std::string toLowerAscii(std::string s) {
    for (auto& c : s)
        if (c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');
    return s;
}

inline std::string toUpperAscii(std::string s) {
    for (auto& c : s)
        if (c >= 'a' && c <= 'z')
            c = static_cast<char>(c - 'a' + 'A');
    return s;
}

// Integer.toString(value, radix) for non-negative values
inline std::string toRadix(long long value, int radix) {
    if (value == 0)
        return "0";
    bool neg = value < 0;
    unsigned long long v = neg ? static_cast<unsigned long long>(-value) : static_cast<unsigned long long>(value);
    std::string s;
    while (v) {
        int d = static_cast<int>(v % static_cast<unsigned>(radix));
        s.insert(s.begin(), static_cast<char>(d < 10 ? '0' + d : 'a' + d - 10));
        v /= static_cast<unsigned>(radix);
    }
    if (neg)
        s.insert(s.begin(), '-');
    return s;
}

// Character.digit(c, 16)
inline int hexDigit(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// antlr.StringUtils
inline std::string stripFront(std::string s, char c) {
    while (!s.empty() && s[0] == c)
        s.erase(0, 1);
    return s;
}

inline std::string stripBack(std::string s, char c) {
    while (!s.empty() && s.back() == c)
        s.pop_back();
    return s;
}

inline std::string stripFront(std::string s, const std::string& remove) {
    bool changed;
    do {
        changed = false;
        for (char c : remove) {
            while (!s.empty() && s[0] == c) {
                changed = true;
                s.erase(0, 1);
            }
        }
    } while (changed);
    return s;
}

inline std::string stripBack(std::string s, const std::string& remove) {
    bool changed;
    do {
        changed = false;
        for (char c : remove) {
            while (!s.empty() && s.back() == c) {
                changed = true;
                s.pop_back();
            }
        }
    } while (changed);
    return s;
}

inline std::string stripFrontBack(const std::string& src, const std::string& head, const std::string& tail) {
    int h = indexOf(src, head);
    int t = lastIndexOf(src, tail);
    if (h == -1 || t == -1)
        return src;
    return substring(src, h + 1, t);
}

// java.util.StringTokenizer with a set of delimiter characters
inline std::vector<std::string> tokenize(const std::string& s, const std::string& delims) {
    std::vector<std::string> result;
    std::string cur;
    for (char c : s) {
        if (delims.find(c) != std::string::npos) {
            if (!cur.empty())
                result.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty())
        result.push_back(cur);
    return result;
}

} // namespace antlrcpp

#endif
