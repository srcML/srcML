##
# @file options.py
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# number representing libsrcml version
SRCML_VERSION_NUMBER = 8000
# string containing libsrcml version
SRCML_VERSION_STRING = "0.8.0"
# srcML language not set
SRCML_LANGUAGE_NONE   = 0
# string for language C
SRCML_LANGUAGE_C      = "C"
# string for language C++
SRCML_LANGUAGE_CXX    = "C++"
# string for language Java
SRCML_LANGUAGE_JAVA   = "Java"
# string for language C#
SRCML_LANGUAGE_CSHARP = "C#"
# string for language C#
SRCML_LANGUAGE_OBJECTIVE_C = "Objective-C"
# string for language XML
SRCML_LANGUAGE_XML    = "xml"
# Create an archive
SRCML_OPTION_ARCHIVE           = 1<<0
# Include line/column position attributes
SRCML_OPTION_POSITION          = 1<<1
# Markup preprocessor elements (default for C, C++, C#)
SRCML_OPTION_CPP_NOMACRO       = 1<<2
# Markup preprocessor elements (default for C, C++)
SRCML_OPTION_CPP               = 1<<2 | 1<<3
# Issue an XML declaration
SRCML_OPTION_XML_DECL          = 1<<4
# Include any XML namespace declarations
SRCML_OPTION_NAMESPACE_DECL    = 1<<5
# Leave as text preprocessor else parts (default: markup)
SRCML_OPTION_CPP_TEXT_ELSE     = 1<<6
# Markup preprocessor @code #if 0 @endcode sections (default: leave as text)
SRCML_OPTION_CPP_MARKUP_IF0    = 1<<7
# Apply transformations to the entire srcML file (default: each unit
SRCML_OPTION_APPLY_ROOT        = 1<<8
# Nest if in else if intead of elseif tag
SRCML_OPTION_NESTIF            = 1<<9
# Output hash attribute on each unit (default: on)
SRCML_OPTION_HASH              = 1<<10
# Wrap function/classes/etc with templates (default: on)
SRCML_OPTION_WRAP_TEMPLATE     = 1<<11
# output is interactive (good for editing applications)
SRCML_OPTION_INTERACTIVE       = 1<<12
# Not sure what this used for
SRCML_OPTION_XPATH_TOTAL       = 1<<13
# expression mode
SRCML_OPTION_EXPRESSION        = 1<<14
# Extra processing of @code#line@endcode for position information
SRCML_OPTION_LINE              = 1<<15
# additional cpp:if/cpp:endif checking
SRCML_OPTION_CPPIF_CHECK       = 1<<16
# debug time attribute
SRCML_OPTION_DEBUG_TIMER       = 1<<17
# turn on optional ternary operator markup
SRCML_OPTION_TERNARY           = 1<<18
# turn on optional ternary operator markup
SRCML_OPTION_PSEUDO_BLOCK      = 1<<19
# Turn on old optional markup behaviour
SRCML_OPTION_OPTIONAL_MARKUP   = 1<<20
# Markups literal in special namespace
SRCML_OPTION_LITERAL           = 1<<21
# Markups modifiers in special namespace
SRCML_OPTION_MODIFIER          = 1<<22
# Markups operator in special namespace
SRCML_OPTION_OPERATOR          = 1<<23
# Markups operator in special namespace
SRCML_OPTION_DEBUG             = 1<<24
# Markups OpenMP in special namespace
SRCML_OPTION_OPENMP            = 1<<25
# Encode the original source encoding as an attribute */
SRCML_OPTION_STORE_ENCODING    = 1<<26

# Return status indicating no errors
SRCML_STATUS_OK                   = 0
# Return status indicating errors occurred
SRCML_STATUS_ERROR                = 1
# Return status indicating an invalid argument
SRCML_STATUS_INVALID_ARGUMENT     = 2
# Return status indicating that their is some problem with the input
SRCML_STATUS_INVALID_INPUT        = 3
# Return status indicating an invalid read I/O operation (such as write on read only archive)
SRCML_STATUS_INVALID_IO_OPERATION = 4
# Return status indicating that their is some problem with the input
SRCML_STATUS_IO_ERROR             = 5
# Return status indicating an unitialized unit
SRCML_STATUS_UNINITIALIZED_UNIT   = 6
# Return status indicating an unset language
SRCML_STATUS_UNSET_LANGUAGE       = 7
# Return status inidicating their are no transformations
SRCML_STATUS_NO_TRANSFORMATION    = 8
