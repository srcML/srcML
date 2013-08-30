#include "srcml.h"
#include "srcml_types.hpp"

/* open a srcML archive for output */
int srcml_write_open_filename(srcml_archive* archive, const char* srcml_filename) {

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            srcml_filename,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

int srcml_write_open_memory(srcml_archive* archive, char** buffer) {

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            buffer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

int srcml_write_open_FILE(srcml_archive* archive, FILE* srcml_file) {

  xmlTextWriterPtr writer = xmlNewTextWriter(xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0)));

  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            writer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

int srcml_write_open_fd(srcml_archive* archive, int srcml_fd) {

  xmlTextWriterPtr writer = xmlNewTextWriter(xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0)));


  archive->type = SRCML_ARCHIVE_WRITE;
  archive->translator = new srcMLTranslator(srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                            0, archive->encoding ? archive->encoding->c_str() : 0,
                                            writer,
                                            archive->options,
                                            archive->directory ? archive->directory->c_str() : 0,
                                            archive->filename ? archive->filename->c_str() : 0,
                                            archive->version ? archive->version->c_str() : 0,
                                            (const char **)&archive->prefixes.front(),
                                            archive->tabstop);

  return SRCML_STATUS_OK;

}

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

/* Read the next unit from the archive */
const srcml_unit* srcml_read_unit_archive(srcml_archive* archive) {

  return srcml_read_unit(archive);

}

const srcml_unit* srcml_read_archive_current_unit(const srcml_archive* archive) {

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  bool done = !archive->reader->readUnitAttributes(&language, &filename, &directory, &version);

  srcml_unit * unit = 0;
  if(!done) {
    unit = srcml_create_unit((srcml_archive *)archive);
    unit->language = language;
    unit->filename = filename;
    unit->directory = directory;
    unit->version = version;
  }

  return unit;

}

int srcml_write_unit(srcml_archive* archive, const srcml_unit* unit) {

  archive->translator->add_unit(unit->unit->c_str());

  return SRCML_STATUS_OK;
}

/* Read the next unit from the archive */
srcml_unit* srcml_read_unit(srcml_archive* archive) {

  std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
  archive->reader->readUnitAttributes(&language, &filename, &directory, &version);
  std::string * read_unit = archive->reader->readsrcML();

  srcml_unit * unit = 0;
  if(read_unit) {
    unit = srcml_create_unit(archive);
    unit->unit = read_unit;
    unit->language = language;
    unit->filename = filename;
    unit->directory = directory;
    unit->version = version;
  }

  return unit;
}


