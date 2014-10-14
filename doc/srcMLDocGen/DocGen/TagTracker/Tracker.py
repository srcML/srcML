import os, sys
import xml.sax.saxutils as saxutils

# A reference to a tag.
class TagReference:
    def __init__(self, pageUrl ="", inPageAnchor = "", namespacePrefix="", tagName="", attrDict=dict()):
        self.page = pageUrl
        self.anchor = inPageAnchor
        self.ns = namespacePrefix
        self.tag = tagName
        self.attrs = attrDict

    def getFormattedTag(self):
        tag = "<" 
        tag += "{0}:{1}".format(self.ns, self.tag)
        if len(self.attrs) > 0:
            tag += " " + " ".join(["{0}=\"{1}\"".format(k, v) for k, v in self.attrs.items()])
        tag +=">"
        return tag

    def getFormatedUrl(self):
        return "{0.page}#{0.anchor}".format(self)

    def __str__(self):
        if len(self.attrs)> 0:
            return "({0.page}, {0.anchor}, {0.ns}, {0.tag}, {1})".format(self, " ".join(["{0}=\"{1}\"".format(k, v) for k, v in self.attrs.items()]))
        else:
            return "({0.page}, {0.anchor}, {0.ns}, {0.tag})".format(self)

# the current tracker data.
class TrackerData:
    def __init__(self):
        self.tagReferences = []
        self.curPage = ""
        self.curAnchor = ""
        self.tagNamesToPages = dict()
        

    def recordTag(self, tagNs, tagName, attrDict):
        self.tagReferences.append(TagReference(self.curPage, self.curAnchor, tagNs, tagName, attrDict))
        currentTag = self.tagReferences[-1]
        tag = currentTag.getFormattedTag()
        if tag not in self.tagNamesToPages:
            self.tagNamesToPages[tag] = set([currentTag.getFormatedUrl()])
        else:
            self.tagNamesToPages[tag].add(currentTag.getFormatedUrl())

    def dumpTrackerRefs(self, out):
        for ref in self.tagReferences:
            out.write(str(ref) + "\n")

    def printDump(self):
        for ref in self.tagReferences:
            print ref

    def dumpByTagToHTML(self, out):
        out.write("<html>\n")
        out.write("    <body>\n")
        for tag, links in self.tagNamesToPages.items():
            out.write("<div class=\"tagDisplay\">\n")
            out.write("<h3>")
            out.write(saxutils.escape(tag))
            out.write("</h3>\n<hr/>\n")
            out.write("<ul>\n")
            for link in links:
                out.write("<li><a href=\"{0}\">{0}</a></li>\n".format(link))
            out.write("</ul>\n")
            out.write("</div>")

        out.write("    </body>\n")
        out.write("<html>\n")
# Static global Variables
trackerData = TrackerData()


# public interface for the tracker data.
def setCurrentPageURL(pageUrl):
    trackerData.curPage = pageUrl

def setCurrentAnchor(anchorId):
    trackerData.curAnchor = anchorId

def recordTag(tagNs="", tagName="", attrDict=dict()):
    trackerData.recordTag(tagNs, tagName, attrDict)
