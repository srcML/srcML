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
  ELEMENT_MAP_DEFAULT(return &srcMLOutput::processText;)

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

  ELEMENT_MAP(SNOP, &srcMLOutput::processToken)
  ELEMENT_MAP(SOPERATOR, &srcMLOutput::processToken)
  ELEMENT_MAP(SMODIFIER, &srcMLOutput::processToken)
  ELEMENT_MAP(SNAME, &srcMLOutput::processToken)
  ELEMENT_MAP(SONAME, &srcMLOutput::processToken)
  ELEMENT_MAP(SCNAME, &srcMLOutput::processToken)
  ELEMENT_MAP(STYPE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCONDITION, &srcMLOutput::processToken)
  ELEMENT_MAP(SBLOCK, &srcMLOutput::processToken)
  ELEMENT_MAP(SINDEX, &srcMLOutput::processToken)
  ELEMENT_MAP(STYPEDEF, &srcMLOutput::processToken)
  ELEMENT_MAP(SENUM, &srcMLOutput::processToken)
  ELEMENT_MAP(SASM, &srcMLOutput::processToken)
  ELEMENT_MAP(SMACRO_CALL, &srcMLOutput::processToken)
  ELEMENT_MAP(SIF_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(STHEN, &srcMLOutput::processToken)
  ELEMENT_MAP(SELSE, &srcMLOutput::processToken)
  ELEMENT_MAP(SWHILE_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SDO_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SFOR_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SFOR_GROUP, &srcMLOutput::processToken)
  ELEMENT_MAP(SFOR_INITIALIZATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SFOR_CONDITION, &srcMLOutput::processToken)
  ELEMENT_MAP(SFOR_INCREMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SEXPRESSION_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SEXPRESSION, &srcMLOutput::processToken)
  ELEMENT_MAP(SFUNCTION_CALL, &srcMLOutput::processToken)
  ELEMENT_MAP(SDECLARATION_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SDECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SDECLARATION_INITIALIZATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SGOTO_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SCONTINUE_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SBREAK_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SLABEL_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SSWITCH, &srcMLOutput::processToken)
  ELEMENT_MAP(SCASE, &srcMLOutput::processToken)
  ELEMENT_MAP(SDEFAULT, &srcMLOutput::processToken)
  ELEMENT_MAP(SFUNCTION_DEFINITION, &srcMLOutput::processToken)
  ELEMENT_MAP(SFUNCTION_DECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SFUNCTION_SPECIFIER, &srcMLOutput::processToken)
  ELEMENT_MAP(SRETURN_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SPARAMETER_LIST, &srcMLOutput::processToken)
  ELEMENT_MAP(SPARAMETER, &srcMLOutput::processToken)
  ELEMENT_MAP(SARGUMENT_LIST, &srcMLOutput::processToken)
  ELEMENT_MAP(SARGUMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(SCLASS, &srcMLOutput::processToken)
  ELEMENT_MAP(SCLASS_DECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SSTRUCT, &srcMLOutput::processToken)
  ELEMENT_MAP(SSTRUCT_DECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SUNION, &srcMLOutput::processToken)
  ELEMENT_MAP(SUNION_DECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SDERIVATION_LIST, &srcMLOutput::processToken)
  ELEMENT_MAP(SPUBLIC_ACCESS, &srcMLOutput::processToken)
  ELEMENT_MAP(SPRIVATE_ACCESS, &srcMLOutput::processToken)
  ELEMENT_MAP(SPROTECTED_ACCESS, &srcMLOutput::processToken)
  ELEMENT_MAP(SMEMBER_INITIALIZATION_LIST, &srcMLOutput::processToken)
  ELEMENT_MAP(SCONSTRUCTOR_DEFINITION, &srcMLOutput::processToken)
  ELEMENT_MAP(SCONSTRUCTOR_DECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SDESTRUCTOR_DEFINITION, &srcMLOutput::processToken)
  ELEMENT_MAP(SDESTRUCTOR_DECLARATION, &srcMLOutput::processToken)
  ELEMENT_MAP(SFRIEND, &srcMLOutput::processToken)
  ELEMENT_MAP(SCLASS_SPECIFIER, &srcMLOutput::processToken)
  ELEMENT_MAP(SEXTERN, &srcMLOutput::processToken)
  ELEMENT_MAP(SNAMESPACE, &srcMLOutput::processToken)
  ELEMENT_MAP(SUSING_DIRECTIVE, &srcMLOutput::processToken)
  ELEMENT_MAP(STRY_BLOCK, &srcMLOutput::processToken)
  ELEMENT_MAP(SCATCH_BLOCK, &srcMLOutput::processToken)
  ELEMENT_MAP(STHROW_STATEMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(STHROW_SPECIFIER, &srcMLOutput::processToken)
  ELEMENT_MAP(STHROW_SPECIFIER_JAVA, &srcMLOutput::processToken)
  ELEMENT_MAP(STEMPLATE, &srcMLOutput::processToken)
  ELEMENT_MAP(STEMPLATE_ARGUMENT, &srcMLOutput::processToken)
  ELEMENT_MAP(STEMPLATE_ARGUMENT_LIST, &srcMLOutput::processToken)
  ELEMENT_MAP(STEMPLATE_PARAMETER, &srcMLOutput::processToken)
  ELEMENT_MAP(STEMPLATE_PARAMETER_LIST, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_DIRECTIVE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_FILENAME, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_ERROR, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_PRAGMA, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_INCLUDE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_DEFINE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_UNDEF, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_LINE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_IF, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_IFDEF, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_IFNDEF, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_THEN, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_ELSE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_ELIF, &srcMLOutput::processToken)
  ELEMENT_MAP(SCPP_ENDIF, &srcMLOutput::processToken)
  ELEMENT_MAP(SMARKER, &srcMLOutput::processToken)
  ELEMENT_MAP(SERROR_PARSE, &srcMLOutput::processToken)
  ELEMENT_MAP(SERROR_MODE, &srcMLOutput::processToken)
  ELEMENT_MAP(SIMPLEMENTS, &srcMLOutput::processToken)
  ELEMENT_MAP(SEXTENDS, &srcMLOutput::processToken)
  ELEMENT_MAP(SIMPORT, &srcMLOutput::processToken)
  ELEMENT_MAP(SPACKAGE, &srcMLOutput::processToken)
  ELEMENT_MAP(SCONCEPT, &srcMLOutput::processToken)
  ELEMENT_MAP(SCONCEPTMAP, &srcMLOutput::processToken)
  ELEMENT_MAP(SAUTO, &srcMLOutput::processToken)
  ELEMENT_MAP(SEMPTY, &srcMLOutput::processToken)

};

#undef ELEMENT_MAP_CALL_NAME
#undef ELEMENT_MAP_FIRST_TYPE
#undef ELEMENT_MAP_SECOND_TYPE
#undef ELEMENT_MAP_DEFAULT
#undef ELEMENT_MAP_CALL
#undef ELEMENT_MAP

srcMLOutput::PROCESS_PTR srcMLOutput::process_table[] = {

  // fill the array with the prefixes
  #define BOOST_PP_LOCAL_MACRO(n) element_process<n>(),
  #define BOOST_PP_LOCAL_LIMITS     (0, 218)
  #include BOOST_PP_LOCAL_ITERATE()
  #undef BOOST_PP_LOCAL_MACRO
  #undef BOOST_PP_LOCAL_LIMITS
};
