/**
 * @file test_LanguageExtensionRegistry.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <LanguageExtensionRegistry.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>

int main() {

    /*
        at
    */
    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.at(0).n == Language::LANGUAGE_CXX);
        assert(reg_ext.at(0).s == "cpp");
    }

    {
        LanguageExtensionRegistry reg_ext;
        try {
         reg_ext.at(0);
         assert(false);
        } catch(LanguageExtensionRegistryError error) {} 
    }


    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        try {
         reg_ext.at(1);
         assert(false);
        } catch(LanguageExtensionRegistryError error) {} 
    }

    /*
        last
    */
    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.last().n == Language::LANGUAGE_CXX);
        assert(reg_ext.last().s == "cpp");
    }

    {
        LanguageExtensionRegistry reg_ext;
        try {
         reg_ext.last();
         assert(false);
        } catch(LanguageExtensionRegistryError error) {} 
    }


    /*
        size
    */
    {
        LanguageExtensionRegistry reg_ext;
        assert(reg_ext.size() == 0);
    }

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.size() == 1);
    }

    /*
      registerUserExt(const char *, int)
    */
    {
        LanguageExtensionRegistry reg_ext;
        assert(reg_ext.registerUserExt("cpp", Language::LANGUAGE_CXX));
        assert(reg_ext.size() == 1);
        assert(reg_ext.at(0).n == Language::LANGUAGE_CXX);
        assert(reg_ext.at(0).s == "cpp");
    }

    {
        LanguageExtensionRegistry reg_ext;
        assert(!reg_ext.registerUserExt("cpp", Language::LANGUAGE_NONE));
        assert(reg_ext.size() == 0);
    }

    /*
      registerUserExt(const char *, int)
    */
    {
        LanguageExtensionRegistry reg_ext;
        assert(reg_ext.registerUserExt("cpp", "C++"));
        assert(reg_ext.size() == 1);
        assert(reg_ext.at(0).n == Language::LANGUAGE_CXX);
        assert(reg_ext.at(0).s == "cpp");
    }

    {
        LanguageExtensionRegistry reg_ext;
        assert(!reg_ext.registerUserExt("cpp", "Foo"));
        assert(reg_ext.size() == 0);
    }

    /*
      getLanguageFromFilename
    */
    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.getLanguageFromFilename("a.cpp") == Language::LANGUAGE_CXX);
    }

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        int lang = reg_ext.getLanguageFromFilename("a.cpp.gz");

        std::cout << "reg_ext." << lang << '\n';
        assert(reg_ext.getLanguageFromFilename("a.cpp.gz") == Language::LANGUAGE_CXX);
    }

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.getLanguageFromFilename("a.cpp.bz2") == Language::LANGUAGE_CXX);
    }


    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.getLanguageFromFilename("a.cpp.xz") == Language::LANGUAGE_CXX);
    }

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.registerUserExt("cpp", "C++");
        assert(reg_ext.getLanguageFromFilename("a.foo") == 0);
    }

    /*
      register_standard_file_extensions()
    */
    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.register_standard_file_extensions();
        assert(reg_ext.size() != 0);
    }

    /*
        c_is_cpp
    */

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.c_is_cpp(false);
        reg_ext.register_standard_file_extensions();
        assert(reg_ext.getLanguageFromFilename("a.h") == Language::LANGUAGE_C);
        assert(reg_ext.getLanguageFromFilename("a.c") == Language::LANGUAGE_C);
    }

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.c_is_cpp(false);
        reg_ext.register_standard_file_extensions();
        assert(reg_ext.getLanguageFromFilename("a.h") == Language::LANGUAGE_C);
        assert(reg_ext.getLanguageFromFilename("a.c") == Language::LANGUAGE_C);
    }

    {
        LanguageExtensionRegistry reg_ext;
        reg_ext.c_is_cpp(true);
        reg_ext.register_standard_file_extensions();
        assert(reg_ext.getLanguageFromFilename("a.h") == Language::LANGUAGE_CXX);
        assert(reg_ext.getLanguageFromFilename("a.c") == Language::LANGUAGE_CXX);
    }   

    return 0;

}