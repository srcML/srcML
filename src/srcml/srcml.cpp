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

#include <archive.h>
#include <archive_entry.h>
//#include <curl/curl.h>
#include <boost/filesystem.hpp>

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct ParseRequest {
    ParseRequest() : buffer(0) {}

    void swap(ParseRequest& other) {

        filename.swap(other.filename);
        buffer.swap(other.buffer);

        //Swap Int
        size ^= other.size;
        other.size ^= size;
        size ^= other.size;
    }

    // empty ParseRequests indicate termination
    bool empty() const {
        return filename.empty() && buffer.empty();
    }

    std::string filename;
    std::vector<char> buffer;
    size_t size;
};

ParseRequest NullParseRequest;

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

void setupLibArchive(archive* a) {

  archive * arch = a;
  // Configure libarchive supported file formats
  archive_read_support_format_ar(arch);
  archive_read_support_format_cpio(arch);
  archive_read_support_format_gnutar(arch);
  archive_read_support_format_iso9660(arch);
  archive_read_support_format_mtree(arch);
  archive_read_support_format_tar(arch);
  archive_read_support_format_xar(arch);
  archive_read_support_format_zip(arch);
  archive_read_support_format_raw(arch);

  /*
    Check libarchive version
    enable version specific features/syntax
  */
  #if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(arch);
  #else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(arch);
    archive_read_support_format_cab(arch);
    archive_read_support_format_lha(arch);
    archive_read_support_format_rar(arch);

    // Compressions
    archive_read_support_filter_all(arch); 
  #endif
}

void * srcml_consume(void * arg) {
  ThreadQueue<ParseRequest, 3> * queue = (ThreadQueue<ParseRequest, 3> *) arg;
  
  return 0;
}

int main(int argc, char * argv[]) {
  srcml_request_t srcml_request = srcmlCLI::parseCLI(argc, argv);

  // Help function was used. srcmlCLI displays, so srcml has no work to do.
  if (srcml_request.help_set)
    return 0;

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

  ThreadQueue<ParseRequest, 10> queue;

  // Check if local files/directories are present on filesystem
  if (!checkLocalFiles(srcml_request.positional_args))
    return 1;
  
  // libsrcML Full API Setup
  srcml_archive * srcml_arch = srcml_create_archive();

  // Set options for the archive
  if (srcml_request.encoding != "") {
    srcml_archive_set_encoding(srcml_arch, srcml_request.encoding.c_str());
  }
  if (srcml_request.filename != "") {
    srcml_archive_set_filename(srcml_arch, srcml_request.filename.c_str());
  }
  if (srcml_request.directory_set) {
    srcml_archive_set_directory(srcml_arch, srcml_request.directory.c_str());
  }
  if (srcml_request.src_versions != "") {
    srcml_archive_set_version(srcml_arch, srcml_request.src_versions.c_str());
  }
  if (srcml_request.markup_options != 0) {
    srcml_archive_set_all_options(srcml_arch, srcml_request.markup_options);
  }

  if (srcml_request.language != "") {
    srcml_archive_set_language(srcml_arch, srcml_request.language.c_str());  
  }
  else {
    srcml_archive_set_language(srcml_arch, SRCML_LANGUAGE_NONE);  
  }

  srcml_archive_set_tabstop(srcml_arch, srcml_request.tabs);

  for (size_t i = 0; i < srcml_request.register_ext.size(); ++i) {
    int pos = srcml_request.register_ext[i].find('=');
    srcml_archive_register_file_extension(srcml_arch, srcml_request.register_ext[i].substr(0,pos).c_str(),
          srcml_request.register_ext[i].substr(pos+1).c_str());
  }

  for (size_t i = 0; i < srcml_request.xmlns_prefix.size(); ++i) {
    int pos = srcml_request.xmlns_prefix[i].find('=');
    srcml_archive_register_namespace(srcml_arch, srcml_request.xmlns_prefix[i].substr(0,pos).c_str(),
           srcml_request.xmlns_prefix[i].substr(pos+1).c_str());
  }

  srcml_write_open_filename(srcml_arch, srcml_request.output.c_str());
  
  // Setup threading
  ThreadQueue<ParseRequest, 3> data_queue;

  pthread_t writer;
  pthread_attr_t tattr;
  int newprio = 90;
  sched_param param;

  pthread_attr_init(&tattr);
  pthread_attr_getschedparam (&tattr, &param);
  param.sched_priority = newprio;
  pthread_attr_setschedparam (&tattr, &param);
  pthread_create(&writer, &tattr, srcml_consume, &data_queue);

  // Main processing loop
  for (size_t i = 0; i < srcml_request.positional_args.size(); ++i) {
    std::string * input_file = &srcml_request.positional_args[i];
    bool stdin = false;

    // libArchive Setup
    archive * arch = archive_read_new();
    archive_entry * arch_entry = archive_entry_new();

    setupLibArchive(arch);

    int valid = 0;

    // Regular file or archive
    if (input_file->compare("-") != 0) {
      valid = archive_read_open_filename(arch, input_file->c_str(), 16384); 
    }
    
    // Stdin
    if (input_file->compare("-") == 0) {
      stdin = true;
      // Check if we are using the terminal interactively
      if(srcml_request.command & SRCML_COMMAND_INTERACTIVE) {
        if (!test_for_stdin())
          return 1; // Stdin was requested, but no data was received
      }
      
      // Setting libarchive's file input to NULL forces libarchive to read stdin  
      valid = archive_read_open_filename(arch, NULL, 16384);
    }

    if (valid == ARCHIVE_OK) {
      while (archive_read_next_header(arch, &arch_entry) == ARCHIVE_OK) { 

        srcml_unit * unit = srcml_create_unit(srcml_arch);
        std::string entry_name = archive_entry_pathname(arch_entry);
        const char * filename = NULL; // Assume Stdin for simplicity
        
        /* 
          The header path for a standard file is just "data".
          That needs to be swapped out with the actual file name from the 
          CLI arg.
        */
        if (entry_name.compare("data") == 0 && !stdin)
          filename = srcml_request.positional_args[i].c_str();

        if (entry_name.compare("data") != 0 && !stdin)
          filename = entry_name.c_str();

        if (filename) {
          const char * language = srcml_archive_check_extension(srcml_arch, filename);
          if (!language) {
            // Extension not supported
            // Skip to next header
            continue;  
          }
        }
        else {
          // Stdin language declared via CLI
          if (srcml_archive_get_language(srcml_arch))
            return 1; // Stdin used with no language specified.
        }

        srcml_unit_set_filename(unit, filename);
        srcml_unit_set_language(unit, ((srcml_archive_get_language(srcml_arch)) ? srcml_request.language.c_str() : srcml_archive_check_extension(srcml_arch, filename)));

        
        const void* buffer;
        const char* cptr;
        size_t size;
        int64_t offset;
        std::vector<char> dbuff;

        while (true) {
          int readStatus = archive_read_data_block(arch, &buffer, &size, &offset);
          cptr = (char*)buffer;
          
          if (readStatus != ARCHIVE_OK) {
            break;
          }
          
          for (size_t i = 0; i < size; ++i) {
            dbuff.push_back(cptr[i]);
          }

          ParseRequest pr;
          pr.filename = filename

          srcml_parse_unit_memory(unit, cptr, size);
          srcml_write_unit(srcml_arch, unit);
        }
      }
    }
    else {
      std::cerr << "Unable to open archive\n";
      return 1;
    }
    archive_read_finish(arch);
  }

  srcml_close_archive(srcml_arch);
  srcml_free_archive(srcml_arch);

  return 0;
}
