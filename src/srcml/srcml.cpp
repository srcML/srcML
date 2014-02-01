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
#include <srcml_cli.hpp>
#include <thread_queue.hpp>
#include <srcml_consume.hpp>
#include <src_input_libarchive.hpp>

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>

#include <stdio.h>
#include <unistd.h>
#include <iostream>

// helper functions
bool checkLocalFiles(std::vector<std::string>& pos_args);
bool test_for_stdin();
void display_long_info(std::vector<std::string>& pos_args);

int main(int argc, char * argv[]) {

  // parse the command line
  srcml_request_t srcml_request = parseCLI(argc, argv);

  // help option was selected, already displayed so no more to do
  if (srcml_request.help_set)
    return 0;

  // version
  if (srcml_request.command & SRCML_COMMAND_VERSION) {
    std::cout <<  srcml_version_string() << "\n";
    std::cout << "srcml Version Number " << srcml_version_number() << "\n";
    std::cout << "libarchive Version " << ARCHIVE_VERSION_NUMBER << "\n";
    return 0;
  }

  // check encoding
  if (srcml_request.encoding != "" && srcml_check_encoding(srcml_request.encoding.c_str()) == 0) {
    std::cerr << argv[0] << ": invalid encoding.\n";
    return 1; //ERROR CODE TBD
  }

  // check language
  if (srcml_request.language != "" && srcml_check_language(srcml_request.language.c_str()) == 0) {
    std::cerr << argv[0] << ": invalid language.\n";
    return 1; //ERROR CODE TBD
  }

  // check tabstop
  if (srcml_request.tabs <= 0) {
    std::cerr << argv[0] << ": invalid tab stop.\n";
    return 1; //ERROR CODE TBD
  }

  // Check if local files/directories are present on filesystem
  if (!checkLocalFiles(srcml_request.positional_args))
    return 1;

  if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
    display_long_info(srcml_request.positional_args);
    return 0;
  }
  
  // create the output archive
  srcml_archive* srcml_arch = srcml_create_archive();

  // set options for the output archive
  if (srcml_request.encoding != "")
    srcml_archive_set_encoding(srcml_arch, srcml_request.encoding.c_str());

  if (srcml_request.filename_set)
    srcml_archive_set_filename(srcml_arch, srcml_request.filename.c_str());

  if (srcml_request.directory_set)
    srcml_archive_set_directory(srcml_arch, srcml_request.directory.c_str());

  if (srcml_request.src_versions_set)
    srcml_archive_set_version(srcml_arch, srcml_request.src_versions.c_str());

  if (srcml_request.markup_options != 0) {
    srcml_archive_enable_option(srcml_arch, srcml_archive_get_options(srcml_arch) | srcml_request.markup_options);
  }

  if (srcml_request.language != "")
    srcml_archive_set_language(srcml_arch, srcml_request.language.c_str());  
  else
    srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);  

  srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

  // archive or not
  if (srcml_request.positional_args.size() == 1 && !(srcml_request.markup_options & SRCML_OPTION_ARCHIVE)) {
    boost::filesystem::path inFile (srcml_request.positional_args[0]);
    if(srcml_request.positional_args[0] == "-" || srcml_archive_check_extension(srcml_arch, srcml_request.positional_args[0].c_str()) || inFile.extension().string() == ".xml")
      srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
  }
  else {
    srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
  }

  // register file extensions
  for (size_t i = 0; i < srcml_request.register_ext.size(); ++i) {
    size_t pos = srcml_request.register_ext[i].find('=');
    srcml_archive_register_file_extension(srcml_arch, srcml_request.register_ext[i].substr(0,pos).c_str(),
          srcml_request.register_ext[i].substr(pos+1).c_str());
  }

  // register xml namespaces
  for (size_t i = 0; i < srcml_request.xmlns_prefix.size(); ++i) {
    size_t pos = srcml_request.xmlns_prefix[i].find('=');
    srcml_archive_register_namespace(srcml_arch, srcml_request.xmlns_prefix[i].substr(0,pos).c_str(),
           srcml_request.xmlns_prefix[i].substr(pos+1).c_str());
  }

  // create the output file
  srcml_write_open_filename(srcml_arch, srcml_request.output.c_str());

  // setup the parsequeue and consuming threads
  ParseQueue queue;
  const int NUM_THREADS = 4;
  pthread_t writer[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; ++i)
      pthread_create(&writer[i], 0, srcml_consume, &queue);

  // setup a request
  ParseRequest request;

  // process the command line inputs
  for (size_t i = 0; i < srcml_request.positional_args.size(); ++i) {
    std::string& input_file = srcml_request.positional_args[i];

    // stdin
    if (input_file.compare("-") == 0) {
      // check if we are using the terminal interactively
      if(srcml_request.command & SRCML_COMMAND_INTERACTIVE) {
        if (!test_for_stdin())
          return 1; // stdin was requested, but no data was received
      }
    }

    // process libarchive input
    src_input_libarchive(queue, srcml_arch, request, input_file, srcml_request.language);   
  }
  
  // end the queue and the threads
  ParseRequest NullParseRequest;
  for (int i = 0; i < NUM_THREADS; ++i)
      queue.push(NullParseRequest);
  for (int i = 0; i < NUM_THREADS; ++i)
      pthread_join(writer[i], NULL);

  // close the created srcML archive
  srcml_close_archive(srcml_arch);
  srcml_free_archive(srcml_arch);

  return 0;
}

// check stdin for data
bool test_for_stdin() {

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

// check for the presence of local files only
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

void display_long_info(std::vector<std::string>& pos_args) {
  for (size_t i = 0; i < pos_args.size(); ++i) {
    boost::filesystem::path localFile (pos_args[i]);
    
    if (is_directory(localFile))
      continue;

    if (localFile.extension().string() == ".xml") {
      int numUnits = 0;
      srcml_archive* srcml_arch = srcml_create_archive();
      srcml_read_open_filename(srcml_arch, pos_args[i].c_str());
     
      while (true) {
        srcml_unit* unit = srcml_read_unit(srcml_arch);

        if (unit == 0)
          break;

        ++numUnits;
        std::string xml = srcml_unit_get_xml(unit);
        std::string unitHead = xml.substr(1,xml.find(">")-1);
        unitHead = unitHead.substr(unitHead.find(" ")+1);

        while (true && numUnits <= 1) {
          size_t pos = unitHead.find(" ");
          if (pos == std::string::npos)
            break;

          std::cout << unitHead.substr(0, pos) << "\n";
          unitHead = unitHead.substr(pos+1);
        }
      }
      std::cout << "units=\"" << numUnits << "\"\n";
    }
  }
}