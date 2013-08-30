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

