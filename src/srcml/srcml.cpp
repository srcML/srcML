/*
  srcml.cpp

  Copyright (C) 2014  SDML (www.srcML.org)

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
#include <parse_queue.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_filesystem.hpp>
#include <srcml_display_info.hpp>
#include <srcml_list_unit_files.hpp>
#include <src_prefix.hpp>

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <stdio.h>
#include <unistd.h>
#include <iostream>

// helper functions
bool checkLocalFiles(std::vector<std::string>& pos_args);
bool test_for_stdin();

// code testing (temporary)
void libarchive2srcml(std::string filename);
void file2srcml(std::string filename);
void direct2srcml(std::string filename);

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
    std::cerr << argv[0] << ": " << srcml_request.tabs << " is invalid tab stop. Tab stops must be 1 or higher.\n";
    return 1; //ERROR CODE TBD
  }

  // Check if local files/directories are present on filesystem
  if (!checkLocalFiles(srcml_request.positional_args))
    return 1;

  // srcml long info
  if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
    srcml_display_info(srcml_request.positional_args);
    return 0;
  }

  // srcml info
  if (srcml_request.command & SRCML_COMMAND_INFO) {
    srcml_display_info(srcml_request.positional_args);
    return 0; 
  }

  // list filenames in srcml archive
  if (srcml_request.command & SRCML_COMMAND_LIST) {
    srcml_list_unit_files(srcml_request.positional_args);
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
    if(srcml_request.positional_args[0] == "-" || srcml_archive_check_extension(srcml_arch, srcml_request.positional_args[0].c_str()) || inFile.extension().compare(".xml") == 0)
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

  // create the srcML output file
  srcml_write_open_filename(srcml_arch, srcml_request.output.c_str());

  // setup the parsing queue
  ParseQueue queue(srcml_request.max_threads);

  // process command line inputs
  BOOST_FOREACH(const std::string& input_file, srcml_request.positional_args) {

// code testing (temporary)
//   direct2srcml(input_file);
//   file2srcml(input_file);
//   libarchive2srcml(input_file);
//      continue;

    // if stdin, then there has to be data
    if ((input_file == "-") && (srcml_request.command & SRCML_COMMAND_INTERACTIVE) && !test_for_stdin()) {
      return 1; // stdin was requested, but no data was received
    }

    // split the URI
    std::string protocol;
    std::string resource;
    src_prefix_split_uri(input_file, protocol, resource);
    // call handler based on prefix
    if ((protocol == "file") && is_directory(boost::filesystem::path(resource))) {
      src_input_filesystem(queue, srcml_arch, resource, srcml_request.language);
    } else if (protocol == "file") {
      src_input_libarchive(queue, srcml_arch, resource, srcml_request.language);
    } else if (protocol == "stdin") {
      src_input_libarchive(queue, srcml_arch, resource, srcml_request.language);
    }
  }
  
  // wait for the parsing queue to finish
  queue.wait();

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
    
    if (pos_args[i] == "-") {
      pos_args[i] = pos_args[i].insert(0,"stdin://");
      return true;
    }
    
    if (pos_args[i].find("http:") == std::string::npos){
      boost::filesystem::path localFile (pos_args[i]);
      if (!exists(localFile)) {
        std::cerr << "File " << pos_args[i] << " not found.\n";
        return false;
      }
      pos_args[i] = pos_args[i].insert(0,"file://");
    }
  }
  return true;
}

// code testing (temporary)

#include <fcntl.h>
#include <archive.h>
#include <archive_entry.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/thread.hpp>

void read_from_pipe (int file) {

    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_fd(arch, file);
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
     
void libarchive2srcml(std::string filename) {

    archive* arch = archive_read_new();
    archive_entry* arch_entry = archive_entry_new();
    archive_read_support_format_raw(arch);
    archive_read_support_compression_all(arch);


    filename = filename.substr(7);

    archive_read_open_filename(arch, filename.c_str(), 16384);
    archive_read_next_header(arch, &arch_entry);

    int datapipe[2];
    pipe(datapipe);

    boost::thread_group reader;
    reader.create_thread( boost::bind(read_from_pipe, datapipe[0]) );

    archive_read_data_into_fd(arch, datapipe[1]);
    
    close(datapipe[1]);

    archive_read_finish(arch);

    reader.join_all();
}

void file2srcml(std::string filename) {

    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_filename(arch, filename.substr(7).c_str());
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

void direct2srcml(std::string filename) {

    // Parse srcml back to source (srcml2src)
    srcml(filename.substr(7).c_str(), "foobar.cpp");
}
