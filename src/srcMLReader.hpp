#ifndef INCLUDED_SRCML_READER_HPP
#define INCLUDED_SRCML_READER_HPP

class srcMLReader {

  xmlTextReader reader;

public:

  srcMLReader(const char * filename);
  ~srcMLReader();

  std::string * read();
  

};

#endif
