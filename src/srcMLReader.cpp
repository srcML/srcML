#include "srcMLReader.hpp"

srcMLReader::srcMLReader(const char * filename) {

  reader = xmlNewTextReaderFilename(filename);

}

srcMLReader::~srcMLReader() {

  xmlFreeTextReader(reader);
  reader = 0;

}

std::string * srcMLReader::read() {

}
