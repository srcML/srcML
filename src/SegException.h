/*
  segException.h

  Copyright (C) 2004, 2005 SDML (www.sdml.info)

  This include file is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This include file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this include file; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Changes segmentation errors to an thrown exception.
*/

#ifndef SEGEXCEPTION_H
#define SEGEXCEPTION_H

#include "project.h"

#include "srcMLException.h"

extern "C" void handle_segmentation(int);

// empty namespace to keep class and variable private
namespace {

  // class to handle at the start of the exception
  class StartException {
  public:
    StartException() {

            pstd::signal(SIGSEGV, handle_segmentation);
            pstd::signal(SIGABRT, handle_segmentation);
    }
  };

  // object for starting exception
  StartException st;
}

extern "C" void handle_segmentation(int) {
  pstd::signal(SIGSEGV, SIG_IGN);
  pstd::signal(SIGABRT, SIG_IGN);

  throw Segmentation_Fault();
}

#endif
