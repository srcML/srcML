/*
  URIStream.hpp

  Copyright (C) 2010-2011  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <libxml/parser.h>
#include <string>

#ifndef URISTREAM_HPP
#define URISTREAM_HPP

class URIStreamFileError {};

class URIStream {
public:
    URIStream(const char* uriname);

    char* readline();

    std::string readlines();

    ~URIStream();

private:
    xmlParserInputBufferPtr input;
    unsigned int startpos;
    unsigned int endpos;
    bool first;
    bool eof;
    bool done;
};


#endif
