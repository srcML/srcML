#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <istream>
#include <antlr/CharBuffer.hpp>
#include <libxml/xmlwriter.h>
#include <cstring>

class UTF8CharBuffer : public antlr::CharBuffer {
public:
        // size of the original character buffer
        static const unsigned int SRCBUFSIZE = 512;

	/// Create a character buffer
        UTF8CharBuffer(const char* ifilename, const char* encoding, std::istream& inp)
	  : antlr::CharBuffer(inp), skipencoding(false), encoding(encoding), pos(0), size(0), eof(false), lastcr(false)
	{
	  // use a libxml2 parser input buffer to support URIs
	  input = xmlParserInputBufferCreateFilename(ifilename, xmlParseCharEncoding(encoding));
	}

	/// Get the next character from the stream
	int getChar();

	~UTF8CharBuffer();

private:
	bool skipencoding;
	const char* encoding;
	xmlCharEncodingHandlerPtr handler;
	xmlBufferPtr buffer;
	xmlBufferPtr utf8buffer;
	unsigned int pos;
	unsigned int size;
	bool eof;
	bool lastcr;
	xmlParserInputBufferPtr input;
};
#endif
