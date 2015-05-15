/**
 * @file src_input_text.cpp
 *
 * @copyright Copyright (C) 2015 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <src_input_text.hpp>
#include <srcml_options.hpp>
#include <src_input_libarchive.hpp>
#include <src_prefix.hpp>

// Convert input to a ParseRequest and assign request to the processing queue
 void src_input_text(ParseQueue& queue,
 	srcml_archive* srcml_arch,
 	const srcml_request_t& srcml_request,
 	const std::string& input_file) {

    // form the parsing request
 	ParseRequest* prequest = new ParseRequest;

 	if (srcml_request.command & SRCML_COMMAND_NOARCHIVE)
 		prequest->disk_dir = srcml_request.output_filename;

    prequest->filename = srcml_request.att_filename;
 	prequest->url = srcml_request.att_url;
 	prequest->version = srcml_request.att_version;
 	prequest->srcml_arch = srcml_arch;
 	prequest->language = srcml_request.att_language ? *srcml_request.att_language : "";

    prequest->status = 0; //!language.empty() ? 0 : SRCML_STATUS_UNSET_LANGUAGE;

    std::string raw_text = src_prefix_resource(input_file);

    // fill up the parse request buffer
    if (!prequest->status) {
    	// copy from the text directly into a buffer
    	// perform newline and tab expansion
    	// TODO: Do this more efficiently
    	// TODO: Make test cases for each part
    	// TODO: Support \nnn, \xnnn, \unnn, \Unnnnnnnn
    	bool startescape = false;
    	for (std::string::const_iterator p = raw_text.begin(); p != raw_text.end(); ++p) {

    		if (!startescape && *p == '\\') {

    			startescape = true;

    		} else if (startescape) {

    			if (*p == 'n') {
    				prequest->buffer.push_back('\n');
    			} else if (*p == 't') {
    				prequest->buffer.push_back('\t');
    			} else if (*p == 'f') {
    				prequest->buffer.push_back('\f');
    			} else if (*p == 'a') {
    				prequest->buffer.push_back('\a');
    			} else if (*p == 'b') {
    				prequest->buffer.push_back('\b');
    			} else if (*p == 'e') {
    				prequest->buffer.push_back('\e');
    			} else if (*p == 'r') {
    				prequest->buffer.push_back('\r');
    			} else if (*p == 'v') {
    				prequest->buffer.push_back('\v');
    			} else {
    				prequest->buffer.push_back('\\');
    				prequest->buffer.push_back(*p);
    			}

   				startescape = false;
    		} else {

    			prequest->buffer.push_back(*p);
    		}
    	}
    	++prequest->loc;
    }

    // schedule for parsing
    queue.schedule(prequest);

}
