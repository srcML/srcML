// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Tool.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.Tool: command-line handling, error reporting, and output files.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_TOOL_HPP
#define ANTLR2CPP_TOOL_HPP

#include "Util.hpp"
#include <string>
#include <vector>
#include <set>
#include <sstream>

namespace antlrcpp {

class NameSpace {
public:
    explicit NameSpace(const std::string& name) : name(name), names(tokenize(name, ":")) {}

    void emitDeclarations(std::ostream& out) const {
        for (auto& s : names)
            out << "ANTLR_BEGIN_NAMESPACE(" << s << ")\n";
    }

    void emitClosures(std::ostream& out) const {
        for (std::size_t i = 0; i < names.size(); ++i)
            out << "ANTLR_END_NAMESPACE\n";
    }

    std::string name;
    std::vector<std::string> names;
};

// Output file that is only written when its contents change (PreservingFileWriter)
class OutputFile {
public:
    explicit OutputFile(std::string path) : path(std::move(path)) {}
    std::ostringstream out;
    void close();

private:
    std::string path;
};

class Tool {
public:
    static const std::string version;

    Tool();

    int doEverything(const std::vector<std::string>& args);

    void error(const std::string& s);
    void error(const std::string& s, const OStr& file, int line, int column);
    void warning(const std::string& s);
    void warning(const std::string& s, const OStr& file, int line, int column);
    void warning(const std::vector<std::string>& s, const OStr& file, int line, int column);
    void toolError(const std::string& s);
    [[noreturn]] void fatalError(const std::string& message);
    [[noreturn]] void panic(const std::string& s);

    bool hasError() const { return errorFlag; }

    std::string fileMinusPath(const std::string& f) const;
    std::string pathToFile(const std::string& f) const;
    std::string getOutputDirectory() const { return outputDir; }
    void setOutputDirectory(const std::string& o) { outputDir = o; }
    void copyFile(const std::string& source, const std::string& dest);
    OutputFile* openOutputFile(const std::string& f);
    void setArgOK(int i) { cmdLineArgValid.insert(i); }
    void setNameSpace(const std::string& name);

    static std::vector<std::string> parseSeparatedList(const std::string& list, char separator);

    std::string grammarFile;
    std::string literalsPrefix = "LITERAL_";
    bool upperCaseMangledLiterals = false;
    NameSpace* nameSpace = nullptr;
    OStr namespaceAntlr;
    OStr namespaceStd;
    bool genHashLines = true;
    bool noConstructors = false;

private:
    void processArguments(const std::vector<std::string>& args);
    void checkForInvalidArguments(const std::vector<std::string>& args);

    bool errorFlag = false;
    std::string outputDir = ".";
    bool outputDirSet = false;
    std::set<int> cmdLineArgValid;
};

} // namespace antlrcpp

#endif
