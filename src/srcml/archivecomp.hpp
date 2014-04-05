/**
 * @file archivecomp.hpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Source input from local files, stdin, and source archives for srcml parsing queue
 */

#ifndef ARCHIVECOMP_HPP
#define ARCHIVECOMP_HPP

#include <archive.h>
#include <string>

int archive_write_set_format_by_extension(struct archive*, const char* extension);
int archive_write_set_compression_by_extension(struct archive*, const char* extension);

bool is_archive(const std::string& input_file_extension);
bool is_compressed(const std::string& input_file_extension);

#endif
