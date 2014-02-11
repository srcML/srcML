/*
  write_request.hpp

  Copyright (C) 2004-2014  SDML (www.srcML.org)

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

/*
  writeRequest contains a collection of information and raw data to be
    queued up for libsrcml processing
*/

#ifndef WRITE_REQUEST_HPP
#define WRITE_REQUEST_HPP

#include <srcml.h>

struct WriteRequest {
    WriteRequest() : srcml_arch(0), unit(0) {}

    void swap(WriteRequest& other) {

        srcml_archive* utempa = srcml_arch;
        srcml_arch = other.srcml_arch;
        other.srcml_arch = utempa;

        srcml_unit* utemp = unit;
        unit = other.unit;
        other.unit = utemp;
    }

    // empty WriteRequests indicate termination
    bool empty() const {
        return srcml_arch == 0 && unit == 0;
    }

    // Fields required by thread to process a unit
    srcml_archive* srcml_arch;
    srcml_unit* unit;
};

#endif
