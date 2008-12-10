#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <istream>
#include <antlr/CharBuffer.hpp>
#include <xmlwriter.h>
#include <cstring>

class UTF8CharBuffer : public antlr::CharBuffer {
public:
	/// Create a character buffer
        UTF8CharBuffer(const char* encoding, std::istream& inp)
	  : antlr::CharBuffer(inp), skipencoding(false), encoding(encoding)
	{
	  handler = xmlFindCharEncodingHandler(encoding);

	  if (std::strcmp(encoding, "UTF-8") == 0)
	    skipencoding = true;
	}

	/// Get the next character from the stream
	int getChar();

private:
	bool skipencoding;
	const char* encoding;
	xmlCharEncodingHandlerPtr handler;
};
#endif
