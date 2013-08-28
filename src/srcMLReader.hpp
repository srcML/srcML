
#include <libxml/xmlreader.h>

#include <string>

#ifndef INCLUDED_SRCML_READER_HPP
#define INCLUDED_SRCML_READER_HPP

class srcMLReader {

  xmlTextReaderPtr reader;
  xmlNodePtr node;
  bool read_root;

public:

  srcMLReader(const char * filename);
  ~srcMLReader();

  void readUnitHeader(std::string ** language, std::string ** filename,
                                 std::string ** directory, std::string ** version);
  std::string * read(std::string ** language, std::string ** filename,
                                 std::string ** directory, std::string ** version);
  

};

#endif
