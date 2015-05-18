/**
 * @file srcml_sax2_reader.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

#include <srcml_sax2_reader.hpp>

#include <srcmlns.hpp>
#include <srcml.h>

#include <stdlib.h>
#include <cstring>

#ifdef _MSC_BUILD
#define strdup _strdup
#endif

/**
 * thread_args
 *
 * Structure to hold information to pass
 * to thread function.
 */
struct thread_args {

    /** control for sax processing */
    srcSAXController * control;

    /** handler with hooks for sax processing */
    srcml_reader_handler * handler;

};

/**
 * start_routine
 * @param arguments thread_args structure with control and handler
 *
 * Starts the parsing of the document.
 */
void * start_routine(void * arguments) {

    thread_args * args = (thread_args *)arguments;

    try {
        args->control->parse(args->handler);
    } catch(SAXError error) {

        if(!(error.error_code == XML_ERR_EXTRA_CONTENT || error.error_code == XML_ERR_DOCUMENT_END)) {
            fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());
            args->handler->stop();
        }

        // might have to release a lock here or set is_done;
    }

    return 0;

}

void srcml_sax2_reader::init_constructor() {


    thread_args args = { &control, &handler };

    thread = new boost::thread(start_routine, &args);
    handler.wait();

}

/**
 * srcml_sax2_reader
 * @param filename name of a file
 * @param encoding the character encoding
 *
 * Construct a srcml_sax2_reader using a filename
 */
srcml_sax2_reader::srcml_sax2_reader(const char * filename, const char * encoding)
    : control(filename, encoding), read_root(false) {

    init_constructor();

}

/**
 * srcml_sax2_reader
 * @param input parser input buffer
 *
 * Construct a srcml_sax2_reader using a parser input buffer
 */
srcml_sax2_reader::srcml_sax2_reader(xmlParserInputBufferPtr input)
    : control(input), read_root(false) {

    init_constructor();

}

/**
 * ~srcml_sax2_reader
 *
 * Destructor a srcml_sax2_reader
 */
srcml_sax2_reader::~srcml_sax2_reader() {

    handler.stop();
    thread->join();
    delete thread;

}

/**
 * read_root_unit_attributes
 * @param language a location to store the language attribute
 * @param url a location to store the url attribute
 * @param version a location to store the version attribute
 * @param attributes array to store other attributes gathered
 * @param prefixes an array to store gathered XML namespace prefixes
 * @param namespaces an array to store gathered XML naamespaces
 * @param processing_instruction a location to store the pre-root processing-instruction
 * @param options a variable to set used options
 * @param tabstop a variable to set the tabstop
 * @param user_macro_list a variable to set the list of user defined macros
 *
 * Read attributes and namespace information fromt the root unit,
 * setting the necessary options.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcml_sax2_reader::read_root_unit_attributes(boost::optional<std::string> & encoding,
                                                 boost::optional<std::string> & language,
                                                 boost::optional<std::string> & url, boost::optional<std::string> & version,
                                                 std::vector<std::string> & attributes,
                                                 std::vector<std::string> & prefixes,
                                                 std::vector<std::string> & namespaces,
                                                 boost::optional<std::pair<std::string, std::string> > & processing_instruction,
                                                 OPTION_TYPE & options,
                                                 size_t & tabstop,
                                                 std::vector<std::string> & user_macro_list) {

    if(read_root || handler.read_root) return 0;

    encoding.swap(handler.archive->encoding);
    language.swap(handler.archive->language);
    url.swap(handler.archive->url);
    version.swap(handler.archive->version);
    attributes.swap(handler.archive->attributes);
    prefixes.swap(handler.archive->prefixes);
    namespaces.swap(handler.archive->namespaces);
    processing_instruction.swap(handler.archive->processing_instruction);
    options = handler.archive->options;
    tabstop = handler.archive->tabstop;
    user_macro_list.swap(handler.archive->user_macro_list);

    read_root = true;

    return 1;
}

/**
 * read_unit_attributes
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param url a location to store the url attribute
 * @param version a location to store the version attribute
 *
 * Read attributes from next unit.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcml_sax2_reader::read_unit_attributes(boost::optional<std::string> & language, boost::optional<std::string> & filename,
                                            boost::optional<std::string> & url, boost::optional<std::string> & version,
                                            boost::optional<std::string> & timestamp, boost::optional<std::string> & hash,
                                            std::vector<std::string> & attributes) {

    if(handler.is_done) return 0;
    handler.skip = true;
    handler.collect_unit_attributes = true;
    handler.resume_and_wait();
    handler.collect_unit_attributes = false;
    handler.skip = false;
    if(handler.is_done) return 0;

    language.swap(handler.unit->language);
    filename.swap(handler.unit->filename);
    url.swap(handler.unit->url);
    version.swap(handler.unit->version);
    hash.swap(handler.unit->hash);
    timestamp.swap(handler.unit->timestamp);
    attributes.swap(handler.unit->attributes);

    return 1;

}


/**
 * read_srcml
 * @param unit location in which to read srcML unit.
 *
 * Read the next unit from a srcML Archive
 * and return in the passed string parameter.
 *
 * @returns 1 on success and 0 if done
 */
int srcml_sax2_reader::read_srcml(boost::optional<std::string> & unit) {

    if(unit) unit = boost::optional<std::string>();

    if(handler.is_done) return 0;
    handler.collect_srcml = true;
    handler.resume_and_wait();
    handler.collect_srcml = false;
    if(handler.is_done) return 0;

    unit.swap(handler.unit->unit);

    return unit ? 1 : 0;
}

/**
 * read_src
 * @param output_buffer output buffer to write text
 *
 * Read the next unit from a srcML Archive
 * and return in the passed string parameter.
 *
 * @returns 1 on success and 0 if done
 */
int srcml_sax2_reader::read_src(xmlOutputBufferPtr output_buffer) {

    if(handler.is_done) return 0;
    control.enable_comment(false);
    control.enable_cdataBlock(false);
    handler.output_buffer = output_buffer;
    handler.collect_src = true;
    handler.resume_and_wait();
    handler.collect_src = false;
    handler.output_buffer = 0;
    control.enable_comment(true);
    control.enable_cdataBlock(true);
    if(handler.is_done) return 0;

    return 1;
}

/**
 * revision_umber
 * @param revision_number number of revision to retrieve
 *
 * Set the reader handler to process only the given revision.
 */
void srcml_sax2_reader::revision_number(boost::optional<size_t> revision_number) {

    handler.revision = revision_number;

}
