#ifndef INCLUDED_SRCML_READER_HANDLER_HPP
#define INCLUDED_SRCML_READER_HANDLER_HPP

#include <srcMLHandler.hpp>
#include <srcml_types.hpp>

#include <libxml/parser.h>
#include <stdio.h>
#include <pthread.h>
#include <Options.hpp>
#include <srcmlns.hpp>
#include <srcml.h>

#include <string>
#include <vector>

#include <cstring>

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
  srcml_archive * archive;

  /** collected unit language */
  srcml_unit * unit;

  /** has reached end of parsing*/
  bool is_done;
  /** has passed root*/
  bool read_root;
  /** stop after collecting unit attribute*/
  bool collect_unit_attributes;
  /** collect srcML as parse*/
  bool collect_srcml;

  /** terminate */
  bool terminate;

  /** track if empty unit */
  bool is_empty;

  /** indicate if we need to wait on the root */
  bool wait_root;

public :

  /** Give access to membeers for srcMLSAX2Reader class */
  friend class srcMLSAX2Reader;

  /**
   * srcMLReaderHandler
   *
   * Constructor.  Sets up mutex, conditions and state.
   */
  srcMLReaderHandler() : unit(0), is_done(false), read_root(false), collect_unit_attributes(false), collect_srcml(false), terminate(false), is_empty(false), wait_root(true) {

    archive = srcml_create_archive();
    archive->prefixes.clear();
    archive->namespaces.clear();


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

    if(is_done) {
      pthread_mutex_unlock(&mutex);
      return;
    }

    if(wait_root) pthread_cond_wait(&is_done_cond, &mutex);
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
   * resume_and_wait
   *
   * Atomic resume SAX2 execution then wait.
   */
  void resume_and_wait() {

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    if(is_done) {

      pthread_mutex_unlock(&mutex);
      return;
    }

    pthread_cond_wait(&is_done_cond, &mutex);
    pthread_mutex_unlock(&mutex);

  }

  /**
   * stop
   *
   * Stops SAX2 parsing Completely.  Parsing
   * Can not be restarted.
   */
  void stop() {

    resume();
    terminate = true;

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

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    // collect attributes
    for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

      std::string attribute = (const char *)attributes[pos];
      std::string value = "";
      value.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);

      if(attribute == "language")
        srcml_archive_set_language(archive, value.c_str());
      else if(attribute == "filename")
        srcml_archive_set_filename(archive, value.c_str());
      else if(attribute == "dir")
        srcml_archive_set_directory(archive, value.c_str());
      else if(attribute == "version")
        srcml_archive_set_version(archive, value.c_str());
      else if(attribute == "tabs")
        archive->tabstop = atoi(value.c_str());
      else if(name == "options") {

	if(value == "ELSEIF")
          options |= OPTION_ELSEIF;

      } else {
        archive->attributes.push_back(attribute);
        archive->attributes.push_back(value);
      }

    }

    // collect namespaces
    for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

      std::string prefix = namespaces[pos] ? (const char *)namespaces[pos] : "";
      std::string ns = namespaces[pos + 1] ? (const char *)namespaces[pos + 1] : "";

      if(ns == SRCML_CPP_NS_URI) {

        if(archive->language != 0) {

          if(*archive->language == "C++" || *archive->language == "C")
            archive->options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
          else if(*archive->language == "C#")
            archive->options |= SRCML_OPTION_CPP_NOMACRO;
          //else
          //options |= SRCML_OPTION_CPP;
        }

      } else if(ns == SRCML_ERR_NS_URI)
        archive->options |= SRCML_OPTION_DEBUG;
      else if(ns == SRCML_EXT_LITERAL_NS_URI)
        archive->options |= SRCML_OPTION_LITERAL;
      else if(ns == SRCML_EXT_OPERATOR_NS_URI)
        archive->options |= SRCML_OPTION_OPERATOR;
      else if(ns == SRCML_EXT_MODIFIER_NS_URI)
        archive->options |= SRCML_OPTION_MODIFIER;
      else if(ns == SRCML_EXT_POSITION_NS_URI)
        archive->options |= SRCML_OPTION_POSITION;

      int index;
      try {

        for(index = 0; index < archive->prefixes.size(); ++index)

          if(archive->namespaces.at(index) == ns) {

            archive->prefixes.at(index) = prefix;
            break;
          }

      } catch(...) {}

      if(index == archive->prefixes.size()) {
        archive->prefixes.push_back(prefix);
        archive->namespaces.push_back(ns);
      }


    }

    // pause
    pthread_mutex_lock(&mutex);
    wait_root = false;
    pthread_cond_broadcast(&is_done_cond);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    read_root = true;

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    unit = srcml_create_unit(archive);
    unit->unit = new std::string();

    is_empty = true;

    // collect attributes
    for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

      std::string attribute = (const char *)attributes[pos];
      std::string value = "";
      value.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);

      if(attribute == "language")
        srcml_unit_set_language(unit, value.c_str());
      else if(attribute == "filename")
        srcml_unit_set_filename(unit, value.c_str());
      else if(attribute == "dir")
        srcml_unit_set_directory(unit, value.c_str());
      else if(attribute == "version")
        srcml_unit_set_version(unit, value.c_str());

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

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if(is_empty) *unit->unit += ">";
    is_empty = true;

    if(collect_srcml) {

      write_startTag(localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted,
                     attributes);

    }

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    pthread_mutex_lock(&mutex);
    is_done = true;
    pthread_cond_broadcast(&is_done_cond);
    pthread_mutex_unlock(&mutex);

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    //if(is_empty) *unit->unit += ">";
    if(collect_srcml) {

      write_endTag(localname, prefix, URI, is_empty);

      // pause
      pthread_mutex_lock(&mutex);
      pthread_cond_broadcast(&is_done_cond);
      pthread_cond_wait(&cond, &mutex);
      pthread_mutex_unlock(&mutex);

    }

    is_empty = false;

    srcml_free_unit(unit);
    unit = 0;

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if(collect_srcml) {

      write_endTag(localname, prefix, URI, is_empty);
    }

    is_empty = false;

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

  }

  /**
   * charactersUnit
   * @param ch the characters 
   * @param len length of the characters
   *
   * Overidden charactersUnit to collect srcML.
   */
  virtual void charactersUnit(const xmlChar * ch, int len) {

#ifdef DEBUG
    std::string chars;
    chars.append((const char *)ch, len);
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if(is_empty) *unit->unit += ">";
    is_empty = false;

    for(int i = 0; i < len; ++i) {
      char character = (char)ch[i];

      if(character == '&')
        (*unit->unit) += "&amp;";
      else if(character == '<')
        (*unit->unit) += "&lt;";
      else if(character == '>')
        (*unit->unit) += "&gt;";
      else
        (*unit->unit) += character;
    }

    if(terminate) stop_parser();

#ifdef DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

  }

private :

  /**
   * write_startTag
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   * @param nb_namespaces number of xml namespaces
   * @param namespaces the prefix/namespaces pairs
   * @param nb_attributes number of attributes
   * @param nb_defaulted number defaulted attributes
   * @param attributes the attributes (name/prefix/uri/value start/value end/)
   *
   * Write out the start tag to the unit string.
   */
  void write_startTag(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                      int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                      const xmlChar ** attributes) {
    *unit->unit += "<";
    if(prefix) {
      *unit->unit += (const char *)prefix;
      *unit->unit += ":";
    }
    *unit->unit += (const char *)localname;

    for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

      if(strcmp((const char *)namespaces[pos + 1], SRCML_CPP_NS_URI) != 0)
        continue;

      *unit->unit += " xmlns";
      if(namespaces[pos]) {

        *unit->unit += ":";
        *unit->unit += (const char *)namespaces[pos];

      }

      *unit->unit += "=\"";
      *unit->unit += (const char *)namespaces[pos + 1];
      *unit->unit += "\"";

    }


    for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

      *unit->unit += " ";
      if(attributes[pos + 1]) {

        *unit->unit += (const char *)attributes[pos + 1];
        *unit->unit += ":";

      }
      *unit->unit += (const char *)attributes[pos];

      *unit->unit += "=\"";
      unit->unit->append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);
      *unit->unit += "\"";


    }
    //*unit->unit += ">";

  }

  /**
   * endElementNs
   * @param localname tag name
   * @param prefix prefix for the tag
   * @param URI uri for tag
   *
   * Write out the end tag to the unit string.
   */
  void write_endTag(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI, bool & is_empty) {

    if(is_empty) {

      *unit->unit += "/>";
      return;

    }

    *unit->unit += "</";
    if(prefix) {

      *unit->unit += (const char *)prefix;
      *unit->unit += ":";

    }
    *unit->unit += (const char *)localname;

    *unit->unit += ">";

  }


};



#endif
