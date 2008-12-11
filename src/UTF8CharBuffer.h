#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <istream>
#include <antlr/CharBuffer.hpp>
#include <xmlwriter.h>
#include <cstring>

// size of the original character buffer
const int SRCBUFSIZE = 1024;

// size of the resulting utf-8 character buffer
const int UTF8BUFSIZE = SRCBUFSIZE * 4;

class UTF8CharBuffer : public antlr::CharBuffer {
public:
	/// Create a character buffer
        UTF8CharBuffer(const char* encoding, std::istream& inp)
	  : antlr::CharBuffer(inp), skipencoding(false), encoding(encoding), pos(0), eof(false)
	{
	  handler = xmlFindCharEncodingHandler(encoding);

	  if (std::strcmp(encoding, "UTF-8") == 0)
	    skipencoding = true;

	  buffer = xmlBufferCreateSize(SRCBUFSIZE);
	  utf8buffer = xmlBufferCreateSize(UTF8BUFSIZE);
	}

	/// Get the next character from the stream
	int getChar();

private:
	bool skipencoding;
	const char* encoding;
	xmlCharEncodingHandlerPtr handler;
	xmlBufferPtr buffer;
	xmlBufferPtr utf8buffer;
	unsigned int pos;
	bool eof;
};
#endif
