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
 *      checked
 *      unchecked
 *      lock 
 */
void xpath_exfun_has_init(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_simple_asm()
 *  
 *  Not sure
 *  
 *  Languages: C, C++
 *  
 *  Works with srcML elements:
 *      ASM
 *  
 */
void xpath_exfun_is_simple_asm(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_within_catch()
 *  
 *  Checks if the current element is within a catch statement, uses scope rules
 *  
 *  Languages: C++, C#, Java
 *  
 *  Works with srcML elements:
 *      Anything
 *  
 */
void xpath_exfun_is_within_catch(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_within_finally()
 *  
 *  Checks if the current element is within a catch statement
 *  
 *  Languages: C#, Java
 *  
 *  Works with srcML elements:
 *      Anything
 *  
 */
void xpath_exfun_is_within_finally(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool has_break()
 *  
 *  Checks using scope rules and attempts to locate a break statement within
 *  some scope.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      switch
 *      do
 *      while
 *      foreach
 *      for
 *      foreach
 *      Anything that has a block as a child
 *  
 */
void xpath_exfun_has_break(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_fixed()
 *  
 *  Checks to see if the current declaration is within a fixed statement's init.
 *  
 *  Languages: C#
 *  
 *  Works with srcML elements:
 *      decl
 *      init
 *      name
 *      type
 *      expr
 *      Any thing that can occur within the init, see grammar
 *  
 */
void xpath_exfun_is_fixed(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_resource()
 *  
 *  Checks to see of the declaration is part of the init block of a using, lock
 *  , synchronized, Java's resource try fixed or related.
 *  
 *  Languages: C#, Java
 *  
 *  Works with srcML elements:
 *      expr
 *      decl_stmt
 *      decl
 *  
 */
void xpath_exfun_is_resource(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_unsafe()
 *  
 *  Determine if something is within the scope of an unsafe block or function or
 *  if the function, function_decl constructor etc... then this also tree.
 *  
 *  Languages: C#
 *  
 *  Works with srcML elements:
 *      function
 *      function_decl
 *      constructor
 *      constructor_decl
 *      destructor
 *      destructor_decl
 *      Anything other then a function declaration.
 *  
 */
void xpath_exfun_is_unsafe(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: bool is_mutually_exclusive()
 *  
 *  checks to see if a declaration is within a lock or synchronized function or
 *  block
 *  
 *  Languages: C#, Java
 *  
 *  Works with srcML elements:
 *      Anything
 *  
 */
void xpath_exfun_is_mutually_exclusive(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set returns()
 *  
 *  Gets return statements that are children of the block related to the current
 *  element function or language element. This is impossible to implement
 *  because I can't use unions to extend possible XPath combinations.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      function
 *      constructor
 *      destructor
 *  
 */
void xpath_exfun_returns(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set throw_stmts()
 *  
 *  Uses scope rules to determine if there is a throw statement within the
 *  current function, constructor etc...
 *  
 *  Languages: C++, C#, Java
 *  
 *  Works with srcML elements:
 *      function
 *      constructor
 *      destructor
 *      delegate
 *      lambda
 *      block
 *      Anything that has a block as a child
 *  
 */
void xpath_exfun_throw_stmts(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set attributes() OR node_set annotations()
 *  
 *  Searches for an attribute or annotation based on language and returns true
 *  if the current item has attributes or annotations. If this is used at the
 *  namespace, global or block level, annotations and attribute definitions are
 *  returned.
 *  
 *  Languages: C++, C#, Java
 *  
 *  Works with srcML elements:
 *      Literally ANYTHING IN JAVA
 *      C++ and C#
 *      class
 *      class_decl
 *      struct
 *      struct_decl
 *      union
 *      union_decl
 *      function
 *      function_decl
 *      constructor
 *      constructor_decl
 *      destructor
 *      destructor_decl
 *  
 */
void xpath_exfun_attributes(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set parameters()
 *  
 *  Returns a set of parameters for a function, constructor, template etc.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      function
 *      function_decl
 *      constructor
 *      constructor_decl
 *      Works for templates and generic functions
 *      template
 *      union
 *      union_decl
 *      class
 *      class_decl
 *      struct
 *      struct_decl
 *      lambda
 *      delegate
 *  
 */
void xpath_exfun_parameters(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set arguments()
 *  
 *  Returns a set of arguments form a call or name.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      name
 *      call
 *  
 */
void xpath_exfun_arguments(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set templates() OR generics()
 *  
 *  Returns the templates associated with what this was called upon. In C# and
 *  Java this returns the argument listassoc
 *  
 *  Languages: C++,C#,Java
 *  
 *  Works with srcML elements:
 *      function
 *      function_decl
 *      constructor
 *      constructor_decl
 *      union
 *      union_decl
 *      class
 *      class_decl
 *      struct
 *      struct_decl
 *  
 */
void xpath_exfun_templates(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set friends()
 *  
 *  Returns all friend declarations.
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
void xpath_exfun_friends(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set enum_decls()
 *  
 *  Returns all of the enum value declarations.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *  
 */
void xpath_exfun_enum_decls(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set initializers()
 *  
 *  Returns all initializers from within the member_list.
 *  
 *  Languages: C++, C#
 *  
 *  Works with srcML elements:
 *      constructor
 *  
 */
void xpath_exfun_initializers(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set owning_function()
 *  
 *  Takes any expression, declaration or any thing and returns a function,
 *  lambda or delegate that it's part of.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      Anything
 *  
 */
void xpath_exfun_owning_function(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set compute_prev_decl_type()
 *  
 *  Computes the node set that attempts to compute the type of a declaration
 *  within a multi-declaration statement. I'd like this to work on any decl but
 *  that isn't possible without implementing this directly in C++.
 *  
 *  Languages: C, C++, C#
 *  
 *  Works with srcML elements:
 *      decl
 *  
 */
void xpath_exfun_compute_prev_decl_type(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set indexing_expr()
 *  
 *  Returns the name that contains the indexing expression.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      name
 *      expr
 *      expr_stmt
 *      init
 *      decl
 *      decl_stmt
 *      argument
 *      param
 *      condition
 *      incr
 *  
 */
void xpath_exfun_indexing_expr(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set true_expr()
 *  
 *  Gets the true expression from a turnary or if. this returns all then's from
 *  all ifs and elseifs
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      if
 *      elseif
 *      turnary
 *  
 */
void xpath_exfun_true_expr(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set false_expr()
 *  
 *  Gets the false expression from a turnary or if.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      if
 *      elseif
 *      turnary
 *  
 */
void xpath_exfun_false_expr(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set capture_variable_names()
 *  
 *  Returns the names of all variables captured used in the capture list.
 *  
 *  Languages: C++
 *  
 *  Works with srcML elements:
 *      lambda
 *      capture
 *  
 */
void xpath_exfun_capture_variable_names(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set captures()
 *  
 *  Returns the names of all arguments within a capture.
 *  
 *  Languages: C++
 *  
 *  Works with srcML elements:
 *      lambda
 *      capture
 *  
 */
void xpath_exfun_captures(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set caught_exception_parameter()
 *  
 *  Collects all of the exception types from all catch clauses and returns them
 *  in a node set.
 *  
 *  Languages: C++, C#, Java
 *  
 *  Works with srcML elements:
 *      try
 *      catch
 *      finally
 *  
 */
void xpath_exfun_caught_exception_parameter(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set remaining_catches()
 *  
 *  Returns all catch statements which follow either the current try or catch.
 *  
 *  Languages: C++, C#, Java
 *  
 *  Works with srcML elements:
 *      try
 *      catch
 *      finally
 *  
 */
void xpath_exfun_remaining_catches(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set condition()
 *  
 *  Gets a condition from a statement with a condition such as a while, or for
 *  statement.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      do
 *      while
 *      for
 *      switch
 *      if
 *      elseif
 *      turnary
 *  
 */
void xpath_exfun_condition(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set function_call_name()
 *  
 *  Gets the name of the function that's being called (this isn't the qualified
 *  name).
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      call
 *  
 */
void xpath_exfun_function_call_name(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set case_labels()
 *  
 *  Returns a node_set that has all case labels.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      switch
 *      block
 *  
 */
void xpath_exfun_case_labels(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set statements()
 *  
 *  Returns all child statements
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      block
 *      unit
 *      if
 *      for
 *      do
 *      while
 *      foreach
 *      else
 *      elseif
 *      fixed
 *      synchronized
 *      fixed
 *      unsafe
 *      using
 *      switch
 *      Anything that can have one ore more sub statements.
 *  
 */
void xpath_exfun_statements(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set declarations()
 *  
 *  Returns all declarations, doesn't include parameters.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      unit
 *      Anything that can have one or more sub statements.
 *  
 */
void xpath_exfun_declarations(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set functions()
 *  
 *  Returns all child functions, constructors, and destructor, as well as their
 *  decl versions.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      unit
 *      Anything that can have one or more sub statements.
 *  
 */
void xpath_exfun_functions(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set member_functions()
 *  
 *  Returns all functions constructors, and destructors, as well as their decl
 *  versions. So long as those occur within a class, struct union, interface,
 *  annotation or enum and are not static.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      unit
 *      Anything that can have one or more sub statements.
 *  
 */
void xpath_exfun_member_functions(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set members()
 *  
 *  Reruns all members functions, constructors, destructors, events and
 *  properties of a class. If this is called when the context is an enum it
 *  returns the enum names as well.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *  
 */
void xpath_exfun_members(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set data_members()
 *  
 *  Returns all non-static data members of a class.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *  
 */
void xpath_exfun_data_members(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set public_members()
 *  
 *  Returns all public members.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *      namespace
 *      package
 *  
 */
void xpath_exfun_public_members(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set private_members()
 *  
 *  Returns all private members.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *      namespace
 *      package
 *  
 */
void xpath_exfun_private_members(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set protected_members()
 *  
 *  Returns all protected members.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *  
 */
void xpath_exfun_protected_members(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set internal_members()
 *  
 *  Returns all internal members.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *      namespace
 *      package
 *  
 */
void xpath_exfun_internal_members(xmlXPathParserContextPtr ctxt, int nargs);

/**
 *  XPath Signature: node_set protected_internal_members()
 *  
 *  Returns all protected internal members.
 *  
 *  Languages: C, C++, C#, Java
 *  
 *  Works with srcML elements:
 *      enum
 *      struct
 *      class
 *      union
 *      namespace
 *  
 */
void xpath_exfun_protected_internal_members(xmlXPathParserContextPtr ctxt, int nargs);

#endif
