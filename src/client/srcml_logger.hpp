/**
 * @file srcml_logger.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

#ifndef SRCML_LOGGER_HPP
#define SRCML_LOGGER_HPP

#include <string>
#include <iostream>
#include <limits.h>
#include <srcml_cli.hpp>

class SRCMLLogger {
public:
    // message type priorities
    static const int CRITICAL_MSG = 1<<4;
    static const int ERROR_MSG    = 1<<3;
    static const int WARNING_MSG  = 1<<2;
    static const int INFO_MSG     = 1<<1;
    static const int DEBUG_MSG    = 1<<0;

    static void set(int options) {
      opts = options;
      error_count = 0;
    }

    static bool errors() {
      return error_count > 0;
    }

    static void log(int msg_type, const std::string& msg_text) {

      // Only print debug messages if the debug mode is enabled
      if ((msg_type == DEBUG_MSG) && !(opts & SRCML_DEBUG_MODE))
        return;

      // If we are in quiet mode only output a message that results in an exit
      if (opts & SRCML_COMMAND_QUIET && !(msg_type == CRITICAL_MSG))
        return;

      switch (msg_type) {
      case CRITICAL_MSG:
        std::cerr << "CRITICAL ";
        ++error_count;
        break;
      case ERROR_MSG:
        std::cerr << "ERROR ";
        ++error_count;
        break;
      case WARNING_MSG:
        std::cerr << "WARNING ";
        ++error_count;
        break;
      case INFO_MSG:
        std::cerr << "INFO ";
        break;
      case DEBUG_MSG:
        std::cerr << "DEBUG ";
        break;
      default:
        break;
      }

      std::cerr << msg_text << "\n";
    }

 private:
  static int opts;
  static int error_count;
};

#endif
