
import os, sys, re, lxml, cStringIO
import lxml.etree as ET
from xml.sax.handler import ContentHandler
from LoadData import *


def getComparableTagName(item):
    return "{0}{1}".format("" if item.ns == "" else (item.ns +":"), item.tag)

# Class that holds everything relating to the grammar
class GrammarDoc:
    def __init__(self):
        self.rules = []
        self.documentation = [] # used to determine the ordering of the documentation
        self.edgeList = []


    def toEBNFText(self, out):
        for rule in self.rules:
            rule.toEBNFText(out)
            out.write("\n\n")

class RuleEdge:
    def __init__(self):
        self.fromTitle = ""
        self.fromObj = None
        self.toTitle = ""
        self.toObj = None
        self.ruleRef = None


# Documentation Structure
class DocumentationBase(object):
    def __init__(self):
        self.desc = ""
        self.parentRules = []
        self.childRules = []
        self.otherLinks = []
        self.languages = []

    # def title(self):
    #     assert False, "Title Not implemented yet"

class RuleDocumentation(DocumentationBase):
    def __init__(self):
        DocumentationBase.__init__(self)
        self.rule = None
        self.name = ""


class TagDocumentation(DocumentationBase):
    def __init__(self):
        DocumentationBase.__init__(self)
        self.ns = ""
        self.tag = ""
        self.tagRules = []
        self.parentTags = []
        self.childTags = []
        self.childAttributes = []

class AttrRuleDocumentation(DocumentationBase):
    def __init__(self):
        DocumentationBase.__init__(self)
        self.attrRule = None
        self.parentTags = []


# Rule Classes
class RuleBase(object):
    def __init__(self):
        self.name = ""
        self.parameters = None
        self.grammar = None
        self.languages = None
        self.desc = ""
        self.title = ""
        self.ruleRefs = []

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

class Rule(RuleBase):
    def __init__(self):
        RuleBase.__init__(self)

    def toEBNFRuleStartText(self, out):
        pass


class TagRule(RuleBase):
    def __init__(self):
        RuleBase.__init__(self)
        self.tagInfo = None

    def toEBNFRuleStartText(self, out):
        out.write(" <{0}{1}>".format("" if self.tagInfo.ns == "" else (self.tagInfo.ns +":"), self.tagInfo.tag))
        if self.tagInfo.attrs != None:
            out.write( " ({0})".format(", ".join([ x.name + ("?" if x.isOptional else "") for x in self.tagInfo.attrs])) )

class AttrRule(RuleBase):
    def __init__(self):
        RuleBase.__init__(self)
        self.attrName = ""

    def toEBNFRuleStartText(self, out):
        out.write(" @{0}".format(self.attrName))

# Rule related classes
class RuleTagInfo:
    def __init__(self):
        self.tag = ""
        self.ns = ""
        self.attrs = None

class AttrUseInfo:
    def __init__(self):
        self.name = ""
        self.isOptional = False


# Grammar Definition Related Classes
class GrammarDef(object):
    def __init__(self, objTypeTitle):
        self.typename = objTypeTitle

    def toEBNFText(self, out):
        assert False, "Part of grammar didn't implement toEBNFText"


class ParamUse(GrammarDef):
    def __init__(self, paramName = ""):
        GrammarDef.__init__(self, "ParamUse")
        self.name = paramName

    def toEBNFText(self, out):
        out.write("${0}".format(self.name))

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


class Literal(GrammarDef):
    def __init__(self, litValue = ""):
        GrammarDef.__init__(self, "Literal")
        self.value = litValue

    def toEBNFText(self, out):
        out.write("\"{0}\"".format(self.value))

class Text(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Text")

    def toEBNFText(self, out):
        out.write("Text")

class Number(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Number")

    def toEBNFText(self, out):
        out.write("Number")


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
        if len(self.subExprs) == 1:
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

class OptionalExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__( self, "OptionalExpr")

    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return "?"


class ZeroOrMoreExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "ZeroOrMoreExpr")
        
    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return "*"

class GroupExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "GroupExpr")
        
    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return ""

class OneOrMoreExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "OneOrMoreExpr")
        
    def EBNFSeparator(self):
        return " "

    def EBNFSuffix(self):
        return "+"

class ChoiceExpr(GrammarExpr):
    def __init__(self):
        GrammarExpr.__init__(self, "ChoiceExpr")
        
    def EBNFSeparator(self):
        return " | " if len(self.subExprs) <= 5 else "\n    | "

    def EBNFSuffix(self):
        return ""
# Tag


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

# Name Attribute
nameAttr = "name"
nsAttr = "ns"
elemAttr = "elem"
ruleAttr = "rule"
isOptionalAttr = "isOptional"
valueAttr = "value"
langAttr = "lang"

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
        if childCount == 0:
            return ruleRef
        elif childCount == 1:
            if children[0].tag != ArgsTag:
                unexpectedOrUnknownTag(children[0])
            else:
                ruleRef.args = buildArgsList(children[0], currentRule)
        else:
            raise Exception("Invalid # of children for a reference to another rule.")

        # Tracking rule References
        newEdge = RuleEdge()
        newEdge.fromTitle = currentRule.name
        newEdge.fromObj = currentRule
        newEdge.toTitle = ruleRef.name
        newEdge.ruleRef = ruleRef
        currentRule.ruleRefs.append(ruleRef)
        grammarDoc.edgeList.append(newEdge)

        return ruleRef


    def buildGrammar(grammarElement, currentRule):
        if grammarElement.tag == OptionalTag:
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
        rule.languages = [x.strip() for x in getAttribOrFail(ruleElem, langAttr).split(',')]
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


    def buildAttrsList(attrsElem):
        ret = []
        for elem in attrsElem.iterchildren():
            if elem.tag == AttrTag:
                info = AttrUseInfo()
                info.name = getAttribOrFail(elem, ruleAttr)
                info.isOptional = bool(getAttribOrDefault(elem, isOptionalAttr, "False")) 
                ret.append(info)

            else:
                unexpectedOrUnknownTag(elem)
        if len(ret) == 0:
            return None
        return ret


    def buildTagInfo(tagElem):
        info = RuleTagInfo()
        info.ns = getAttribOrDefault(tagElem, nsAttr, "")
        info.tag = getAttribOrFail(tagElem, elemAttr)
        for elem in tagElem.iterchildren():
            if elem.tag == AttrsTag:
                info.attrs = buildAttrsList(elem)

            else:
                unexpectedOrUnknownTag(elem)
        return info


    def buildTagRule(ruleElem):
        rule = TagRule()
        rule.name = getAttribOrFail(ruleElem, nameAttr)
        rule.languages = [x.strip() for x in getAttribOrFail(ruleElem, langAttr).split(',')]
        rule.title = getAttribOrDefault(ruleElem, titleAttr, "")
        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem, rule)

            elif elem.tag == TagTag:
                rule.tagInfo = buildTagInfo(elem)

            elif elem.tag == DescTag:
                rule.desc = extractSubText(elem)

            elif elem.tag == ParamsTag:
                rule.parameters = buildParameterList(elem)

            else:
                unexpectedOrUnknownTag(elem)
        assert rule.tagInfo != None, "Post condition violation tag rule's tag information not set"
        assert rule.grammar != None, "Post condition violation tag rule grammar not set"
        return rule


    def buildAttrRule(ruleElem):
        rule = AttrRule()
        rule.name = getAttribOrFail(ruleElem, nameAttr)
        rule.languages = [x.strip() for x in getAttribOrFail(ruleElem, langAttr).split(',')]
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
                        link.ns = getAttribOrFail(refElem, titleAttr)
                        link.url = getAttribOrFail(refElem, urlAttr)
                        ruleToAddTo.otherLinks.append(link)
                    else:
                        unexpectedOrUnknownTag(refElem)
            else:
                unexpectedOrUnknownTag(elem)

    def buildRuleDocumentation(ruleElem):
        rule = RuleDocumentation()
        rule.name = getAttribOrFail(ruleElem, ruleAttr)
        buildRestOfRuleDoc(ruleElem, rule)

    def buildAttrRuleDocumentation(ruleElem):
        rule = AttrRuleDocumentation()
        rule.name = getAttribOrFail(ruleElem, ruleAttr)
        buildRestOfRuleDoc(ruleElem, rule)

    def buildTagDocumentation(ruleElem):
        rule = TagDocumentation()
        rule.tag = getAttribOrFail(ruleElem, elemAttr)
        rule.ns = getAttribOrDefault(ruleElem, nsAttr, "")
        buildRestOfRuleDoc(ruleElem, rule)

    def buildDocumentation(docElem):
        for elem in docElem.iterchildren():
            if elem.tag == RuleTag:
                grammarDoc.documentation.append(buildRuleDocumentation(elem))

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

    # Verifying Correct Root element.
    verifyNodeNameOrFail(grammarElem, GrammarTag)

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

    # Constructing the rest of the grammar.
    print "Extracting Documented Tags"
    print "Extracting Documented Attributes"
    print "Extracting Documented Rules"

    print "Extracting TagRules"
    print "Extracting AttributeRules"
    print "Extracting Rules"

    print "Verifying Documentation"
    print "Establishing & Verifying Reference Graph"
    print "Extracting parent & child rules from graph"
    print "Extracting Extracting parent & child tags from graph"

    print "Computing Possible Rule Languages"
    return grammarDoc
    
if __name__ == "__main__":
    grmmr = loadGrammar("/home/brian/Projects/srcML/doc/srcMLDocGen/DocData/Grammar/LanguageGrammar.xml")
    assert grmmr != None, "Didn't pass test"
    outFile = open("../GrammarOutput.txt", "w")
    grmmr.toEBNFText(outFile)
    outFile.close()
    # print out.getvalue()