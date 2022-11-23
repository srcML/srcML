// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file SRCMLStatus.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_STATUS_HPP
#define SRCML_STATUS_HPP

#include <string>
#include <iostream>
#include <limits.h>
#include <srcml_cli.hpp>
#include <srcml_options.hpp>

enum { ERROR_MSG    = 1<<3,
       WARNING_MSG  = 1<<2,
       INFO_MSG     = 1<<1,
       DEBUG_MSG    = 1<<0
};

class SRCMLStatus {
public:

    static bool errors() {
      return error_count > 0;
    }

    static void startlog(int msg_type) {

      if ((msg_type == ERROR_MSG) || (msg_type == WARNING_MSG))
        ++error_count;

      // Only print debug messages if the debug mode is enabled
      if ((msg_type == DEBUG_MSG) && !(option(SRCML_DEBUG_MODE)))
        return;

      // If we are in quiet mode only output a message that results in an exit
      if (option(SRCML_COMMAND_QUIET) && !(msg_type == ERROR_MSG))
        return;

      // @TODO Figure out why this code is not used
      // switch (msg_type) {
      // case ERROR_MSG:
      //   std::cerr << "ERROR ";
      //   break;
      // case WARNING_MSG:
      //   std::cerr << "WARNING ";
      //   break;
      // case INFO_MSG:
      //   std::cerr << "INFO ";
      //   break;
      // case DEBUG_MSG:
      //   std::cerr << "DEBUG ";
      //   break;
      // default:
      //   break;
      // }
    }

    static void log(int msg_type, std::string_view msg_text) {

      // Only print debug messages if the debug mode is enabled
      if ((msg_type == DEBUG_MSG) && !(option(SRCML_DEBUG_MODE)))
        return;

      startlog(msg_type);

      std::cerr << msg_text << "\n";
    }

 private:
  static int opts;
  static int error_count;
};

template <typename T>
inline SRCMLStatus operator<<(SRCMLStatus status, T value) {

  std::cerr << value;

  return status;
}

// convenience functions for logging
// allows for a format string with multiple types of arguments

inline SRCMLStatus SRCMLstatus(int msg_type) {

  SRCMLStatus::startlog(msg_type);

  return SRCMLStatus();
}

inline void SRCMLstatus(int msg_type, std::string_view msg_text) {

  SRCMLStatus::log(msg_type, msg_text);
}

template<typename T, typename... Args>
inline void SRCMLstatus(int msg_type, std::string_view format, T value, Args... args) {

  // replace the first argument in the format with the value
  // note: Ignoring the format type
  std::ostringstream msg_text;
  const char* s = format.data();
  while (s && *s) {
    if (*s == '%' && *++s != '%') {
      msg_text << value;
      ++s;
      break;
    }

    msg_text << *s++;
  }
  msg_text << s;

  // handle the rest of the arguments
  SRCMLstatus(msg_type, msg_text.str(), args...);
}

#endif
