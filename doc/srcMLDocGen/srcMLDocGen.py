#!/usr/bin/python

import os, sys, traceback
import django
from django.conf import settings
from django.template import Template, Context, loader
from DocGen import *
from DocGen.templatetags import *

# -------------------------------------------------
#                     Main
# -------------------------------------------------

class PageLink:
	def __init__(self, pageTitle, pageLink):
		self.title = pageTitle
		self.link = pageLink

def genElementsPage(tagDoc):
    global pageLinks
    out = open(tagDoc.outputFileName, "w")
    fileTemplate = loader.get_template("Tags.html")
    page = fileTemplate.render(Context({"doc": tagDoc}))
    pageLinks.append(PageLink(tagDoc.title, tagDoc.outputFileName))
    out.write(page)
    out.close()	

def genMainPage(maingPageName, pageLinks):
    out = open(maingPageName, "w")
    fileTemplate = loader.get_template("MainPage.html")
    pageLinks.sort(key=lambda x: x.title)
    page = fileTemplate.render(Context({"pageLinks": pageLinks}))
    out.write(page)
    out.close()	

def genDocFile(docConfig):
    global pageLinks
    pageLinks.append(PageLink(docConfig.title, docConfig.outputFileName))
    out = open(docConfig.outputFileName, "w")
    fileTemplate = loader.get_template("DefaultPage.html")
    page = fileTemplate.render(Context({"doc": docConfig}))
    out.write(page)
    out.close()

#
# Generate documentation index for a language.
#
def genDocIndex(docConfig):
    global pageLinks
    splitName = docConfig.outputFileName.split(".")
    splitName.insert(-1, "index")
    indexFileName = ".".join(splitName)

    # indexEntries = []
    lang = docConfig.srcMLLanguage
    pageName = docConfig.outputFileName
    indexDictionary = dict()

    for cat in docConfig.categories:
        catTitle = cat.title
        for entry in cat.entries:
            indexEntry = IndexEntry()
            indexEntry.title = entry.title
            indexEntry.shortTitle = entry.shortTitle
            indexEntry.category = catTitle
            indexEntry.isOperator = False
            indexEntry.language = lang
            indexEntry.element = entry.elements
            indexEntry.subelements = entry.subelements
            indexEntry.linkName = entry.linkName()
            indexEntry.basePageName = pageName
            if not indexEntry.indexLetter() in indexDictionary:
                indexDictionary[indexEntry.indexLetter()] = [indexEntry]
            else:
                indexDictionary[indexEntry.indexLetter()].append(indexEntry)

        for opEntry in cat.operators:
            indexEntry = IndexEntry()
            indexEntry.title = opEntry.title
            indexEntry.shortTitle = ""
            indexEntry.category = catTitle
            indexEntry.isOperator = True
            indexEntry.language = lang
            indexEntry.op = opEntry.op
            indexEntry.element = [Element()]
            indexEntry.element[0].tag="operator"
            indexEntry.element[0].ns="op"
            indexEntry.linkName = "operators"
            indexEntry.basePageName = pageName
            if not indexEntry.indexLetter() in indexDictionary:
                indexDictionary[indexEntry.indexLetter()] = [indexEntry]
            else:
                indexDictionary[indexEntry.indexLetter()].append(indexEntry)
    for entry in indexDictionary.items():
    	entry[1].sort()

    pageLinks.append(PageLink(docConfig.title + " Index", indexFileName))
    out = open(indexFileName, "w")
    fileTemplate = loader.get_template("IndexPage.html")
    sortedIndexItems = indexDictionary.items()
    sortedIndexItems.sort(key=lambda x: x[0].lower())
    page = fileTemplate.render(Context({"indexItems": sortedIndexItems, "pageTitle": docConfig.title}))
    out.write(page)
    out.close()

def writeAllTagNames():
	out = open("taglisting.txt", "w")
	for tag in getTagListing():
		out.write("{0}\n".format(tag))
	out.close()



DocConfigFileName = "DocConfig.xml"
TagDocFileName = "TagDoc.xml"
pageLinks = []
if __name__ == "__main__":

    if not settings.configured:
        django.conf.settings.configure(DEBUG=True, TEMPLATE_DEBUG=True, TEMPLATE_DIRS=("Templates", ), INSTALLED_APPS=("DocGen",))

    for root, dirs, files in os.walk(os.path.abspath("./DocData")):
        if os.path.basename(root) == "DocData":
            continue

        if DocConfigFileName in files:
            print "-" * 80
            print "Located a Page Creation Document (DocConfig.xml) in", root
            try:
                docConfig = loadXmlDocFile(root, DocConfigFileName, True)
                if docConfig != None:
                    print "Beginning HTML Generation"
                    genDocFile(docConfig)
                    print "HTML Generation Complete"

                    print "Generating HTML Index"
                    genDocIndex(docConfig)
                    print "Index HTML Generation Complete"
            except Exception as e:
                print "Failed with exception: ", traceback.format_exc(e)
        
        if TagDocFileName in files:
            print "-" * 80
            print "Located a Page Creation Document (TagDoc.xml) in", root
            try:
                tagDoc = loadTagDoc(root, TagDocFileName, True)
                print "Beginning HTML generation"
                genElementsPage(tagDoc)
                print "HTML Generation Complete"
            except Exception as e:
                print "Failed with exception: ", traceback.format_exc(e)	
    print "-" * 80
    print "Writing tag names"
    writeAllTagNames()
    print "-" * 80
    print "Writing main page"
    genMainPage("MainPage.html", pageLinks)
    print "-" * 80