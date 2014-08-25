
import os, sys, re, lxml, cStringIO
import lxml.etree as ET
from xml.sax.handler import ContentHandler
from LoadData import *

# Class that holds everything
class GrammarDoc:
    def __init__(self):
        self.rules = []

    def toEBNFText(self, out):
        for rule in self.rules:
            rule.toEBNFText(out)
            out.write("\n\n")

# Rule Classes
class RuleBase(object):
    def __init__(self):
        self.name = ""
        self.parameters = None
        self.grammar = None

    def toEBNFText(self, out):
        out.write(self.name + " ")
        self.toEBNFRuleStartText(out)
        if self.parameters != None:
            out.writ(" [")
            out.write(", ".join(self.parameters))
            out.write(" ]")
        out.write(" := ")
        for grmmr in self.grammar:
            grmmr.toEBNFText(out)
        out.write("\n;")

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
        out.write("<{0}{1}> ".format("" if self.tagInfo.ns == "" else (self.tagInfo.ns +":"), self.tagInfo.tag))
        if self.tagInfo.attrs != None:
            out.write( "({0})".format(", ".join([ x.name + ("?" if x.isOptional else "") for x in self.tagInfo.attrs])) )

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
        self.attr = None

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
        out.write(" ${0}".format(self.name))

class RuleRef(GrammarDef):
    def __init__(self, refName = ""):
        GrammarDef.__init__(self, "RuleRef")
        self.name = refName
        self.args = None

    def toEBNFText(self, out):
        out.write(" {0}".format(self.name))
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
                    out.write(" {0}".format(self.EBNFSeparator()))
            out.write(")")
            out.write(self.EBNFSuffix())

class OptionalExpr(GrammarExpr):
    def __init__(self):
        super(GrammarExpr, self).__init__("OptionalExpr")

    def EBNFSeparator(self):
        return ""

    def EBNFSuffix(self):
        return "?"


class ZeroOrMoreExpr(GrammarExpr):
    def __init__(self):
        super(GrammarExpr, self).__init__("ZeroOrMoreExpr")
        
    def EBNFSeparator(self):
        return ""

    def EBNFSuffix(self):
        return "*"

class OneOrMoreExpr(GrammarExpr):
    def __init__(self):
        super(GrammarExpr, self).__init__("OneOrMoreExpr")
        
    def EBNFSeparator(self):
        return ""

    def EBNFSuffix(self):
        return "+"

class ChoiceExpr(GrammarExpr):
    def __init__(self):
        super(GrammarExpr, self).__init__("ChoiceExpr")
        
    def EBNFSeparator(self):
        return "|"

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


# Name Attribute
nameAttr = "name"
nsAttr = "ns"
elemAttr = "elem"
ruleAttr = "rule"
isOptionalAttr = "isOptional"
valueAttr = "value"

def loadGrammar(fileName):
    def buildExpr(element, exprToBuild):
        for elem in element.iterchildren():
            exprToBuild.subexprs.append(buildGrammar(elem))
        if len(exprToBuild.subExprs) == 0:
            raise Exception("All expressions must have at least one child element " + formatElementErrorMsg(element))
    
    def buildArgsList(argsListElem):
        ret = []
        for elem in argsListElem.iterchildren():
            if elem.tag == ArgTag:
                if len(list(elem)) != 1:
                    raise Exception("Invalid number of grammar elements for argument")
                else:
                    ret.append(buildGrammar(elem))
            else:
                unexpectedOrUnknownTag(elem)
        if len(ret) == 0:
            return None
        return ret

    def buildRef(refElem):
        ruleRef = RuleRef(getAttribOrFail(refElem, nameAttr))
        children = list(refElem)
        childCount = len(children)
        if childCount == 0:
            return ruleRef
        elif childCount == 1:
            if children[0].tag != ArgsTag:
                unexpectedOrUnknownTag(children[0])
            else:
                ruleRef.args = buildArgsList(children[0])
        else:
            raise Exception("Invalid # of children for a reference to another rule.")
        return ruleRef


    def buildGrammar(grammarElement):
        if grammarElement.tag == OptionalTag:
            expr = OptionalExpr()
            buildExpr(grammarElement, expr)
            return expr

        elif grammarElement.tag == ZeroOrMoreTag:
            expr = ZeroOrMoreExpr()
            buildExpr(grammarElement, expr)
            return expr

        elif grammarElement.tag == OneOrMoreTag:
            expr = OneOrMoreExpr()
            buildExpr(grammarElement, expr)
            return expr

        elif grammarElement.tag == ChoiceTag:
            expr = ChoiceExpr()
            buildExpr(grammarElement, expr)
            return expr

        elif grammarElement.tag == LiteralTag:
            return Literal(getAttribOrFail(grammarElement, valueAttr))

        elif grammarElement.tag == ParamUseTag:
            return ParamUse(getAttribOrFail(grammarElement, nameAttr))

        elif grammarElement.tag == RefTag:
            return buildRef(grammarElement)

        elif grammarElement.tag == TextTag:
            return Text()

        else:
            unexpectedOrUnknownTag(grammarElement)

    def buildGrammarDef(defElem):
        ret = []
        for elem in defElem.iterchildren():
            ret.append(buildGrammar(elem))

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

    def buildRule(ruleElem):
        rule = Rule()
        rule.name = getAttribOrFail(ruleElem, nameAttr)

        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem)
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

        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem)

            elif elem.tag == TagTag:
                rule.tagInfo = buildTagInfo(elem)

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

        for elem in ruleElem.iterchildren():
            if elem.tag == DefTag:
                rule.grammar = buildGrammarDef(elem)
            elif elem.tag == AttrTag:
                rule.attrName = getAttribOrFail(elem, nameAttr)
            elif elem.tag == ParamsTag:
                rule.parameters = buildParameterList(elem)
            else:
                unexpectedOrUnknownTag(elem)
        assert rule.grammar != None, "Post condition violation attribute rule grammar not set"
        assert rule.attrName != "", "Post condition violation attribute rule's attribute information not set"
        return rule




    grammarTree = ET.parse(fileName)
    grammarDoc = GrammarDoc()
    grammarElem = grammarTree.getroot()

    # Verifying Correct Root element.
    verifyNodeNameOrFail(grammarElem, GrammarTag)

    for elem in grammarElem.iterchildren():
        if elem.tag == RuleTag:
            grammarDoc.rules.append(buildRule(elem)) 

        elif elem.tag == TagRuleTag:
            grammarDoc.rules.append(buildTagRule(elem)) 

        elif elem.tag == AttrRuleTag:
            grammarDoc.rules.append(buildAttrRule(elem))

        else:
            unexpectedOrUnknownTag(elem)

    return grammarDoc
    
if __name__ == "__main__":
    # print "Testing"
    grmmr = loadGrammar("/home/brian/Projects/srcML/doc/srcMLDocGen/DocData/Grammar/LanguageGrammar.xml")
    # print "Test Completed"
    assert grmmr != None, "Didn't pass test"
    out = cStringIO.StringIO()
    grmmr.toEBNFText(out)
    print out.getvalue()


# class RuleGrmr: pass
# class TagRuleGrmr: pass
# class AttrRuleGrmr: pass
# class GrammarGrmr: pass
# class TagInfoGrmr: pass
# class RefGrmr: pass
# class OptionalGrmr: pass
# class ZeroOrMoreGrmr: pass
# class OneOrMoreGrmr: pass
# class ChoiceGrmr: pass
# class TagGrmr: pass
# class TextGrmr: pass
# class LiteralGrmr: pass

# class GrammarContentHandler(ContentHandler):
#     def __init__(self, documentLocator, contentHandlerStack, validChildElemNames = set(["ZeroOrMore", "OneOrMore", "Choice", "Ref", "Text", "Literal", "Tag" ,"Optional", "Choice"])):
#         self.locator = documentLocator
#         self.handlerStack = contentHandlerStack
#         self.validChildren = validChildElemNames

#     def returnResult(self, value, tagName):
#         current = self.handlerStack.pop(-1)
#         self.handlerStack[-1].recieveResult(value, current, tagName)

#     #   this needs to be implemented by another 
#     #   GrammarContentHandler in order to receive 
#     #   results from other handlers
#     #   further down the stack.
#     def recieveResult(self, value, previousContentHandler, tagName):
#         raise Exception("Current Content handler can't receive results.")

#     #   Creates an instance of a type based on a tag name 
#     #   (only of those within validChildElemName are allowed)
#     #   and pushes that into the stack and creates the correct instance of that
#     #   element then invokes the startElementNS for that element with the given
#     #   parameters
#     def createPushAndCall(self, name, qname, attrs):
#         instance = None
#         if qname not in self.validChildren:
#             raise Exception("Invalid element: {0}. At Line: {1} Col: {2}".format(qname, self.locator, self.locator))
#         else:
#             target = getattr(GrammarHandlerTypes, GrammarHandlerTypes.handlerToTypeName[qname])
#             instance = target(self.locator, self.handlerStack)
#             self.handlerStack.append(instance)
#             instance.startElementNS(name, qname, attrs)

#     def startElementNS(self, name, qname, attributes):
#         self.createPushAndCall(name, qname, attributes)



        
# class GrammarHandlerTypes:
#     handlerToTypeName = {
#         "TagRule":"TagRuleHandler",
#         "AttrRule":"AttrRuleHandler",
#         "Rule":"RuleHandler",
#         "Optional":"OptionalGrammarHandler",
#         "ZeroOrMore":"ZeroOrMoreGrammarHandler",
#         "Choice":"ChoiceGrammarHandlere",
#         "Ref":"RefGrammarHandler",
#         "Text": "TextGrammarHandler",
#         "Literal": "LiteralGrammarHandler",
#         "Tag":"TagGrammarHandler"
#     }

#     class TagRuleHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.tagRule = TagRuleGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             if qname == "TagRule":
#                 self.returnResult(self.tagRule, qname)


#     class AttrRuleHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.attrRule = AttrRuleGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             if qname == "AttrRule":
#                 self.returnResult(self.attrRule, qname)


#     class RuleHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.rule = RuleGrmr

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             if qname == "Rule":
#                 self.returnResult(self.rule, qname)


#     class TagGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack, set([]))
#             self.tag = TagGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.tag, qname)

#         def characters(self, data):
#             pass


#     class OptionalGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.optional = OptonalGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.optional, qname)

#         def characters(self, data):
#             pass

#     class ZeroOrMoreGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.zeroOrMore = ZeroOrMoreGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.zeroOrMore, qname)

#         def characters(self, data):
#             pass

#     class OneOrMoreGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.oneOrMore = OneOrMoreGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.oneOrMore, qname)

#         def characters(self, data):
#             pass

#     class ChoiceGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack)
#             self.choice = ChoiceGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.choice, qname)

#         def characters(self, data):
#             pass

#     class RefGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack, set([]))
#             self.ref = RefGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.ref, qname)

#         def characters(self, data):
#             pass

#     class TextGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack, set([]))
#             self.text = TextGrmr()

#         def recieveResult(self, value, previousContentHandler, tagName):
#             pass

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.text, qname)

#         def characters(self, data):
#             pass

#     class LiteralGrammarHandler(GrammarContentHandler):
#         def __init__(self, documentLocator, contentHandlerStack):
#             GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack, set([]))
#             self.literal = LiteralGrmr()

#         def startElementNS(self, name, qname, attributes):
#             pass

#         def endElementNS(self, ns_name, qname):
#             self.returnResult(self.literal, qname)


# class GrammarTagGrammarHandler(GrammarContentHandler):
#     def __init__(self, documentLocator, contentHandlerStack, initialSAXContentHandler):
#         GrammarContentHandler.__init__(self, documentLocator, contentHandlerStack, set(["Rule", "TagRule", "AttrRule"]))
#         self.grammar = GrammarGrmr()
#         self.baseParser = initialSAXContentHandler

#     def recieveResult(self, value, previousContentHandler, tagName):
#         pass

#     def startElementNS(self, name, qname, attrs):
#         if qname != "Grammar":
#             self.createPushAndCall(name, qname, attrs)

#     def endElementNS(self, ns_name, qname):
#         self.baseParser.grammar = self.grammar
#         self.handlerStack.pop(-1)

# class GrammarHandler(ContentHandler):
#     def __init__(self):
#         self.locator = None
#         self.grammar = None
#         self.grammarStack = []

#     def setDocumentLocator(self, locator):
#         print dir(locator)
#         print locator == None
#         self.locator = locator

#     def startDocument(self):
#         print "Starting Document!"

#     def endDocument(self):
#         print "Parsed Document"

#     def startElementNS(self, name, qname, attributes):
#         if len(self.grammarStack) == 0:
#             self.grammarStack.append(GrammarTagGrammarHandler(self.locator, self.grammarStack, self))
#         self.grammarStack[-1].startElementNS(name, qname, attributes)

#     def endElementNS(self, nsName, qname):
#         if len(self.grammarStack) != 0:
#             self.grammarStack[-1].endElementNS(nsName, qname)
#         else:
#             print "Called endElementNS when empty"

#     def characters(self, data):
#         pass


# def loadGrammar(fileName):
#     handler = GrammarHandler()
#     lxml.sax.saxify(ET.parse(fileName), handler)
#     print "Processed File"
#     return handler.grammar
#     pass