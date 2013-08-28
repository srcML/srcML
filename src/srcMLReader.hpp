
#include <libxml/xmlreader.h>

#include <string>

#ifndef INCLUDED_SRCML_READER_HPP
#define INCLUDED_SRCML_READER_HPP

class srcMLReader {

  xmlTextReaderPtr reader;
  bool read_root;

public:

  srcMLReader(const char * filename);
  ~srcMLReader();

  std::string * read();
  

};

#endif
