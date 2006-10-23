/*
  xmloutput.h

  Copyright (C) 2006  SDML (www.sdml.info)

  The extractor is free software; you can redistribute it and/or modify
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

  An identity output function for libxml2 TextReader and TextWriter.
*/

#ifndef _INCLUDED_XMLRW_H
#define _INCLUDED_XMLRW_H

#ifdef LIBXML_ENABLED
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#else
#include "xmlreader.h"
#include "xmlwriter.h"
#endif

// output the current node in reader to writer in XML
void outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer);

#endif
