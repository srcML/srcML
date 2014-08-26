from django.conf import settings
from django import template
import os, sys, re
import xml.sax.saxutils as SAXUtils
import cStringIO
from DocGen import *

register = template.Library()

@register.filter(name = "makeTagLink")
def makeTagLink(tagToLink):
    return "tag_{0}{1}".format("" if tagToLink.ns == "" else (tagToLink.ns + "_"), tagToLink.tag)

@register.filter(name = "makeRefLink")
def makeTagLink(ruleToLink):
    return "ref_{0}".format(ruleToLink.name)

@register.filter(name = "makeDisplayableTag")
def makeDisplayableTag(toDisplay):
    return "{0}{1}".format("" if toDisplay.ns == "" else (toDisplay.ns + ":"), toDisplay.tag)
