/**
 * @file test_srcml_write_unit.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
 */

/*

  Test cases for srcml_archive_check_extension
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

int main() {

    std::string xml_decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    std::string start_root_tag = "<unit xmlns=\"http://www.sdml.info/srcML/src\">";
    std::string end_root_tag = "</unit>";
    std::string empty_root_tag = "<unit xmlns=\"http://www.sdml.info/srcML/src\"/>";

    std::string srcml_unit = "<unit/>";
    std::string srcml_unit_short = "<";

    std::string srcml_text = "srcML";
    std::string srcml_text_short = "s";


    /*
      srcml_write_raw
    */

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw(archive, srcml_unit.c_str());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + srcml_unit + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw(archive, srcml_unit.c_str());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + start_root_tag + srcml_unit + end_root_tag + "\n");
        free(s);
    }


    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw(archive, srcml_text.c_str());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + srcml_text + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw(archive, srcml_text.c_str());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + start_root_tag + srcml_text + end_root_tag + "\n");
        free(s);
    }

    {
        dassert(srcml_write_raw(0, srcml_unit.c_str()), SRCML_STATUS_INVALID_ARGUMENT);

    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        dassert(srcml_write_raw(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + empty_root_tag + "\n");
        free(s);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_write_raw(archive, srcml_unit.c_str()), SRCML_STATUS_INVALID_IO_OPERATION);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    /*
      srcml_write_raw_len
    */

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_unit.c_str(), srcml_unit.size());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + srcml_unit + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_unit.c_str(), srcml_unit.size());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + start_root_tag + srcml_unit + end_root_tag + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_unit.c_str(), 1);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + srcml_unit_short + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_unit.c_str(), 1);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + start_root_tag + srcml_unit_short + end_root_tag + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_text.c_str(), srcml_text.size());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + srcml_text + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_text.c_str(), srcml_text.size());
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + start_root_tag + srcml_text + end_root_tag + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_text.c_str(), 1);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + srcml_text_short + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_text.c_str(), 1);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + start_root_tag + srcml_text_short + end_root_tag + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_text.c_str(), 0);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + "\n");
        free(s);
    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        srcml_write_raw_len(archive, srcml_text.c_str(), 1);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + empty_root_tag + "\n");
        free(s);
    }

    {
        dassert(srcml_write_raw_len(0, srcml_unit.c_str(), srcml_unit.size()), SRCML_STATUS_INVALID_ARGUMENT);

    }

    {
        char * s = 0;
        int size;
        srcml_archive * archive = srcml_create_archive();
        srcml_write_open_memory(archive, &s, &size);
        dassert(srcml_write_raw_len(archive, 0, srcml_unit.size()), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_close_archive(archive);
        srcml_free_archive(archive);

        dassert(s, xml_decl + empty_root_tag + "\n");
        free(s);
    }

    {
        srcml_archive * archive = srcml_create_archive();
        dassert(srcml_write_raw_len(archive, srcml_unit.c_str(), srcml_unit.size()), SRCML_STATUS_INVALID_IO_OPERATION);
        srcml_close_archive(archive);
        srcml_free_archive(archive);
    }

    srcml_cleanup_globals();

    return 0;

}
