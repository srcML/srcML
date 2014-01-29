/*
  srcml.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

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
  The srcml program to transform to/from the srcML format, plus provides a variety of
  querying and transformation features.

  Replaces the src2srcml and srcml2src of the original srcML toolkit.
*/
#include <srcml.h>
#include <srcmlCLI.hpp>
#include <thread_queue.hpp>
#include <src_input_libarchive.hpp>

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

bool test_for_stdin() {
  fd_set fds;

  /* 
   Need a timeout so the application doesn't
    hang waiting for input that never comes
  */
  struct timeval timeout;

  // Init file descriptor with stdin
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  // Set timeout
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  // Use select to see if stdin has data
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

bool checkLocalFiles(std::vector<std::string>& pos_args) {
  for (size_t i = 0; i < pos_args.size(); ++i) {
    if (pos_args[i] == "/dev/stdin")
      pos_args[i] = "-";
    
    if (pos_args[i] != "-") {
      if (pos_args[i].find("http:") == std::string::npos){
        boost::filesystem::path localFile (pos_args[i]);
        if (!exists(localFile)) {
          std::cerr << "File " << pos_args[i] << " not found.\n";
          return false;
        }
      }
    }
  }
  return true;
}

boost::mutex mtx;

// Consumption thread function
void * srcml_consume(void * arg) {
  ThreadQueue<ParseRequest, 10> * queue = (ThreadQueue<ParseRequest, 10> *) arg;
  
  while (true) {
    ParseRequest pr;
    queue->pop(pr);
    
    // Check if termination queue item has been found  
    if (pr.empty())
      break;

    if (pr.lang.compare("xml") != 0) {
      // Build, parse, and write srcml unit
      srcml_unit * unit = srcml_create_unit(pr.srcml_arch);
      srcml_unit_set_filename(unit, pr.filename.c_str());
      srcml_unit_set_language(unit, pr.lang.c_str());
      srcml_parse_unit_memory(unit, &pr.buffer[0], pr.buffer.size());
      mtx.lock();
      srcml_write_unit(pr.srcml_arch, unit);
      mtx.unlock();
    }
    else {
      // Stuff to read srcml back to src
      srcml_archive* arch = srcml_create_archive();
      srcml_read_open_filename(arch, pr.filename.c_str());
      srcml_unit* unit;
      while (true) {
        unit = srcml_read_unit(arch);
        if (unit == 0)
          break;
        srcml_unparse_unit_filename(unit, srcml_unit_get_filename(unit));
        srcml_free_unit(unit);
      }

      srcml_close_archive(arch);
      srcml_free_archive(arch);
    }
  }

  return 0;
}

int main(int argc, char * argv[]) {
  srcml_request_t srcml_request = srcmlCLI::parseCLI(argc, argv);

  // Help function was used. srcmlCLI displays, so srcml has no work to do.
  if (srcml_request.help_set)
    return 0;
  
  if (srcml_request.command & SRCML_COMMAND_VERSION) {
    std::cerr <<  "srcML Version Number " << srcml_version_number() << "\n";
    std::cerr <<  "srcML Version String " << srcml_version_string() << "\n";
    return 0;
  }

  // Ensure all global flags are valid
  if (srcml_request.encoding != "" && srcml_check_encoding(srcml_request.encoding.c_str()) == 0) {
    std::cerr << "Invalid Encoding.\n";
    return 1; //ERROR CODE TBD
  }

  if (srcml_request.language != "" && srcml_check_language(srcml_request.language.c_str()) == 0) {
    std::cerr << "Invalid Language.\n";
    return 1; //ERROR CODE TBD
  }

  if (srcml_request.tabs <= 0) {
    std::cerr << "Invalid Tab Stop.\n";
    return 1; //ERROR CODE TBD
  }

  // Check if local files/directories are present on filesystem
  if (!checkLocalFiles(srcml_request.positional_args))
    return 1;
  
  // libsrcML Full API Setup
  srcml_archive * srcml_arch = srcml_create_archive();

  // Set options for the archive
  if (srcml_request.encoding != "")
    srcml_archive_set_encoding(srcml_arch, srcml_request.encoding.c_str());

  if (srcml_request.filename != "")
    srcml_archive_set_filename(srcml_arch, srcml_request.filename.c_str());

  if (srcml_request.directory_set)
    srcml_archive_set_directory(srcml_arch, srcml_request.directory.c_str());

  if (srcml_request.src_versions != "")
    srcml_archive_set_version(srcml_arch, srcml_request.src_versions.c_str());

  if (srcml_request.markup_options != 0)
    srcml_archive_set_options(srcml_arch, srcml_request.markup_options);

  if (srcml_request.language != "")
    srcml_archive_set_language(srcml_arch, srcml_request.language.c_str());  
  else
    srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);  

  srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

  if (srcml_request.positional_args.size() == 1 && !(srcml_request.markup_options & SRCML_OPTION_ARCHIVE)) {
    boost::filesystem::path inFile (srcml_request.positional_args[0]);
    if(srcml_request.positional_args[0] == "-" || srcml_archive_check_extension(srcml_arch, srcml_request.positional_args[0].c_str()) || inFile.extension().string() == ".xml")
      //std::cerr << "ARCHIVE OFF\n";
      srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
  }
  else {
    //std::cerr << "ARCHIVE ON\n";
    srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
  }

  for (size_t i = 0; i < srcml_request.register_ext.size(); ++i) {
    size_t pos = srcml_request.register_ext[i].find('=');
    srcml_archive_register_file_extension(srcml_arch, srcml_request.register_ext[i].substr(0,pos).c_str(),
          srcml_request.register_ext[i].substr(pos+1).c_str());
  }

  for (size_t i = 0; i < srcml_request.xmlns_prefix.size(); ++i) {
    size_t pos = srcml_request.xmlns_prefix[i].find('=');
    srcml_archive_register_namespace(srcml_arch, srcml_request.xmlns_prefix[i].substr(0,pos).c_str(),
           srcml_request.xmlns_prefix[i].substr(pos+1).c_str());
  }

  srcml_write_open_filename(srcml_arch, srcml_request.output.c_str());

  ThreadQueue<ParseRequest, 10> queue;

  const int NUM_THREADS = 4;
  pthread_t writer[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; ++i)
      pthread_create(&writer[i], 0, srcml_consume, &queue);

  // Setup a request
  ParseRequest request;

  // Mark the end of input for the threaded queue
  ParseRequest NullParseRequest;

  // Main processing loop
  for (size_t i = 0; i < srcml_request.positional_args.size(); ++i) {
    std::string& input_file = srcml_request.positional_args[i];

    // Stdin
    if (input_file.compare("-") == 0) {
      // Check if we are using the terminal interactively
      if(srcml_request.command & SRCML_COMMAND_INTERACTIVE) {
        if (!test_for_stdin())
          return 1; // Stdin was requested, but no data was received
      }
    }
    src_input_libarchive::process(queue, srcml_arch, request, input_file, srcml_request.language);   
  }
  
  // Mark end of input
  for (int i = 0; i < NUM_THREADS; ++i)
      queue.push(NullParseRequest);
  for (int i = 0; i < NUM_THREADS; ++i)
      pthread_join(writer[i], NULL);

  srcml_close_archive(srcml_arch);
  srcml_free_archive(srcml_arch);

  return 0;
}
