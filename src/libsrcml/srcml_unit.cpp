#include "srcml.h"
#include "srcml_types.hpp"

#include "../srcMLUtility.hpp"

void srcml_parse_unit_internal(srcml_unit * unit, int lang) {

  xmlBuffer * output_buffer = xmlBufferCreate();
  unit->archive->translator->translate_separate(0, unit->directory ? unit->directory->c_str() : 0,
                                                unit->filename ? unit->filename->c_str() : 0,
                                                unit->version ? unit->version->c_str() : 0, lang, output_buffer);

  int length = strlen((const char *)output_buffer->content);
  while(length > 0 && output_buffer->content[length - 1] == '\n')
    --length;
  if(unit->unit) delete unit->unit;
  unit->unit = new std::string((const char *)output_buffer->content, length);
  xmlBufferFree(output_buffer);

}

/* Convert to srcml and append to the archive */
// TODO What is this supposed to do.  Right now append unit to archive, that is noop.
int srcml_parse_unit_archive (srcml_archive* archive, srcml_unit* unit) {

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_filename(srcml_unit* unit, const char* src_filename) {

  int lang = unit->language ? srcml_check_language(unit->language->c_str()) : Language::getLanguageFromFilename(src_filename, unit->archive->registered_languages);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  unit->archive->translator->setInput(src_filename);

  srcml_parse_unit_internal(unit, lang);


  return SRCML_STATUS_OK;

}

int srcml_parse_unit_memory(srcml_unit* unit, const char* src_buffer, size_t buffer_size) {

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  unit->archive->translator->setInputString(src_buffer, (int)buffer_size);
  srcml_parse_unit_internal(unit, lang);

  unit->archive->options = save_options;

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_FILE(srcml_unit* unit, FILE* src_file) {

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFile(src_file, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
  unit->archive->translator->setInput(input);

  srcml_parse_unit_internal(unit, lang);
  xmlFreeParserInputBuffer(input);

  unit->archive->options = save_options;

  return SRCML_STATUS_OK;

}

int srcml_parse_unit_fd(srcml_unit* unit, int src_fd) {

  int lang = srcml_check_language(unit->language ? unit->language->c_str() : 0);

  OPTION_TYPE save_options = unit->archive->options;

  if(lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX)
    unit->archive->options |= OPTION_CPP;
  else if (lang == Language::LANGUAGE_CSHARP)
    unit->archive->options |= OPTION_CPP_NOMACRO;

  xmlParserInputBufferPtr input = xmlParserInputBufferCreateFd(src_fd, unit->archive->encoding ? xmlParseCharEncoding(unit->archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
  unit->archive->translator->setInput(input);

  srcml_parse_unit_internal(unit, lang);
  xmlFreeParserInputBuffer(input);

  unit->archive->options = save_options;

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_filename(srcml_unit* unit, const char* src_filename) {

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(src_filename, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"), unit->archive->options & SRCML_OPTION_COMPRESS);
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_memory(srcml_unit* unit, char** src_buffer) {

  xmlBufferPtr buffer = xmlBufferCreate();
  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  (*src_buffer) = strdup((const char *)buffer->content);
  xmlBufferFree(buffer);

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_FILE(srcml_unit* unit, FILE* srcml_file) {

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  return SRCML_STATUS_OK;

}

int srcml_unparse_unit_fd(srcml_unit* unit, int srcml_fd) {

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(unit->archive->encoding ? unit->archive->encoding->c_str() : "ISO-8859-1"));
  srcMLUtility utility(unit->unit->c_str(), unit->unit->size(), 0, unit->archive->options);
  utility.extract_text(output_buffer);

  return SRCML_STATUS_OK;

}

