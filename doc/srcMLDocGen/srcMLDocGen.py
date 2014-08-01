#!/usr/bin/python
#   srcMLDocGen is used to generate markdown files from srcML so that they can be
#   easily formatting and made to look nice without a lot of effort.
#
#   use grip to evaluate the created.md files.

import os, sys, traceback
import django
from django.conf import settings
from django.template import Template, Context, loader
from DocGen import *

# -------------------------------------------------
#                     Main
# -------------------------------------------------

DocConfigFileName = "DocConfig.xml"
def genDocFile(docConfig):
	# How to generate the template.
	out = open(docConfig.outputFileName, "w")
	fileTemplate = loader.get_template("DefaultPage.html")
	page = fileTemplate.render(Context({"doc": docConfig}))
	out.write(page)
	out.close()
	

if __name__ == "__main__":

    if not settings.configured:
        django.conf.settings.configure(DEBUG=True, TEMPLATE_DEBUG=True, TEMPLATE_DIRS=("Templates", "."))

    for root, dirs, files in os.walk(os.path.abspath("./DocData")):
        if os.path.basename(root) == "DocData":
            continue

        # print os.path.basename(root)
        if DocConfigFileName in files:
            print "-" * 80
            print "Located a Page Creation Document in", root
            try:
                docConfig = loadXmlDocFile(root, DocConfigFileName, True)
                if docConfig != None:
                	print "Beginning HTML Generation"
                	genDocFile(docConfig)
                	print "HTML Generation"
            except Exception as e:
                print "Failed with exception: ", traceback.format_exc(e)
        
    print "-" * 80
