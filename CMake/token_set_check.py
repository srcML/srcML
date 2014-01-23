#!/usr/bin/python

import sys

token_sets = ['_tokenSet_1_data_', '_tokenSet_13_data_', '_tokenSet_23_data_']
token_sets_data = [

set(['DO', 'RETURN', 'DEFAULT', 'UNSAFE', 'WHILE', 'EXTERN', 'CASE', 'SWITCH', 'CATCH', 'IMPORT', 'ASM', 'TYPEDEF', 'CHECKED', 'GOTO', 'FOR', 'PACKAGE', 'LOCK', 'NAMESPACE', 'YIELD', 'ELSE', 'TRY', 'FINALLY', 'FOREACH', 'FIXED', 'IF', 'UNCHECKED', 'BREAK', 'CONTINUE', 'TEMPLATE', 'USING', 'THROW']),

 set(['ELIF', 'GROUP', 'JOIN', 'REGION', 'LINE', 'FINAL', 'SELECT', 'SET', 'GET', 'ASCENDING', 'OVERRIDE', 'BY', 'DEFINE', 'ORDERBY', 'UNDEF', 'CHECKED', 'INTO', 'EQUALS', 'YIELD', 'ADD', 'DESCENDING', 'PRAGMA', 'ENDIF', 'ASYNC', 'INCLUDE', 'WHERE', 'NAME', 'ON', 'FROM', 'ERRORPREC', 'ENDREGION', 'THIS', 'SIGNAL', 'REMOVE', 'LET', 'IFDEF', 'IFNDEF', 'SUPER', 'UNCHECKED', 'VOID']),

 set(['VOLATILE', 'LBRACKET', 'PROTECTED', 'LINE', 'BY', 'DEFINE', 'CHECKED', 'ENUM', 'ENDIF', 'WHERE', 'ON', 'PARTIAL', 'ENDREGION', 'THIS', 'REGION', 'THREADLOCAL', 'TRANSIENT', 'MAIN', 'GROUP', 'SYNCHRONIZED', 'UNSAFE', 'STATIC', 'MUTABLE', 'DCOLON', 'FINAL', 'SELECT', 'GET', 'EXPLICIT', 'READONLY', 'LET', 'ORDERBY', 'RVALUEREF', 'UNDEF', 'UNION', 'EQUALS', 'VIRTUAL', 'OPERATOR', 'INCLUDE', 'ERRORPREC', 'DOTDOTDOT', 'REMOVE', 'PUBLIC', 'DELEGATE', 'IFNDEF', 'UNCHECKED', 'LPAREN', 'DEFAULT', 'DESTOP', 'EXTERN', 'NEW', 'REF', 'NATIVE', 'SET', 'ASCENDING', 'STRICTFP', 'FRIEND', 'OUT', 'ADD', 'DESCENDING', 'ASYNC', 'JOIN', 'NAME', 'QMARK', 'INTERNAL', 'PARAMS', 'INLINE', 'EVENT', 'ELIF', 'CONST', 'ABSTRACT', 'REFOPS', 'MULTOPS', 'PRIVATE', 'OVERRIDE', 'ATSIGN', 'CLASS', 'INTO', 'YIELD', 'TEMPOPS', 'PRAGMA', 'IN', 'IMPLICIT', 'FROM', 'STRUCT', 'SIGNAL', 'CONSTEXPR', 'SEALED', 'IFDEF', 'SUPER', 'RESTRICT', 'ALIGNAS', 'VOID', 'DECLTYPE', 'TYPENAME'])]

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
