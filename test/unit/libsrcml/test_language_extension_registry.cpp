/**
 * @file test_language_extension_registry.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

#include <language_extension_registry.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>
#include <dassert.hpp>

int main() {

    /*
        at
    */
    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(get_language(reg_ext.at(0)), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext.at(0)), "cpp");
    }

    {
        language_extension_registry reg_ext;
        try {
         reg_ext.at(0);
         assert(false);
        } catch(language_extension_registry_error error) {} 
    }


    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        try {
         reg_ext.at(1);
         assert(false);
        } catch(language_extension_registry_error error) {} 
    }

    /*
        last
    */
    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(get_language(reg_ext.last()), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext.last()), "cpp");
    }

    {
        language_extension_registry reg_ext;
        try {
         reg_ext.last();
         assert(false);
        } catch(language_extension_registry_error error) {} 
    }


    /*
        size
    */
    {
        language_extension_registry reg_ext;
        dassert(reg_ext.size(), 0);
    }

    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(reg_ext.size(), 1);
    }

    /*
      register_user_ext(const char *, int)
    */
    {
        language_extension_registry reg_ext;
        assert(reg_ext.register_user_ext("cpp", Language::LANGUAGE_CXX));
        dassert(reg_ext.size(), 1);
        dassert(get_language(reg_ext.at(0)), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext.at(0)), "cpp");
    }

    {
        language_extension_registry reg_ext;
        assert(!reg_ext.register_user_ext("cpp", Language::LANGUAGE_NONE));
        dassert(reg_ext.size(), 0);
    }

    /*
      register_user_ext(const char *, int)
    */
    {
        language_extension_registry reg_ext;
        assert(reg_ext.register_user_ext("cpp", "C++"));
        dassert(reg_ext.size(), 1);
        dassert(get_language(reg_ext.at(0)), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext.at(0)), "cpp");
    }

    {
        language_extension_registry reg_ext;
        assert(!reg_ext.register_user_ext("cpp", "Foo"));
        dassert(reg_ext.size(), 0);
    }

    /*
      get_language_from_filename
    */
    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(reg_ext.get_language_from_filename("a.cpp"), Language::LANGUAGE_CXX);
    }

    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        int lang = reg_ext.get_language_from_filename("a.cpp.gz");

        std::cout << "reg_ext." << lang << '\n';
        dassert(reg_ext.get_language_from_filename("a.cpp.gz"), Language::LANGUAGE_CXX);
    }

    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(reg_ext.get_language_from_filename("a.cpp.bz2"), Language::LANGUAGE_CXX);
    }


    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(reg_ext.get_language_from_filename("a.cpp.xz"), Language::LANGUAGE_CXX);
    }

    {
        language_extension_registry reg_ext;
        reg_ext.register_user_ext("cpp", "C++");
        dassert(reg_ext.get_language_from_filename("a.foo"), 0);
    }

    /*
      register_standard_file_extensions()
    */
    {
        language_extension_registry reg_ext;
        reg_ext.register_standard_file_extensions();
        dassert(!reg_ext.size(), 0);
    }

    /*
        c_is_cpp
    */

    {
        language_extension_registry reg_ext;
        reg_ext.c_is_cpp(false);
        reg_ext.register_standard_file_extensions();
        dassert(reg_ext.get_language_from_filename("a.h"), Language::LANGUAGE_C);
        dassert(reg_ext.get_language_from_filename("a.c"), Language::LANGUAGE_C);
    }

    {
        language_extension_registry reg_ext;
        reg_ext.c_is_cpp(false);
        reg_ext.register_standard_file_extensions();
        dassert(reg_ext.get_language_from_filename("a.h"), Language::LANGUAGE_C);
        dassert(reg_ext.get_language_from_filename("a.c"), Language::LANGUAGE_C);
    }

    {
        language_extension_registry reg_ext;
        reg_ext.c_is_cpp(true);
        reg_ext.register_standard_file_extensions();
        dassert(reg_ext.get_language_from_filename("a.h"), Language::LANGUAGE_CXX);
        dassert(reg_ext.get_language_from_filename("a.c"), Language::LANGUAGE_CXX);
    }

    /*
        append
    */
    {

        language_extension_registry reg_ext_one;
        language_extension_registry reg_ext_two;
        reg_ext_two.register_user_ext("cpp", "C++");

        dassert(reg_ext_one.size(), 0);

        reg_ext_one.append(reg_ext_two);

        dassert(reg_ext_one.size(), 1);
        dassert(get_language(reg_ext_one.at(0)), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext_one.at(0)), "cpp");
    }

    {

        language_extension_registry reg_ext_one;
        reg_ext_one.register_user_ext("cpp", "C++");  
        language_extension_registry reg_ext_two;
        reg_ext_two.register_user_ext("h", "C");  
        reg_ext_two.register_user_ext("cs", "C#");  
        reg_ext_two.register_user_ext("m", "Objective-C");  
        reg_ext_two.register_user_ext("java", "Java");  

        dassert(reg_ext_one.size(), 1);
        dassert(get_language(reg_ext_one.at(0)), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext_one.at(0)), "cpp");

        reg_ext_one.append(reg_ext_two);

        dassert(reg_ext_one.size(), 5);
        dassert(get_language(reg_ext_one.at(0)), Language::LANGUAGE_CXX);
        dassert(get_extension(reg_ext_one.at(0)), "cpp");
        dassert(get_language(reg_ext_one.at(1)), Language::LANGUAGE_C);
        dassert(get_extension(reg_ext_one.at(1)), "h");
        dassert(get_language(reg_ext_one.at(2)), Language::LANGUAGE_CSHARP);
        dassert(get_extension(reg_ext_one.at(2)), "cs");
        dassert(get_language(reg_ext_one.at(3)), (Language::LANGUAGE_OBJECTIVE_C | Language::LANGUAGE_C));
        dassert(get_extension(reg_ext_one.at(3)), "m");
        dassert(get_language(reg_ext_one.at(4)), Language::LANGUAGE_JAVA);
        dassert(get_extension(reg_ext_one.at(4)), "java");
    }

    return 0;

}
