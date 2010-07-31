/*
  ExtractUnitsSrc.cpp

  Copyright (C) 2010  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


*/

#ifndef INCLUDED_EXTRACTUNITSSRC_H
#define INCLUDED_EXTRACTUNITSSRC_H

#include "ProcessUnit.h"

#ifdef __GNUC__
#define EOL "\n"
#define EOL_SIZE 1
#else
#define EOL "\r\n"
#define EOL_SIZE 2
#endif

class ExtractUnitsSrc : public ProcessUnit {
 public :
 ExtractUnitsSrc(const char* to_dir, const char* output_filename, const char* output_encoding)
   : to_directory(to_dir), output_filename(output_filename) {

    handler = xmlFindCharEncodingHandler(output_encoding);
  }

 private :
    const char* to_directory;
    const char* output_filename;
    xmlCharEncodingHandlerPtr handler;

 public :

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    if (to_directory) {

      // start the path with the (optional) target directory
      path = to_directory;

      // append the directory attribute
      int dir_index = find_attribute_index(nb_attributes, attributes, UNIT_ATTRIBUTE_DIRECTORY);
      if (dir_index != -1) {
	
	if (!path.empty() && path[path.size() - 1] != '/')
	  path += '/';

	path.append((const char*) attributes[dir_index + 3], (const char*) attributes[dir_index + 4]);
      }

      // find the filename attribute
      int filename_index = find_attribute_index(nb_attributes, attributes, UNIT_ATTRIBUTE_FILENAME);
      bool foundfilename = filename_index != -1;

      // filename is required
      if (!foundfilename) {
        fprintf(stderr, "Missing filename attribute\n");
        return;
      }

      // append the filename
      if (!path.empty() && path[path.size() - 1] != '/')
        path += '/';
      path.append((const char*) attributes[filename_index + 3], (const char*) attributes[filename_index + 4]);

      // output file status message if in verbose mode
      if (isoption(*(pstate->poptions), OPTION_VERBOSE))
        fprintf(stderr, "%ld\t%s\n", pstate->count, path.c_str());

    } else if (output_filename) {

      path = output_filename;

    } else {

      path = "-";
    }

    // now create the file itself
    output_buffer = xmlOutputBufferCreateFilename(path.c_str(), handler, isoption(*(pstate->poptions), OPTION_COMPRESSED));
    if (output_buffer == NULL) {
      fprintf(stderr, "Output buffer error\n");
      xmlStopParser(ctxt);
    }
  }

  virtual void charactersUnit(void* ctx, const xmlChar* ch, int len) {

#ifdef __GNUC__
    xmlOutputBufferWrite(output_buffer, len, (const char*) ch);
#else
    const char* c = (const char*) ch;
    int pos = 0;
    const char* chend = (const char*) ch + len;
    while (c < chend) {

      switch (*c) {
      case '\n' :
	xmlOutputBufferWrite(output_buffer, pos, (const char*)(BAD_CAST c - pos));
	pos = 0;
	xmlOutputBufferWrite(output_buffer, EOL_SIZE, EOL);
	break;

      default :
	++pos;
	break;
      };
      ++c;
    }

    xmlOutputBufferWrite(output_buffer, pos, (const char*)(BAD_CAST c - pos));
#endif
  }

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // finish up this file
    xmlOutputBufferClose(output_buffer);

    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(*(pstate->poptions), OPTION_TERMINATE)) {
      xmlStopParser(ctxt);
      throw TerminateLibXMLError();
    }


  }

 private :
  std::string path;
  xmlOutputBufferPtr output_buffer;
};

#endif
