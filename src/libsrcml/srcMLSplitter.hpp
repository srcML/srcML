// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLSplitter.hpp
 *
 * @copyright Copyright (C) 2022 srcML, LLC. (www.srcML.org)
 *
 * Split a srcML file into separate units
 */

#ifndef SRCMLSPLITTER_HPP
#define SRCMLSPLITTER_HPP

#include <string>
#include <string_view>
#include <libxml/xmlreader.h>

struct srcml_archive;
struct srcml_unit;

class srcMLSplitter {
public:
    srcMLSplitter(srcml_archive* archive, xmlParserInputBufferPtr inputBuffer = nullptr);
    void setSkip() { skipMode = true; }
    int nextUnit(srcml_unit* unit, bool stopRoot = false);
    // std::string_view unitLanguage() const { return language; }
    // std::string_view unitFilename() const { return filename; }
    // int unitLOC() const { return loc; }

    ~srcMLSplitter() {
        fprintf(stderr, "DEBUG:  %s %s %d totalBytes: %d\n", __FILE__,  __FUNCTION__, __LINE__,  (int) totalBytes);
    }
public:
    srcml_archive* archive;
    xmlParserInputBufferPtr inputBuffer = nullptr;
    int refillContent(std::string_view& content);
    std::string_view content;
    int totalBytes = 0;
    int textSize = 0;
    bool doneReading = false;
    bool isArchive = false;
    int loc = 0;
    int depth = 0;
    int lastRead = 0;
    bool skipMode = false;
    bool isDone = false;
    bool firstAfterRoot = false;
    srcml_unit* unitSave = nullptr;
    bool inUnit = false;
    const char* unitStart = nullptr;
};

#endif
