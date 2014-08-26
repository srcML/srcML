from django.conf import settings
from django import template
import os, sys, re
import xml.sax.saxutils as SAXUtils
import cStringIO
from DocGen import *

register = template.Library()

@register.filter(name = "makeTagLink")
def makeTagLink(tagToLink):
    if isinstance(tagToLink, str):
        return "tag_" + tagToLink
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
