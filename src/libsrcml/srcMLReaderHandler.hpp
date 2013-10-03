#ifndef INCLUDED_SRCML_READER_HANDLER_HPP
#define INCLUDED_SRCML_READER_HANDLER_HPP

#include <srcMLHandler.hpp>

#include <string>
#include <vector>

#include <libxml/parser.h>
#include <stdio.h>
#include <pthread.h>
#include <Options.hpp>

class srcMLReaderHandler : public srcMLHandler {

private :

  // threading
  pthread_mutex_t mutex;
  pthread_mutex_t is_done_mutex;
  pthread_cond_t cond;
  pthread_cond_t is_done_cond;

  // collecting variables
  std::string root_language;
  std::string root_filename;
  std::string root_directory;
  std::string root_version;
  std::vector<std::string> attributes;
  std::vector<std::string> prefixes;
  std::vector<std::string> namespaces;
  OPTION_TYPE options;
  int tabstop;


public :

  srcMLReaderHandler() {

    pthread_mutex_init(&mutex, 0);
    pthread_mutex_init(&is_done_mutex, 0);
    pthread_cond_init(&cond, 0);
    pthread_cond_init(&is_done_cond, 0);

  }

  ~srcMLReaderHandler() {

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&is_done_mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&is_done_cond);

  }

  void wait() {

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&is_done_cond, &mutex);
    pthread_mutex_unlock(&mutex);

  }

  void resume() {

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

  }

  virtual void startDocument() {}

  virtual void endDocument() {}

  virtual void startRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                         int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar ** attributes) {

    // collect attributes
    for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

      std::string attribute = (const char *)attributes[pos];
      std::string value = "";
      value.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);

      if(attribute == "language")
        root_language = value;
      else if(attribute == "filename")
        root_filename = value;
      else if(attribute == "directory")
        root_directory = attribute;
      else if(attribute == "version")
        root_version = value;
      else {
        this->attributes.push_back(attribute);
        this->attributes.push_back(value);
      }

    }

    // collect namespaces
    for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

      std::string attribute = (const char *)namespaces[pos];
      std::string value = "";
      value.append((const char *)namespaces[pos + 3], namespaces[pos + 4] - namespaces[pos + 3]);

      std::string prefix = (const char *)namespaces[pos];
      std::string ns = (const char *)namespaces[pos + 1];

      if(ns == SRCML_CPP_NS_URI) {

        if(language != "") {

          if(language == "C++" || language == "C")
            options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
          else if(language == "C#")
            options |= SRCML_OPTION_CPP_NOMACRO;
          //else
          //options |= SRCML_OPTION_CPP;
        }

      } else if(ns == SRCML_ERR_NS_URI)
        options |= SRCML_OPTION_DEBUG;
      else if(ns == SRCML_EXT_LITERAL_NS_URI)
        options |= SRCML_OPTION_LITERAL;
      else if(ns == SRCML_EXT_OPERATOR_NS_URI)
        options |= SRCML_OPTION_OPERATOR;
      else if(ns == SRCML_EXT_MODIFIER_NS_URI)
        options |= SRCML_OPTION_MODIFIER;
      else if(ns == SRCML_EXT_POSITION_NS_URI)
        options |= SRCML_OPTION_POSITION;

      int i;
      try {

        for(i = 0; i < prefixes.size(); ++i)

          if(this->namespaces.at(i) == ns) {

            prefixes.at(i) = prefix;
            break;
          }

      } catch(...) {}

      if(i == prefixes.size()) {
        prefixes.push_back(prefix);
        this->namespaces.push_back(ns);
      }


    }

    // pause
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&is_done_cond);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

  }

  virtual void startUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                         int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar ** attributes) {}

  virtual void startElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                              int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                              const xmlChar ** attributes) {}

  virtual void endRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {}

  virtual void endUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

    // pause
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&is_done_cond);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

  }

  virtual void endElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {}

  virtual void characters(const xmlChar * ch, int len) {}

};

#endif
