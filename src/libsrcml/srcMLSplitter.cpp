// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLSplitter.cpp
 *
 * @copyright Copyright (C) 2022 srcML, LLC. (www.srcML.org)
 *
 * Split a srcML file into separate units
 */

#include <srcMLSplitter.hpp>
#include <bitset>
#include <iostream>
#include <optional>
#include <iomanip>
#include <iconv.h>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <srcml_types.hpp>
#include <libxml/parserInternals.h>
#include <libxml/xmlIO.h>
#include <libxml/tree.h>

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

const int BLOCK_SIZE = 4096;
const int BUFFER_SIZE = 16 * 16 * BLOCK_SIZE;

const std::bitset<128> xmlNameMask("00000111111111111111111111111110100001111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");

constexpr auto WHITESPACE = " \n\t\r"sv;
constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

// trace parsing
#ifdef TRACE
#undef TRACE
#define HEADER(m) std::clog << std::setw(10) << std::left << m << '\t' << loc << '\t' << depth << std::string_view("       ").substr(0, depth) << " "
#define TRACE0() ""
#define TRACE1(l1, n1)                         l1 << ":|" << n1 << "| "
#define TRACE2(l1, n1, l2, n2)                 TRACE1(l1,n1)             << TRACE1(l2,n2)
#define TRACE3(l1, n1, l2, n2, l3, n3)         TRACE2(l1,n1,l2,n2)       << TRACE1(l3,n3)
#define TRACE4(l1, n1, l2, n2, l3, n3, l4, n4) TRACE3(l1,n1,l2,n2,l3,n3) << TRACE1(l4,n4)
#define GET_TRACE(_2,_3,_4,_5,_6,_7,_8,_9,NAME,...) NAME
#define TRACE(m,...) HEADER(m) << GET_TRACE(__VA_ARGS__, TRACE4, _UNUSED, TRACE3, _UNUSED, TRACE2, _UNUSED, TRACE1, TRACE0, TRACE0)(__VA_ARGS__) << '\n';
#else
#define TRACE(...)
#endif

#if !defined(_MSC_VER)
#include <sys/uio.h>
#include <unistd.h>
#define READ read
#else
#include <BaseTsd.h>
#include <io.h>
typedef SSIZE_T ssize_t;
#define READ _read
#endif

srcMLSplitter::srcMLSplitter(srcml_archive* archive, xmlParserInputBufferPtr inputBuffer)
: archive(archive), inputBuffer(inputBuffer) {

    // parse before root
    TRACE("START DOCUMENT");
    int bytesRead = refillContent(content);
    if (bytesRead < 0) {
        std::cerr << "parser error : File input error\n";
        exit(1);
    }
    if (bytesRead == 0 && content.empty()) {
        std::cerr << "parser error : Empty file\n";
        exit(1);
    }
    totalBytes += bytesRead;
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    if (content[0] == '<' && content[1] == '?' && content[2] == 'x' && content[3] == 'm' && content[4] == 'l' && content[5] == ' ') {
        // parse XML declaration
        assert(content.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
        content.remove_prefix("<?xml"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        // parse required version
        std::size_t nameEndPosition = content.find_first_of("= ");
        const std::string_view attr(content.substr(0, nameEndPosition));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        const char delimiter = content[0];
        if (delimiter != '"' && delimiter != '\'') {
            std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = content.find(delimiter);
        if (valueEndPosition == content.npos) {
            std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
            exit(1);
        }
        if (attr != "version"sv) {
            std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
            exit(1);
        }
        [[maybe_unused]] const std::string_view version(content.substr(0, valueEndPosition));
        content.remove_prefix(valueEndPosition);
        content.remove_prefix("\""sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        // parse optional encoding and standalone attributes
        std::optional<std::string_view> encoding;
        std::optional<std::string_view> standalone;
        if (content[0] != '?') {
            std::size_t nameEndPosition = content.find_first_of("= ");
            if (nameEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute in XML declaration\n";
                exit(1);
            }
            const std::string_view attr2(content.substr(0, nameEndPosition));
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            assert(content.compare(0, "="sv.size(), "="sv) == 0);
            content.remove_prefix("="sv.size());
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            char delimiter2 = content[0];
            if (delimiter2 != '"' && delimiter2 != '\'') {
                std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            content.remove_prefix("\""sv.size());
            std::size_t valueEndPosition = content.find(delimiter2);
            if (valueEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            if (attr2 == "encoding"sv) {
                encoding = content.substr(0, valueEndPosition);
                archive->encoding = encoding;
            } else if (attr2 == "standalone"sv) {
                standalone = content.substr(0, valueEndPosition);
            } else {
                std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            content.remove_prefix(valueEndPosition + 1);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
        }
        if (content[0] != '?') {
            std::size_t nameEndPosition = content.find_first_of("= ");
            if (nameEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute in XML declaration\n";
                exit(1);
            }
            const std::string_view attr2(content.substr(0, nameEndPosition));
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            content.remove_prefix("="sv.size());
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            const char delimiter2 = content[0];
            if (delimiter2 != '"' && delimiter2 != '\'') {
                std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            content.remove_prefix("\""sv.size());
            std::size_t valueEndPosition = content.find(delimiter2);
            if (valueEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            if (!standalone && attr2 == "standalone"sv) {
                standalone = content.substr(0, valueEndPosition);
            } else {
                std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
                exit(1);
            }
            // assert(content[valueEndPosition + 1] == '"');
            content.remove_prefix(valueEndPosition + 1);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
        }
        TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
        assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
        content.remove_prefix("?>"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }
    if (content[1] == '!' && content[0] == '<' && content[2] == 'D' && content[3] == 'O' && content[4] == 'C' && content[5] == 'T' && content[6] == 'Y' && content[7] == 'P' && content[8] == 'E' && content[9] == ' ') {
        // parse DOCTYPE
        assert(content.compare(0, "<!DOCTYPE "sv.size(), "<!DOCTYPE "sv) == 0);
        content.remove_prefix("<!DOCTYPE"sv.size());
        int depthAngleBrackets = 1;
        bool inSingleQuote = false;
        bool inDoubleQuote = false;
        bool inComment = false;
        std::size_t p = 0;
        while ((p = content.find_first_of("<>'\"-"sv, p)) != content.npos) {
            if (content.compare(p, "<!--"sv.size(), "<!--"sv) == 0) {
                inComment = true;
                p += "<!--"sv.size();
                continue;
            } else if (content.compare(p, "-->"sv.size(), "-->"sv) == 0) {
                inComment = false;
                p += "-->"sv.size();
                continue;
            }
            if (inComment) {
                ++p;
                continue;
            }
            if (content[p] == '<' && !inSingleQuote && !inDoubleQuote) {
                ++depthAngleBrackets;
            } else if (content[p] == '>' && !inSingleQuote && !inDoubleQuote) {
                --depthAngleBrackets;
            } else if (content[p] == '\'') {
                inSingleQuote = !inSingleQuote;
            } else if (content[p] == '"') {
                inDoubleQuote = !inDoubleQuote;
            }
            if (depthAngleBrackets == 0)
                break;
            ++p;
        }
        [[maybe_unused]] const std::string_view contents(content.substr(0, p));
        TRACE("DOCTYPE", "contents", contents);
        content.remove_prefix(p);
        assert(content[0] == '>');
        content.remove_prefix(">"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }

    /*
      Anything before the root unit, including the (optional) XML declaration and comments
      are parsed. However, we need to parse the root element for a non-archive unit,
      plus the first nested unit for an archive. The nextUnit() knows how to do this.
    */
    unitSave = new srcml_unit;
    inUnit = false;
    nextUnit(unitSave, true);
    firstAfterRoot = true;
    inUnit = true;
}

int srcMLSplitter::nextUnit(srcml_unit* unit, bool stopRoot) {

    if (isDone)
        return 0;

    if (firstAfterRoot) {
        unit->filename = std::move(unitSave->filename);
        unit->hash = std::move(unitSave->hash);
        unit->language = std::move(unitSave->language);
        unit->version = std::move(unitSave->version);
        unit->timestamp = std::move(unitSave->timestamp);
        unit->content_begin = unitSave->content_begin;
        firstAfterRoot = false;
    }

    unit->encoding = archive->encoding;
    std::string srcml = saveCharacters;
    std::string src = saveCharacters;
    saveCharacters.clear();
    // bool inUnit = false;
    // const char* unitStart = nullptr;
    while (true) {

        if (doneReading) {
            if (content.empty())
                break;
        } else if (content.size() < BLOCK_SIZE) {
            if (inUnit) {
                srcml.append(unitStart, std::distance(unitStart, &content[0]));
            }
            // refill content preserving unprocessed
            int bytesRead = refillContent(content);
            if (bytesRead < 0) {
                std::cerr << "parser error : File input error\n";
                return 1;
            }
            if (bytesRead == 0) {
                doneReading = true;
            }
            totalBytes += bytesRead;
            if (inUnit) {
                unitStart = &content[0];
            }
        }
        if (content[0] == '&') {
            // parse character entity references
            std::string_view unescapedCharacter;
            std::string_view escapedCharacter;
            if (content[1] == 'l' && content[2] == 't' && content[3] == ';') {
                unescapedCharacter = "<";
                escapedCharacter = "&lt;"sv;
            } else if (content[1] == 'g' && content[2] == 't' && content[3] == ';') {
                unescapedCharacter = ">";
                escapedCharacter = "&gt;"sv;
            } else if (content[1] == 'a' && content[2] == 'm' && content[3] == 'p' && content[4] == ';') {
                unescapedCharacter = "&";
                escapedCharacter = "&amp;"sv;
            } else {
                unescapedCharacter = "&";
                escapedCharacter = "&"sv;
            }
            assert(content.compare(0, escapedCharacter.size(), escapedCharacter) == 0);
            content.remove_prefix(escapedCharacter.size());
            [[maybe_unused]] const std::string_view characters(unescapedCharacter);
            if (inUnit) {
                src += characters;
            }
            TRACE("CHARACTERS", "characters", characters);
            ++textSize;
        } else if (content[0] != '<') {
            // parse character non-entity references
            assert(content[0] != '<' && content[0] != '&');
            std::size_t characterEndPosition = content.find_first_of("<&");
            const std::string_view characters(content.substr(0, characterEndPosition));
            if (inUnit)
                src += characters;
            TRACE("CHARACTERS", "characters", characters);
            if (inUnit)
                unit->loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
            loc = unit->loc;
            textSize += static_cast<int>(characters.size());
            // check for nested unit is false
            if (stopRoot) {
                if (characters.find_first_not_of(WHITESPACE) != characters.npos ) {
                    srcml_archive_enable_solitary_unit(archive);
                    return 2;
                }
                saveCharacters = characters;
            }
            content.remove_prefix(characters.size());
        } else if (content[1] == '!' /* && content[0] == '<' */ && content[2] == '-' && content[3] == '-') {
            // parse XML comment
            assert(content.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
            content.remove_prefix("<!--"sv.size());
            std::size_t tagEndPosition = content.find("-->"sv);
            if (tagEndPosition == content.npos && !doneReading) {
                if (inUnit) {
                    srcml.append(unitStart, std::distance(unitStart, &content[0]));
                }
                // refill content preserving unprocessed
                int bytesRead = refillContent(content);
                if (bytesRead < 0) {
                    std::cerr << "parser error : File input error\n";
                    return 1;
                }
                if (bytesRead == 0) {
                    doneReading = true;
                }
                totalBytes += bytesRead;
                tagEndPosition = content.find("-->"sv);
                if (tagEndPosition == content.npos) {
                    std::cerr << "parser error : Unterminated XML comment\n";
                    return 1;
                }
                if (inUnit) {
                    unitStart = &content[0];
                }
            }
            [[maybe_unused]] const std::string_view comment(content.substr(0, tagEndPosition));
            TRACE("COMMENT", "content", comment);
            content.remove_prefix(tagEndPosition);
            content.remove_prefix("-->"sv.size());
        } else if (content[1] == '!' /* && content[0] == '<' */ && content[2] == '[' && content[3] == 'C' && content[4] == 'D' &&
                   content[5] == 'A' && content[6] == 'T' && content[7] == 'A' && content[8] == '[') {
            // check for nested unit is false
            if (stopRoot) {
                srcml_archive_enable_solitary_unit(archive);
                return 2;
            }
            // parse CDATA
            content.remove_prefix("<![CDATA["sv.size());
            std::size_t tagEndPosition = content.find("]]>"sv);
            if (tagEndPosition == content.npos && !doneReading) {
                if (inUnit) {
                    srcml.append(unitStart, std::distance(unitStart, &content[0]));
                }
                // refill content preserving unprocessed
                int bytesRead = refillContent(content);
                if (bytesRead < 0) {
                    std::cerr << "parser error : File input error\n";
                    return 1;
                }
                if (bytesRead == 0) {
                    doneReading = true;
                }
                totalBytes += bytesRead;
                tagEndPosition = content.find("]]>"sv);
                if (tagEndPosition == content.npos) {
                    std::cerr << "parser error : Unterminated CDATA\n";
                    return 1;
                }
                if (inUnit) {
                    unitStart = &content[0];
                }
            }
            const std::string_view characters(content.substr(0, tagEndPosition));
            TRACE("CDATA", "characters", characters);
            textSize += static_cast<int>(characters.size());
            if (inUnit)
                unit->loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
            loc = unit->loc;
            content.remove_prefix(tagEndPosition);
            content.remove_prefix("]]>"sv.size());
        } else if (content[1] == '?' /* && content[0] == '<' */) {
            // parse processing instruction
            assert(content.compare(0, "<?"sv.size(), "<?"sv) == 0);
            content.remove_prefix("<?"sv.size());
            std::size_t tagEndPosition = content.find("?>"sv);
            if (tagEndPosition == content.npos) {
                std::cerr << "parser error: Incomplete XML declaration\n";
                return 1;
            }
            std::size_t nameEndPosition = content.find_first_of(NAMEEND);
            if (nameEndPosition == content.npos) {
                std::cerr << "parser error : Unterminated processing instruction\n";
                return 1;
            }
            [[maybe_unused]] const std::string_view target(content.substr(0, nameEndPosition));
            [[maybe_unused]] const std::string_view data(content.substr(nameEndPosition, tagEndPosition - nameEndPosition));
            TRACE("PI", "target", target, "data", data);
            content.remove_prefix(tagEndPosition);
            assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
            content.remove_prefix("?>"sv.size());
        } else if (content[1] == '/' /* && content[0] == '<' */) {
            // check for nested unit is false
            if (stopRoot) {
                srcml_archive_enable_solitary_unit(archive);
                return 2;
            }
            // parse end tag
            auto savePrevSize = &content[0] - unitStart + 1;
            assert(content.compare(0, "</"sv.size(), "</"sv) == 0);
            content.remove_prefix("</"sv.size());
            if (content[0] == ':') {
                std::cerr << "parser error : Invalid end tag name\n";
                return 1;
            }
            std::size_t nameEndPosition = content.find_first_of(NAMEEND);
            if (nameEndPosition == content.size()) {
                std::cerr << "parser error : Unterminated end tag '" << content.substr(0, nameEndPosition) << "'\n";
                return 1;
            }
            size_t colonPosition = 0;
            if (content[nameEndPosition] == ':') {
                colonPosition = nameEndPosition;
                nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
            }
            const std::string_view qName(content.substr(0, nameEndPosition));
            if (qName.empty()) {
                std::cerr << "parser error: EndTag: invalid element name\n";
                return 1;
            }
            [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
            [[maybe_unused]] const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
            --depth;
            TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            assert(content.compare(0, ">"sv.size(), ">"sv) == 0);
            content.remove_prefix(">"sv.size());
            if (inUnit && localName == "unit"sv) {
                inUnit = false;
                unit->content_end = savePrevSize;
                srcml.append(unitStart, std::distance(unitStart, &content[0]));
                unitStart = &content[0];
                content.remove_prefix(std::max<int>(0, content.find_first_not_of(WHITESPACE)));
                unit->srcml = std::move(srcml);
                unit->src = std::move(src);
                return 2;
            }
            if (depth == 0)
                break;
        } else if (content[0] == '<') {
            // parse start tag
            assert(content.compare(0, "<"sv.size(), "<"sv) == 0);
            auto preserveStart = &content[0];
            // content.remove_prefix("<"sv.size());
            if (content[0 + 1] == ':') {
                std::cerr << "parser error : Invalid start tag name\n";
                return 1;
            }
            std::size_t nameEndPosition = content.find_first_of(NAMEEND, 1);
            if (nameEndPosition == content.size()) {
                std::cerr << "parser error : Unterminated start tag '" << content.substr(1, nameEndPosition) << "'\n";
                return 1;
            }
            size_t colonPosition = 0;
            if (content[nameEndPosition] == ':') {
                colonPosition = nameEndPosition;
                nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
            }
            const std::string_view qName(content.substr(0 + 1, nameEndPosition - 1));
            if (qName.empty()) {
                std::cerr << "parser error: StartTag: invalid element name\n";
                return 1;
            }
            [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition - 1));
            [[maybe_unused]] const std::string_view localName(qName.substr(colonPosition ? colonPosition: 0, nameEndPosition));
            if (localName == "unit"sv) {

                if (inUnit)
                    isArchive = true;

                inUnit = true;
                srcml.clear();
                src.clear();
                unit->loc = 0;
                loc = 0;
                unitStart = preserveStart;
            } else if (stopRoot) {
                // check for nested unit is false
                srcml_archive_enable_solitary_unit(archive);
                return 2;
            }
            // content.remove_prefix("<"sv.size());
            std::string_view unitLocalName = localName;
            TRACE("START TAG", "qName", qName, "prefix", prefix, "localName", localName);
            content.remove_prefix(nameEndPosition);
            content.remove_prefix(content.find_first_not_of(WHITESPACE));
            while (xmlNameMask[content[0]]) {
                if (content[0] == 'x' && content[1] == 'm' && content[2] == 'l' && content[3] == 'n' && content[4] == 's' && (content[5] == ':' || content[5] == '=')) {
                    // parse XML namespace
                    assert(content.compare(0, "xmlns"sv.size(), "xmlns"sv) == 0);
                    content.remove_prefix("xmlns"sv.size());
                    std::size_t nameEndPosition = content.find('=');
                    if (nameEndPosition == content.npos) {
                        std::cerr << "parser error : incomplete namespace\n";
                        return 1;
                    }
                    std::size_t prefixSize = 0;
                    if (content[0] == ':') {
                        content.remove_prefix(":"sv.size());
                        --nameEndPosition;
                        prefixSize = nameEndPosition;
                    }
                    [[maybe_unused]] const std::string_view prefix(content.substr(0, prefixSize));
                    content.remove_prefix(nameEndPosition);
                    content.remove_prefix("="sv.size());
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                    if (content.empty()) {
                        std::cerr << "parser error : incomplete namespace\n";
                        return 1;
                    }
                    const char delimiter = content[0];
                    if (delimiter != '"' && delimiter != '\'') {
                        std::cerr << "parser error : incomplete namespace\n";
                        return 1;
                    }
                    content.remove_prefix("\""sv.size());
                    std::size_t valueEndPosition = content.find(delimiter);
                    if (valueEndPosition == content.npos) {
                        std::cerr << "parser error : incomplete namespace\n";
                        return 1;
                    }
                    [[maybe_unused]] const std::string_view uri(content.substr(0, valueEndPosition));
                    TRACE("NAMESPACE", "prefix", prefix, "uri", uri);

                    std::string suri(uri);
                    std::string sprefix(prefix);
                    srcml_uri_normalize(suri);
                    srcml_archive_register_namespace(archive, sprefix.data(), suri.data());

                    content.remove_prefix(valueEndPosition);
                    assert(content.compare(0, "\""sv.size(), "\""sv) == 0);
                    content.remove_prefix("\""sv.size());
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                } else {
                    // parse attribute
                    std::size_t nameEndPosition = content.find_first_of(NAMEEND);
                    if (nameEndPosition == content.size()) {
                        std::cerr << "parser error : Empty attribute name" << '\n';
                        return 1;
                    }
                    size_t colonPosition = 0;
                    if (content[nameEndPosition] == ':') {
                        colonPosition = nameEndPosition;
                        nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
                    }
                    const std::string_view qName(content.substr(0, nameEndPosition));
                    [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
                    [[maybe_unused]] const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
                    content.remove_prefix(nameEndPosition);
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                    if (content.empty()) {
                        std::cerr << "parser error : attribute " << qName << " incomplete attribute\n";
                        return 1;
                    }
                    if (content[0] != '=') {
                        std::cerr << "parser error : attribute " << qName << " missing =\n";
                        return 1;
                    }
                    content.remove_prefix("="sv.size());
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                    const char delimiter = content[0];
                    if (delimiter != '"' && delimiter != '\'') {
                        std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
                        return 1;
                    }
                    content.remove_prefix("\""sv.size());
                    std::size_t valueEndPosition = content.find(delimiter);
                    if (valueEndPosition == content.npos) {
                        std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
                        return 1;
                    }
                    [[maybe_unused]] const std::string_view value(content.substr(0, valueEndPosition));
                    // convert special srcML escaped element to characters
                    if (unitLocalName == "escape"sv && localName == "char"sv && inUnit) {
                        // use strtol() instead of atoi() since strtol() understands hex encoding of '0x0?'
                        char escapeValue = (char)strtol(value.data(), NULL, 0);
                        src += escapeValue;
                    }
                    TRACE("ATTRIBUTE", "qName", qName, "prefix", prefix, "localName", localName, "value", value);
                    // process Root Unit
                    if (depth == 0) {

                        // Note: these are ignore instead of placing in attributes.
                        if (localName == "timestamp"sv)
                            ;
                        else if (localName == "language"sv)
                            ;
                        else if (localName == "revision"sv)
                            archive->revision = value;
                        else if (localName == "filename"sv)
                            ;
                        else if (localName == "url"sv) {
                            std::string svalue(value);
                            srcml_archive_set_url(archive, svalue.data());
                        }
                        else if (localName == "version"sv) {
                            std::string svalue(value);
                            srcml_archive_set_version(archive, svalue.data());
                            srcml_unit_set_version(unit, svalue.data());
                        } else if (localName == "tabs"sv) {
                            std::string svalue(value);
                            archive->tabstop = static_cast<std::size_t>(atoi(svalue.data()));
                        } else if (localName == "options"sv) {

                            std::size_t commaPos = 0;
                            std::size_t prevCommaPos = 0;
                            while(prevCommaPos < value.size()) {

                                commaPos = value.find(",", commaPos);
                                std::string_view option(&value[prevCommaPos], commaPos - prevCommaPos);
                                prevCommaPos = commaPos;
                                ++prevCommaPos;

                                if (option == "XMLDECL"sv)
                                    archive->options |= SRCML_OPTION_NO_XML_DECL;
                                else if (option == "NAMESPACEDECL"sv)
                                    archive->options |= SRCML_OPTION_NAMESPACE_DECL;
                                else if (option == "CPP_TEXT_ELSE"sv)
                                    archive->options |= SRCML_OPTION_CPP_TEXT_ELSE;
                                else if (option == "CPP_MARKUP_IF0"sv)
                                    archive->options |= SRCML_OPTION_CPP_MARKUP_IF0;
                                else if (option == "LINE"sv)
                                    archive->options |= SRCML_OPTION_LINE;
                            }

                        } else if (localName == "hash"sv)
                            ;
                        else {
                            // @FIXME Should this be qName?
                            archive->attributes.emplace_back(localName);
                            archive->attributes.emplace_back(value);
                        }
                    }
                    if (unitLocalName == "unit"sv) {
                        src.clear();
                        if (localName == "filename"sv)
                            unit->filename = value;
                        else if (localName == "hash"sv)
                            unit->hash = value;
                        else if (localName == "language"sv)
                            unit->language = value;
                        else if (localName == "version"sv)
                            unit->version = value;
                        else if (localName == "timestamp"sv)
                            unit->timestamp = value;
                    }
                    content.remove_prefix(valueEndPosition);
                    content.remove_prefix("\""sv.size());
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                }
            }
            if (content[0] == '>') {
                content.remove_prefix(">"sv.size());
                if (unitLocalName == "unit"sv) {
                    unit->content_begin = &content[0] - unitStart;

                    // check for nested unit is true
                    if (stopRoot && depth > 0) {
                        saveCharacters.clear();
                        srcml_archive_disable_solitary_unit(archive);
                        return 2;
                    }
                }
                ++depth;
            } else if (content[0] == '/' && content[1] == '>') {
                assert(content.compare(0, "/>"sv.size(), "/>") == 0);
                content.remove_prefix("/>"sv.size());
                TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
                if (depth == 0)
                    break;

                if (inUnit && localName == "unit"sv) {
                    inUnit = false;
                    srcml.append(unitStart, std::distance(unitStart, &content[0]));
                    unitStart = &content[0];
                    unit->content_begin = (int) srcml.size();
                    content.remove_prefix(content.find_first_not_of(WHITESPACE));
                    unit->srcml = std::move(srcml);
                    unit->src = std::move(src);
                    assert(depth < 2);
                    return 2;
                }
            }
        } else {
            std::cerr << "parser error : invalid XML document\n";
            return 1;
        }
    }
    content.remove_prefix(content.find_first_not_of(WHITESPACE) == -1ul ? content.size() : content.find_first_not_of(WHITESPACE));
    while (!content.empty() && content[0] == '<' && content[1] == '!' && content[2] == '-' && content[3] == '-') {
        // parse XML comment
        assert(content.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
        std::size_t tagEndPosition = content.find("-->"sv);
        if (tagEndPosition == content.npos && !doneReading) {
            if (inUnit) {
                srcml.append(unitStart, std::distance(unitStart, &content[0]));
            }
            // refill content preserving unprocessed
            int bytesRead = refillContent(content);
            if (bytesRead < 0) {
                std::cerr << "parser error : File input error\n";
                return 1;
            }
            if (bytesRead == 0) {
                doneReading = true;
            }
            totalBytes += bytesRead;
            tagEndPosition = content.find("-->"sv);
            if (tagEndPosition == content.npos) {
                std::cerr << "parser error : Unterminated XML comment\n";
                return 1;
            }
            if (inUnit) {
                unitStart = &content[0];
            }
        }
        content.remove_prefix("<!--"sv.size());
        [[maybe_unused]] const std::string_view comment(content.substr(0, tagEndPosition));
        TRACE("COMMENT", "content", comment);
        content.remove_prefix(tagEndPosition);
        assert(content.compare(0, "-->"sv.size(), "-->"sv) == 0);
        content.remove_prefix("-->"sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE) == -1ul ? content.size() : content.find_first_not_of(WHITESPACE));
    }
    if (!content.empty()) {
        std::cerr << "parser error : extra content at end of document\n";
        return 1;
    }
    TRACE("END DOCUMENT");

    isDone = true;

    return isArchive ? 0 : 3;
}

int srcMLSplitter::refillContent(std::string_view& content) {

    // mark the part of the buffers already consumed
    const int shrinkSize = content.empty() ? lastRead : &content[0] - (const char*) xmlBufContent(inputBuffer->buffer);
    xmlBufShrink(inputBuffer->buffer, shrinkSize);

    // read and append to current data
    int val = xmlParserInputBufferRead(inputBuffer, BUFFER_SIZE);
    int bytesRead = 0;
    if (val == -1) {
        bytesRead = 0;
    } else {
        bytesRead = val;
    }

    // record for shrinkSize on next call
    lastRead = bytesRead;

    // set content to the start of the buffer
    content = std::string_view((const char*) xmlBufContent(inputBuffer->buffer), xmlBufUse(inputBuffer->buffer));

    return bytesRead;
}
