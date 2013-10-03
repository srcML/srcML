#ifndef INCLUDED_SRCMLHANDLERTHREAD_HPP
#define INCLUDED_SRCMLHANDLERTHREAD_HPP

#include <srcMLHandler.hpp>

#include <libxml/parser.h>
#include <stdio.h>
#include <pthread.h>

class srcMLReaderHandler : public srcMLHandler {

private :

  pthread_mutex_t mutex;
  pthread_mutex_t is_done_mutex;
  pthread_cond_t cond;
  pthread_cond_t is_done_cond;

  bool is_done;

public :

  srcMLReaderHandler() : is_done(false) {

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
