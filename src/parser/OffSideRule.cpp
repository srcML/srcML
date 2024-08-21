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
 * Whenever a `blockStartToken` is found, an INDENT token is generated. Whenever the next line starts with
 * less indentation that the previous line, a DEDENT token is generated (ignores blank lines).
 * 
 * INDENT and DEDENT are meant to handle blocks in languages such as Python that use a colon to represent
 * the start of a block (as opposed to curly braces, etc.).
 */
antlr::RefToken OffSideRule::nextToken() {
    if (buffer.empty()) {
        const auto& token = input.nextToken();

        if (debugInfo) std::cerr << "[ ] line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << (token->getText() == "\n" ? "\\n" : token->getText()) << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

        // Update the current indentation level at the start of each new line
        if (token->getColumn() == 1 && token->getType() != srcMLParser::EOL) {
            prevColStart = currentColStart;
            currentColStart = token->getColumn();

            if (token->getType() == srcMLParser::WS) {
                currentColStart = token->getText().length() + 1;  // e.g., 4 spaces starting at column 1, text starting at column 5
            }

            if (debugInfo) std::cerr << "[1] current and previous column values updated\n";
        }

        // [INDENT] The token matches the token used to indicate the start of a block
        if (token->getType() == blockStartToken) {
            const auto& nextToken = input.nextToken();

            // Check if LA(1) == EOL; valid indentation
            if (nextToken->getType() == srcMLParser::EOL && nextToken->getColumn() > 1) {
                token->setType(srcMLParser::INDENT);
                buffer.emplace_back(nextToken);
                numIndents++;

                if (debugInfo) std::cerr << "[I] line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << token->getText() << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

                return token;
            }

            // Check if LA(1) == WS; could be valid indentation
            if (nextToken->getType() == srcMLParser::WS) {
                const auto& extraToken = input.nextToken();
                buffer.emplace_back(extraToken);

                // Check if LA(2) == EOL; valid indentation
                if (extraToken->getType() == srcMLParser::EOL && extraToken->getColumn() > 1) {
                    token->setType(srcMLParser::INDENT);
                    buffer.emplace_back(nextToken);
                    numIndents++;

                    if (debugInfo) std::cerr << "[I] line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << token->getText() << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

                    return token;
                }
            }

            buffer.emplace_back(nextToken);
            return token;
        }

        // [DEDENT] Files that do not end with a newline need to be handled differently
        if (token->getType() == srcMLParser::EOF_ && token->getText().empty() && numIndents > 0) {
            buffer.emplace_back(token);

            auto dedentToken = srcMLToken::factory();
            dedentToken->setType(srcMLParser::DEDENT);
            dedentToken->setColumn(token->getColumn());
            dedentToken->setLine(token->getLine());

            for (int i = 0; i < numIndents - 1; i++) {
                buffer.emplace_back(dedentToken);
            }

            numIndents = 0;

            if (debugInfo) std::cerr << "[D] line: '" << dedentToken->getLine() << "', col: '" << dedentToken->getColumn() << "', text: '" << dedentToken->getText() << "', type: '" << dedentToken->getType() << "', indents: '" << numIndents << "'\n";

            return dedentToken;
        }

        // [DEDENT] A newline ('\n') token could indicate the end of a block
        if (token->getType() == srcMLParser::EOL && token->getColumn() > 1 && numIndents > 0) {
            while (true) {
                const auto& nextToken = input.nextToken();

                // The next line starts with indentation, so record where the next token would start
                if (nextToken->getType() == srcMLParser::WS) {
                    buffer.emplace_back(nextToken);
                    prevColStart = currentColStart;
                    currentColStart = nextToken->getText().length() + 1;  // e.g., 4 spaces starting at column 1, text starting at column 5

                    while (!blankLineBuffer.empty()) {
                        auto blankLine = srcMLToken::factory();
                        blankLine->setType(blankLineBuffer.back()->getType());
                        blankLine->setText(blankLineBuffer.back()->getText());
                        blankLine->setColumn(blankLineBuffer.back()->getColumn());
                        blankLine->setLine(blankLineBuffer.back()->getLine());

                        buffer.emplace_back(blankLine);
                        blankLineBuffer.pop_back();
                    }

                    if (currentColStart < prevColStart) {
                        auto dedentToken = srcMLToken::factory();
                        dedentToken->setType(srcMLParser::DEDENT);
                        dedentToken->setColumn(1);
                        dedentToken->setLine(token->getLine() + 1);

                        buffer.emplace_back(dedentToken);
                        numIndents--;
                    }

                    return token;
                }

                // The next line is a blank line, so look for the next non-blank-line token
                else if (nextToken->getType() == srcMLParser::EOL && nextToken->getColumn() == 1) {
                    blankLineBuffer.emplace_back(nextToken);
                    continue;
                }

                // The next token is the end of the file, so generate DEDENT tokens equal to the remaining INDENT tokens
                else if (nextToken->getType() == srcMLParser::EOF_) {
                    buffer.emplace_back(nextToken);

                    for (int i = 0; i < numIndents; i++) {
                        auto dedentToken = srcMLToken::factory();
                        dedentToken->setType(srcMLParser::DEDENT);
                        dedentToken->setLine(nextToken->getLine());
                        dedentToken->setColumn(numIndents - i);

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

                    while (!blankLineBuffer.empty()) {
                        auto blankLine = srcMLToken::factory();
                        blankLine->setType(blankLineBuffer.back()->getType());
                        blankLine->setText(blankLineBuffer.back()->getText());
                        blankLine->setColumn(blankLineBuffer.back()->getColumn());
                        blankLine->setLine(blankLineBuffer.back()->getLine());

                        buffer.emplace_back(blankLine);
                        blankLineBuffer.pop_back();
                    }

                    if (currentColStart < prevColStart) {
                        for (int i = 0; i < numIndents; i++) {
                            auto dedentToken = srcMLToken::factory();
                            dedentToken->setType(srcMLParser::DEDENT);
                            dedentToken->setLine(nextToken->getLine());
                            dedentToken->setColumn(numIndents - i);

                            buffer.emplace_back(dedentToken);
                        }

                        numIndents = 0;
                    }

                    return token;
                }
            }
        }

        return token;
    }
    else {
        auto token = srcMLToken::factory();
        token->setType(buffer.back()->getType());
        token->setText(buffer.back()->getText());
        token->setColumn(buffer.back()->getColumn());
        token->setLine(buffer.back()->getLine());
        token->setFilename(buffer.back()->getFilename());

        if (debugInfo) std::cerr << "[B] line: '" << token->getLine() << "', col: '" << token->getColumn() << "', text: '" << (token->getText() == "\n" ? "\\n" : token->getText()) << "', type: '" << token->getType() << "', indents: '" << numIndents << "'\n";

        // Update the current indentation level at the start of each new line
        if (token->getColumn() == 1 && token->getType() != srcMLParser::EOL) {
            prevColStart = currentColStart;
            currentColStart = token->getColumn();

            if (token->getType() == srcMLParser::WS) {
                currentColStart = token->getText().length() + 1;  // e.g., 4 spaces starting at column 1, text starting at column 5
            }

            if (debugInfo) std::cerr << "[2] current and previous column values updated\n";
        }

        buffer.pop_back();
        return token;
    }
}

/**
 * Assigns the value of `token` to `blockStartToken`.
 * 
 * Existing logic was built around the colon (`:`) in Python.
 * Other values may not work as expected, especially in niche situations.
 * 
 * Refer to `srcMLParserTokenTypes.hpp` for all supported values.
 */
void OffSideRule::setBlockStartToken(int token) {
    blockStartToken = token;
}

/**
 * Returns `blockStartToken`.
 */
int OffSideRule::getBlockStartToken() const {
    return blockStartToken;
}
