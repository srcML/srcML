import os, sys, lxml, cStringIO, itertools
import lxml.etree as ET
from LoadData import *

# Attempts to validate the example srcML so that if it changes I know
# which example failed and where.

class ValidationLookUp:
    def __init__(self):
        self.validators = dict()

    def validateAndReport(self, toValidate):
        pass

def loadValidators(directory):
    validationManager = ValidationLookUp()
    return ValidationLookUp