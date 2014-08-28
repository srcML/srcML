
import os, sys, re, lxml, cStringIO, itertools
import lxml.etree as ET
from xml.sax.handler import ContentHandler
from LoadData import *


def getComparableTagName(item):
    return "{0}{1}".format("" if item.ns == "" else (item.ns +":"), item.tag)

# Class that holds everything relating to the grammar
class GrammarDoc:
    def __init__(self):
        self.languages = []
        self.rules = []
        self.documentation = [] # used to determine the ordering of the documentation
        self.edgeList = []


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
        out.write(" <{0}{1}>".format("" if self.tagInfo.ns == "" else (self.tagInfo.ns + ":"), self.tagInfo.tag))
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

class Empty(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Number")

    def toEBNFText(self, out):
        out.write("Number")

class Identifier(GrammarDef):
    def __init__(self):
        GrammarDef.__init__(self, "Identifier")

    def toEBNFText(self, out):
        out.write("Identifier")

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
EmptyTag = "Empty"
IdentifierTag = "Identifier"

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
        assert rule.tagInfo != None, "Post condition violation tag rule's tag information not set"
        assert rule.grammar != None, "Post condition violation tag rule grammar not set"
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

    for rule in grammarAttrRules.items():
        if rule[1].name in documentedAttrs:
            documentedAttrs[rule[1].name].attrRule = rule[1]

        else:
            undocumentedRules.append(rule)
            print "        Missing documentation for attribute:", rule[1].name

    for rule in grammarRules.items():
        if rule[0] in documentedRules:
            documentedRules[rule[0]].rule = rule[1]

        else:
            undocumentedRules.append(rule)
            print "        Missing documentation for Regular Rule:", rule[0]



    print "    Checking Documentation To Rules"
    documentationWithoutARule = []
    for doc in documentedTags.items():
        if len(doc[1].tagRules) == 0:
            documentationWithoutARule.append(doc[1])
            print "        Extra documentation for Tag: {0}".format(getComparableTagName(doc[1])) 
    
    for doc in documentedAttrs.items():
        if doc[1].attrRule == None:
            documentationWithoutARule.append(doc[1])
            print "        Extra documentation for attribute ", doc[1].name

    for doc in documentedRules.items():
        if doc[1].rule == None:
            documentationWithoutARule.append(doc[1])
            print "        Extra documentation for rule ", doc[1].name

        

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

    if len(notVisitedVertices) > 0:
        print "    Parts of grammar is not reachable. The following rules are unreachable:"
        for unreachableItem in notVisitedVertices:
            print "        {0}".format(unreachableItem) 



    print "Extracting parent & child rules from graph"
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
    
    for e in grammarDoc.edgeList:
        if e.fromTitle == "cppDirective":
            print e.fromTitle

    for tagDoc in documentedTags.items():
        # Getting parent rules
        for tagRule in tagDoc[1].tagRules:
            # if tagRule.name == "cppDirective":
                
            #     print "Processing cpp directive!"
            #     print edgesByToTitle[tagRule.name]
            #     if tagRule.name in edgesByToTitle:
            #         locatedRules = 
            #         print "Located Rules: ", locatedRules
            #         tagDoc[1].parentRules += locatedRules
            #         print "Parent Rules!"
            #         for r in tagDoc[1].parentRules:
            #             print "  Rule: %s"%r.name
            #     else:
            #         print "Didn't locate parent rule edges!"

            #     print "~"*80
            # else:
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


    return grammarDoc
    
if __name__ == "__main__":
    grmmr = loadGrammar("/home/brian/Projects/srcML/doc/srcMLDocGen/DocData/Grammar/LanguageGrammar.xml")
    assert grmmr != None, "Didn't pass test"
    outFile = open("../GrammarOutput.txt", "w")
    grmmr.toEBNFText(outFile)
    outFile.close()
    outFile = open("../grammarDocumentation.txt","w")
    grmmr.dumpDocumentation(outFile)
    outFile.close()
    # print out.getvalue()