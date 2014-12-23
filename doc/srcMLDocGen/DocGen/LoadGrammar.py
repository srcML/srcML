
import os, sys, re, lxml, cStringIO, itertools
import lxml.etree as ET
from lxml import *
from xml.sax.handler import ContentHandler
from LoadData import *
from LoadGrammarHowTo import *


def getComparableTagName(item):
    return "{0}{1}".format("" if item.ns == "" else (item.ns +":"), item.tag)


# -----------------------------------------------------------------
#                    GrammarDoc Class
# -----------------------------------------------------------------
# Class that holds everything relating to the grammar
class GrammarDoc:
    def __init__(self):
        self.languages = []
        self.rules = []
        self.documentation = [] # used to determine the ordering of the documentation
        self.edgeList = []
        self.howToReadDoc = None
        self.compiledGrammar = None

    def dumpDocumentation(self, out):
        for doc in self.documentation:
            doc.dump(out)
            out.write("\n")

        out.write("\n\n\n Graph Documentation\n\n\n")
        for e in self.edgeList:
            out.write("""
Edge
    From: {0.fromTitle}
    To: {0.toTitle}
    From Obj: {0.fromObj} 
    to Obj: {0.toObj} 
""".format(e, e.toObj))

    def toEBNFText(self, out):
        for rule in self.rules:
            rule.toEBNFText(out)
            out.write("\n\n")

    def assertValid(self, treeOrNodeToValidate):
        validationInfo = ValidationInfo(self, treeOrNodeToValidate)
        if not validationInfo.startValidation():
            validationInfo.dump()
            raise Exception("Validation Failed!")

# -----------------------------------------------------------------
#                           Graph Stuff 
# -----------------------------------------------------------------
class RuleEdge:
    def __init__(self):
        self.fromTitle = ""
        self.fromObj = None
        self.toTitle = ""
        self.toObj = None
        self.ruleRef = None


# -----------------------------------------------------------------
#           Documentation Classes
# -----------------------------------------------------------------

# Documentation Structure
class DocumentationBase(object):
    def __init__(self):
        self.desc = ""
        self.typename = self.__class__.__name__
        self.parentRules = []
        self.childRules = []
        self.parentTags = []
        self.childTags = []
        self.otherLinks = []
        self.languages = []

    def dumpDocumentationBaseFields(self, out):
        out.write("    desc: \"{0}\"\n".format(self.desc.strip()))
        out.write("    parentRules: ")
        if len(self.parentRules) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for parentRule in self.parentRules:
                out.write("        {0}\n".format(parentRule.name))

        out.write("    childRules: ")
        if len(self.childRules) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for chldRule in self.childRules:
                out.write("        {0}\n".format(chldRule.name))

        out.write("    parentTags: ")
        if len(self.parentTags) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for parentTag in self.parentTags:
                out.write("        {0}\n".format(parentTag))

        out.write("    childTags: ")
        if len(self.childTags) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for childTag in self.childTags:
                out.write("        {0}\n".format(childTag))

        out.write("    OtherLinks: ")
        if len(self.otherLinks) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for lnk in self.otherLinks:
                out.write("        Title: {0.title} URL: {0.url}\n".format(lnk))

        out.write("    Languages: {0}\n".format(", ".join(self.languages)if len(self.languages) else "Not Set" ))

class RuleDocumentation(DocumentationBase):
    def __init__(self):
        DocumentationBase.__init__(self)
        self.rule = None
        self.name = ""

    def dump(self, out):
        out.write("{0}\n".format(self.__class__.__name__))
        out.write("    name: {0}\n".format(self.name))
        out.write("    rule: {0}\n".format(self.rule))
        self.dumpDocumentationBaseFields(out)



class TagDocumentation(DocumentationBase):
    def __init__(self):
        DocumentationBase.__init__(self)
        self.ns = ""
        self.tag = ""
        self.tagRules = []
        self.attributes = []

    def dump(self, out):
        out.write("{0}\n".format(self.__class__.__name__))
        out.write("    {0}\n".format(getComparableTagName(self)))
        out.write("    TagRules: ")
        if len(self.tagRules) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for tagRule in self.tagRules:
                out.write("        Name: {0} Title: {1}\n".format(tagRule.name, tagRule.title))
                out.write("            Desc: \"{0}\"\n".format(tagRule.desc.strip()))
                out.write("            RuleRefs: ")
                if len(tagRule.ruleRefs) == 0:
                    out.write("None\n")
                else:
                    out.write("\n")
                    for ref in tagRule.ruleRefs:
                        out.write("                {0}\n".format(ref.name))
                        if ref.args != None:
                            for arg in ref.args:
                                out.write("                    ")
                                arg.toEBNFText(out)
                                out.write("\n")
        out.write("    attributes: ")
        if len(self.attributes) == 0:
            out.write("None\n")
        else:
            out.write("\n")
            for attr in self.attributes:
                out.write("        {0}\n".format(attr))
        self.dumpDocumentationBaseFields(out)

class AttrRuleDocumentation(DocumentationBase):
    def __init__(self):
        DocumentationBase.__init__(self)
        self.name = ""
        self.attrRule = None
        self.parentTags = []

    def dump(self, out):
        out.write("{0}\n".format(self.__class__.__name__))
        out.write("    {0}\n".format(self.name))
        out.write("    AttrRule: ")
        if self.attrRule == None:
            out.write("None\n")
        else:
            out.write("\n")
            out.write("        Name: {0} Title: {1} AttrName: {2} \n".format(self.attrRule.name, self.attrRule.title, self.attrRule.attrName))
            out.write("            Desc: \"{0}\"\n".format(self.attrRule.desc.strip()))
        self.dumpDocumentationBaseFields(out)

# ------------------------------------------------------------------
#                   Validation Helper classes
# ------------------------------------------------------------------
class ValidationInfo:
    def __init__(self, grammarInput, nodeToValidate):
        self.ruleStack = []
        self.grammarDefinitionStack = []
        self.parameterLookUpStack = []
        self.ruleLookUp = dict([(x.name,x) for x in grammarInput.rules])
        self.initialRule = grammarInput.rules[0]
        self.elementStack = [nodeToValidate if isinstance(nodeToValidate, ET._Element) else nodeToValidate.getroot()]
        self.grammar = grammarInput
        self.validationLocation = self.elementStack[0]
        self.independentRules = [x for x in grammarInput.rules if x.canOccurAnyWhere]
        self.grammarLog = []

    def dump(self):
        for entry in self.grammarLog:
            print entry

    def startValidation(self):
        return self.initialRule.validate(self)

    def currentRule(self):
        return self.ruleStack[-1]

    def getParameter(self, paramName):
        return self.parameterLookUpStack[-1][param]

    def pushAndMapParams(self, ruleName, parameterValueList):
        self.parameterLookUpStack.apped(dict(map(lambda p,v:(p,v), self.ruleLookUp[ruleName].parameters, parameterValueList)))

    def pushRule(self, rule):
        self.ruleStack.append(rule)
        self.grammarLog.append("Entered: " + rule.name)

    def popRuleStack(self):
        if self.ruleStack[-1].parameters != None:
            self.parameterLookUpStack.pop()
        temp = self.ruleStack.pop()
        self.grammarLog.append("Exited: " + temp.name)

# ------------------------------------------------------------------
#                   Rule Classes
# ------------------------------------------------------------------
class RuleBase(object):
    def __init__(self):
        self.name = ""
        self.parameters = None
        self.grammar = None
        self.languages = None
        self.desc = ""
        self.title = ""
        self.ruleRefs = []
        self.canOccurAnyWhere = False

    def toEBNFText(self, out):
        out.write(self.name)
        self.toEBNFRuleStartText(out)
        if self.parameters != None:
            out.write(" [")
            out.write(", ".join(self.parameters))
            out.write("]")
        out.write(" := ")
        for grmmr in self.grammar:
            grmmr.toEBNFText(out)
            out.write(" ")
        out.write("\n    ;")

    def toEBNFRuleStartText(self, out):
        assert False, "Rule didn't implement toEBNFRuleStartText"

    def ruleValidation(self, validationUnfo):
        assert False, "ruleValidation Not implemented by rule"

    def validate(self, validationInfo):
        validationInfo.pushRule(self)
        if self.ruleValidation(validationInfo):
            for expr in self.grammar:
                if not expr.validate(validationInfo):
                    validationInfo.popRuleStack()
                    return False
            validationInfo.popRuleStack()
            return True
        else:
            validationInfo.popRuleStack()
            return False




class Rule(RuleBase):
    def __init__(self):
        RuleBase.__init__(self)

    def toEBNFRuleStartText(self, out):
        pass        

    def ruleValidation(self, validationUnfo):
        return True


class TagRule(RuleBase):
    def __init__(self):
        RuleBase.__init__(self)
        self.tagInfo = None

    def toEBNFRuleStartText(self, out):
        out.write(" <{0}{1}>".format("" if self.tagInfo.ns == "" else (self.tagInfo.ns + ":"), self.tagInfo.tag))
        if self.tagInfo.attrs != None:
            out.write( " ({0})".format(", ".join([ x.name + ("?" if x.isOptional else "") for x in self.tagInfo.attrs])) )

    def ruleValidation(self, validationInfo):
        return tagInfo.validate(validationInfo)

class AttrRule(RuleBase):
    def __init__(self):
        RuleBase.__init__(self)
        self.attrName = ""

    def toEBNFRuleStartText(self, out):
        out.write(" @{0}".format(self.attrName))

    def validate(self, validationInfo):
        # if self.attrName in validationInfo.currentElement:
        # else:
        return False


# ------------------------------------------------------------------
#                   Grammar Definition classes
# ------------------------------------------------------------------
# Rule related classes
class RuleTagInfo:
    def __init__(self):
        self.tag = ""
        self.ns = ""
        self.attrs = None

    def validate(self, validationInfo):
        return False

class AttrUseInfo:
    def __init__(self):
        self.name = ""
        self.isOptional = False

    def validate(self, validationInfo):
        return False


# Grammar Definition Related Classes
class GrammarDef(object):
    def __init__(self, objTypeTitle):
        self.typename = objTypeTitle

    def toEBNFText(self, out):
        assert False, "Part of grammar didn't implement toEBNFText"

    def validate(self, validationInfo):
        validationInfo.grammarDefinitionStack.append(self)
        ret = self.testGrammar(validationInfo)
        validationInfo.grammarDefinitionStack.pop()

    def testGrammar(self, validationInfo):
        assert False, "Sub class doesn't implement textGrammar"


class ParamUse(GrammarDef):
    def __init__(self, paramName = ""):
        GrammarDef.__init__(self, "ParamUse")
        self.name = paramName

    def toEBNFText(self, out):
        out.write("${0}".format(self.name))

    def testGrammar(self, validationInfo):
        return False

class RuleRef(GrammarDef):
    def __init__(self, refName = ""):
        GrammarDef.__init__(self, "RuleRef")
        self.name = refName
        self.args = None

    def toEBNFText(self, out):
        out.write("{0}".format(self.name))
        if self.args != None:
            out.write("[")
            for arg in self.args:
                if isinstance(arg, list):
                    for grmmr in arg:
                        grmmr.toEBNFText(out)
                else:
                    arg.toEBNFText(out)

                if arg != self.args[-1]:
                    out.write(", ")
            out.write("]")

    def testGrammar(self, validationInfo):
        return False

class Literal(GrammarDef):
    def __init__(self, litValue = ""):
        GrammarDef.__init__(self, "Literal")
        self.value = litValue

    def toEBNFText(self, out):
        out.write("\"{0}\"".format(self.value))

    def testGrammar(self, validationInfo):
        return False

class Text(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Text")

    def toEBNFText(self, out):
        out.write("Text")

    def testGrammar(self, validationInfo):
        return False

class Number(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Number")

    def toEBNFText(self, out):
        out.write("Number")

    def testGrammar(self, validationInfo):
        return False

class Empty(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Number")

    def toEBNFText(self, out):
        out.write("Number")

    def testGrammar(self, validationInfo):
        return False

class Identifier(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Identifier")

    def toEBNFText(self, out):
        out.write("Identifier")

    def testGrammar(self, validationInfo):
        return False

class Operator(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Operator")

    def toEBNFText(self, out):
        out.write("Operator")

    def testGrammar(self, validationInfo):
        return False

class NotImplementedYet(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "NotImplementedYet")

    def toEBNFText(self, out):
        out.write("NotImplementedYet")

    def testGrammar(self, validationInfo):
        return False


# ------------------------------------------------------------------
#         Complex Grammar Expression Definition classes
# ------------------------------------------------------------------
# Class that's used to represent other parts of the language
# that have a many to one relationship (i.e. groupings of things)
class GrammarExpr(GrammarDef):
    def __init__(self, typeNameTitle):
        GrammarDef.__init__(self, typeNameTitle)
        self.subExprs = []

    def EBNFSeparator(self):
        assert False, "GrammarExpr didn't implement EBNFSeparator"

    def EBNFSuffix(self):
        assert False, "GrammarExpr didn't implement EBNFSuffix"

    def toEBNFText(self, out):
        if len(self.subExprs) == 0:
            raise Exception("Invalid # of subelements within {0}".format(self.__class__.__name__))
        elif len(self.subExprs) == 1:
            self.subExprs[0].toEBNFText(out)
            out.write(self.EBNFSuffix())
        else:
            out.write("(")
            for expr in self.subExprs:
                expr.toEBNFText(out)
                if expr != self.subExprs[-1]:
                    out.write("{0}".format(self.EBNFSeparator()))
            out.write(")")
            out.write(self.EBNFSuffix())


class TagExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__( self, "TagExpr")
        self.tagInfo = None

    def toEBNFText(self, out):
        out.write(" <{0}{1}>".format("" if self.tagInfo.ns == "" else (self.tagInfo.ns + ":"), self.tagInfo.tag))
        if self.tagInfo.attrs != None:
            out.write( " ({0})".format(", ".join([ x.name + ("?" if x.isOptional else "") for x in self.tagInfo.attrs])) )
        out.write("{ ")
        for expr in self.subExprs:
            expr.toEBNFText(out)
            out.write(" ")
        out.write("}")

    def testGrammar(self, validationInfo):
        return False


class OptionalExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__( self, "OptionalExpr")

    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return "?"

    def testGrammar(self, validationInfo):
        return False


class ZeroOrMoreExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "ZeroOrMoreExpr")
        
    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return "*"

    def testGrammar(self, validationInfo):
        return False

class GroupExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "GroupExpr")
        
    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return ""

    def testGrammar(self, validationInfo):
        return False

class OneOrMoreExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "OneOrMoreExpr")
        
    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return "+"

    def testGrammar(self, validationInfo):
        return False

class ChoiceExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "ChoiceExpr")
        
    def EBNFSeparator(self):
        return " | " if len(self.subExprs) <= 5 else "\n    | "

    def EBNFSuffix(self):
        return ""

    def testGrammar(self, validationInfo):
        return False


# Tag Names
GrammarTag = "Grammar"
RuleTag = "Rule"
TagRuleTag = "TagRule"
AttrRuleTag = "AttrRule"
TagTag = "Tag"
AttrsTag = "Attrs"
AttrTag = "Attr"
DefTag = "Def"
ParamsTag = "Params"
ParamTag = "Param"
ArgsTag = "Args"
ArgTag = "Arg"
OptionalTag = "Optional"
ParamUseTag = "ParamUse"
RefTag = "Ref"
LiteralTag = "Literal"
ZeroOrMoreTag = "ZeroOrMore"
OneOrMoreTag = "OneOrMore"
ChoiceTag = "Choice"
TextTag = "Text"
GroupTag = "Group"
DocumentationTag = "Documentation"
RefsTag = "Refs"
SeeTag = "See"
NumberTag = "Number"
EmptyTag = "Empty"
IdentifierTag = "Identifier"
NoImplTag = "NoImpl"
OperatorTag = "Operator"
HowToReadTag = "HowToRead"

# Name Attribute
nameAttr = "name"
nsAttr = "ns"
elemAttr = "elem"
ruleAttr = "rule"
isOptionalAttr = "isOptional"
valueAttr = "value"
langAttr = "lang"
languageOrderingAttr = "languageOrdering"
urlAttr = "url"
canOccurAnyWhereAttr = "canOccurAnyWhere"

def loadGrammar(fileName):
    def buildExpr(element, exprToBuild, currentRule):
        for elem in element.iterchildren():
            exprToBuild.subExprs.append(buildGrammar(elem, currentRule))

        if len(exprToBuild.subExprs) == 0:
            raise Exception("All expressions must have at least one child element " + formatElementErrorMsg(element))
    
    def buildArgsList(argsListElem, currentRule):
        ret = []
        for elem in argsListElem.iterchildren():
            if elem.tag == ArgTag:
                if len(list(elem)) != 1:
                    raise Exception("Invalid number of grammar elements for argument")
                else:
                    for argElem in elem.iterchildren():
                        ret.append(buildGrammar(argElem, currentRule))
            else:
                unexpectedOrUnknownTag(elem)
        if len(ret) == 0:
            return None
        return ret

    def buildRef(refElem, currentRule):
        ruleRef = RuleRef(getAttribOrFail(refElem, nameAttr))
        children = list(refElem)
        childCount = len(children)
        
        # Tracking rule References
        newEdge = RuleEdge()
        newEdge.fromTitle = currentRule.name
        newEdge.fromObj = currentRule
        newEdge.toTitle = ruleRef.name
        newEdge.ruleRef = ruleRef
        currentRule.ruleRefs.append(ruleRef)
        grammarDoc.edgeList.append(newEdge)
        
        if childCount == 0:
            return ruleRef
        elif childCount == 1:
            if children[0].tag != ArgsTag:
                unexpectedOrUnknownTag(children[0])
            else:
                ruleRef.args = buildArgsList(children[0], currentRule)
        else:
            raise Exception("Invalid # of children for a reference to another rule.")
        return ruleRef

    def buildTagGrammar(tagElement, currentRule):
        expr = TagExpr()
        expr.tagInfo = RuleTagInfo()
        expr.tagInfo.ns = getAttribOrDefault(tagElement, nsAttr, "")
        expr.tagInfo.tag = getAttribOrFail(tagElement, elemAttr)
        for elem in tagElement.iterchildren():
            if elem.tag == AttrsTag:
                expr.tagInfo.attrs = buildAttrsList(elem, currentRule)
            else: 
                expr.subExprs.append(buildGrammar(elem, currentRule))
        return expr


    def buildGrammar(grammarElement, currentRule):
        if grammarElement.tag == TagTag:
            return buildTagGrammar(grammarElement, currentRule)
        elif grammarElement.tag == OptionalTag:
            expr = OptionalExpr()
            buildExpr(grammarElement, expr, currentRule)
            return expr

        elif grammarElement.tag == ZeroOrMoreTag:
            expr = ZeroOrMoreExpr()
            buildExpr(grammarElement, expr, currentRule)
            return expr

        elif grammarElement.tag == OneOrMoreTag:
            expr = OneOrMoreExpr()
            buildExpr(grammarElement, expr, currentRule)
            return expr

        elif grammarElement.tag == ChoiceTag:
            expr = ChoiceExpr()
            buildExpr(grammarElement, expr, currentRule)
            return expr

        elif grammarElement.tag == GroupTag:
            expr = GroupExpr()
            buildExpr(grammarElement, expr, currentRule)
            return expr

        elif grammarElement.tag == LiteralTag:
            return Literal(getAttribOrFail(grammarElement, valueAttr))

        elif grammarElement.tag == ParamUseTag:
            paramUse = ParamUse(getAttribOrFail(grammarElement, nameAttr))
            if currentRule == None:
                raise Exception("Invalid current Rule!")

            # Verifying parameter usage
            if currentRule.parameters != None and paramUse.name not in currentRule.parameters:
                raise Exception("Invalid parameter usage. Within Rule: " + currentRule.name + formatElementErrorMsg(grammarElement))
            return paramUse

        elif grammarElement.tag == RefTag:
            return buildRef(grammarElement, currentRule)

        elif grammarElement.tag == TextTag:
            return Text()

        elif grammarElement.tag == NumberTag:
            return Number()

        elif grammarElement.tag == EmptyTag:
            return Empty()

        elif grammarElement.tag == IdentifierTag:
            return Identifier()

        elif grammarElement.tag == OperatorTag:
            return Operator()
            
        elif grammarElement.tag == NoImplTag:
            countHelper.notImplemnted += 1
            return NotImplementedYet()
        else:
            unexpectedOrUnknownTag(grammarElement)

    def buildGrammarDef(defElem, currentRule):
        ret = []
        for elem in defElem.iterchildren():
            ret.append(buildGrammar(elem, currentRule))

        if len(ret) == 0:
            return None
        return ret

    def buildParameterList(paramList):
        ret = []
        for elem in paramList.iterchildren():
            if elem.tag == ParamTag:
                param = getAttribOrFail(elem, nameAttr)
                ret.append(param)
            else:
                unexpectedOrUnknownTag(elem)
        if len(ret) == 0:
            return None
        return ret


    # Rule Rules
    def buildRule(ruleElem):
        rule = Rule()
        rule.name = getAttribOrFail(ruleElem, nameAttr)
        rule.canOccurAnyWhere = getAttribOrDefault(ruleElem, canOccurAnyWhereAttr, "false").lower() == "true"
        validateRuleName(rule, ruleElem)
        rule.languages = [x.strip() for x in getAttribOrFail(ruleElem, langAttr).split(',')]
        validateLanguages(rule.languages, ruleElem)
        rule.title = getAttribOrDefault(ruleElem, titleAttr, "")
        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem, rule)

            elif elem.tag == DescTag:
                rule.desc = extractSubText(elem)

            elif elem.tag == ParamsTag:
                rule.params = buildParameterList(elem)

            else:
                unexpectedOrUnknownTag(elem)
        assert rule.grammar != None, "Post condition violation rule grammar not set"
        return rule


    def buildAttrsList(attrsElem, currentRule):
        ret = []
        for elem in attrsElem.iterchildren():
            if elem.tag == AttrTag:
                info = AttrUseInfo()
                info.name = getAttribOrFail(elem, ruleAttr)
                info.isOptional = getAttribOrDefault(elem, isOptionalAttr, "").lower() == "true"
                ret.append(info)

                # Adding edge for rule to attribute
                newEdge = RuleEdge()
                newEdge.fromTitle = currentRule.name
                newEdge.fromObj = currentRule
                newEdge.toTitle = info.name
                newEdge.ruleRef = info
                grammarDoc.edgeList.append(newEdge)

            else:
                unexpectedOrUnknownTag(elem)
        if len(ret) == 0:
            return None
        return ret


    def buildTagInfo(tagElem, currentRule):
        info = RuleTagInfo()
        info.ns = getAttribOrDefault(tagElem, nsAttr, "")
        info.tag = getAttribOrFail(tagElem, elemAttr)
        for elem in tagElem.iterchildren():
            if elem.tag == AttrsTag:
                info.attrs = buildAttrsList(elem, currentRule)

            else:
                unexpectedOrUnknownTag(elem)
        return info


    def buildTagRule(ruleElem):
        rule = TagRule()
        rule.name = getAttribOrFail(ruleElem, nameAttr)
        validateRuleName(rule, ruleElem)
        rule.languages = [x.strip() for x in getAttribOrFail(ruleElem, langAttr).split(',')]
        validateLanguages(rule.languages, ruleElem)
        rule.title = getAttribOrDefault(ruleElem, titleAttr, "")
        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem, rule)

            elif elem.tag == TagTag:
                rule.tagInfo = buildTagInfo(elem, rule)

            elif elem.tag == DescTag:
                rule.desc = extractSubText(elem)

            elif elem.tag == ParamsTag:
                rule.parameters = buildParameterList(elem)

            else:
                unexpectedOrUnknownTag(elem)
        assert rule.tagInfo != None, "Post condition violation tag rule's tag information not set" + formatElementErrorMsg(ruleElem)
        assert rule.grammar != None, "Post condition violation tag rule grammar not set" + formatElementErrorMsg(ruleElem)
        return rule


    def buildAttrRule(ruleElem):
        rule = AttrRule()
        rule.name = getAttribOrFail(ruleElem, nameAttr)
        validateRuleName(rule, ruleElem)
        rule.languages = [x.strip() for x in getAttribOrFail(ruleElem, langAttr).split(',')]
        validateLanguages(rule.languages, ruleElem)
        rule.title = getAttribOrDefault(ruleElem, titleAttr, "")
        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem, rule)

            elif elem.tag == DescTag:
                rule.desc = extractSubText(elem)

            elif elem.tag == AttrTag:
                rule.attrName = getAttribOrFail(elem, nameAttr)

            elif elem.tag == ParamsTag:
                rule.parameters = buildParameterList(elem)

            else:
                unexpectedOrUnknownTag(elem)
        assert rule.grammar != None, "Post condition violation attribute rule grammar not set"
        assert rule.attrName != "", "Post condition violation attribute rule's attribute information not set"
        return rule


    def buildRestOfRuleDoc(ruleElem, ruleToAddTo):
        for elem in ruleElem.iterchildren():
            if elem.tag == DescTag:
                ruleToAddTo.desc = extractSubText(elem)
            elif elem.tag == RefsTag:
                for refElem in elem.iterchildren():
                    if refElem.tag == SeeTag:
                        link = URLRefLink()
                        link.title = getAttribOrFail(refElem, titleAttr)
                        link.url = getAttribOrFail(refElem, urlAttr)
                        ruleToAddTo.otherLinks.append(link)
                    else:
                        unexpectedOrUnknownTag(refElem)
            else:
                unexpectedOrUnknownTag(elem)

    def buildRuleDocumentation(documentationElem):
        doc = RuleDocumentation()
        doc.name = getAttribOrFail(documentationElem, ruleAttr)
        buildRestOfRuleDoc(documentationElem, doc)
        validateDocName(doc, documentationElem)
        return doc

    def buildAttrRuleDocumentation(documentationElem):
        doc = AttrRuleDocumentation()
        doc.name = getAttribOrFail(documentationElem, ruleAttr)
        buildRestOfRuleDoc(documentationElem, doc)
        validateDocName(doc, documentationElem)
        return doc

    def buildTagDocumentation(documentationElem):
        doc = TagDocumentation()
        doc.tag = getAttribOrFail(documentationElem, elemAttr)
        doc.ns = getAttribOrDefault(documentationElem, nsAttr, "")
        buildRestOfRuleDoc(documentationElem, doc)
        validateTagDoc(doc, documentationElem)
        return doc

    def buildHowToRead(element):
        currentDir = os.path.dirname(fileName)
        howToFile = getAttribOrFail(element, fileAttr)
        currentHowToPath = os.path.join(currentDir, howToFile)
        if not os.path.exists(currentHowToPath):
            raise Exception("Unable to locate how to file location: " + currentHowToPath)

        print "    " +("-"*76)
        print "    Processing how to file: {0}".format(currentHowToPath) 
        ret = buildHowToReadDocumentation(currentHowToPath)
        assert ret != None, "Didn't get documentation"
        print "    " +("-"*76)
        return ret


    def buildDocumentation(docElem):
        for elem in docElem.iterchildren():
            if elem.tag == RuleTag:
                grammarDoc.documentation.append(buildRuleDocumentation(elem))

            elif elem.tag == HowToReadTag:
                grammarDoc.howToReadDoc = buildHowToRead(elem)

            elif elem.tag == AttrRuleTag:
                grammarDoc.documentation.append(buildAttrRuleDocumentation(elem))

            elif elem.tag == TagTag:
                grammarDoc.documentation.append(buildTagDocumentation(elem))

            else:
                unexpectedOrUnknownTag(elem)

    currentRule = None
    grammarTree = ET.parse(fileName)
    grammarDoc = GrammarDoc()
    grammarElem = grammarTree.getroot()

    class SpecialCount: pass
    countHelper = SpecialCount()
    countHelper.notImplemnted = 0
    countHelper.unconnectedGraphRules = 0
    countHelper.missingReferences = 0

    # Verifying Correct Root element.
    verifyNodeNameOrFail(grammarElem, GrammarTag)

    grammarDoc.languages = getAttribOrFail(grammarElem, languageOrderingAttr).split(",")

    # This is used for validating that all languages read in are valid and correct.
    validLanguageSet = set([lang for lang in grammarDoc.languages])
    def validateLanguages(langs, elem):
        for l in langs:
            if l not in validLanguageSet:
                raise Exception("Invalid language attribute set. Language: {0} is NOT a valid language ".format(l) + formatElementErrorMsg(elem))

    # Used for validating the names of all rules.
    invalidRuleNames = set()
    def validateRuleName(rule, elem):
        if rule.name in invalidRuleNames:
            raise Exception("Duplicate Rule name: {0}. RuleType: {1}. ".format(rule.name, elem.tag) + formatElementErrorMsg(elem))
        else:
            invalidRuleNames.add(rule.name)

    # Used for validating the documentation side of things.
    invalidDocName = set()
    invalidTagDoc = set()
    def validateDocName(documentation, elem):
        if documentation.name in invalidRuleNames:
            raise Exception("Duplicate documentation name: {0}. Documentation Type: {1}. ".format(documentation.name, elem.tag) + formatElementErrorMsg(elem))
        else:
            invalidDocName.add(documentation.name)

    def validateTagDoc(documentation, elem):
        tag = getComparableTagName(documentation)
        if tag in invalidRuleNames:
            raise Exception("Duplicate tag for documentation: {0}. Documentation Type: {1}. ".format(tag, elem.tag) + formatElementErrorMsg(elem))
        else:
            invalidTagDoc.add(tag)

    for elem in grammarElem.iterchildren():
        if elem.tag == DocumentationTag:
            buildDocumentation(elem)

        elif elem.tag == RuleTag:
            grammarDoc.rules.append(buildRule(elem)) 

        elif elem.tag == TagRuleTag:
            grammarDoc.rules.append(buildTagRule(elem)) 

        elif elem.tag == AttrRuleTag:
            grammarDoc.rules.append(buildAttrRule(elem))

        else:
            unexpectedOrUnknownTag(elem)


    # -------------------------------------
    # Verifying Grammar
    # -------------------------------------

    # Separating different types of documentation
    documentedTags = dict([(getComparableTagName(tagDoc), tagDoc) for tagDoc in grammarDoc.documentation if isinstance(tagDoc, TagDocumentation)])
    documentedAttrs = dict([(attrDoc.name, attrDoc) for attrDoc in grammarDoc.documentation if isinstance(attrDoc, AttrRuleDocumentation)])
    documentedRules = dict([(ruleDoc.name, ruleDoc) for ruleDoc in grammarDoc.documentation if isinstance(ruleDoc, RuleDocumentation)])

    # Separating tag rules
    grammarTagRules = [rule for rule in grammarDoc.rules if isinstance(rule, TagRule)]
    grammarAttrRules = dict([(rule.name, rule) for rule in grammarDoc.rules if isinstance(rule, AttrRule)])
    grammarRules = dict([(rule.name, rule) for rule in grammarDoc.rules if isinstance(rule, Rule)])


    print "Building Documentation"
    print "    Checking Rules to Documentation"
    countHelper.missingDocumentation = 0
    undocumentedRules = []
    for rule in grammarTagRules:
        tag = getComparableTagName(rule.tagInfo)
        if tag in documentedTags:
            documentedTags[tag].tagRules.append(rule)
            if rule.tagInfo.attrs != None:
                for attr in rule.tagInfo.attrs:
                    located = False
                    for item in documentedTags[tag].attributes:
                        if item.name == attr.name:
                            located = True
                    if located:
                        documentedTags[tag].attributes.append(attr)
        else:
            undocumentedRules.append(rule)
            print "        Missing documentation for tag: {0} within Rule: {1}".format(tag, rule.name) 
            countHelper.missingDocumentation += 1

    for rule in grammarAttrRules.items():
        if rule[1].name in documentedAttrs:
            documentedAttrs[rule[1].name].attrRule = rule[1]

        else:
            undocumentedRules.append(rule)
            print "        Missing documentation for attribute:", rule[1].name
            countHelper.missingDocumentation += 1

    for rule in grammarRules.items():
        if rule[0] in documentedRules:
            documentedRules[rule[0]].rule = rule[1]

        else:
            undocumentedRules.append(rule)
            print "        Missing documentation for Regular Rule:", rule[0]
            countHelper.missingDocumentation += 1



    print "    Checking Documentation To Rules"
    countHelper.extraDocumentation = 0
    documentationWithoutARule = []
    for doc in documentedTags.items():
        if len(doc[1].tagRules) == 0:
            documentationWithoutARule.append(doc[1])
            print "        Extra documentation for Tag: {0}".format(getComparableTagName(doc[1])) 
            countHelper.extraDocumentation += 1
    
    for doc in documentedAttrs.items():
        if doc[1].attrRule == None:
            documentationWithoutARule.append(doc[1])
            print "        Extra documentation for attribute ", doc[1].name
            countHelper.extraDocumentation += 1

    for doc in documentedRules.items():
        if doc[1].rule == None:
            documentationWithoutARule.append(doc[1])
            print "        Extra documentation for rule ", doc[1].name
            countHelper.extraDocumentation += 1

        

    print "Completing graph references"
    rulesByName = dict([(rule.name, rule) for rule in grammarDoc.rules])
    namesMissingFromGraph = []
    for edge in grammarDoc.edgeList:
        if edge.ruleRef == None:
            raise Exception("Didn't correctly set the rule reference for edge")
        if edge.toTitle in rulesByName:
            edge.toObj = rulesByName[edge.toTitle]
        else:
            print "    Missing Reference: {0} From within Rule: {1}".format(edge.toTitle, edge.fromTitle) 
            namesMissingFromGraph.append(edge)
            countHelper.missingReferences += 1



    print "Verifying That Graph is connected"
    notVisitedVertices = set([ e.fromTitle for e in grammarDoc.edgeList])
    notVisitedVertices |= set([ e.toTitle for e in grammarDoc.edgeList])

    queue = [startRule for startRule in grammarDoc.edgeList if grammarDoc.edgeList[0].fromTitle == startRule.fromTitle]
    notVisitedVertices.discard(grammarDoc.edgeList[0].fromTitle)

    while len(queue) != 0:
        current = queue.pop(0)
        if current.toTitle in notVisitedVertices:
            notVisitedVertices.discard(current.toTitle)
            for e in grammarDoc.edgeList:
                if current.toTitle == e.fromTitle:
                    queue.append(e)

    countHelper.unconnectedGraphRules = len(notVisitedVertices)
    if len(notVisitedVertices) > 0:
        print "    Parts of grammar is not reachable. The following rules are unreachable:"
        for unreachableItem in notVisitedVertices:
            print "        {0}".format(unreachableItem) 
            # countHelper.unconnectedGraphRules += 1

    edgesByFromTitle = dict()
    for e in grammarDoc.edgeList:
        if e.fromTitle in edgesByFromTitle:
            edgesByFromTitle[e.fromTitle].append(e)
        else:
            edgesByFromTitle.update({e.fromTitle: [e]})

    edgesByToTitle = dict()
    for e in grammarDoc.edgeList:
        if e.toTitle in edgesByToTitle:
            edgesByToTitle[e.toTitle].append(e)
        else:
            edgesByToTitle.update({e.toTitle: [e]})

    print "    Not in Edge List"
    countHelper.notInEdgeList = 0
    notInEdgeList = []
    for rule in grammarDoc.rules:
        if rule.name not in edgesByToTitle and rule.name not in edgesByFromTitle:
            print "        {0}".format(rule.name) 
            notInEdgeList.append(rule)
            countHelper.notInEdgeList += 1



    print "Extracting parent & child rules from graph"
    for tagDoc in documentedTags.items():
        # Getting parent rules
        for tagRule in tagDoc[1].tagRules:
            if tagRule.name in edgesByToTitle:
                tagDoc[1].parentRules += [rulesByName[edge.fromTitle] for edge in edgesByToTitle[tagRule.name] if edge.fromTitle in rulesByName and edge.fromObj not in tagDoc[1].parentRules]

        # Getting child rules
        for tagRule in tagDoc[1].tagRules:
            if tagRule.name in edgesByFromTitle:
                tagDoc[1].childRules += [rulesByName[edge.toTitle] for edge in edgesByFromTitle[tagRule.name] if edge.toTitle in rulesByName and edge.toObj not in tagDoc[1].childRules]

    for ruleDoc in documentedRules.items():
        # Getting parent rules
        rule = ruleDoc[1].rule
        if rule.name in edgesByToTitle:
            ruleDoc[1].parentRules += [rulesByName[edge.fromTitle] for edge in edgesByToTitle[rule.name] if edge.fromTitle in rulesByName and edge.fromObj not in ruleDoc[1].parentRules]

        # Getting child rules
        if rule.name in edgesByFromTitle:
            ruleDoc[1].childRules += [rulesByName[edge.toTitle] for edge in edgesByFromTitle[rule.name] if edge.toTitle in rulesByName and edge.toObj not in ruleDoc[1].childRules]

    for attrDoc in documentedAttrs.items():
        # Getting parent rules
        rule = attrDoc[1].attrRule
        if rule.name in edgesByToTitle:
            attrDoc[1].parentRules += [rulesByName[edge.fromTitle] for edge in edgesByToTitle[rule.name] if edge.fromTitle in rulesByName and edge.fromObj not in attrDoc[1].parentRules]

        # Getting child rules
        if rule.name in edgesByFromTitle:
            attrDoc[1].childRules += [rulesByName[edge.toTitle] for edge in edgesByFromTitle[rule.name] if edge.toTitle in rulesByName and edge.toObj not in attrDoc[1].childRules]


    print "Extracting Extracting parent & child tags from graph"
    for doc in grammarDoc.documentation:
        for parentRule in doc.parentRules:
            if isinstance(parentRule, TagRule):
                tagName = getComparableTagName(parentRule.tagInfo)
                if tagName not in doc.parentTags:
                    doc.parentTags.append(tagName)

        for childRule in doc.childRules:
            if isinstance(childRule, TagRule):
                tagName = getComparableTagName(childRule.tagInfo)
                if tagName not in doc.childTags:
                    doc.childTags.append(tagName)

    print "Extracting Attributes for documentation"
    for tagDoc in documentedTags.values():
        for tagRule in tagDoc.tagRules:
            if tagRule.tagInfo == None:
                # This should never occur
                raise Exception("Unable to extract attributes, invalid tag info!")
            if tagRule.tagInfo.attrs != None:
                for attr in tagRule.tagInfo.attrs:
                    if attr.name not in tagDoc.attributes:
                        tagDoc.attributes.append(attr.name)

    print "Unique-ing Links and tag names."
    def uniqueList(seq, key=lambda x:x):
        seen = set()
        seen_add = seen.add
        return [x for x in seq if not (key(x) in seen or seen_add(key(x)))]


    for tagDocumentation in documentedTags.values():
        tagDocumentation.parentRules = uniqueList(tagDocumentation.parentRules, key=lambda x: x.name)
        tagDocumentation.childRules = uniqueList(tagDocumentation.childRules, key=lambda x: x.name)

    for ruleDocumentation in documentedRules.values():
        ruleDocumentation.parentRules = uniqueList(ruleDocumentation.parentRules, key=lambda x: x.name)
        ruleDocumentation.childRules = uniqueList(ruleDocumentation.childRules, key=lambda x: x.name)

    for attrDocumentation in documentedAttrs.values():
        attrDocumentation.parentRules = uniqueList(attrDocumentation.parentRules, key=lambda x: x.name)
        attrDocumentation.childRules = uniqueList(attrDocumentation.childRules, key=lambda x: x.name)

    print "Computing Languages for Documentation"
    for tagDoc in documentedTags.values():
        tagDoc.languages = []
        for l in grammarDoc.languages:
            for tagRule in tagDoc.tagRules:
                if l in tagRule.languages:
                    tagDoc.languages.append(l)
                    break

    for ruleDoc in documentedRules.values():
        if ruleDoc.rule != None:
            ruleDoc.languages = ruleDoc.rule.languages

    for attrDoc in documentedAttrs.values():
        if attrDoc.attrRule != None:
            attrDoc.languages = attrDoc.attrRule.languages

    print "    "+("-"*76)
    print "    Summary"
    print "    # of extra documentation: {0}".format(countHelper.extraDocumentation)
    print "    # of missing documentation: {0}".format(countHelper.missingDocumentation)
    print "    # of rules not implemented yet: {0}".format(countHelper.notImplemnted)
    print "    # of unconnected graph rules: {0}".format(countHelper.unconnectedGraphRules)
    print "    # of missing references rules: {0}".format(countHelper.missingReferences)
    print "    # of rules not in edge list: {0}".format(countHelper.notInEdgeList)
    print "    Total # of rules: {0}".format(len(grammarDoc.rules))
    print "    "+("-"*76)
    
    def compileGrammarToValidator(languageGrammar):
        startingRule = languageGrammar.rules[0]
        print startingRule.title
    grammarDoc.compiledGrammar = compileGrammarToValidator(grammarDoc)
    return grammarDoc



# def GenerateRelaxNGFromGrammar(outputLocation, grammar):
#     def handleDispatch(node):
#         traversalStack.append(node)
#         if isinstance(node, ParamUse):
#             pass

#         elif isinstance(node, RuleRef):
#             if node.args != None:
#                 if node.name == "cppDirective":
#                     treeBuilder.start("element", {"name":"cpp:directive"})
#                     treeBuilder.start("text", {})
#                     treeBuilder.end("text")                    
#                     treeBuilder.end("element")                    
#                 else:
#                     raise Exception("Super derp. located another rule with parameters that isn't cppDirective: {0}".format(node.name))
#             else:
#                 treeBuilder.start("ref", {"name":node.name})
#                 treeBuilder.end("ref")
#             pass

#         elif isinstance(node, Literal):
#             treeBuilder.start("text", {})
#             treeBuilder.end("text")

#         elif isinstance(node, NotImplementedYet):
#             raise Exception("Not implemented yet")

#         elif isinstance(node, Operator):
#             treeBuilder.start("text", {})
#             treeBuilder.end("text")

#         elif isinstance(node, Text):
#             treeBuilder.start("text", {})
#             treeBuilder.end("text")

#         elif isinstance(node, Number):
#             treeBuilder.start("text", {})
#             treeBuilder.end("text")

#         elif isinstance(node, Empty):
#             treeBuilder.start("empty", {})
#             treeBuilder.end("empty")

#         elif isinstance(node, Identifier):
#             treeBuilder.start("text", {})
#             treeBuilder.end("text")

#         elif isinstance(node, TagExpr):
#             treeBuilder.start("element", {"name":getComparableTagName(node.tagInfo)})
#             dispatchFromList(node.subExprs)
#             treeBuilder.end("element")

#         elif isinstance(node, OptionalExpr):
#             treeBuilder.start("optional", {})
#             dispatchFromList(node.subExprs)
#             treeBuilder.end("optional")

#         elif isinstance(node, ZeroOrMoreExpr):
#             treeBuilder.start("zeroOrMore", {})
#             dispatchFromList(node.subExprs)
#             treeBuilder.end("zeroOrMore")

#         elif isinstance(node, GroupExpr):
#             treeBuilder.start("group", {})
#             dispatchFromList(node.subExprs)
#             treeBuilder.end("group")

#         elif isinstance(node, OneOrMoreExpr):
#             treeBuilder.start("oneOrMore", {})
#             dispatchFromList(node.subExprs)
#             treeBuilder.end("oneOrMore")

#         elif isinstance(node, ChoiceExpr):
#             treeBuilder.start("choice", {})
#             dispatchFromList(node.subExprs)
#             treeBuilder.end("choice")

#         else:
#             raise Exception("Unknown or unhandled grammar type: {0}".format(node.__class__.__name__))
#         traversalStack.pop()


#     def dispatchFromList(listOfGrammarItems):
#         for item in listOfGrammarItems:
#             handleDispatch(item)
#     treeBuilder = ET.TreeBuilder()
#     # ns="http://www.sdml.info/srcML/src" xmlns:cpp="http://www.sdml.info/srcML/cpp" xmlns="http://relaxng.org/ns/structure/1.0"
#     nsMap = {"cpp":"http://www.sdml.info/srcML/cpp"}

#     attrs = dict()
#     attrs["ns"] = "http://www.sdml.info/srcML/src"
#     attrs["xmlns"] = "http://relaxng.org/ns/structure/1.0"
#     treeBuilder.start("grammar", attrs, nsMap)


#     traversalStack = []
#     for rule in grammar.rules:
#         if rule.parameters != None:
#             print "Rule with parameters: ", rule.name
#             continue

#         traversalStack.append(rule)
#         if isinstance(rule, Rule):
#             if rule.name == "start":
#                 treeBuilder.start("start", {})
#             else:
#                 treeBuilder.start("define", {"name":rule.name})
#             dispatchFromList(rule.grammar)
#             if rule.name == "start":
#                 treeBuilder.end("start")
#             else:
#                 treeBuilder.end("define")

#         elif isinstance(rule, TagRule):
#             treeBuilder.start("define", {"name":rule.name})
#             treeBuilder.start("element", {"name":getComparableTagName(rule.tagInfo)})
#             if rule.tagInfo.attrs != None:
#                 for attr in rule.tagInfo.attrs:
#                     if attr.isOptional:
#                         treeBuilder.start("optional", {})
#                         treeBuilder.start("ref", {"name":attr.name})
#                         treeBuilder.end("ref")
#                         treeBuilder.end("optional")
#                     else:
#                         treeBuilder.start("ref", {"name":attr.name})
#                         treeBuilder.end("ref")

#             # build element stuff here
#             dispatchFromList(rule.grammar)
#             treeBuilder.end("element")
#             treeBuilder.end("define")

#         elif isinstance(rule, AttrRule):
#             treeBuilder.start("define", {"name":rule.name})
#             # build attribute stuff here
#             treeBuilder.start("attribute", {"name":rule.attrName})
#             dispatchFromList(rule.grammar)
#             treeBuilder.end("attribute")
#             treeBuilder.end("define")

#         else:
#             raise Exception("Unhandled rule type: {0}".format(rule.__class__.__name__))
#         traversalStack.pop()

#     treeBuilder.end("grammar")
#     doc = treeBuilder.close()

    # validationFileName = "validationTreeFromGrammar.rng"
    # validationTreeStrm = open(validationFileName,"w")
    # validationTreeStrm.write(ET.tostring(doc, pretty_print=True))
    # validationTreeStrm.close()
    # validationDoc = ET.fromstring(ET.tostring(doc, pretty_print=True))
    # relaxDoc = ET.RelaxNG(etree=validationDoc)
    # relaxDoc = ET.RelaxNG(etree=ET.parse("validationTreeFromGrammar.rng"))
    
    # return relaxDoc

    
# if __name__ == "__main__":

#     grmmr = loadGrammar("/home/brian/Projects/srcML/doc/srcMLDocGen/DocData/Grammar/LanguageGrammar.xml")
#     assert grmmr != None, "Didn't pass test"
#     outFile = open("../GrammarOutput.txt", "w")
#     grmmr.toEBNFText(outFile)
#     outFile.close()
#     outFile = open("../grammarDocumentation.txt","w")
#     grmmr.dumpDocumentation(outFile)
#     outFile.close()
#     # print out.getvalue()
