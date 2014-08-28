import sys

class Element(object):
    """ Stores information about tags related to this description. """
    def __init__(self):
        self.tag = ""
        self.ns = ""
        self.desc = ""
        self.markupOptionsRequired = []
        self.attributeStr = ""

    def QName(self):
        nameFormat = ""
        if self.ns == "" or self.ns == None:
            nameFormat = "&lt;{0.tag}"
        else:
            nameFormat = "&lt;{0.ns}:{0.tag}"

        if self.attributeStr != "":
            nameFormat += " {0.attributeStr}"
        nameFormat += "&gt;"
        return nameFormat.format(self)

class Subelement(Element):
    """ Stores information related to sub elements. """
    def __init__(self):
        super(Subelement, self).__init__()

class XPathExample:
    def __init__(self):
        self.desc = ""
        self.xpath = ""

class Example:
    def __init__(self):
        self.title = ""
        self.sourceCodeFile = "" 
        self.sourceCode = ""
        self.srcML = ""

class DocEntry:
    """ This is an entry within the documentation. """
    def __init__(self):
        self.title = ""
        self.shortTitle = ""
        self.desc = ""
        self.elements = []
        self.subelements = []
        self.examples = []
        self.xpathQueries = []

    def linkName(self):
        return "-".join(self.title.strip().replace("#","").replace(",", "").lower().split())


class OperatorEntry:
    """ This is an operator entry within the document that tell me to extract
        an operator from the srcML example.
    """
    def __init__(self):
        self.title = ""
        self.op = ""
        self.exampleFile = ""
        self.srcML = ""
        self.sourceCode = ""


class Category:
    def __init__(self):
        self.title = ""
        self.entries = []
        self.operators = []


class DocConfig:
    """ Manages all pages related to a single topic/language."""
    def __init__(self):
        self.srcMLLanguage = "NOT SET"
        self.title = ""
        self.outputFileName = ""
        self.desc = ""
        self.categories = []
        self.entries = []
        self.operators = []

    def hasOperators(self):
        for cat in self.categories:
            if len(cat.operators) > 0:
                return True
        if len(self.operators)>0:
            return True
        return False            


class IndexEntry:
    """ An entry within the index of links that's sorted via alphabetic order instead of
        logical related element ordering
    """
    def __init__(self):
        self.title = ""
        self.shortTitle = ""
        self.category = ""
        self.isOperator = False
        self.language = ""
        self.element = []
        self.subelements = []
        self.linkName = ""
        self.basePageName = ""
        self.op = ""

    def __lt__(self, other):
        return self.title.lower() < other.title.lower()

    def indexLetter(self):
        return self.title[0].upper()


# class TagExample:
#     def __init__(self):
#         self.title = ""
#         self.text = ""    

# class TagAttr:
#     def __init__(self):
#         self.name = ""    
#         self.valueDesc = ""
#         self.languages = []
#         self.desc = ""
#         self.exampleValue = ""

# class NsEntry:
#     """ Documentation for an XML namespaces within srcML. """
#     def __init__(self):
#         self.prefix = ""
#         self.url = ""
#         self.desc = ""

# class TagUseCase:
#     def __init__(self):
#         self.title = ""
#         self.languages = []
#         self.desc = ""
#         self.attrs = []
#         self.examples = []
#         self.grammar = ""
#         self.subelements = []

# class TerminalValue:
#     def __init__(self):
#         self.value = ""
#         self.languages = []

# class TerminalUseCase:
#     """ Represents a use case for which an element of the grammar that don't have child elements, only text. """
#     def __init__(self):
#         self.title = ""
#         self.desc = ""
#         self.attrs = []
#         self.languages = []
#         self.grammar = ""
#         self.values = []

# class TagRef:
#     def __init__(self):
#         self.ns = ""
#         self.tag = ""

#     def comparableTagName(self):
#         out = ""
#         if self.ns != "":
#             out +="{0.ns}:"
#         out += "{0.tag}"
#         return out.format(self)
        
#     def link(self):
#         return self.ns + "_" + self.tag


class URLRefLink:
    def __init__(self):
        self.title = ""
        self.url = ""



# class TagInfo:
#     """ Contains information about a single tag. """
#     def __init__(self):
#         self.ns = ""
#         self.tag = ""
#         self.desc = ""
#         self.useCases = []
#         self.refs = []
#         self.parentElements = []
#         self.subelements = []

#     def QNameTagStyle(self):
#         out = "<"
#         if self.ns != "":
#             out +="{0.ns}:"
#         out += "{0.tag}>"
#         return out.format(self)

#     def link(self):
#         return self.ns + "_" + self.tag

#     def comparableTagName(self):
#         out = ""
#         if self.ns != "":
#             out +="{0.ns}:"
#         out += "{0.tag}"
#         return out.format(self)

# class RelaxNGEntry:
#     def __init__(self):
#         self.title = ""
#         self.grammarText = ""
#         self.desc = ""

#     def link(self):
#         return "-".join(self.title.strip().replace("#","").replace(",", "").replace("'","").lower().split())

# class RelaxNGHelpDoc:
#     def __init__(self):
#         self.title = ""
#         self.desc = ""
#         self.entries = []

# class TagDoc:
#     """ Contains information about a all tags within the system. """
#     def __init__(self):
#         self.title = ""
#         self.outputFileName = ""
#         self.relaxNGHelp = None
#         self.namespaces = []
#         self.tags = []
#         self.desc = ""
#         self.languages = list(["C", "C++", "C#", "Java"])
        
#     def maxLanguageCount(self):
#         self.languageMax = tagsMax
#         return 

#     def link(self):
#         return self.outputFileName
