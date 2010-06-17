
#include "URIStream.h"

URIStream::URIStream(const char* uriname)
  : size(0), startpos(0), endpos(0), eof(false)
{
    input = xmlParserInputBufferCreateFilename(uriname, XML_CHAR_ENCODING_NONE);
}

char* URIStream::getline() {

      // need to refill the buffer
      if (size == 0 || endpos >= size) {

	// previous fill found eof
	if (eof)
	  return 0;

	// refill the buffer
	input->buffer->use = 0;
	size = xmlParserInputBufferGrow(input, 4096);
	
	// found problem or eof
	if (size == -1 || size == 0) {
	  eof = true;
	  return 0;
	}
      }

      // find start of first line
      while (endpos < size && input->buffer->content[endpos] != '\n')
	++endpos;

      if (startpos >= endpos)
	return 0;

      input->buffer->content[endpos] = '\0';

      char* line = (char*) input->buffer->content + startpos;

      startpos = endpos + 1;

      return line;
}

URIStream::~URIStream() {

    xmlFreeParserInputBuffer(input);
}
