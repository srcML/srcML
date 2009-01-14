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

#define ELEMENT_MAP_CALL_NAME element_process
#define ELEMENT_MAP_FIRST_TYPE int
#define ELEMENT_MAP_SECOND_TYPE srcMLOutput::PROCESS_PTR
#define ELEMENT_MAP_DEFAULT(s) template <ELEMENT_MAP_FIRST_TYPE n> inline ELEMENT_MAP_SECOND_TYPE \
  ELEMENT_MAP_CALL_NAME() { s }

#define ELEMENT_MAP_CALL(t) ELEMENT_MAP_CALL_NAME <srcMLParserTokenTypes::t>()
#define ELEMENT_MAP(t, s) template <> inline ELEMENT_MAP_SECOND_TYPE ELEMENT_MAP_CALL(t) { return s; }

// map the token types to specific strings
namespace {

  // base member
  ELEMENT_MAP_DEFAULT(return &srcMLOutput::processToken;)

  ELEMENT_MAP(SUNIT, &srcMLOutput::processUnit)
  ELEMENT_MAP(SSINGLE, &srcMLOutput::processText)
  ELEMENT_MAP(START_ELEMENT_TOKEN, &srcMLOutput::processText)
  ELEMENT_MAP(COMMENT_START, &srcMLOutput::processBlockCommentStart)
  ELEMENT_MAP(COMMENT_END, &srcMLOutput::processEndBlockToken)

  ELEMENT_MAP(LINECOMMENT_START, &srcMLOutput::processLineCommentStart)
  ELEMENT_MAP(LINECOMMENT_END, &srcMLOutput::processEndLineToken)

#if DEBUG
  ELEMENT_MAP(SMARKER, &srcMLOutput::processMarker)
#endif
  ELEMENT_MAP(SPUBLIC_ACCESS_DEFAULT, &srcMLOutput::processAccess)
  ELEMENT_MAP(SPRIVATE_ACCESS_DEFAULT, &srcMLOutput::processAccess)
  ELEMENT_MAP(SSTRING, &srcMLOutput::processString)
  ELEMENT_MAP(SCHAR, &srcMLOutput::processChar)
  ELEMENT_MAP(SLITERAL, &srcMLOutput::processLiteral)
  ELEMENT_MAP(SBOOLEAN, &srcMLOutput::processBoolean)
  ELEMENT_MAP(SINTERFACE, &srcMLOutput::processInterface)
  ELEMENT_MAP(CONTROL_CHAR, &srcMLOutput::processEscape)
};

#undef ELEMENT_MAP_CALL_NAME
#undef ELEMENT_MAP_FIRST_TYPE
#undef ELEMENT_MAP_SECOND_TYPE
#undef ELEMENT_MAP_DEFAULT
#undef ELEMENT_MAP_CALL
#undef ELEMENT_MAP

srcMLOutput::PROCESS_PTR srcMLOutput::process_table[] = {

  // fill the array with the prefixes
  #define BOOST_PP_LOCAL_MACRO(n) &srcMLOutput::processText,
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
