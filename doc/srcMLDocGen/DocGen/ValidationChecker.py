import os, sys, lxml, cStringIO, itertools, re
import lxml.etree as ET
from LoadData import *

# Attempts to validate the example srcML so that if it changes I know
# which example failed and where.
rngExtMatch = re.compile(r"\.rng$", re.I)
class ValidationFailureReport:
    def __init__(self, validationLookUp, rngValidatorName="", exceptionErrorMessage=""):
        self.exampleTitle = validationLookUp.currentExampleTitle
        self.exampleFileName = validationLookUp.currentExamplePath
        self.exampleIndex = validationLookUp.currentExampleIndex
        self.entryTitle = validationLookUp.currentEntryTitle
        self.docTitle = validationLookUp.currentDocTitle
        self.docConfigPath = validationLookUp.currentDocPath
        self.language = validationLookUp.currentsrcMLLang
        self.validatorName = rngValidatorName
        self.errorMessage = exceptionErrorMessage
        self.docToBeValidated = None
        self.exampleHasValidator = False

    def __str__(self):
        if self.docToBeValidated == None:
            return """Doc Title: {0.docTitle}
Doc Path: {0.docConfigPath}
srcML Lang: {0.language}
Entry: {0.entryTitle}
Example Index: {0.exampleIndex}
Example Title: {0.exampleTitle}
Example File: {0.exampleFileName}
Validator Name: {0.validatorName}
Error Message: {0.errorMessage}""".format(self)
        else:
            return """Doc Title: {0.docTitle}
Doc Path: {0.docConfigPath}
srcML Lang: {0.language}
Entry: {0.entryTitle}
Example Index: {0.exampleIndex}
Example Title: {0.exampleTitle}
Example File: {0.exampleFileName}
Validator Name: {0.validatorName}
Error Message: {0.errorMessage}
{1}""".format(self, ET.tostring(self.docToBeValidated))


class ValidationLookUp:
    def __init__(self):
        self.validators = dict()
        self.reports = []
        self.currentDocPath = ""
        self.currentDocTitle = ""
        self.currentsrcMLLang = ""
        self.currentEntryTitle = ""
        self.currentExampleIndex = ""
        self.currentExamplePath = ""
        self.currentExampleTitle = ""

    def validateAndReport(self, validatorName, docToValidate):
        if validatorName not in self.validators:
            raise Exception("Missing validator. The validator with name: {0} does not exist".format(validatorName))
        try:
            self.validators[validatorName].assertValid(docToValidate)
        except Exception as e:
            # @TODO I need to output something here so that I know when validation failed due to
            # some kind of change or bug etc...
            print "Validation Failure"
            print "    Entry: ", self.currentEntryTitle
            print "    File: ", self.currentExamplePath
            print "    ", str(e)
            report = ValidationFailureReport(self, rngValidatorName=validatorName, exceptionErrorMessage=str(e))
            report.docToBeValidated = docToValidate
            # report.startingElement = docToValidate.getroot()
            report.exampleHasValidator = True
            self.reports.append(report)


    def recordMissingValidator(self):
        self.reports.append(ValidationFailureReport(self, exceptionErrorMessage="Missing validator attribute within XML."))

    def loadValidator(self, rngFileRoot, rngFileName):
        fullPath = os.path.join(rngFileRoot, rngFileName)        
        grammar = ET.RelaxNG(file=fullPath)
        self.validators.update({rngFileName:grammar})

    def makeReport(self, out):
        numberOfReportsWithoutValidator = 0
        for rep in self.reports:
            if not rep.exampleHasValidator:
                numberOfReportsWithoutValidator += 1
            out.write("-"*80)
            out.write("\n")
            out.write(str(rep))
            out.write("\n")

        out.write("Number of Reports missing validators: {0}".format(numberOfReportsWithoutValidator))


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

def recordMissingValidator():
    validationManager.recordMissingValidator()

def validate(validatorName, tree):
    validationManager.validateAndReport(validatorName, tree)

validationManager = ValidationLookUp()

def loadValidators(directory):
    global validationManager

    for root, dirs, files in os.walk(os.path.abspath(directory)):
        for fileName in files:
            if rngExtMatch.search(fileName) != None:
                validationManager.loadValidator(root, fileName)
            

