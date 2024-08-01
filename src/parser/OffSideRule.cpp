// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OffSideRule.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects INDENT and DEDENT tokens to the token stream
 */

#include <OffSideRule.hpp>

/**
 * An abstract method for getting the next token.
 *
 * ** Currently unfinished; DEDENT token logic is not fully implemented. **
 *
 * INDENT and DEDENT tokens are generated using the column position values of the first non-whitespace
 * character in the non-blank lines of a file. INDENT and DEDENT are meant to handle blocks in languages
 * such as Python that use a colon to represent the start of a block (as opposed to curly braces, etc.).
*/
antlr::RefToken OffSideRule::nextToken() {
    if (buffer.empty()) {
        const auto& token = input.nextToken();

        if (debugInfo) std::cerr << "line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << token->getText() << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

        // Set the initial indentation level
        // @TODO: Assume the starting indentation level was 1 and the starting current level is also 1 

        // [INDENT] A colon (':') token could indicate start of a block
        if (token->getType() == srcMLParser::COLON) {
            const auto& nextToken = input.nextToken();
            buffer.emplace_back(nextToken);

            // Generate an INDENT token because the colon is followed by a newline
            // Note: Colons could be part of a comment, a slicing operator, etc.
            if (nextToken->getType() == srcMLParser::EOL) {
                auto indentToken = srcMLToken::factory();
                indentToken->setType(srcMLParser::INDENT);
                indentToken->setText("<block>");

                // <block> must start BEFORE the colon
                buffer.emplace_back(token);
                numIndents++;

                return indentToken;
            }

            return token;
        }

        // [DEDENT] Files that do not end with a newline need to be handled differently
        if (token->getType() == srcMLParser::EOF_ && token->getText().empty() && numIndents > 0) {
            buffer.emplace_back(token);

            auto dedentToken = srcMLToken::factory();
            dedentToken->setType(srcMLParser::DEDENT);
            dedentToken->setText("</block>");
            dedentToken->setColumn(1);

            for (int i = 0; i < numIndents - 1; i++) {
                buffer.emplace_back(dedentToken);
            }

            numIndents = 0;
            return dedentToken;
        }

        // [DEDENT] A newline ('\n') token could indicate the end of a block
        if (token->getType() == srcMLParser::EOL && token->getColumn() > 1 && numIndents > 0) {
            int blankLines = 0;

            while (true) {
                const auto& nextToken = input.nextToken();

                // The next line starts with indentation, so record where the next token would start
                if (nextToken->getType() == srcMLParser::WS) {
                    buffer.emplace_back(nextToken);
                    prevColStart = currentColStart;
                    currentColStart = nextToken->getText().length() + 1;  // e.g., 4 spaces starting at column 1, text starting at column 5

                    if (currentColStart < prevColStart) {
                        auto dedentToken = srcMLToken::factory();
                        dedentToken->setType(srcMLParser::DEDENT);
                        dedentToken->setText("</block>");
                        dedentToken->setColumn(1);

                        buffer.emplace_back(dedentToken);
                        numIndents--;
                    }

                    while (blankLines > 0) {
                        auto blankLine = srcMLToken::factory();
                        blankLine->setType(srcMLParser::EOL);
                        blankLine->setText("\n");
                        blankLine->setColumn(1);

                        buffer.emplace_back(blankLine);
                        blankLines--;
                    }

                    return token;
                }

                // The next line is a blank line, so look for the next non-blank-line token
                else if (nextToken->getType() == srcMLParser::EOL && nextToken->getColumn() == 1) {
                    //buffer.emplace_back(nextToken);
                    blankLines++;
                    continue;
                }

                // The next token is the end of the file, so generate DEDENT tokens equal to the remaining INDENT tokens
                else if (nextToken->getType() == srcMLParser::EOF_) {
                    buffer.emplace_back(nextToken);

                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    dedentToken->setText("</block>");

                    for (int i = 0; i < numIndents; i++) {
                        buffer.emplace_back(dedentToken);
                    }

                    numIndents = 0;
                    return token;
                }

                // The next token starts at column 1
                else {
                    buffer.emplace_back(nextToken);
                    prevColStart = currentColStart;
                    currentColStart = 1;

                    while (blankLines > 0) {
                        auto blankLine = srcMLToken::factory();
                        blankLine->setType(srcMLParser::EOL);
                        blankLine->setText("\n");
                        blankLine->setColumn(1);

                        buffer.emplace_back(blankLine);
                        blankLines--;
                    }

                    if (currentColStart < prevColStart) {
                        while (numIndents > 0) {
                            auto dedentToken = srcMLToken::factory();
                            dedentToken->setType(srcMLParser::DEDENT);
                            dedentToken->setText("</block>");
                            dedentToken->setColumn(1);

                            buffer.emplace_back(dedentToken);
                            numIndents--;
                        }
                    }

                    return token;
                }
            }
        }

/* ... */

        // // [DEDENT] At the end of a line, potentially generate DEDENT tokens
        // if (token->getType() == srcMLParser::EOL && numIndents > 0) {
        //     const auto& nextToken = input.nextToken();
        //     buffer.emplace_back(nextToken);

        //     if (nextToken->getColumn() == 1) {
        //         prevColStart = currentColStart;
        //         currentColStart = 1;

        //         // Record the column value after the spaces
        //         if (token->getType() == srcMLParser::WS) {
        //             currentColStart = token->getText().length();
        //         }

        //         if (currentColStart < prevColStart) {
        //             auto dedentToken = srcMLToken::factory();
        //             dedentToken->setType(srcMLParser::DEDENT);
        //             dedentToken->setText("</block>");
        //             dedentToken->setColumn(1);

        //             buffer.emplace_back(dedentToken);
        //             numIndents--;
        //         }
        //     }

        //     return token;
        // }

        // // [DEDENT] At EOF, generate DEDENT tokens equal to the number of INDENT tokens
        // //          Triggers if the file has a blank line at the end of the file
        // if (token->getType() == srcMLParser::EOL && numIndents > 0) {
        //     const auto& nextToken = input.nextToken();
        //     buffer.emplace_back(nextToken);

        //     if (nextToken->getType() == srcMLParser::EOF_) {
        //         auto dedentToken = srcMLToken::factory();
        //         dedentToken->setType(srcMLParser::DEDENT);
        //         dedentToken->setText("</block>");

        //         for (int i = 0; i < numIndents; i++) {
        //             buffer.emplace_back(dedentToken);
        //         }

        //         numIndents = 0;
        //         return token;
        //     }
        // }

        // // [DEDENT] At EOF, generate DEDENT tokens equal to the number of INDENT tokens
        // //          Triggers if the file does NOT have a blank line at the end of the file
        // if (token->getType() == srcMLParser::EOF_ && numIndents > 0) {
        //     buffer.emplace_back(token);

        //     auto dedentToken = srcMLToken::factory();
        //     dedentToken->setType(srcMLParser::DEDENT);
        //     dedentToken->setText("</block>");

        //     for (int i = 0; i < numIndents - 1; i++) {
        //         buffer.emplace_back(dedentToken);
        //     }

        //     numIndents = 0;
        //     return dedentToken;
        // }

        return token;
    }
    else {
        auto token = srcMLToken::factory();
        token->setType(buffer.back()->getType());
        token->setText(buffer.back()->getText());
        token->setColumn(buffer.back()->getColumn());
        token->setLine(buffer.back()->getLine());
        token->setFilename(buffer.back()->getFilename());

        if (debugInfo) std::cerr << "*** line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << token->getText() << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

        buffer.pop_back();
        return token;
    }
}




/* INCORRECT VERSION
antlr::RefToken OffSideRule::nextToken() {
    if (buffer.empty()) {
        const auto& token = input.nextToken();
        bool isBlankLine = (token->getType() == srcMLParser::EOL && token->getColumn() == 1);

        // Look ahead once at a blank line to see if a DEDENT token should be generated
        // Skip blank lines otherwise
        if (isBlankLine) {
            if (numIndents > 0) {
                const auto& nextToken = input.nextToken();
                buffer.emplace_back(nextToken);

                // Skip consecutive blank lines until a line with content is reached
                if (nextToken->getType() == srcMLParser::EOL && nextToken->getColumn() == 1) {
                    return token;
                }

                if (nextToken->getColumn() == 1) {
                    if (nextToken->getType() == srcMLParser::WS) {
                        prevStartCol = currentStartCol;
                        currentStartCol = nextToken->getText().length();
                    }
                    else {
                        prevStartCol = currentStartCol;
                        currentStartCol = 0;
                    }
                }

                if (prevStartCol > currentStartCol) {
                    if (debugInfo) std::cerr << "--> GENERATE DEDENT TOKEN\n";
                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    dedentToken->setText("</block>");

                    buffer.emplace_back(token);
                    numIndents--;

                    return dedentToken;
                }
            }

            return token;
        }

        // Handle the first line of the file
        if (prevStartCol == -1) {
            // Do not record whitespace
            if (token->getType() == srcMLParser::WS) {
                return token;
            }

            prevStartCol = token->getColumn();
            currentStartCol = token->getColumn();
        }
        // Set the number of spaces the current line is indented
        else if (token->getColumn() == 1) {
            if (token->getType() == srcMLParser::WS) {
                prevStartCol = currentStartCol;
                currentStartCol = token->getText().length();
            }
            else {
                prevStartCol = currentStartCol;
                currentStartCol = 0;
            }
        }

        // Generate a DEDENT token if there is at least one INDENT token and the indentation is decreased
        if (token->getColumn() == 1 && numIndents > 0) {
            if (debugInfo) std::cerr << "(p: " << prevStartCol << ") vs. (c: " << currentStartCol << ")\n";
            if (prevStartCol > currentStartCol) {
                if (debugInfo) std::cerr << "--> GENERATE DEDENT TOKEN\n";
                auto dedentToken = srcMLToken::factory();
                dedentToken->setType(srcMLParser::DEDENT);
                dedentToken->setText("</block>");

                buffer.emplace_back(token);
                numIndents--;

                return dedentToken;
            }
        }

        // A colon (':') token could indicate a block
        if (token->getType() == srcMLParser::COLON) {
            const auto& nextToken = input.nextToken();
            buffer.emplace_back(nextToken);

            // Generate an INDENT token because the colon is followed by a newline
            // Note: Colons could be part of a comment, a slicing operator, etc.
            if (nextToken->getType() == srcMLParser::EOL) {
                auto indentToken = srcMLToken::factory();
                indentToken->setType(srcMLParser::INDENT);
                indentToken->setText("<block>");

                buffer.emplace_back(indentToken);
                numIndents++;
            }

            return token;
        }

        // Generate DEDENT tokens equal to the number of INDENT tokens if applicable at EOF

        // Note: We need to look ahead for EOF or else it will not handle all DEDENT tokens
        //       Look ahead after every newline to find EOF? Try to reduce look aheads
        if (token->getType() == srcMLParser::EOL && numIndents > 0) {
            const auto& nextToken = input.nextToken();
            buffer.emplace_back(nextToken);

            if (nextToken->getType() == srcMLParser::EOF_) {
                auto dedentToken = srcMLToken::factory();
                dedentToken->setType(srcMLParser::DEDENT);
                dedentToken->setText("</block>");

                for (int i = 0; i < numIndents; i++) {
                    buffer.emplace_back(dedentToken);
                }

                numIndents = 0;
                return token;
            }
        }

        if (debugInfo) std::cerr << "line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << token->getText() << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

        return token;
    }
    else {
        auto token = srcMLToken::factory();
        token->setType(buffer.back()->getType());
        token->setText(buffer.back()->getText());

        if (debugInfo) std::cerr << "*** line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << token->getText() << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

        buffer.pop_back();
        return token;
    }
}*/
