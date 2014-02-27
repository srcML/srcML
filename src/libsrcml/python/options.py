##
# @file options.py
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
SRCML_VERSION_NUMBER = 10000
# string containing libsrcml version
SRCML_VERSION_STRING = "libsrcml 0.9"
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
# string for language XML
SRCML_LANGUAGE_XML    = "xml"
# Create an archive
SRCML_OPTION_ARCHIVE           = 1<<0
# Markups literal in special namespace
SRCML_OPTION_LITERAL           = 1<<1
# Markups modifiers in special namespace
SRCML_OPTION_MODIFIER          = 1<<2
# Markups operator in special namespace
SRCML_OPTION_OPERATOR          = 1<<3
# Markups operator in special namespace
SRCML_OPTION_DEBUG             = 1<<4
# Include line/column position attributes
SRCML_OPTION_POSITION          = 1<<5
# Markup preprocessor elements (default for C, C++)
SRCML_OPTION_CPP               = 1<<6
# Markup preprocessor elements (default for C, C++, C#)
SRCML_OPTION_CPP_NOMACRO       = 1<<7
# Do not issue an XML declaration
SRCML_OPTION_XML_DECL       = 1<<8
# Do not include any XML namespace declarations
SRCML_OPTION_NAMESPACE_DECL = 1<<9
# Leave as text preprocessor else parts (default: markup)
SRCML_OPTION_CPP_TEXT_ELSE     = 1<<10
# Markup preprocessor @code #if 0 @endcode sections (default: leave as text)
SRCML_OPTION_CPP_MARKUP_IF0    = 1<<11
# Apply transformations to the entire srcML file (default: each unit
SRCML_OPTION_APPLY_ROOT        = 1<<12
# Compress the output file
SRCML_OPTION_COMPRESS          = 1<<13
# Return status indicating no errors
SRCML_STATUS_OK    = 0
# Return status indicating errors occurred
SRCML_STATUS_ERROR = 1
