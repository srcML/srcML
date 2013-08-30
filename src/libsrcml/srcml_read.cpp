#include "srcml.h"
#include "srcml_types.hpp"

void srcml_read_internal(srcml_archive * archive) {

  archive->type = SRCML_ARCHIVE_READ;

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  bool done = !archive->reader->readRootUnitAttributes(&language, &filename, &directory, &version,
                                                       archive->attributes, archive->prefixes,
                                                       archive->namespaces,
                                                       archive->options,
                                                       archive->tabstop);
  if(!done) {

    archive->language = language;
    archive->filename = filename;
    archive->directory = directory;
    archive->version = version;

  }

}

/* open a srcML archive for reading */
int srcml_read_open_filename(srcml_archive* archive, const char* srcml_filename) {

  archive->reader = new srcMLReader(srcml_filename);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_memory(srcml_archive* archive, const char* buffer, size_t buffer_size) {

  archive->input = xmlParserInputBufferCreateMem(buffer, buffer_size, xmlParseCharEncoding(0));
  archive->reader = new srcMLReader(archive->input);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_FILE(srcml_archive* archive, FILE* srcml_file) {

  archive->input = xmlParserInputBufferCreateFile(srcml_file, xmlParseCharEncoding(0));
  archive->reader = new srcMLReader(archive->input);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

int srcml_read_open_fd(srcml_archive* archive, int srcml_fd) {

  archive->input = xmlParserInputBufferCreateFd(srcml_fd, xmlParseCharEncoding(0));
  archive->reader = new srcMLReader(archive->input);
  srcml_read_internal(archive);

  return SRCML_STATUS_OK;

}

