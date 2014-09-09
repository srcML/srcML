import os, sys, lxml, cStringIO, itertools, re
import lxml.etree as ET
from LoadData import *

# Attempts to validate the example srcML so that if it changes I know
# which example failed and where.
rngExtMatch = re.compile(r"\.rng$", re.I)
class ValidationFailureReport:
    def __init__(self, validationLookUp, exceptionErrorMessage=""):
        self.exampleTitle = validationLookUp.currentExampleTitle
        self.exampleFileName = validationLookUp.currentExamplePath
        self.exampleIndex = validationLookUp.currentExampleIndex
        self.entryTitle = validationLookUp.currentEntryTitle
        self.docTitle = validationLookUp.currentDocTitle
        self.docConfigPath = validationLookUp.currentDocPath
        self.language = validationLookUp.currentsrcMLLang
        self.errorMessage = exceptionErrorMessage
        self.docToBeValidated = None

    def __str__(self):
        if self.docToBeValidated == None:
            return """Doc Title: {0.docTitle}
Doc Path: {0.docConfigPath}
srcML Lang: {0.language}
Entry: {0.entryTitle}
Example Index: {0.exampleIndex}
Example Title: {0.exampleTitle}
Example File: {0.exampleFileName}
Error Message: {0.errorMessage}""".format(self)
        else:
            return """Doc Title: {0.docTitle}
Doc Path: {0.docConfigPath}
srcML Lang: {0.language}
Entry: {0.entryTitle}
Example Index: {0.exampleIndex}
Example Title: {0.exampleTitle}
Example File: {0.exampleFileName}
Error Message: {0.errorMessage}
{1}""".format(self, ET.tostring(self.docToBeValidated))


class ValidationLookUp:
    def __init__(self):
        self.validator = None
        self.reports = []
        self.currentDocPath = ""
        self.currentDocTitle = ""
        self.currentsrcMLLang = ""
        self.currentEntryTitle = ""
        self.currentExampleIndex = ""
        self.currentExamplePath = ""
        self.currentExampleTitle = ""
        self.totalValidations = 0
        self.passedValidations = 0
        self.failedValidations = 0

    def validateAndReport(self, docToValidate):
        if self.validator == None:
            raise Exception("Missing validator. Grammar validation not set.")
        self.totalValidations += 1
        validatedDoc = ET.fromstring(ET.tostring(docToValidate, pretty_print=True))
        try:
            self.validator.assertValid(validatedDoc)
            self.passedValidations += 1
        except Exception as e:
            self.failedValidations += 1
            print "Validation Failure"
            print "    Entry: ", self.currentEntryTitle
            print "    File: ", self.currentExamplePath
            print "    ", str(e)
            report = ValidationFailureReport(self, exceptionErrorMessage=(str(e)))
            report.docToBeValidated = validatedDoc
            self.reports.append(report)


    def makeReport(self, out):
        out.write(("-"*80) +"\n")
        out.write("                 Summary\n")
        out.write("-"*80 + "\n")
        out.write("    Passed: {0}\n".format(self.passedValidations))
        out.write("    Failed: {0}\n".format(self.failedValidations))
        out.write("    total: {0}\n".format(self.totalValidations))
        out.write(("-"*80) +"\n")
        out.write("\n\n")
        for rep in self.reports:
            out.write("-"*80)
            out.write("\n")
            out.write(str(rep))
            out.write("\n")





def setValidatorCurrentDocPath(path):
    validationManager.currentDocPath = path

def setValidatorCurrentDocTitle(title):
    validationManager.currentDocTitle = title

def setValidatorCurrentsrcMLLang(lang):
    validationManager.currentsrcMLLang = lang

def setValidatorCurrentDocEntryTitle(entryTitle):
    validationManager.currentEntryTitle = entryTitle
    validationManager.exampleIndex = 0

def setValidatorCurrentExamplePath(examplePath):
    validationManager.currentExamplePath = examplePath

def incrementValidatorExampleIndex():
    validationManager.exampleIndex += 1

def setValidatorCurrentExampleTitle(title):
    validationManager.currentExampleTitle = title

def validate(tree):
    validationManager.validateAndReport(tree)

validationManager = ValidationLookUp()

def setValidator(validator):
    validationManager.validator = validator