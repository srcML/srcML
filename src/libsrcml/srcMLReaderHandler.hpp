#ifndef INCLUDED_SRCML_READER_HANDLER_HPP
#define INCLUDED_SRCML_READER_HANDLER_HPP

#include <srcMLHandler.hpp>

#include <string>
#include <vector>

#include <libxml/parser.h>
#include <stdio.h>
#include <pthread.h>
#include <Options.hpp>
#include <srcmlns.hpp>
#include <srcml.h>

/**
 * srcMLReaderHandler
 *
 * Inherits from srcMLHandler to provide hooks into
 * SAX2 parsing. Provides starting and stoping using
 * threads.  Collects attributes, namespaces and srcML
 * from units.
 */
class srcMLReaderHandler : public srcMLHandler {

private :

  /** mutex to halt both threads on */
  pthread_mutex_t mutex;
  /** sax stop/start condition */
  pthread_cond_t cond;
  /** calling thread stop/start condition */
  pthread_cond_t is_done_cond;

  /** collected root language */
  std::string root_language;
  /** collected root filename */
  std::string root_filename;
  /** collected root directory */
  std::string root_directory;
  /** collected root version */
  std::string root_version;
  /** collected root attributes */
  std::vector<std::string> attributes;
  /** collected root prefixes */
  std::vector<std::string> prefixes;
  /** collected root namespaces */
  std::vector<std::string> namespaces;
  /** collected root options */
  OPTION_TYPE options; 
  /** collected root tabstop */
  int tabstop;

  /** collected unit language */
  std::string unit_language;
  /** collected unit filename */
  std::string unit_filename;
  /** collected unit directory */
  std::string unit_directory;
  /** collected unit version */
  std::string unit_version;

  /** collect srcML */
  std::string unit;

  /** has reached end of parsing*/
  bool is_done;
  /** stop after collecting unit attribute*/
  bool collect_unit_attributes;
  /** collect srcML as parse*/
  bool collect_srcml;

public :

  //** Give access to membeers for srcMLSAX2Reader class */
  friend class srcMLSAX2Reader;

  /**
   * srcMLReaderHandler
   *
   * Constructor.  Sets up mutex, conditions and state.
   */
  srcMLReaderHandler() : is_done(false), collect_unit_attributes(false), collect_srcml(false) {

    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&cond, 0);
    pthread_cond_init(&is_done_cond, 0);

  }

  /**
   * ~srcMLReaderHandler
   *
   * Destructor, deletes mutex and conditions.
   */
  ~srcMLReaderHandler() {

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&is_done_cond);

  }

  /**
   * wait
   *
   * Allows calling thread to wait until reached
   * end of unit.
   */
  void wait() {

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&is_done_cond, &mutex);
    pthread_mutex_unlock(&mutex);

  }

  /**
   * resume
   *
   * Resume SAX2 execution.
   */
  void resume() {

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

  }

  /**
   * startRoot
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   * @param nb_namespaces number of xml namespaces
   * @param namespaces the prefix/namespaces pairs
   * @param nb_attributes number of attributes
   * @param nb_defaulted number defaulted attributes
   * @param attributes the attributes (name/prefix/uri/value start/value end/)
   *
   * Overidden startRoot to handle collection of root attributes. Stop before continue
   */
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
      else if(attribute == "dir")
        root_directory = value;
      else if(attribute == "version")
        root_version = value;
      else if(attribute == "tabs")
        tabstop = atoi(value.c_str());
      else {
        this->attributes.push_back(attribute);
        this->attributes.push_back(value);
      }

    }

    // collect namespaces
    for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

      std::string prefix = namespaces[pos] ? (const char *)namespaces[pos] : "";
      std::string ns = namespaces[pos + 1] ? (const char *)namespaces[pos + 1] : "";

      if(ns == SRCML_CPP_NS_URI) {

        if(root_language != "") {

          if(root_language == "C++" || root_language == "C")
            options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
          else if(root_language == "C#")
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

      int index;
      try {

        for(index = 0; index < prefixes.size(); ++index)

          if(this->namespaces.at(index) == ns) {

            prefixes.at(index) = prefix;
            break;
          }

      } catch(...) {}

      if(index == prefixes.size()) {
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

  /**
   * startUnit
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   * @param nb_namespaces number of xml namespaces
   * @param namespaces the prefix/namespaces pairs
   * @param nb_attributes number of attributes
   * @param nb_defaulted number defaulted attributes
   * @param attributes the attributes (name/prefix/uri/value start/value end/)
   *
   * Overidden startUnit to handle collection of Unit attributes and tag. Stop before continue
   * if collecting attributes.
   */
  virtual void startUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                         int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar ** attributes) {

    // collect attributes
    for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

      std::string attribute = (const char *)attributes[pos];
      std::string value = "";
      value.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);

      if(attribute == "language")
        unit_language = value;
      else if(attribute == "filename")
        unit_filename = value;
      else if(attribute == "directory")
        unit_directory = attribute;
      else if(attribute == "version")
        unit_version = value;

    }

    if(collect_unit_attributes) {

      // pause
      pthread_mutex_lock(&mutex);
      pthread_cond_broadcast(&is_done_cond);
      pthread_cond_wait(&cond, &mutex);
      pthread_mutex_unlock(&mutex);

    }

    if(collect_srcml) {

      write_startTag(localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted,
                     attributes);

    }

  }

  /**
   * startElementNs
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   * @param nb_namespaces number of xml namespaces
   * @param namespaces the prefix/namespaces pairs
   * @param nb_attributes number of attributes
   * @param nb_defaulted number defaulted attributes
   * @param attributes the attributes (name/prefix/uri/value start/value end/)
   *
   * Overidden startElementNs to handle collection of srcML elements.
   */
  virtual void startElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                              int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                              const xmlChar ** attributes) {

    if(collect_srcml) {

      write_startTag(localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted,
                     attributes);

    }

  }

  /**
   * endRoot
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   *
   * Overidden endRoot to indicate done with parsing and free any waiting process.
   */
  virtual void endRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

    is_done = true;
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&is_done_cond);
    pthread_mutex_unlock(&mutex);

  }

  /**
   * endUnit
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   *
   * Overidden endUnit to collect srcml and stop parsing.  Clear collect srcML after pause.
   */
  virtual void endUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

    if(collect_srcml) {

      write_endTag(localname, prefix, URI);

      // pause
      pthread_mutex_lock(&mutex);
      pthread_cond_broadcast(&is_done_cond);
      pthread_cond_wait(&cond, &mutex);
      pthread_mutex_unlock(&mutex);

    }

    unit.clear();

  }

  /**
   * endElementNs
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   *
   * Overidden endElementNs to collect srcML.
   */
  virtual void endElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

    if(collect_srcml) {

      write_endTag(localname, prefix, URI);
    }

  }

  virtual void charactersUnit(const xmlChar * ch, int len) {

    unit.append((const char *)ch, len);

  }

private :

  void write_startTag(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                      int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                      const xmlChar ** attributes) {
    unit += "<";
    if(prefix) {
      unit += (const char *)prefix;
      unit += ":";
    }
    unit += (const char *)localname;

    for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

      unit += " xmlns";
      if(namespaces[pos]) {

        unit += ":";
        unit += (const char *)namespaces[pos];

      }

      unit += "=\"";
      unit += (const char *)namespaces[pos + 1];
      unit += "\"";

    }


    for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

      unit += " ";
      if(attributes[pos + 1]) {

        unit += (const char *)attributes[pos + 1];
        unit += ":";

      }
      unit += (const char *)attributes[pos];

      unit += "=\"";
      unit.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);
      unit += "\"";


    }
    unit += ">";

  }

  void write_endTag(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

    unit += "</";
    if(prefix) {

      unit += (const char *)prefix;
      unit += ":";

    }
    unit += (const char *)localname;

    unit += ">";

  }

};



#endif
