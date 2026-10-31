// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Position.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Position in source code Definitions of srcML namespace URI's
 */

#ifndef INCLUDED_POSITION_HPP
#define INCLUDED_POSITION_HPP

#include <ANTLRRuntime.hpp>

struct Position {

    Position() = default;

    Position(int line, int column)
        : line(line), column(column)
    {}

    Position(antlr::RefToken token)
        : line(token->getLine()), column(token->getColumn())
    {}

    void set(antlr::RefToken token) {
        line = token->getLine();
        column = token->getColumn() - 1;
    }

    void append(std::string_view text, size_t tabsize) {

        // Update line and column
        // * When you reach a newline, reset column
        // * Tabs are expanded
        // * Unicode continuation characters are ignored
        // * Combining marks start for the range 0x0300 through 0x036F
        for (auto p = text.begin(); p != text.end(); ++p) {
            if (*p == '\n') {
                ++line;
                column = 0;
            } else if (*p == '\t') {
                column = ((column / (int)tabsize) + 1) * (int)tabsize;
            } else if ((*p & 0xC0) != 0x80 && (unsigned char)*p != 0xCC) {
                ++column;
            }
        }
    }

    bool operator==(const Position& other) {

        return line == other.line && column == other.column;
    }

    bool operator!=(const Position& other) {

        return !(*this == other);
    }

    bool operator<(const Position& other) {

        return line < other.line || (line == other.line && column < other.column);
    }

    int line = 1;
    int column = 0;
};

struct PositionRange {
    Position start;
    Position end;
};

#endif
