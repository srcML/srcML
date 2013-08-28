
#include <libxml/xmlreader.h>


#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_READER_HPP
#define INCLUDED_SRCML_READER_HPP

class srcMLReader {

  xmlTextReaderPtr reader;
  xmlNodePtr node;
  bool read_root;
  std::vector<xmlNodePtr> save_nodes;
  bool done;

public:

  srcMLReader(const char * filename);
  ~srcMLReader();

  void readUnitAttributesInternal(std::string ** language, std::string ** filename,
                                  std::string ** directory, std::string ** version);
  void readUnitAttributes(std::string ** language, std::string ** filename,
                          std::string ** directory, std::string ** version);
  std::string * read();


};

#endif
