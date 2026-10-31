// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Preprocessor.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.preprocessor, which handles grammar inheritance by producing
 * expanded grammar files.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_PREPROCESSOR_HPP
#define ANTLR2CPP_PREPROCESSOR_HPP

#include <string>
#include <vector>

namespace antlrcpp {

class Tool;

class PreprocessorTool {
public:
    PreprocessorTool(Tool* tool, const std::vector<std::string>& args);

    // false on error
    bool preprocess();
    std::vector<std::string> preprocessedArgList() const { return args; }

private:
    void processArguments(const std::vector<std::string>& incomingArgs);

    Tool* antlrTool;
    std::string grammarFileName;
    std::vector<std::string> args;
    std::vector<std::string> grammars;
    bool haveGrammars = false;
};

} // namespace antlrcpp

#endif
