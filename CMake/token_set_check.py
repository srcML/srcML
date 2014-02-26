#!/usr/bin/python

##
# @file token_set_check.py
#
# @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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

import sys

token_sets = ['_tokenSet_1_data_', '_tokenSet_13_data_', '_tokenSet_22_data_', '_tokenSet_26_data_']
token_sets_data = [

set(['DO', 'RETURN', 'DEFAULT', 'UNSAFE', 'WHILE', 'EXTERN', 'CASE', 'SWITCH', 'CATCH', 'IMPORT', 'ASM', 'TYPEDEF', 'CHECKED', 'GOTO', 'FOR', 'PACKAGE', 'LOCK', 'NAMESPACE', 'YIELD', 'ELSE', 'TRY', 'FINALLY', 'FOREACH', 'FIXED', 'IF', 'UNCHECKED', 'BREAK', 'CONTINUE', 'TEMPLATE', 'USING', 'THROW', 'ASSERT', 'MACRO_CASE', 'FOREVER']),

 set(['ELIF', 'GROUP', 'JOIN', 'REGION', 'LINE', 'FINAL', 'SELECT', 'SET', 'GET', 'ASCENDING', 'OVERRIDE', 'BY', 'DEFINE', 'ORDERBY', 'UNDEF', 'CHECKED', 'INTO', 'EQUALS', 'YIELD', 'ADD', 'DESCENDING', 'PRAGMA', 'ENDIF', 'ASYNC', 'INCLUDE', 'WHERE', 'NAME', 'ON', 'FROM', 'ERRORPREC', 'ENDREGION', 'THIS', 'SIGNAL', 'REMOVE', 'LET', 'IFDEF', 'IFNDEF', 'SUPER', 'UNCHECKED', 'VOID']),

 set(['VOLATILE', 'LBRACKET', 'PROTECTED', 'LINE', 'BY', 'DEFINE', 'CHECKED', 'ENUM', 'ENDIF', 'WHERE', 'ON', 'PARTIAL', 'ENDREGION', 'THIS', 'REGION', 'THREADLOCAL', 'TRANSIENT', 'MAIN', 'GROUP', 'SYNCHRONIZED', 'UNSAFE', 'STATIC', 'MUTABLE', 'DCOLON', 'FINAL', 'SELECT', 'GET', 'EXPLICIT', 'READONLY', 'LET', 'ORDERBY', 'RVALUEREF', 'UNDEF', 'UNION', 'EQUALS', 'VIRTUAL', 'OPERATOR', 'INCLUDE', 'ERRORPREC', 'DOTDOTDOT', 'REMOVE', 'PUBLIC', 'DELEGATE', 'IFNDEF', 'UNCHECKED', 'LPAREN', 'DEFAULT', 'DESTOP', 'EXTERN', 'NEW', 'REF', 'NATIVE', 'SET', 'ASCENDING', 'STRICTFP', 'FRIEND', 'OUT', 'ADD', 'DESCENDING', 'ASYNC', 'JOIN', 'NAME', 'QMARK', 'INTERNAL', 'PARAMS', 'INLINE', 'EVENT', 'ELIF', 'CONST', 'ABSTRACT', 'REFOPS', 'MULTOPS', 'PRIVATE', 'OVERRIDE', 'ATSIGN', 'CLASS', 'INTO', 'YIELD', 'TEMPOPS', 'PRAGMA', 'IN', 'IMPLICIT', 'FROM', 'STRUCT', 'SIGNAL', 'CONSTEXPR', 'SEALED', 'IFDEF', 'SUPER', 'RESTRICT', 'ALIGNAS', 'VOID', 'DECLTYPE', 'TYPENAME', 'MACRO_TYPE_NAME', 'MACRO_SPECIFIER']),

set(["EXTERN", "RESTRICT", "CONSTEXPR", "THREADLOCAL", "ALIGNAS", "INLINE", "MACRO_SPECIFIER",
     "PUBLIC", "PRIVATE", "PROTECTED", "VIRTUAL", "FRIEND", "EXPLICIT", "NEW", "STATIC", "CONST", "MUTABLE",
     "VOLATILE", "TRANSIENT", "FINAL", "ABSTRACT", "SYNCHRONIZED", "NATIVE", "STRICTFP", "REF", "OUT",
     "IN", "INTERNAL", "SEALED", "OVERRIDE", "IMPLICIT", "DELEGATE", "UNSAFE", "READONLY", "PARTIAL",
     "EVENT", "ASYNC", "PARAMS"])]

if len(sys.argv) < 2 :
    print "token_set_check.py file"
    exit()

filename=sys.argv[1]

for set_pos in range(len(token_sets)) :

    file = open(filename, "r")
    lines = file.read().split("\n")
    file.close()

    start_line = 0
    for i in range(len(lines)) :
        start_line = i
        if lines[i].find(token_sets[set_pos]) != -1 :
            break

    tokens_in_set = set()
    data_line = start_line + 1
    while lines[data_line][0] == '/' :
        tokens_in_set.update(lines[data_line][2:].split())
        data_line = data_line + 1

    if len(token_sets_data[set_pos] - tokens_in_set) == 0 and len(tokens_in_set - token_sets_data[set_pos]) == 0:
        print token_sets[set_pos] + " is valid."
    else :
        print "Fix " + token_sets[set_pos] + "."
        print (token_sets_data[set_pos] - tokens_in_set) | (tokens_in_set - token_sets_data[set_pos])
