/**
 * @file srcml_sax2_reader.hpp
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

#ifndef INCLUDED_SRCML_SAX2_READER_HPP
#define INCLUDED_SRCML_SAX2_READER_HPP

#include <srcml_reader_handler.hpp>

#include <srcSAXController.hpp>

#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/optional.hpp>

/**
 * srcml_sax2_reader
 *
 * Extend XML Text Reader interface to
 * progressively read a srcML Archive collecting
 * units and reading unit attributes.
 */
class srcml_sax2_reader {

private :

    /** control for sax parsing */
    srcSAXController control;
    /** boolean for marking if read root */
    bool read_root;
    /** handler with hooks for sax parsing */
    srcml_reader_handler handler;

    /** thread running execution */
    boost::thread * thread;

    void init_constructor();

public :

    // constructors
    srcml_sax2_reader(const char * filename, const char * encoding = 0);
    srcml_sax2_reader(xmlParserInputBufferPtr input);


    // destructors
    ~srcml_sax2_reader();

    // read attribute and namespace information from root unit.  Does not advance read.
    int read_root_unit_attributes(boost::optional<std::string> & encoding, 
                                  boost::optional<std::string> & language,
                                  boost::optional<std::string> & url, boost::optional<std::string> & version,
                                  std::vector<std::string> & attributes,
                                  std::vector<std::string> & prefixes,
                                  std::vector<std::string> & namespaces,
                                  boost::optional<std::pair<std::string, std::string> > & processing_instruction,
                                  OPTION_TYPE & options,
                                  size_t & tabstop,
                                  std::vector<std::string> & user_macro_list);


    /* finds next unit tag if not current unit and sets attributes.  Consumes unit.
       Unit is still avaible for readsrcML or read.  But not readUnitAttributes.
    */
    int read_unit_attributes(boost::optional<std::string> & language, boost::optional<std::string> & filename,
                             boost::optional<std::string> & url, boost::optional<std::string> & version,
                             boost::optional<std::string> & timestamp, boost::optional<std::string> & hash,
                             std::vector<std::string> & attributes);

    // reads the next unit and returns it in parameter as srcML
    int read_srcml(boost::optional<std::string> & unit);

    // reads the next unit and returns it in parameter as src
    int read_src(xmlOutputBufferPtr output_buffer);

    // set the revision number to retrieve
    void revision_number(boost::optional<size_t> revision_number);


};

#endif
