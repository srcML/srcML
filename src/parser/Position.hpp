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

#include <antlr/TokenStream.hpp>

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

    void append(std::string_view text) {

        for (auto p = text.begin(); p != text.end(); ++p) {
            if (*p == '\n') {
                ++line;
                column = 0;
            } else if (*p == '\t') {
                column = ((column / tabsize) + 1) * tabsize;
            } else {
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

    int tabsize = 8;
    int line = 1;
    int column = 0;
};

#endif
