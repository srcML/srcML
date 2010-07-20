
#include "URIStream.h"
#include <cstring>

URIStream::URIStream(const char* uriname, char eolchar)
  : eol(eolchar), startpos(0), endpos(-1), first(true), eof(false), done(false)
{
  if (!(input = xmlParserInputBufferCreateFilename(uriname, XML_CHAR_ENCODING_NONE)))
    throw URIStreamFileError();

  // get some data into the buffer
  int size = xmlParserInputBufferGrow(input, 4096);

  // found problem or eof
  if (size == -1 || size == 0)
    done = true;
}

char* URIStream::getline() {

  if (done)
    return 0;

  endpos = startpos;

  // find a line in the buffer
  while (input->buffer->content[endpos] != eol) {

    ++endpos;

    // need to refill the buffer
    if (endpos >= input->buffer->use) {

      // need to refill, but previous fill found eof
      if (eof)
	break;

      // shrink the part of the buffer that we are not using yet
      // this is a large buffer, so this will not happen very often, and
      // only if libxml decides for this input source it should
      int removed = xmlBufferShrink(input->buffer, startpos >= 1 ? startpos - 1 : 0);
      endpos -= removed;
      startpos -= removed;

      // refill the buffer
      // put an appropriate value for the length, but note that libxml
      // basically uses 4 or a min value (which is currently around 4096)
      int size = xmlParserInputBufferGrow(input, 4096);

      // found problem or eof
      if (size == -1 || size == 0)
	eof = true;
    }
  }

  // special case
  if (startpos >= input->buffer->use)
    return 0;

  // replace the newline character with a null to turn it into single string
  input->buffer->content[endpos] = '\0';

  // current line starts at the startpos
  char* line = (char*) input->buffer->content + startpos;

  // skip past for the next line
  startpos = endpos + 1;

  return line;
}

URIStream::~URIStream() {

    xmlFreeParserInputBuffer(input);
}
