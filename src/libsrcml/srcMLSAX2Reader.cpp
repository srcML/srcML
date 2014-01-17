/**
 * @file srcMLSAX2Reader.cpp
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcMLSAX2Reader.hpp>

#include <srcmlns.hpp>
#include <srcml.h>

#include <stdlib.h>
#include <string.h>

/**
 * thread_args
 *
 * Structure to hold information to pass
 * to thread function.
 */
struct thread_args {

  /** control for sax processing */
  srcMLControlHandler * control;

  /** handler with hooks for sax processing */
  srcMLReaderHandler * handler;

};

/**
 * start_routine
 * @param arguments thread_args structure with control and handler
 *
 * Starts the parsing of the document.
 */
void * start_routine(void * arguments) {

  thread_args * args = (thread_args *)arguments;

  args->control->parse(args->handler);

  return 0;

}

/**
 * srcMLSAX2Reader
 * @param filename name of a file
 *
 * Construct a srcMLSAX2Reader using a filename
 */
srcMLSAX2Reader::srcMLSAX2Reader(const char * filename) 
  : control(filename) {

  thread_args args = { &control, &handler };

  thread = new boost::thread(start_routine, &args);
  handler.wait();

}

/**
 * srcMLSAX2Reader
 * @param input parser input buffer
 *
 * Construct a srcMLSAX2Reader using a parser input buffer
 */
srcMLSAX2Reader::srcMLSAX2Reader(xmlParserInputBufferPtr input) 
  : control(input) {

  thread_args args = { &control, &handler };

  thread = new boost::thread(start_routine, &args);
  handler.wait();

}

/**
 * ~srcMLSAX2Reader
 *
 * Destructor a srcMLSAX2Reader
 */
srcMLSAX2Reader::~srcMLSAX2Reader() {

  handler.stop();
  thread->join();
  delete thread;

}

/**
 * readRootUnitAttributes
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param directory a location to store the directory attribute
 * @param version a location to store the version attribute
 * @param attributes array to store other attributes gathered
 * @param prefixes an array to store gathered XML namespace prefixes
 * @param namespaces an array to store gathered XML naamespaces
 * @param options a variable to set used options
 * @param tabstop a variable to set the tabstop
 *
 * Read attributes and namespace information fromt the root unit,
 * setting the necessary options.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcMLSAX2Reader::readRootUnitAttributes(std::string *& language, std::string *& filename,
                                        std::string *& directory, std::string *& version,
                                        std::vector<std::string> & attributes,
                                        std::vector<std::string> & prefixes,
                                        std::vector<std::string> & namespaces,
                                        OPTION_TYPE & options,
                                        int & tabstop) {

  if(handler.read_root) return 0;

  if(handler.archive->language) language = new std::string(*handler.archive->language);
  if(handler.archive->filename) filename = new std::string(*handler.archive->filename);
  if(handler.archive->directory) directory = new std::string(*handler.archive->directory);
  if(handler.archive->version) version = new std::string(*handler.archive->version);
  attributes = handler.archive->attributes;
  prefixes = handler.archive->prefixes;
  namespaces = handler.archive->namespaces;
  options = handler.archive->options;
  tabstop = handler.archive->tabstop;

  return 1;
}

/**
 * readUnitAttributes
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param directory a location to store the directory attribute
 * @param version a location to store the version attribute
 *
 * Read attributes from next unit.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcMLSAX2Reader::readUnitAttributes(std::string *& language, std::string *& filename,
                                    std::string *& directory, std::string *& version) {

  if(handler.is_done) return 0;

  handler.collect_unit_attributes = true;
  handler.resume_and_wait();
  handler.collect_unit_attributes = false;

  if(handler.is_done) return 0;

  if(handler.unit->language) language = new std::string(*handler.unit->language);
  if(handler.unit->filename) filename = new std::string(*handler.unit->filename);
  if(handler.unit->directory) directory = new std::string(*handler.unit->directory);
  if(handler.unit->version) version = new std::string(*handler.unit->version);

  return 1;

}

/**
 * readsrcML
 * 
 * Read the next unit from a srcML Archive
 * and return it as a std::string. Uses
 * readsrcML(xmlTextWriterPtr writer).
 *
 * @returns string on success and finished return a 0.
 */
std::string * srcMLSAX2Reader::readsrcML() {

  if(handler.is_done) return 0;
  handler.collect_srcml = true;
  handler.resume_and_wait();
  handler.collect_srcml = false;

  if(handler.is_done) return 0;

  std::string * unit = 0;
  try {
    if(handler.unit->unit) unit = new std::string(*handler.unit->unit);
  } catch(...) {}

  return unit;

}
