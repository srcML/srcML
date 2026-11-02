// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file main.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * antlrcpp: replacement for the ANTLR 2.7.7 tool (antlr.Tool) that reads
 * ANTLR 2 grammar files and generates the C++ lexer and parser.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "Tool.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace {

void help() {
    std::cerr << "usage: antlrcpp [args] file.g\n"
                 "  -o outputDir       specify output directory where all output generated.\n"
                 "  -glib superGrammar specify location of supergrammar file.\n"
                 "  -trace             have all rules call traceIn/traceOut.\n"
                 "  -traceLexer        have lexer rules call traceIn/traceOut.\n"
                 "  -traceParser       have parser rules call traceIn/traceOut.\n"
                 "  -traceTreeParser   have tree parser rules call traceIn/traceOut.\n"
                 "  -h|-help|--help    this message\n";
}

} // namespace

int main(int argc, char* argv[]) {
    std::cerr << "ANTLR Parser Generator   Version " << antlrcpp::Tool::version << "   1989-2005\n";

    std::vector<std::string> args(argv + 1, argv + argc);
    bool showHelp = args.empty();
    for (auto& arg : args) {
        if (arg == "-h" || arg == "-help" || arg == "--help") {
            showHelp = true;
            break;
        }
    }
    if (showHelp) {
        help();
        return 0;
    }

    antlrcpp::Tool tool;
    return tool.doEverything(args);
}
