// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Tool.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.Tool.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "Tool.hpp"
#include "Preprocessor.hpp"
#include "ANTLRParser.hpp"
#include "MakeGrammar.hpp"
#include "LLkAnalyzer.hpp"
#include "CppCodeGenerator.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <cstdlib>

namespace antlrcpp {

const std::string Tool::version = "2.7.7 (2006-11-01)";

namespace {

std::string readFile(const std::string& path, bool& ok) {
    std::ifstream in(path, std::ios::binary);
    ok = static_cast<bool>(in);
    std::stringstream buf;
    if (ok)
        buf << in.rdbuf();
    return buf.str();
}

} // namespace

void OutputFile::close() {
    std::string contents = out.str();
    bool ok = false;
    std::string existing = readFile(path, ok);
    if (ok && existing == contents)
        return;
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file) {
        std::cerr << "error: cannot write to '" << path << "'\n";
        std::exit(1);
    }
    file << contents;
}

Tool::Tool() = default;

void Tool::error(const std::string& s) {
    errorFlag = true;
    std::cerr << "error: " << s << '\n';
}

void Tool::error(const std::string& s, const OStr& file, int line, int column) {
    errorFlag = true;
    std::cerr << formatLocation(file, line, column) << s << '\n';
}

void Tool::warning(const std::string& s) {
    std::cerr << "warning: " << s << '\n';
}

void Tool::warning(const std::string& s, const OStr& file, int line, int column) {
    std::cerr << formatLocation(file, line, column) << "warning:" << s << '\n';
}

void Tool::warning(const std::vector<std::string>& s, const OStr& file, int line, int column) {
    if (s.empty())
        panic("bad multi-line message to Tool.warning");
    std::cerr << formatLocation(file, line, column) << "warning:" << s[0] << '\n';
    for (std::size_t i = 1; i < s.size(); i++)
        std::cerr << formatLocation(file, line, column) << "    " << s[i] << '\n';
}

void Tool::toolError(const std::string& s) {
    errorFlag = true;
    std::cerr << "error: " << s << '\n';
}

void Tool::fatalError(const std::string& message) {
    std::cerr << message << '\n';
    std::exit(1);
}

void Tool::panic(const std::string& s) {
    fatalError("panic: " + s);
}

std::string Tool::fileMinusPath(const std::string& f) const {
    auto endOfPath = f.find_last_of("/\\");
    if (endOfPath == std::string::npos)
        return f;
    return f.substr(endOfPath + 1);
}

std::string Tool::pathToFile(const std::string& f) const {
    auto endOfPath = f.find_last_of("/\\");
    if (endOfPath == std::string::npos)
        return "./";
    return f.substr(0, endOfPath + 1);
}

void Tool::copyFile(const std::string& source, const std::string& dest) {
    if (!std::filesystem::is_regular_file(source))
        throw std::runtime_error("FileCopy: no such source file: " + source);
    std::filesystem::copy_file(source, dest, std::filesystem::copy_options::overwrite_existing);
}

OutputFile* Tool::openOutputFile(const std::string& f) {
    if (outputDirSet) {
        std::error_code ec;
        std::filesystem::create_directories(outputDir, ec);
    }
    return new OutputFile(outputDir + "/" + f);
}

void Tool::setNameSpace(const std::string& name) {
    if (!nameSpace)
        nameSpace = new NameSpace(stripFrontBack(name, "\"", "\""));
}

std::vector<std::string> Tool::parseSeparatedList(const std::string& list, char separator) {
    return tokenize(list, std::string(1, separator));
}

void Tool::processArguments(const std::vector<std::string>& args) {
    for (std::size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-o") {
            setArgOK(static_cast<int>(i));
            if (i + 1 >= args.size()) {
                error("missing output directory with -o option; ignoring");
            } else {
                i++;
                setOutputDirectory(args[i]);
                outputDirSet = true;
                setArgOK(static_cast<int>(i));
            }
        } else if (args[i] == "-diagnostic" || args[i] == "-html" || args[i] == "-docbook") {
            fatalError("option " + args[i] + " is not supported");
        } else if (!args[i].empty() && args[i][0] != '-') {
            grammarFile = args[i];
            setArgOK(static_cast<int>(i));
        }
    }
}

void Tool::checkForInvalidArguments(const std::vector<std::string>& args) {
    for (std::size_t a = 0; a < args.size(); a++)
        if (cmdLineArgValid.count(static_cast<int>(a)) == 0)
            warning("invalid command-line argument: " + args[a] + "; ignored");
}

int Tool::doEverything(const std::vector<std::string>& args) {
    PreprocessorTool preTool(this, args);
    bool preprocessOk = preTool.preprocess();
    std::vector<std::string> modifiedArgs = preTool.preprocessedArgList();

    processArguments(modifiedArgs);
    if (!preprocessOk)
        return 1;

    bool ok = false;
    std::string input = readFile(grammarFile, ok);
    if (!ok)
        fatalError("cannot open grammar file " + grammarFile);

    LLkAnalyzer analyzer(this);
    MakeGrammar behavior(this, args, &analyzer);
    parseGrammar(input, grammarFile, &behavior, this);
    if (hasError())
        fatalError("Exiting due to errors.");
    checkForInvalidArguments(modifiedArgs);

    if (behavior.language != "Cpp")
        panic("Cannot instantiate code-generator: antlr." + behavior.language + "CodeGenerator");

    CppCodeGenerator codeGen;
    codeGen.setBehavior(&behavior);
    codeGen.setAnalyzer(&analyzer);
    codeGen.setTool(this);
    codeGen.gen();
    return hasError() ? 1 : 0;
}

} // namespace antlrcpp
