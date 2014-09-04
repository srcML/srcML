import os, sys, lxml, cStringIO, itertools
import lxml.etree as ET
from LoadData import *

class HowToReadDocumentation:
    def __init__(self):
        self.title = ""
        self.desc = ""
        self.entries = []
    def __str__(self):
        return """HowToReadDocumentation
  Title: {0.title}
  Description: {0.desc}
  Entries:
{1}
""".format(self, "\n".join([str(e) for e in self.entries]))

class HowToReadEntry:
    def __init__(self):
        self.title = ""
        self.desc = ""
        self.grammarExamples = []
    def __str__(self):
        return """        Entry
          Title: {0.title}
          Desc: {0.desc}
          Title: {1}
""".format(self, "\n".join([str(gEx) for gEx in self.grammarExamples]))

class HowToReadExample:
    def __init__(self):
        self.title = ""
        self.type = ""
        self.text = ""

# Tags
HowToReadTag = "HowToRead"
# exampleTag
EntryTag = "Entry"
typeAttr = "type"
# Attributes

# Used to read the howtoread documentations
# into a data structure and return it to the user.
def buildHowToReadDocumentation(fileName):

    def buildEntry(entryElem):
        entry = HowToReadEntry()
        entry.title = getAttribOrFail(entryElem, titleAttr)
        for elem in entryElem.iterchildren():
            if elem.tag == DescTag:
                entry.desc = extractSubText(elem).strip()

            elif elem.tag == ExampleTag:
                example = HowToReadExample()
                example.title = getAttribOrDefault(elem, titleAttr, "").strip()
                example.type = getAttribOrFail(elem, typeAttr).lower().strip()
                if example.type != "pre" and example.type != "svg":
                    raise Exception("Invalid type for grammar example. expected pre OR svg got: {0}".format(example.type) + formatElementErrorMsg(elem))
                example.text = extractSubText(elem).strip()
                entry.grammarExamples.append(example)

            else:
                unexpectedOrUnknownTag(elem)
        return entry

        

    howToReadTree = ET.parse(fileName)
    howToReadDoc = HowToReadDocumentation()
    rootElement = howToReadTree.getroot()

    # Verifying part of document format.
    verifyNodeNameOrFail(rootElement, HowToReadTag)

    # extracting title.
    howToReadDoc.title = getAttribOrFail(rootElement, titleAttr)


    for elem in rootElement.iterchildren():
        if elem.tag == DescTag:
            howToReadDoc.desc = extractSubText(elem).strip()

        elif elem.tag == EntryTag:
            howToReadDoc.entries.append(buildEntry(elem))

        else:
            unexpectedOrUnknownTag(elem)

    # print howToReadDoc
    return howToReadDoc