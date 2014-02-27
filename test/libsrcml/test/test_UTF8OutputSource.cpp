/**
 * @file test_UTF8OutputSource.cpp
 *
 * @copyright Copyright (C) 2014  SDML (www.srcML.org)
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
  Test cases for UTF8OutputSource.
*/

#include <UTF8OutputSource.hpp>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <fstream>
#include "dassert.hpp"


int main() {

    /*

      writeString()

    */

    {

	{
	    UTF8OutputSource utf8("a.cpp", "ISO-8859-1");
	    dassert(utf8.writeString("abc", 3), 1);
	}

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

	dassert(src, "abc");

	unlink("a.cpp");

    }

    {

	char * s;
	size_t i;

	{
	    UTF8OutputSource utf8(&s, &i, "ISO-8859-1");
	    dassert(utf8.writeString("abc", 3), 1);
	}

	dassert(i, 3);
	dassert(s, std::string("abc"));

    }

    {

        FILE * file = fopen("a.cpp", "w");
	{
	    UTF8OutputSource utf8(file, "ISO-8859-1");
	    dassert(utf8.writeString("abc", 3), 1);
	}

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

	dassert(src, "abc");

	unlink("a.cpp");

    }

    {

        int fd = open("a.cpp", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	{
	    UTF8OutputSource utf8(fd, "ISO-8859-1");
	    dassert(utf8.writeString("abc", 3), 1);
	}
        close(fd);

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

	dassert(src, "abc");

        unlink("a.cpp");

    }

    {

	{
	    UTF8OutputSource utf8("a.cpp", "ISO-8859-1");
	    dassert(utf8.writeString("/** \u00fe\u00ff */", 11), 1);
	}

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

	dassert(src, "/** \xfe\xff */");

        unlink("a.cpp");

    }

    {

	char * s;
	size_t i;
	{
	    UTF8OutputSource utf8(&s, &i, "ISO-8859-1");
	    dassert(utf8.writeString("/** \u00fe\u00ff */", 11), 1);
	}

	dassert(s, std::string("/** \xfe\xff */"));

    }

    {

	char * s;
	size_t i;
	{
	    UTF8OutputSource utf8(&s, &i, "UTF-8");
	    dassert(utf8.writeString("/** \u00fe\u00ff */", 11), 1);
	}

	dassert(s, std::string("/** \u00fe\u00ff */"));

    }

    {

	{
	    UTF8OutputSource utf8("a.cpp", "UTF-8");
	    dassert(utf8.writeString("/** \u00fe\u00ff */", 11), 1);
	}

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

	dassert(src, "/** \u00fe\u00ff */");

        unlink("a.cpp");
    }

    {

	{
	    UTF8OutputSource utf8("a.cpp", "ISO-8859-1");
	    for(int i = 0; i < 4096; ++i)
		dassert(utf8.writeString("a", 1), 1);
	}

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

	dassert(src.size(), 4096);
	for(int i = 0; i < 4096; ++i)
	    dassert(src[i], 'a');

        unlink("a.cpp");

    }

    {

	char * s;
	size_t i;
	{
	    UTF8OutputSource utf8(&s, &i, "ISO-8859-1");
	    for(int i = 0; i < 4096; ++i)
		dassert(utf8.writeString("a", 1), 1);
	}

	dassert(i, 4096);
        for(int i = 0; i < 4096; ++i)
            dassert(s[i], 'a');

    }

    {

	{
	    UTF8OutputSource utf8("a.cpp", "UTF-8");
            for(int i = 0; i < 4096; ++i)
                dassert(utf8.writeString("a", 1), 1);
	}

	std::string src;
        std::ifstream in("a.cpp");
	char c;
	while(in.get(c))
	    src += c;

        dassert(src.size(), 4096);
        for(int i = 0; i < 4096; ++i)
            dassert(src[i], 'a');

        unlink("a.cpp");

    }

    {

	char * s;
	size_t i;
	{
	    UTF8OutputSource utf8(&s, &i, "UTF-8");
            for(int i = 0; i < 4096; ++i)
                dassert(utf8.writeString("a", 1), 1);
	}

        dassert(i, 4096);
        for(int i = 0; i < 4096; ++i)
            dassert(s[i], 'a');

    }

    {

	{
	    UTF8OutputSource utf8("a.cpp", "ISO-8859-1");
	    dassert(utf8.writeString("/** \ufeff */", 11), 0);
	}

        unlink("a.cpp");

    }

    return 0;
}
