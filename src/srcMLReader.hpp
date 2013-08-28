
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_READER_HPP
#define INCLUDED_SRCML_READER_HPP

class srcMLReader {

  xmlParserInputBufferPtr input;
  xmlTextReaderPtr reader;
  xmlNodePtr node;
  bool is_archive;
  std::vector<xmlNodePtr> save_nodes;
  bool done;

public:

  srcMLReader(const char * filename);
  srcMLReader(const char * buffer, int size);
  srcMLReader(FILE * file);
  srcMLReader(int fd);
  srcMLReader(xmlParserInputBufferPtr input);
  ~srcMLReader();

  void readUnitAttributesInternal(std::string ** language, std::string ** filename,
                                  std::string ** directory, std::string ** version);
  int readUnitAttributes(std::string ** language, std::string ** filename,
                          std::string ** directory, std::string ** version);
  int readsrcML(xmlTextWriterPtr writer);
  int read(xmlOutputBufferPtr output_buffer);
  std::string * readsrcML();
  std::string * read();

};

#endif
