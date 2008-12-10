#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <istream>
#include <antlr/CharBuffer.hpp>

class UTF8CharBuffer : public antlr::CharBuffer {
public:
	/// Create a character buffer
	UTF8CharBuffer(std::istream& inp)
	  : antlr::CharBuffer(inp)
	{}

	/// Get the next character from the stream
	int getChar();

private:
};

#endif
