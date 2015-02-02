/**
 * @file test_UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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
  Test cases for UTF8CharBuffer.
*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#ifndef _MSC_BUILD
#include <unistd.h>
#else
#include <io.h>
#endif
#include <cassert>
#include <fstream>
#include <UTF8CharBuffer.hpp>
#include <boost/optional.hpp>

int main() {

    std::ofstream file("a.cpp");
    file << "abc";
    file.close();

    file.open("bom.cpp");
    file << "\xEF\xBB\xBF""abc";
    file.close();

    file.open("iso.cpp");
    file << "/** ";
    file << (char)254;
    file << (char)255;
    file << " */";
    file.close();

    file.open("utf8.cpp");
    file << "/** ";
    file << "þÿ";
    file << " */";
    file.close();

    file.open("long.cpp");
    for(int i = 0; i < 4096; ++i)
        file << 'a';
    file.close();

    /*

      UTF8CharBuffer(const char * ifilename, const char * encoding)

    */

    {
        try {
            UTF8CharBuffer utf8("a.cpp", "ISO-8859-1", 0);
        } catch(...) { assert(false); }
    }

    {
        try {
            UTF8CharBuffer utf8((const char *)NULL, "ISO-8859-1", 0);
            assert(false);
        } catch(...) {}
    }

    {
        try {
            UTF8CharBuffer utf8("a.cpp", NULL, 0);
        } catch(...) { assert(false); }
    }

    /*

      UTF8CharBuffer(char * c_buffer, size_t buffer_size, const char * encoding)

    */

    {
        const char * s = "";
        size_t i = 1;
        try {
            UTF8CharBuffer utf8(s, i, "ISO-8859-1", 0);
        } catch(...) { assert(false); }
    }

    {
        size_t i;
        try {
            UTF8CharBuffer utf8(NULL, i, "ISO-8859-1", 0);
            assert(false);
        } catch(...) {}
    }

    {
        const char * s = "";
        size_t i = 1;
        try {
            UTF8CharBuffer utf8(s, i, NULL, 0);
        } catch(...) { assert(false); }
    }

    {
        const char * s = "";
        size_t i = 0;
        try {
            UTF8CharBuffer utf8(s, i, NULL, 0);
        } catch(...) { assert(false); }
    }

    /*

      UTF8CharBuffer(FILE * file, const char * encoding)

    */

    {
        FILE * f = fopen("a.cpp", "r");
        try {
            UTF8CharBuffer utf8(f, "ISO-8859-1", 0);
        } catch(...) { assert(false); }
    }

    {
        try {
            UTF8CharBuffer utf8((FILE *)NULL, "ISO-8859-1", 0);
            assert(false);
        } catch(...) {}
    }

    {
        FILE * f = fopen("a.cpp", "r");
        try {
            UTF8CharBuffer utf8(f, NULL, 0);
        } catch(...) { assert(false); }
    }

    /*

      UTF8CharBuffer(int fd, const char * encoding)

    */

    {
        int f = open("a.cpp", O_RDONLY);
        try {
            UTF8CharBuffer utf8(f, "ISO-8859-1", 0);
        } catch(...) { assert(false); }
    }

    {
        try {
            UTF8CharBuffer utf8(-1, "ISO-8859-1", 0);
            assert(false);
        } catch(...) {}
    }

    {
        int f = open("a.cpp", O_RDONLY);
        try {
            UTF8CharBuffer utf8(f, NULL, 0);
        } catch(...) { assert(false); }
    }

    /*

      getChar()

    */

    {

        UTF8CharBuffer utf8("a.cpp", "UTF-8", 0);
        assert(utf8.getChar() == 'a');
        assert(utf8.getChar() == 'b');
        assert(utf8.getChar() == 'c');

    }

    {

        UTF8CharBuffer utf8("abc", 3, "UTF-8", 0);
        assert(utf8.getChar() == 'a');
        assert(utf8.getChar() == 'b');
        assert(utf8.getChar() == 'c');

    }


    {

        FILE * file = fopen("a.cpp", "r");
        {
            UTF8CharBuffer utf8(file, "UTF-8", 0);
            assert(utf8.getChar() == 'a');
            assert(utf8.getChar() == 'b');
            assert(utf8.getChar() == 'c');
        }
        fclose(file);

    }

    {

        int fd = open("a.cpp", O_RDONLY);
        {
            UTF8CharBuffer utf8(fd, "UTF-8", 0);
            assert(utf8.getChar() == 'a');
            assert(utf8.getChar() == 'b');
            assert(utf8.getChar() == 'c');
        }
        close(fd);

    }

    {
        unsigned char s[9];

        s[0] = '/';
        s[1] = '*';
        s[2] = '*';
        s[3] = ' ';
        s[4] = 254;
        s[5] = 255;
        s[6] = ' ';
        s[7] = '*';
        s[8] = '/';

        UTF8CharBuffer utf8((const char *)s, 9, "ISO-8859-1", 0);
        assert(utf8.getChar() == '/');
        assert(utf8.getChar() == '*');
        assert(utf8.getChar() == '*');
        assert(utf8.getChar() == ' ');
        assert(utf8.getChar() == 195);
        assert(utf8.getChar() == 190);
        assert(utf8.getChar() == 195);
        assert(utf8.getChar() == 191);
        assert(utf8.getChar() == ' ');
        assert(utf8.getChar() == '*');
        assert(utf8.getChar() == '/');

    }

    {

        UTF8CharBuffer utf8("iso.cpp", "ISO-8859-1", 0);
        assert(utf8.getChar() == '/');
        assert(utf8.getChar() == '*');
        assert(utf8.getChar() == '*');
        assert(utf8.getChar() == ' ');
        assert(utf8.getChar() == 195);
        assert(utf8.getChar() == 190);
        assert(utf8.getChar() == 195);
        assert(utf8.getChar() == 191);
        assert(utf8.getChar() == ' ');
        assert(utf8.getChar() == '*');
        assert(utf8.getChar() == '/');

    }

    {

        UTF8CharBuffer utf8("long.cpp", "UTF-8", 0);
        for(int i = 0; i < 4096; ++i)
            assert(utf8.getChar() == 'a');

        assert(utf8.getChar() == -1);

    }

    {

        std::vector<char> s(4096, 'a');

        UTF8CharBuffer utf8(&s.front(), 4096, "UTF-8", 0);
        for(int i = 0; i < 4096; ++i)
            assert(utf8.getChar() == 'a');

        assert(utf8.getChar() == -1);

    }

    {

        UTF8CharBuffer utf8("long.cpp", "ISO-8859-1", 0);
        for(int i = 0; i < 4096; ++i)
            assert(utf8.getChar() == 'a');

        assert(utf8.getChar() == -1);

    }

    {

        std::vector<char> s(4096, 'a');

        UTF8CharBuffer utf8(&s.front(), 4096, "ISO-8859-1", 0);
        for(int i = 0; i < 4096; ++i)
            assert(utf8.getChar() == 'a');

        assert(utf8.getChar() == -1);

    }

    /*
      hash
    */

    {

        boost::optional<std::string> hash;
        {

            UTF8CharBuffer utf8("iso.cpp", "ISO-8859-1", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("594bb6deba9a7dbec4f4a560333a11fe1d4a3ad9"));

    }

    {

        boost::optional<std::string> hash;
        {

            UTF8CharBuffer utf8("utf8.cpp", "UTF-8", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("45933290ed0df422e5936271ad6e08a77fd714bf"));

    }

    {

        boost::optional<std::string> hash;
        {

            UTF8CharBuffer utf8("/** \xfe\xff */", 9, "ISO-8859-1", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("594bb6deba9a7dbec4f4a560333a11fe1d4a3ad9"));

    }

    {

        boost::optional<std::string> hash;
        {

            UTF8CharBuffer utf8("/** þÿ */", 11, "UTF-8", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("45933290ed0df422e5936271ad6e08a77fd714bf"));

    }

    {

        boost::optional<std::string> hash;
        {

            FILE * f = fopen("iso.cpp", "r");
            UTF8CharBuffer utf8(f, "ISO-8859-1", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("594bb6deba9a7dbec4f4a560333a11fe1d4a3ad9"));

    }

    {

        boost::optional<std::string> hash;
        {

            FILE * f = fopen("utf8.cpp", "r");
            UTF8CharBuffer utf8(f, "UTF-8", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("45933290ed0df422e5936271ad6e08a77fd714bf"));

    }

    {

        boost::optional<std::string> hash;
        {

            int fd = open("iso.cpp", O_RDONLY);
            UTF8CharBuffer utf8(fd, "ISO-8859-1", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("594bb6deba9a7dbec4f4a560333a11fe1d4a3ad9"));

    }

    {

        boost::optional<std::string> hash;
        {

            int fd = open("utf8.cpp", O_RDONLY);
            UTF8CharBuffer utf8(fd, "UTF-8", &hash);
            assert(utf8.getChar() == '/');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 190);
            assert(utf8.getChar() == 195);
            assert(utf8.getChar() == 191);
            assert(utf8.getChar() == ' ');
            assert(utf8.getChar() == '*');
            assert(utf8.getChar() == '/');

        }

        assert(hash == std::string("45933290ed0df422e5936271ad6e08a77fd714bf"));

    }

    /** test auto BOM detection */

    {

        UTF8CharBuffer utf8("bom.cpp", 0, 0);
        assert(utf8.getChar() == 'a');
        assert(utf8.getChar() == 'b');
        assert(utf8.getChar() == 'c');

    }

    {

        UTF8CharBuffer utf8("\xEF\xBB\xBF""abc", 6, 0, 0);
        assert(utf8.getChar() == 'a');
        assert(utf8.getChar() == 'b');
        assert(utf8.getChar() == 'c');

    }


    {

        FILE * file = fopen("bom.cpp", "r");
        {
            UTF8CharBuffer utf8(file, 0, 0);
            assert(utf8.getChar() == 'a');
            assert(utf8.getChar() == 'b');
            assert(utf8.getChar() == 'c');
        }
        fclose(file);

    }

    {

        int fd = open("bom.cpp", O_RDONLY);
        {
            UTF8CharBuffer utf8(fd, 0, 0);
            assert(utf8.getChar() == 'a');
            assert(utf8.getChar() == 'b');
            assert(utf8.getChar() == 'c');
        }
        close(fd);

    }

    /*

    getEncoding

    */

    {

        UTF8CharBuffer utf8("a.cpp", 0, 0);
        assert(*utf8.getEncoding() == "ISO-8859-1");

    }

    {

        UTF8CharBuffer utf8("bom.cpp", 0, 0);
        assert(*utf8.getEncoding() == "UTF-8");

    }

    {

        UTF8CharBuffer utf8("a.cpp", "UTF-8", 0);
        assert(*utf8.getEncoding() == "UTF-8");

    }

    unlink("a.cpp");
    unlink("bom.cpp");
    unlink("iso.cpp");
    unlink("utf8.cpp");
    unlink("long.cpp");

    return 0;
}
