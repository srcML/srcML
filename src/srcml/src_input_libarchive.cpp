/*
  src_input_libarchive.cpp

  Copyright (C) 2004-2013  SDML (www.srcML.org)

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
  src_input_libarchive assigns local files, stdin, and archival input to the 
    srcml parsing queue
*/

#include <src_input_libarchive.hpp>
#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>


// Set the options for libarchive to process input files
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

// Convert input to a ParseRequest and assign request to the processing queue
void makeRequest(ParseQueue& queue, srcml_archive* srcml_arch, ParseRequest& request, const std::string& input_file, const std::string& lang) {

  // libArchive Setup
  archive * arch = archive_read_new();
  archive_entry * arch_entry = archive_entry_new();

  setupLibArchive(arch);

  if (archive_read_open_filename(arch, (input_file.compare("-") != 0 ? input_file.c_str() : NULL), 16384)!= ARCHIVE_OK) {
    std::cerr << "Unable to open archive\n";
    exit(1);
  }
    
  // Stdin
  bool stdin = input_file.compare("-") == 0;

  while (archive_read_next_header(arch, &arch_entry) == ARCHIVE_OK) { 
    std::string entry_name = archive_entry_pathname(arch_entry);
    std::string filename = "";
    /* 
      The header path for a standard file is just "data".
      That needs to be swapped out with the actual file name from the 
      CLI arg.
    */
    if (entry_name.compare("data") == 0 && !stdin)
      filename = input_file.c_str();

    if (entry_name.compare("data") != 0 && !stdin)
      filename = entry_name.c_str();

    if (!stdin) {
      const char * language = (lang.compare("xml") == 0) ?  lang.c_str() : srcml_archive_check_extension(srcml_arch, filename.c_str());
      if (!language) {
        // Extension not supported
        // Skip to next header
        continue;  
      }
    }
    else {
      // Stdin language declared via CLI
      if (!srcml_archive_get_language(srcml_arch)) {
        std::cerr << "Using stdin requires a defined language\n";
        exit(1); // Stdin used with no language specified.
      }
    }

    request.buffer.clear();
    while (true) {
      
      const char* buffer;
      size_t size;
      int64_t offset;
      
      if (archive_read_data_block(arch, (const void**) &buffer, &size, &offset) != ARCHIVE_OK)
        break;

      request.buffer.insert(request.buffer.end(), buffer, buffer + size);
    }

    request.filename = filename;
    request.srcml_arch = srcml_arch;
    request.lang = ((srcml_archive_get_language(srcml_arch) || lang.compare("xml") == 0) ? lang.c_str() : srcml_archive_check_extension(srcml_arch, filename.c_str()));
    
    // Hand request off to the processing queue
    queue.push(request);
  }
  archive_read_finish(arch);
}

// Public function used for adding tasks to the parse queue
void src_input_libarchive(ParseQueue& queue, srcml_archive* srcml_arch, ParseRequest& req, const std::string& input, const std::string& lang) {

  boost::filesystem::path localPath(input);

  // input is a single file
  if (!is_directory(localPath)) {
    makeRequest(queue, srcml_arch, req, localPath.string(), (localPath.extension().string().compare(".xml") == 0) ? "xml" : lang);
    return;
  }

  // input is a directory
  for (boost::filesystem::recursive_directory_iterator end, dir(localPath); dir != end; ++dir) {
    if(is_regular_file(*dir)) {
      if (srcml_archive_check_extension(srcml_arch, dir->path().string().c_str()) || dir->path().extension().string() == ".xml")
        makeRequest(queue, srcml_arch, req, dir->path().string(), (dir->path().extension().string().compare(".xml") == 0) ? "xml" : lang);
    }
  }
}
