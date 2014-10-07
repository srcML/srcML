/**
 * @file srcmlexfun_impl.hpp
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
#ifndef INCLUDED_SRCEXFUN_IMPL_HPP
#define INCLUDED_SRCEXFUN_IMPL_HPP
#include <libxml/xpath.h>

/**
 *  XPath Signature: bool has_return()
 *
 *  Languages: C, C++, C#, Java
 *
 *  Checks all blocks children and all subsequent children for a return statement.
 *  This is scope respecting, in that it doesn't search within closures functions
 *  or local classes.
 *
 *  Works with srcml elements: 
 *      block
 *      function
 *      while
 *      if
 *      elseif
 *      else
 *      try
 *      catch
 *      finally
 *      do
 *      for
 *      foreach
 *      switch
 *      lambda
 *      delegate
 *      using_stmt
 *      fixed
 *      lock
 *      synchronized
 *      fixed
 *      unsafe
 *      constructor
 *      destructor
 *      static
 *      checked
 *      unchecked
 *  
 */
void xpath_exfun_has_return(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_nested()
 *
 *  Checks to see if a statement is inside another statement (for all statements),
 *  for type declarations it checks if the type inside another type declaration,
 *  for namespaces it checks if it is inside another namespace (or package for Java).
 *  For constructors, functions and destructors it is nested if it's within a class
 *  or namespace.
 *
 *  Languages: C, C++, C#, Java
 *
 *  Works on srcml elements:
 *      decl_stmt
 *      empty_stmt
 *      if
 *      switch
 *      else
 *      elseif
 *      for
 *      do
 *      while
 *      foreach
 *      break
 *      continue
 *      return
 *      default
 *      block
 *      label
 *      goto
 *      asm
 *      synchronized
 *      assert
 *      fixed
 *      using_stmt
 *      unsafe
 *      lock
 *      try
 *      catch
 *      finally
 *      typedef
 *      throw
 *      function
 *      function_decl
 *      constructor
 *      constructor_decl
 *      destructor
 *      destructor_decl
 *      property
 *      event
 *      union
 *      union_decl
 *      class
 *      class_decl
 *      struct
 *      struct_decl
 *      enum
 *      namespace
 *      package
 *
 */
void xpath_exfun_is_nested(xmlXPathParserContextPtr ctxt, int nargs);

/** 
 *  XPath Signature: bool is_partial_template_specialization()
 *
 *  Checks to see if a class template is a partial specialization. This is done by 
 *  checking for a template arguments and a template within the name of the class,
 *  struct or union.
 *
 *  Language: C++
 *
 *  Works with srcML elements:
 *     union
 *     union_decl
 *     class
 *     class_decl
 *     struct
 *     struct_decl
 */
void xpath_exfun_is_partial_template_specialization(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_local()
 *
 *  Checks variable declarations to see if they are local to a function. For, classes
 *  and other type declarations occur within a function.
 *
 *  Languages: C, C++, C#, Java
 *
 *  Works with srcML elements:
 *     class
 *     class_decl
 *     union
 *     union_decl
 *     struct
 *     struct_decl
 *     decl
 *     decl_stmt
 *     type
 *     param
 *     argument

 */
void xpath_exfun_is_local(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool has_init()
 *
 *  Checks for an initialization within a declaration statement or the init
 *  that's part of other statements such as a for, using_stmt, or synchronize.
 *
 *  Languages: C, C++, C#, Java
 *
 *  Works with srcML elements:
 *      decl
 *      decl_stmt
 *      param
 *      argument
 */
void xpath_exfun_has_init(xmlXPathParserContextPtr ctxt, int nargs);
#endif
