// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OffSideRule.cpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects INDENT and DEDENT tokens to the token stream
 */

#include <cmath>
#include <OffSideRule.hpp>

/**
 * An abstract method for getting the next token.
 * 
 * Whenever a `blockStartToken` is found, an INDENT token is generated. Whenever the next line
 * starts with less indentation that the previous line, at least 1 DEDENT token is generated.
 * 
 * INDENT and DEDENT are meant to handle blocks (which start with `:`) in Python.
 */
antlr::RefToken OffSideRule::nextToken() {
    // There are no backlogged tokens
    if (buffer.empty()) {
        const auto& token = input.nextToken();  // reads in a token

        // Detect if currently in/out of `()`, `{}`, or `[]`
        checkBracketToken(token);

        // Detect if the statement should have a block
        expectBlockCheck(token);

        // [INDENT] The token matches the token used to indicate the start of a block
        if (token->getType() == blockStartToken && expectBlock && numBrackets == 0) {
            token->setType(srcMLParser::INDENT);
            ++numIndents;
            expectBlock = false;
            recordToken = true;

            handleBlocks(token);
        }

        // Place unhandled token in buffer to ensure it is not lost
        if (buffer.empty())
            buffer.emplace_back(token);
    }

    // Return the token that was most recently added to the buffer
    antlr::RefToken bufferToken = buffer.back();
    buffer.pop_back();
    return bufferToken;
}

/**
 * Checks for indentation/dedentation levels in a block. Handles nested blocks, too.
 * 
 * Indentation should stay consistent when inside the block for best results.
 * Indentation is determined based on the first non-EOL/WS/WS_EOL/Comment token in the block.
 * 
 * Operates under the assumption `token` is an INDENT token.
 */
void OffSideRule::handleBlocks(antlr::RefToken token) {
    int numTokensAfterIndent = 0;  // restricted to 0-2 (inclusive)
    bool stopCounting = false;

    while (true) {
        const auto& nextToken = input.nextToken();  // reads in a token

        // incrementing to detect the first and second token after the INDENT
        if (!stopCounting)
            ++numTokensAfterIndent;

        // the backslash does NOT indicate a one-line statement
        if (numTokensAfterIndent < 3 && nextToken->getType() == srcMLParser::EOL_BACKSLASH) {
            indentBuffer.emplace_back(nextToken);
            isOneLineStatement = false;
            checkOneLineStatement = false;
            stopCounting = true;  // do not need to count after the second token

            continue;
        }

        // do not need to count after the second token
        if (!stopCounting && numTokensAfterIndent > 2)
            stopCounting = true;

        int startingIndents = numIndents;
        bool addNextToken = true;  // Ensure nextToken was not already added to the indent buffer

        // Detect if currently in/out of `()`, `{}`, or `[]`
        checkBracketToken(nextToken);

        // Detect if the statement should have a block
        expectBlockCheck(nextToken);

        // [DEDENT] If the statement begins and ends on a single line, end it at EOL
        if (isOneLineStatement && nextToken->getType() == srcMLParser::EOL && numBrackets == 0) {
            indentBuffer.emplace_back(nextToken);

            auto dedentToken = srcMLToken::factory();
            dedentToken->setType(srcMLParser::DEDENT);
            indentBuffer.emplace_back(dedentToken);

            --numIndents;
            isOneLineStatement = false;
            statementContinues = false;
            addNextToken = false;
        }

        // Check if the statement begins and ends on a single line
        if (checkOneLineStatement) {
            isOneLineStatement = false;

            switch (tokenAfterIndent->getType()) {
                // Typical multi-line statement with a block
                case srcMLParser::EOL:
                case srcMLParser::WS_EOL:
                    break;

                // Whitespace alone is not enough to determine a one-line statement
                case srcMLParser::WS: {
                    // WS + non-Comment token indicates a one-line statement
                    if (!srcMLParser::comment_py_token_set.member(nextToken->getType()))
                        isOneLineStatement = true;

                    break;
                }
                // All other non-comment tokens indicate a one-line statement
                default:
                    if (!srcMLParser::comment_py_token_set.member(tokenAfterIndent->getType()))
                        isOneLineStatement = true;

                    break;
            }

            checkOneLineStatement = false;
        }

        // Record the token directly after INDENT
        if (recordToken) {
            tokenAfterIndent = nextToken;
            checkOneLineStatement = true;
            recordToken = false;
        }

        // [INDENT] The token matches the token used to indicate the start of a block
        if (nextToken->getType() == blockStartToken && expectBlock && numBrackets == 0) {
            nextToken->setType(srcMLParser::INDENT);
            ++numIndents;
            expectBlock = false;
            recordToken = true;
        }

        // The current statement in a block ends with a line continuation character;
        // the code on the next line should NOT end the current block (if applicable)
        if (numIndents > 0 && nextToken->getType() == srcMLParser::EOL_BACKSLASH)
            statementContinues = true;

        // [DEDENT] Found an invalid line continuation character in (at least) one block
        if (statementContinues && (nextToken->getType() == srcMLParser::EOL || nextToken->getType() == srcMLParser::WS_EOL)) {
            indentBuffer.emplace_back(nextToken);

            for (int i = 0; i < numIndents; ++i) {
                auto dedentToken = srcMLToken::factory();
                dedentToken->setType(srcMLParser::DEDENT);
                indentBuffer.emplace_back(dedentToken);
            }

            numIndents = 0;
            statementContinues = false;
            break;
        }

        // Record the indentation level at the start of a line (if in a block)
        // Do not record indentation level if the first statements are one-line statements
        if (numIndents > 0 && nextToken->getColumn() == 1 && nextToken->getType() == srcMLParser::WS && !isOneLineStatement) {
            const auto& postWSToken = input.nextToken();  // reads in a token

            // found WS + EOF_; ensure any DEDENT tokens are placed before WS + EOF_
            if (postWSToken->getType() == srcMLParser::EOF_) {
                // this WS functions as WS_EOL, so change it to WS_EOL
                nextToken->setType(srcMLParser::WS_EOL);

                indentBuffer.emplace_back(nextToken);

                for (int i = 0; i < numIndents; ++i) {
                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    indentBuffer.emplace_back(dedentToken);
                }

                indentBuffer.emplace_back(postWSToken);
    
                numIndents = 0;
                break;
            }

            // Special case: found WS + a comment, or a line continuation character on the previous line
            if (srcMLParser::comment_py_token_set.member(postWSToken->getType()) || statementContinues) {
                indentBuffer.emplace_back(nextToken);

                // Detect if currently in/out of `()`, `{}`, or `[]`
                checkBracketToken(postWSToken);

                // Detect if the statement should have a block
                expectBlockCheck(postWSToken);

                indentBuffer.emplace_back(postWSToken);
                statementContinues = false;
                continue;
            }
            else
                tempPostWSToken = postWSToken;

            // The number of spaces per indent was not initialized yet
            if (numSpacesPerIndent == -1)
                numSpacesPerIndent = static_cast<int>(ceil(static_cast<double>(nextToken->getText().length()) / numIndents));

            // For ceil to work as intended, one of the values must be a float
            int numExpectedIndents = static_cast<int>(ceil(static_cast<double>(nextToken->getText().length()) / numSpacesPerIndent));

            // [DEDENT] There is less indentation on the current line than the previous line
            if (numExpectedIndents < numIndents) {
                indentBuffer.emplace_back(nextToken);

                // Add the extra token from earlier to the original buffer, if applicable
                if (tempPostWSToken->getType() != 0) {
                    indentBuffer.emplace_back(tempPostWSToken);

                    // Detect if currently in/out of `()`, `{}`, or `[]`
                    checkBracketToken(tempPostWSToken);

                    // Detect if the statement should have a block
                    expectBlockCheck(tempPostWSToken);

                    tempPostWSToken = srcMLToken::factory();
                }

                for (int i = 0; i < (numIndents - numExpectedIndents); ++i) {
                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    indentBuffer.emplace_back(dedentToken);
                }

                numIndents = numExpectedIndents;
                addNextToken = false;
            }
        }

        // [DEDENT] Close any INDENT tokens if the start of the next non-EOL/WS/WS_EOL/Comment line is 1
        if (
            numIndents > 0
            && !isOneLineStatement
            && nextToken->getColumn() == 1
            && nextToken->getType() != srcMLParser::EOL
            && nextToken->getType() != srcMLParser::WS
            && nextToken->getType() != srcMLParser::WS_EOL
            && !srcMLParser::comment_py_token_set.member(nextToken->getType())
        ) {
            indentBuffer.emplace_back(nextToken);

            // Add the extra token from earlier to the original buffer, if applicable
            if (tempPostWSToken->getType() != 0) {
                indentBuffer.emplace_back(tempPostWSToken);

                // Detect if currently in/out of `()`, `{}`, or `[]`
                checkBracketToken(tempPostWSToken);

                // Detect if the statement should have a block
                expectBlockCheck(tempPostWSToken);

                tempPostWSToken = srcMLToken::factory();
            }

            for (int i = 0; i < numIndents; ++i) {
                auto dedentToken = srcMLToken::factory();
                dedentToken->setType(srcMLParser::DEDENT);
                indentBuffer.emplace_back(dedentToken);
            }

            numIndents = 0;
        }

        // [DEDENT] Close remaining INDENT tokens if at the end of the file
        else if (numIndents > 0 && nextToken->getType() == srcMLParser::EOF_) {
            indentBuffer.emplace_back(nextToken);

            // Add the extra token from earlier to the original buffer, if applicable
            if (tempPostWSToken->getType() != 0) {
                indentBuffer.emplace_back(tempPostWSToken);

                // Detect if currently in/out of `()`, `{}`, or `[]`
                checkBracketToken(tempPostWSToken);

                // Detect if the statement should have a block
                expectBlockCheck(tempPostWSToken);

                tempPostWSToken = srcMLToken::factory();
            }

            for (int i = 0; i < numIndents; ++i) {
                auto dedentToken = srcMLToken::factory();
                dedentToken->setType(srcMLParser::DEDENT);
                indentBuffer.emplace_back(dedentToken);
            }

            numIndents = 0;
        }

        // Place unhandled token in buffer to ensure it is not lost
        else {
            // Is false if any DEDENT tokens were generated before this if-else block
            // In that case, nextToken was already added to the indent buffer
            if (addNextToken) {
                indentBuffer.emplace_back(nextToken);

                // Add the extra token from earlier to the original buffer, if applicable
                if (tempPostWSToken->getType() != 0) {
                    indentBuffer.emplace_back(tempPostWSToken);

                    // Detect if currently in/out of `()`, `{}`, or `[]`
                    checkBracketToken(tempPostWSToken);

                    // Detect if the statement should have a block
                    expectBlockCheck(tempPostWSToken);

                    tempPostWSToken = srcMLToken::factory();
                }
            }
        }

        // Block is finished once there are zero INDENTs remaining
        if (startingIndents > 0 && numIndents == 0)
            break;
    }

    arrangeBlockEnds();
    buffer.emplace_back(token);
}

/**
 * Approximates where a block "should" end, then moves DEDENT tokens to that location.
 * 
 * Should only be called after generating DEDENT tokens via the `handleBlocks` function.
 */
void OffSideRule::arrangeBlockEnds() {
    while (!indentBuffer.empty()) {
        // Found at least 1 DEDENT token, so re-arrange them to fit closer to where the block "ends"
        if (indentBuffer.back()->getType() == srcMLParser::DEDENT) {
            int numDedents = 0;

            // Record and remove all DEDENTs from the indent buffer
            while (indentBuffer.back()->getType() == srcMLParser::DEDENT) {
                ++numDedents;
                indentBuffer.pop_back();
            }

            // Top of indent buffer is the first token after the block ends
            // Remove it from the indent buffer and add it to the original buffer
            buffer.emplace_back(indentBuffer.back());
            indentBuffer.pop_back();

            // Remove tokens until we find a non-EOL/WS/WS_EOL/Comment token
            // Add any token removed this way to the temporary buffer
            while (!indentBuffer.empty()) {
                if (
                    indentBuffer.back()->getType() == srcMLParser::EOL
                    || indentBuffer.back()->getType() == srcMLParser::WS
                    || indentBuffer.back()->getType() == srcMLParser::WS_EOL
                    || srcMLParser::comment_py_token_set.member(indentBuffer.back()->getType())
                ) {
                    tempBuffer.emplace_front(indentBuffer.back());
                    indentBuffer.pop_back();
                }
                else
                    break;
            }

            // Handle invalid indentation or blocks with only comments as their content
            if (indentBuffer.empty()) {
                // Empty the rest of the temporary buffer and add its contents to the original buffer
                while (!tempBuffer.empty()) {
                    buffer.emplace_back(tempBuffer.back());
                    tempBuffer.pop_back();
                }

                // Add the DEDENT token(s) to the original buffer
                for (int i = 0; i < numDedents; ++i) {
                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    buffer.emplace_back(dedentToken);
                }

                break;
            }

            // DEDENT tokens do not contain line/column info, so they are handled differently
            // Occurs if a one-line statement is the last/only content in a block
            // (e.g., a Python class with only a one-line function in the class)
            if (indentBuffer.back()->getType() == srcMLParser::DEDENT) {
                // Empty the rest of the temporary buffer and add its contents to the original buffer
                while (!tempBuffer.empty()) {
                    buffer.emplace_back(tempBuffer.back());
                    tempBuffer.pop_back();
                }

                // Add the DEDENT token(s) to the original buffer
                for (int i = 0; i < numDedents; ++i) {
                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    buffer.emplace_back(dedentToken);
                }
            }
            else {
                int blockEndLine = indentBuffer.back()->getLine();  // DEDENT will go on the line after this line number (where the block "ends")

                // Empty the temporary buffer into the original buffer until reaching the block end line
                if (!tempBuffer.empty()) {
                    while (tempBuffer.back()->getLine() != blockEndLine) {
                        buffer.emplace_back(tempBuffer.back());
                        tempBuffer.pop_back();
                    }
                }

                // Add the DEDENT token(s) to the original buffer
                for (int i = 0; i < numDedents; ++i) {
                    auto dedentToken = srcMLToken::factory();
                    dedentToken->setType(srcMLParser::DEDENT);
                    buffer.emplace_back(dedentToken);
                }

                // Empty the rest of the temporary buffer and add its contents to the original buffer
                while (!tempBuffer.empty()) {
                    buffer.emplace_back(tempBuffer.back());
                    tempBuffer.pop_back();
                }
            }
        }
        // Add non-DEDENT tokens back to the original buffer so they are not lost
        else {
            buffer.emplace_back(indentBuffer.back());
            indentBuffer.pop_back();
        }
    }
}

/**
 * Detects opening and closing brackets (e.g., `()`, `{}`, and `[]`).
 * 
 * Ensures certain colon (`:`) tokens do not start blocks in Python.
 * Examples including array slicing, dictionairies, and type annotations.
 * 
 * Operates under the assumption the code contains balanced brackets.
 */
void OffSideRule::checkBracketToken(antlr::RefToken token) {
    if (srcMLParser::left_bracket_py_token_set.member(token->getType())) {
        ++numBrackets;
        bracketBuffer.emplace_front(token->getType());
    }
    else if (!bracketBuffer.empty() && srcMLParser::right_bracket_py_token_set.member(token->getType())) {
        --numBrackets;
        bracketBuffer.pop_front();
    }
}

/**
 * Checks if a block is expected because a line starts with `token` in Python.
 * 
 * Such tokens include keywords (e.g., `if`), specifiers (e.g., `async`), and attributes (e.g., `@`).
 */
void OffSideRule::expectBlockCheck(antlr::RefToken token) {
    if ((token->getColumn() == 1 || delayExpectBlockCheck) && numBrackets == 0) {
        if (delayExpectBlockCheck)
            delayExpectBlockCheck = false;

        if (srcMLParser::expect_blocks_py_token_set.member(token->getType()))
            expectBlock = true;

        if (token->getType() == srcMLParser::PY_FUNCTION || token->getType() == srcMLParser::CLASS)
            isFunctionOrClass = true;

        // whitespace at column 1 is indentation
        if (token->getType() == srcMLParser::WS)
            delayExpectBlockCheck = true;
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
