from django.conf import settings
from django import template
import os, sys, re
import xml.sax.saxutils as SAXUtils
import cStringIO
from DocGen import *
import DocGen as DG

register = template.Library()

@register.filter(name = "makeTagLink")
def makeTagLink(tagToLink):
    if isinstance(tagToLink, str):
        return "tag_" + tagToLink.replace(":", "_")
    else:
        return "tag_{0}{1}".format("" if tagToLink.ns == "" else (tagToLink.ns + "_"), tagToLink.tag)

@register.filter(name = "makeRefLink")
def makeRefLink(ruleToLink):
    return "ref_{0}".format(ruleToLink.name)

@register.filter(name = "makeRuleLink")
def makeRuleLink(rule):
    return "rule_{0}".format(rule.name)

@register.filter(name = "makeAttrRuleLink")
def makeAttrRuleLink(attrRule):
    return "attr_{0}".format(attrRule.name)

@register.filter(name = "makeDisplayableTag")
def makeDisplayableTag(toDisplay):
    if isinstance(toDisplay, str):
        return toDisplay
    else:
        return "{0}{1}".format("" if toDisplay.ns == "" else (toDisplay.ns + ":"), toDisplay.tag)



# class RuleTagInfo:
#     def __init__(self):
#         self.tag = ""
#         self.ns = ""
#         self.attrs = None

# class AttrUseInfo:
#     def __init__(self):
#         self.name = ""
#         self.isOptional = False
#
# Terminal Grammar nodes
# class ParamUse(GrammarDef):
# class RuleRef(GrammarDef):
# class Literal(GrammarDef):
# class Text(GrammarDef):
# class Number(GrammarDef):
# class Empty(GrammarDef):
# class Identifier(GrammarDef):

# Grammar nodes with sub-expressions
# class TagExpr(GrammarExpr):
# class OptionalExpr(GrammarExpr):
# class ZeroOrMoreExpr(GrammarExpr):
# class GroupExpr(GrammarExpr):
# class OneOrMoreExpr(GrammarExpr):
# class ChoiceExpr(GrammarExpr):


@register.filter(name="BuildAnnotatedEBNF", is_safe=True)
def BuildAnnotatedEBNF(grammarRule):

    # Utility functions.
    def createTagStart(tagName, tagClass, toolTip = ""):
        if toolTip != "":
            out.write("<{0} class=\"{1}\" title=\"{2}\">".format(tagName, tagClass, toolTip))
        else:
            out.write("<{0} class=\"{1}\">".format(tagName, tagClass))

    def createTagEnd(tagName):
        out.write("</{0}>".format(tagName))

    def writeLink(url, cls, txt):
        out.write("<a href=\"#{0}\" class=\"{1}\">{2}</a>".format(url, cls, txt))

    def writeSubExprSuffix(value):
        out.write("<span class=\"gExprSuffix\">{0}</span>".format(value))

    def writeSubExpr(subExprNode, tagClass, separator, writeParens=True):
        if writeParens:
            if len(subExprNode.subExprs) > 1:
                tagClass += " gHasParens"
            if len(subExprNode.subExprs) > 5:
                tagClass += " gGeaterThenFive"

        createTagStart("span", tagClass)
        if writeParens:
            if len(subExprNode.subExprs) > 1:
                out.write("(")
        for expr in subExprNode.subExprs:
            dispatchGrammarCreation(expr)
            if expr != subExprNode.subExprs[-1]:
                if separator == "" or separator == " ":
                    createTagStart("span", "gSpaceSeparator")
                else:
                    createTagStart("span", "gSeparator")
                    out.write("{0}".format(separator))
                createTagEnd("span")
        if writeParens:
            if len(subExprNode.subExprs) > 1:
                out.write(")")
        createTagEnd("span")

    def dispatchGrammarCreation(node):
        if isinstance(node, DG.ParamUse):
            createTagStart("span", "gParamUse")
            out.write("$" + node.name)
            createTagEnd("span")

        elif isinstance(node, DG.RuleRef):
            createTagStart("span", "gRef")
            writeLink(makeRuleLink(node), "gRuleRef", node.name)
            if node.args != None:
                if len(node.args) > 0:
                    createTagStart("span", "gArgs")
                    out.write("[")
                    for arg in node.args:
                        createTagStart("span", "gArg")
                        dispatchGrammarCreation(arg)
                        createTagEnd("span") # end gArg
                        if arg != node.args[-1]:
                            out.write(",")
                    out.write("]")
                    createTagEnd("span") # end gArgs
            createTagEnd("span") # end gRef

        elif isinstance(node, DG.Literal):
            createTagStart("span", "gLiteral", "The text within the &quot;s.")
            out.write("\"{0}\"".format(node.value))
            createTagEnd("span")

        elif isinstance(node, DG.NotImplementedYet):
            createTagStart("span", "gNotImplemented gKW", "This has yet to be implemented yet!")
            out.write("NotImplementedYet")
            createTagEnd("span")

        elif isinstance(node, DG.Operator):
            createTagStart("span", "gOperator gKW", "Any operator.")
            out.write("Operator")
            createTagEnd("span")

        elif isinstance(node, DG.Text):
            createTagStart("span", "gText gKW", "Any text, this may include whitespace and special characters.")
            out.write("Text")
            createTagEnd("span")

        elif isinstance(node, DG.Number):
            createTagStart("span", "gNumber gKW", "Any floating point or integer representation.")
            out.write("Number")
            createTagEnd("span")

        elif isinstance(node, DG.Empty):
            createTagStart("span", "gEmpty gKW", "This element has no content.")
            out.write("Empty")
            createTagEnd("span")

        elif isinstance(node, DG.Identifier):
            createTagStart("span", "gIdentifier gKW", "Any valid identifier in any of the supported languages.")
            out.write("Identifier")
            createTagEnd("span")

        elif isinstance(node, DG.TagExpr):
            createTagStart("span", "gTag")
            createTagStart("span", "gTagName")
            out.write("&lt;")
            writeLink(makeTagLink(node.tagInfo), "gTagLink", DG.getComparableTagName(node.tagInfo))
            out.write("&gt;")
            createTagEnd("span") # end tagName
            if node.tagInfo.attrs != None:
                createTagStart("span", "gAttrs")
                out.write("(")
                for attr in node.tagInfo.attrs:
                    createTagStart("span", "gAttr")
                    writeLink(makeAttrRuleLink(attr), "gAttrLink", attr.name)
                    createTagEnd("span") # end attr
                    if attr.isOptional:
                        out.write("?")
                    if attr != node.tagInfo.attrs[-1]:
                        out.write(",")
                out.write(")")
                createTagEnd("span") # end attrs
            out.write("{")
            writeSubExpr(node, "gTagSubExpr", "", False)
            out.write("}")
            createTagEnd("span") # end tag

        elif isinstance(node, DG.OptionalExpr):
            writeSubExpr(node, "gOptional", "")
            writeSubExprSuffix("?")

        elif isinstance(node, DG.ZeroOrMoreExpr):
            writeSubExpr(node, "gZeroOrMore", "")
            writeSubExprSuffix("*")

        elif isinstance(node, DG.GroupExpr):
            writeSubExpr(node, "gGroup", "")

        elif isinstance(node, DG.OneOrMoreExpr):
            writeSubExpr(node, "gOneOrMore", "")
            writeSubExprSuffix("+")

        elif isinstance(node, DG.ChoiceExpr):
            writeSubExpr(node, "gChoice", "|")
            
        else:
            raise Exception("Unknown or unhandled grammar type: {0}".format(node.__class__.__name__))


    def ruleDispatch(rule):
        createTagStart("span", "gRule")
        createTagStart("span", "gRuleLHS")
        createTagStart("span", "gCurrentRuleName")
        out.write(rule.name)
        createTagEnd("span") # end currentRuleName
        if isinstance(rule, DG.Rule):
            pass
        elif isinstance(rule, DG.TagRule):
            createTagStart("span", "gTag")
            createTagStart("span", "gTagName")
            out.write("&lt;")
            writeLink(makeTagLink(rule.tagInfo), "gTagLink", DG.getComparableTagName(rule.tagInfo))
            out.write("&gt;")
            createTagEnd("span") # end tagName
            if rule.tagInfo.attrs != None:
                createTagStart("span", "gAttrs")
                out.write("(")
                for attr in rule.tagInfo.attrs:
                    createTagStart("span", "gAttr")
                    writeLink(makeAttrRuleLink(attr), "gAttrLink", attr.name)
                    createTagEnd("span") # end attr
                    if attr.isOptional:
                        out.write("?")
                    if attr != rule.tagInfo.attrs[-1]:
                        out.write(",&nbsp;")
                out.write(")")
                createTagEnd("span") # end attrs
            createTagEnd("span") # end tag

        elif isinstance(rule, DG.AttrRule):
            createTagStart("span", "gAttrName")
            out.write("@")
            writeLink(makeAttrRuleLink(rule), "gAttrLink", rule.attrName)
            createTagEnd("span") # end attr
        else:
            raise Exception("Unknown type of rule: {0}".format(rule.__class__.__name__))

        if rule.parameters != None:
            createTagStart("span", "gParameters")
            out.write("[")
            out.write(",".join([("<span class=\"gParam\">{0}</span>".format(param)) for param in rule.parameters]))
            out.write("]")
            createTagEnd("span") #end gParameters
        createTagEnd("span") #end ruleLHS
        out.write(":")
        # Constructing RHS
        createTagStart("span", "gRuleRHS")
        for node in rule.grammar:
            dispatchGrammarCreation(node)
            # out.write(" ")
        createTagEnd("span") #end ruleRHS
        createTagEnd("span") #end rule
        createTagStart("span", "gRuleEnd")
        out.write(";")
        createTagEnd("span") #end ruleEnd

    out = cStringIO.StringIO()

    ruleDispatch(grammarRule)

    return out.getvalue()

