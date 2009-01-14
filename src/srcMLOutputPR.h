/*
  srcMLOutputPR.h

  Copyright (C) 2005, 2006  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Declaration of process pointer table for srcMLOutput.
*/

enum {
  PROCESSTOKEN,
  PROCESSUNIT,
  PROCESSTEXT,
  PROCESSBLOCKCOMMENTSTART,
  PROCESSENDBLOCKTOKEN,
  PROCESSLINECOMMENTSTART,
  PROCESSENDLINETOKEN,
  //  PROCESSMARKER,
  PROCESSACCESS,
  PROCESSSTRING,
  PROCESSCHAR,
  PROCESSLITERAL,
  PROCESSBOOLEAN,
  PROCESSINTERFACE,
  PROCESSESCAPE,
};


srcMLOutput::PROCESS_PTR num2process[] = {
  &srcMLOutput::processToken,
  &srcMLOutput::processUnit,
  &srcMLOutput::processText,
  &srcMLOutput::processBlockCommentStart,
  &srcMLOutput::processEndBlockToken,
  &srcMLOutput::processLineCommentStart,
  &srcMLOutput::processEndLineToken,
  //  &srcMLOutput::processMarker,
  &srcMLOutput::processAccess,
  &srcMLOutput::processString,
  &srcMLOutput::processChar,
  &srcMLOutput::processLiteral,
  &srcMLOutput::processBoolean,
  &srcMLOutput::processInterface,
  &srcMLOutput::processEscape,
};


#define ELEMENT_MAP_CALL_NAME element_process
#define ELEMENT_MAP_FIRST_TYPE int
#define ELEMENT_MAP_SECOND_TYPE int
#define ELEMENT_MAP_DEFAULT(s) template <ELEMENT_MAP_FIRST_TYPE n> inline ELEMENT_MAP_SECOND_TYPE \
  ELEMENT_MAP_CALL_NAME() { s }

#define ELEMENT_MAP_CALL(t) ELEMENT_MAP_CALL_NAME <srcMLParserTokenTypes::t>()
#define ELEMENT_MAP(t, s) template <> inline ELEMENT_MAP_SECOND_TYPE ELEMENT_MAP_CALL(t) { return s; }

// map the token types to specific strings
namespace {

  // base member
  ELEMENT_MAP_DEFAULT(return PROCESSTOKEN;)

  ELEMENT_MAP(SUNIT, PROCESSUNIT)
  ELEMENT_MAP(SSINGLE, PROCESSTEXT)
  ELEMENT_MAP(START_ELEMENT_TOKEN, PROCESSTEXT)
  ELEMENT_MAP(COMMENT_START, PROCESSBLOCKCOMMENTSTART)
  ELEMENT_MAP(COMMENT_END, PROCESSENDBLOCKTOKEN)

  ELEMENT_MAP(LINECOMMENT_START, PROCESSLINECOMMENTSTART)
  ELEMENT_MAP(LINECOMMENT_END, PROCESSENDLINETOKEN)

#if DEBUG
  ELEMENT_MAP(SMARKER, PROCESSMARKER)
#endif
  ELEMENT_MAP(SPUBLIC_ACCESS_DEFAULT, PROCESSACCESS)
  ELEMENT_MAP(SPRIVATE_ACCESS_DEFAULT, PROCESSACCESS)
  ELEMENT_MAP(SSTRING, PROCESSSTRING)
  ELEMENT_MAP(SCHAR, PROCESSCHAR)
  ELEMENT_MAP(SLITERAL, PROCESSLITERAL)
  ELEMENT_MAP(SBOOLEAN, PROCESSBOOLEAN)
  ELEMENT_MAP(SINTERFACE, PROCESSINTERFACE)
  ELEMENT_MAP(CONTROL_CHAR, PROCESSESCAPE)
};

#undef ELEMENT_MAP_CALL_NAME
#undef ELEMENT_MAP_FIRST_TYPE
#undef ELEMENT_MAP_SECOND_TYPE
#undef ELEMENT_MAP_DEFAULT
#undef ELEMENT_MAP_CALL
#undef ELEMENT_MAP

char srcMLOutput::process_table[] = {

  // fill the array with the prefixes
  #define BOOST_PP_LOCAL_MACRO(n) PROCESSTEXT,
  #define BOOST_PP_LOCAL_LIMITS     (0, 107)
  #include BOOST_PP_LOCAL_ITERATE()
  #undef BOOST_PP_LOCAL_MACRO
  #undef BOOST_PP_LOCAL_LIMITS

  // fill the array with the prefixes
  #define BOOST_PP_LOCAL_MACRO(n) element_process<n>(),
  #define BOOST_PP_LOCAL_LIMITS     (108, 218)
  #include BOOST_PP_LOCAL_ITERATE()
  #undef BOOST_PP_LOCAL_MACRO
  #undef BOOST_PP_LOCAL_LIMITS
};
