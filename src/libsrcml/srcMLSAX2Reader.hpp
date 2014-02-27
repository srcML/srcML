/**
 * @file srcMLSAX2Reader.hpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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

#ifndef INCLUDED_SRCML_SAX2_READER_HPP
#define INCLUDED_SRCML_SAX2_READER_HPP

#include <srcMLReaderHandler.hpp>

#include <srcMLControlHandler.hpp>

#include <UTF8Source.hpp>

#include <Options.hpp>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/optional.hpp>

/**
 * srcMLSAX2Reader
 *
 * Extend XML Text Reader interface to
 * progressively read a srcML Archive collecting
 * units and reading unit attributes.
 */
class srcMLSAX2Reader {

private :

    /** control for sax parsing */
    srcMLControlHandler control;
    /** boolean for marking if read root */
    bool read_root;
    /** handler with hooks for sax parsing */
    srcMLReaderHandler handler;

    /** thread running execution */
    boost::thread * thread;

public :

    // constructors
    srcMLSAX2Reader(const char * filename);
    srcMLSAX2Reader(xmlParserInputBufferPtr input);

    // destructors
    ~srcMLSAX2Reader();

    // read attribute and namespace information from root unit.  Does not advance read.
    int readRootUnitAttributes(boost::optional<std::string> & language, boost::optional<std::string> & filename,
                               boost::optional<std::string> & directory, boost::optional<std::string> & version,
                               std::vector<std::string> & attributes,
                               std::vector<std::string> & prefixes,
                               std::vector<std::string> & namespaces,
                               OPTION_TYPE & options,
                               int & tabstop,
                               std::vector<std::string> & user_macro_list);


    /* finds next unit tag if not current unit and sets attributes.  Consumes unit.
       Unit is still avaible for readsrcML or read.  But not readUnitAttributes.
    */
    int readUnitAttributes(boost::optional<std::string> & language, boost::optional<std::string> & filename,
                           boost::optional<std::string> & directory, boost::optional<std::string> & version);

    // reads the next unit and returns it in parameter as srcML
    int readsrcML(boost::optional<std::string> & unit);

    // reads the next unit and returns it in parameter as src
    int readsrc(UTF8Source & output_handler);

};

#endif
