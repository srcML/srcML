#ifndef TEXTTOKENFILTER_HPP
#define TEXTTOKENFILTER_HPP

#include <srcMLParser.hpp>

class TextTokenFilter : public antlr::TokenStream {
private:
    antlr::TokenStream& input;
    static int tokenType;

public:
    TextTokenFilter(antlr::TokenStream& input) : input(input) {}

    virtual antlr::RefToken nextToken() {
        antlr::RefToken token = input.nextToken();

        // save token if not a skipped token
        if (!srcMLParser::skip_tokens_set.member(token->getType())) {
            tokenType = token->getType();
        }

        return token;
    }

    static int lastTokenType() {
        return tokenType;
    }
};

int TextTokenFilter::tokenType = 0;

#endif
