/*
  src_input_stdin.cpp

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
  src_input_stdin helper functions for dealing with stdin as an input source
*/

#include <src_input_stdin.hpp>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

// check stdin for data
bool src_input_stdin() {

    // init file descriptor with stdin
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    /*
      Need a timeout so the application doesn't
      hang waiting for input that never comes
    */
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // use select to see if stdin has data
    int selectRetVal = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);

    if (selectRetVal == -1) {
        std::cerr << "SELECT FAILED!\n";
        return false;
    }
    if (selectRetVal == 0) {
        std::cerr << "NO DATA TO FETCH!\n";
        return false;
    }
    return true;
}
