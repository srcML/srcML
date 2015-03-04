#!/usr/bin/python

import os, sys, traceback, re
import django
from django.conf import settings
from django.template import Template, Context, loader
from DocGen import *
from DocGen.ValidationChecker import *
from DocGen.templatetags import *
import DocGen.TagTracker

# -------------------------------------------------
#                     Main
# -------------------------------------------------

class PageLink:
    def __init__(self, pageTitle, pageLink):
        self.title = pageTitle
        self.link = pageLink

def genMainPage(maingPageName, pageLinks, nav):
    out = open(maingPageName, "w")
    fileTemplate = loader.get_template("MainPage.html")
    pageLinks.sort(key=lambda x: x.title)
    page = fileTemplate.render(Context({"pageLinks": pageLinks, "nav":nav}))
    out.write(page)
    out.close()    

def genDocFile(docConfig, nav):
    global pageLinks
    pageLinks.append(PageLink(docConfig.title, docConfig.outputFileName))
    out = open(docConfig.outputFileName, "w")
    fileTemplate = loader.get_template("DefaultPage.html")
    page = fileTemplate.render(Context({"doc": docConfig, "pageName" : docConfig.outputFileName, "grammarFileName" : grammarOutputFileName, "nav":nav}))
    out.write(page)
    out.close()

def generateSrcMLGrammar(fileName, grmmr, nav):
    global pageLinks
    pageLinks.append(PageLink("srcML Grammar", fileName))
    out = open(fileName, "w")
    fileTemplate = loader.get_template("Grammar.html")
    page = fileTemplate.render(Context({"doc": grmmr, "title": "srcML Grammar", "nav":nav}))
    out.write(page)
    out.close()

def getDocIndexFileName(docConfig):
    splitName = docConfig.outputFileName.split(".")
    splitName.insert(-1, "index")
    return ".".join(splitName)

def genLangSupportFile(langSupport, nav):
    global pageLinks
    # print langSupport
    pageLinks.append(PageLink("srcML Language Support", langSupport.outputFile))
    out = open(langSupport.outputFile, "w")
    fileTemplate = loader.get_template("LanguageSupport.html")
    page = fileTemplate.render(
        Context({
            "doc" : langSupport,
            "title" : "srcML Language Support",
            "nav" : nav
        })
    )
    out.write(page)
    out.close()

def genXPathExtFuncPage(extFuncDocInfo, grammarOutputFileName, nav):
    pageLinks.append(PageLink("srcML XPath Extension Functions", extFuncDocInfo.outputFile))
    out = open(extFuncDocInfo.outputFile, "w")
    fileTemplate = loader.get_template("XPathExtFunc.html")
    page = fileTemplate.render(
        Context({
            "doc" : extFuncDocInfo,
            "title" : "srcML XPath Extension Functions",
            "gramarPageName":grammarOutputFileName,
            "nav" : nav
        })
    )
    out.write(page)
    out.close()

#
# Generate documentation index for a language.
#
def genDocIndex(docConfig, nav):
    global pageLinks
    indexFileName = getDocIndexFileName(docConfig)
    
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
    page = fileTemplate.render(Context({"indexItems": sortedIndexItems, "pageTitle": docConfig.title, "nav":nav}))
    out.write(page)
    out.close()

def writeAllTagNames():
    out = open("taglisting.txt", "w")
    tagListTemp = list(getTagListing())
    tagListTemp.sort()
    for tag in tagListTemp:
        out.write("{0}\n".format(tag))
    out.close()


DocConfigFileName = "DocConfig.xml"
TagDocFileName = "TagDoc.xml"
grammarFile = "LanguageGrammar.xml"
grammarOutputFileName = "srcMLGrammar.html"
languageSupportFileName = "LanguageSupportInfo.xml"
xpathExtFuncDataFileName = "XPathExtensionFunctions/XPathExtFuncData.xml"

# languageSupportOutputFileName = "LanguageSupportInfo.html"
pageLinks = []

class DocConfigPageLink:
    def __init__(self):
        self.navTitle = ""
        self.pageURL = ""
        self.pageName = ""
        self.indexPageURL = ""
        self.indexPageName = ""

class Navigation:
    def __init__(self):
        self.home = PageLink("","")
        self.menuNavigation = []
        self.grammarPage = PageLink("","")
        self.langSupport = PageLink("","")
        self.xpathExtFunc = PageLink("", "")

class PagesToGenerate:
    def __init__(self):
        self.languageGrammar = None
        self.mainPageTitle = "srcML Language Documentation"
        self.mainPageURL = "index.html"
        self.docConfigs = []
        self.nav = Navigation()
        self.languageSupportInfo = None
        self.xpathExtFuncInfo = None

    def buildNavigation(self):
        self.docConfigs.sort(key=lambda x:x.title)
        self.nav.home.title = "Directory"
        self.nav.home.link = "index.html"
        self.nav.grammarPage = PageLink("srcML Grammar", grammarOutputFileName)
        self.nav.langSupport = PageLink("Language Support", self.languageSupportInfo.outputFile)
        self.nav.xpathExtFunc = PageLink("XPath Extension Functions", self.xpathExtFuncInfo.outputFile)
        for docConfig in self.docConfigs:
            configMenu = DocConfigPageLink()
            configMenu.navTitle = docConfig.navTitle
            configMenu.pageURL = docConfig.outputFileName
            configMenu.pageName = docConfig.navSubTitle
            configMenu.indexPageURL = getDocIndexFileName(docConfig)
            configMenu.indexPageName = "Index"
            self.nav.menuNavigation.append(configMenu)

    def genPages(self):
        self.buildNavigation()

        print 80*"-"
        print "Generating Language Support "
        print "Beginning HTML generation"
        genLangSupportFile(self.languageSupportInfo, self.nav)
        print "HTML Generation Complete"

        print 80*"-"
        print "Generating XPath Extension Function Documentation"
        print "Beginning HTML generation"
        genXPathExtFuncPage(self.xpathExtFuncInfo, grammarOutputFileName, self.nav)
        print "HTML Generation Complete"

        print 80*"-"
        print "Generated Grammar Page"
        print "Beginning HTML generation"
        generateSrcMLGrammar(grammarOutputFileName, languageGrammar, self.nav)
        print "HTML Generation Complete"

        for docConfig in self.docConfigs:
            print 80*"-"
            print "Page Generation"
            print "Beginning HTML Generation"
            genDocFile(docConfig, self.nav)
            print "HTML Generation Complete"
            print "Generating HTML Index"
            genDocIndex(docConfig, self.nav)
            print "Index HTML Generation Complete"
        print "-" * 80
        print "Writing main page"
        genMainPage("index.html", pageLinks, self.nav)


pagesToGenerate = PagesToGenerate()
grammarToGenerate = None

if __name__ == "__main__":

    if not settings.configured:
        django.conf.settings.configure(DEBUG=True, TEMPLATE_DEBUG=True, TEMPLATE_DIRS=("Templates", ), INSTALLED_APPS=("DocGen",))

    django.setup()

    print "-"*80
    print "Language Support"
    pagesToGenerate.languageSupportInfo = loadLanguageSupport("DocData/" + languageSupportFileName)

    print "-"*80
    print "XPath Extension Functions"
    pagesToGenerate.xpathExtFuncInfo = loadXPathExtFuncData(os.path.join("DocData", xpathExtFuncDataFileName))

    print "-"*80
    print "Loading Grammar"
    grammarRoot = "DocData/Grammar/"
    print "Located a Language Grammar Document (LanguageGrammar.xml) in", grammarRoot
    grammarPath = os.path.join(grammarRoot, grammarFile)

    if not os.path.exists(grammarPath):
        raise Exception("missing Grammar file at location:", grammarPath)
    try:
        languageGrammar = loadGrammar(grammarPath)
        pagesToGenerate.languageGrammar = languageGrammar
        setValidator(languageGrammar)

    except Exception as e:
        print "Failed with exception: ", traceback.format_exc(e)
    print "Loading grammar complete"

    for root, dirs, files in os.walk(os.path.abspath("./DocData")):
        if os.path.basename(root) == "DocData":
            continue

        if DocConfigFileName in files:
            print "-" * 80
            print "Located a Page Creation Document (DocConfig.xml) in", root
            try:
                docConfig = loadXmlDocFile(root, DocConfigFileName, True)
                pagesToGenerate.docConfigs.append(docConfig)
            except Exception as e:
                print "Failed with exception: ", traceback.format_exc(e)

    print "Generating pages"
    pagesToGenerate.genPages()

    print "-" * 80
    print "Writing tag names"
    writeAllTagNames()
    print "-" * 80
    print "Dumping tracker info"
    tempOutFile = open("tagLocationDoc.html", "w")
    DocGen.TagTracker.Tracker.trackerData.dumpByTagToHTML(tempOutFile)
    tempOutFile.close()
    # print "-" * 80
    # print "Dumping validation report"
    # validationReportFile = open("ValidationReport.txt","w")
    # validationManager.makeReport(validationReportFile)
    # validationReportFile.close()
    print "-" * 80