#!/usr/bin/python

import os, sys, traceback
import django
from django.conf import settings
from django.template import Template, Context, loader
from DocGen import *

# -------------------------------------------------
#                     Main
# -------------------------------------------------

def genDocFile(docConfig):
    # How to generate the template.
    out = open(docConfig.outputFileName, "w")
    fileTemplate = loader.get_template("DefaultPage.html")
    page = fileTemplate.render(Context({"doc": docConfig}))
    out.write(page)
    out.close()

#
# Generate documentation index for a language.
#
def genDocIndex(docConfig):
	splitName = docConfig.outputFileName.split(".")
	splitName.insert(-2, "index")
	indexFileName = ".".join(splitName)
	print indexFileName
    # out = open(indexFileName, "w")



DocConfigFileName = "DocConfig.xml"

if __name__ == "__main__":

    if not settings.configured:
        django.conf.settings.configure(DEBUG=True, TEMPLATE_DEBUG=True, TEMPLATE_DIRS=("Templates", ), INSTALLED_APPS=("DocGen",))

    for root, dirs, files in os.walk(os.path.abspath("./DocData")):
        if os.path.basename(root) == "DocData":
            continue

        if DocConfigFileName in files:
            print "-" * 80
            print "Located a Page Creation Document in", root
            try:
                docConfig = loadXmlDocFile(root, DocConfigFileName, True)
                if docConfig != None:
                    print "Beginning HTML Generation"
                    genDocFile(docConfig)
                    print "HTML Generation Complete"
                    print "Generating HTML Index"
                    print "Index HTML Generation COmplete"
            except Exception as e:
                print "Failed with exception: ", traceback.format_exc(e)
        
    print "-" * 80