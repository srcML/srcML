/**
 * @file srcmlexfun_impl.hpp
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
#ifndef INCLUDED_SRCEXFUN_IMPL_HPP
#define INCLUDED_SRCEXFUN_IMPL_HPP
#include <libxml/xpath.h>

/**
 *  XPath Signature: bool has_return()
 *  
 *  Checks all block children and all subsequent children for a return
 *  statement. This is scope respecting, in that it doesn't search within
 *  closures functions or local classes.
 *  
 *  has_return can be called on almost anything but the typical case 
 *  is for it to be used on statements or functions, The behavior it has
 *  when called on non-complex statements (those that don't combine multiple
 *  statements using a block such as return, throw, expr_stmt and decl_stmt)
 *  is to search those statements for a return including the sub elements 
 *  excluding  lambdas and delegates, This means that because only the sub
 *  elements are searched that a return doesn't have a return (because it is
 *  one).
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      block
 *      function
 *      constructor
 *      destructor
 *      while
 *      if
 *      then
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
 *      unsafe
 *      static
 *      checked
 *      unchecked
 *  
 */
void xpath_exfun_has_return(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool has_throw()
 *  
 *  Just like has_return but for throw instead.
 *  
 *  Languages: C++, C#, Java
 *  
 *  Works with srcML elements:
 *      block
 *      function
 *      constructor
 *      destructor
 *      while
 *      if
 *      then
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
 *      unsafe
 *      static
 *      checked
 *      unchecked
 *  
 */
void xpath_exfun_has_throw(xmlXPathParserContextPtr ctxt, int nargs);


/**
 *  XPath Signature: bool is_nested()
 *  
 *  Checks to see if a class or type declaration is nested within another
 *  scope. This function only works for parts of the language which
 *  can occur at a global scope.
 *
 *  Handled specially:
 *  asm
 *      - Checks to see of the current ASM statement is within another ASM statement.
 *
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      asm
 *      typedef
 *      using
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
 *  
 */
void xpath_exfun_is_nested(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_class_template_partial_specialization()
 *  
 *  Checks to see if a class template is a partial specialization. This is done
 *  by checking for a template arguments and a template within the name of the
 *  class, struct or union.
 *  
 *  Languages: C++
 *  
 *  Works with srcML elements:
 *      union
 *      union_decl
 *      class
 *      class_decl
 *      struct
 *      struct_decl
 *  
 */
void xpath_exfun_is_class_template_partial_specialization(xmlXPathParserContextPtr ctxt, int nargs);

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
 *      using_stmt
 *      using
 *      try
 *      synchronized
 *      fixed
 *      lock 
 */
void xpath_exfun_has_init(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool has_break()
 *  
 *  Checks using scope rules and attempts to locate a break statement within
 *  a loop construct or switch statement. This respects the scope of other loops
 *  by not searching their children when they are not the contexts of the
 *  predicate.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works any expression like statement:
 *      switch
 *      do
 *      while
 *      foreach
 *      for
 *
 */
void xpath_exfun_has_break(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_unsafe()
 *  
 *  Checks to see if the current statement, declaration or other element is within
 *  an unsafe block.
 *  
 *  Languages: C#
 *  
 *  Works with srcML elements:
 *      Anything
 *
 */
void xpath_exfun_is_unsafe(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_mutually_exclusive()
 *  
 *  Checks to see if a declaration is within a lock or synchronized function or
 *  block.
 *  
 *  Languages: C#, Java
 *  
 *  Works with srcML elements:
 *      Anything
 *  
 */
void xpath_exfun_is_mutually_exclusive(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_upper_bound()
 *  Languages java
 *  Checks for an upper bound generic parameter (Not sure if I can actually do this but I'm gonna try).
 *      argument
 */
// void xpath_exfun_is_upper_bound(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_lower_bound()
 *  Languages java
 *  Checks for a lower bound on a generic parameter (Not sure if I can actually do this but I'm gonna try).
 *      argument
 *
 */
// void xpath_exfun_is_lower_bound(xmlXPathParserContextPtr ctxt, int nargs);
#endif
