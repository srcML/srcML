
#include "URIStream.h"

URIStream::URIStream(const char* uriname)
  : startpos(0), endpos(-1), first(true), eof(false)
{
    input = xmlParserInputBufferCreateFilename(uriname, XML_CHAR_ENCODING_NONE);
}

char* URIStream::getline() {

  endpos = startpos;

  // find a line in the buffer
  while (first || input->buffer->content[endpos] != '\n') {

    first = false;

    // need to refill the buffer
    if (endpos >= input->buffer->use) {

      // need to refill, but previous fill found eof
      if (eof)
	break;

      // shrink the part of the buffer that we are not using yet
      // this is a large buffer, so this will not happen very often
      xmlBufferShrink(input->buffer, startpos >= 1 ? startpos - 1 : 0);
      endpos -= startpos;
      startpos = 0;

      // refill the buffer
      // basically, choice is 4 or 4096
      int size = xmlParserInputBufferGrow(input, 4096);
      fprintf(stderr, "REFILL\n");
      // found problem or eof
      if (size == -1 || size == 0)
	eof = true;
    }

    ++endpos;
  }

  if (startpos >= input->buffer->use)
    return 0;

  // replace the newline with a null to turn it into single string
  input->buffer->content[endpos] = '\0';

  // current line starts at the startpos
  char* line = (char*) input->buffer->content + startpos;

  // start over for the next line
  startpos = endpos + 1;

  return line;
}

URIStream::~URIStream() {

    xmlFreeParserInputBuffer(input);
}
