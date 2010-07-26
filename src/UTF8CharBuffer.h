/*

  CharBuffer for antlr that uses a libxml input routines.
  This provides for any necessary encoding conversion to UTF8,
  so using libxml for XML output is direct.

  libxml stores data internally in UTF*
*/

#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <antlr/CharBuffer.hpp>
#include <libxml/parser.h>
#include <cstring>

class UTF8FileError {};

class UTF8CharBuffer : public antlr::CharBuffer {
public:
        // size of the original character buffer
        static const unsigned int SRCBUFSIZE = 512;

	// Create a character buffer
        UTF8CharBuffer(const char* ifilename, const char* encoding);

	/// Get the next character from the stream
	int getChar();

	~UTF8CharBuffer();

private:
	int pos;
	int size;
	bool eof;
	bool lastcr;
	xmlParserInputBufferPtr input;
};
#endif
