##
# @file TestSuiteGeneratorBase.py
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

TEST_IS_NOT_NULL = "notnull"
TEST_ARE_EQUAL = "equal"
TEST_FILE_EXISTS = "fileExistsTest"
TEST_BUFFER_HAS_CONTENT = "bufferHasContent"
TEST_INT_HAS_CONTENT = "intHasContent"

# Special Function calls used by the deriving class to
# handle non-portable concepts like file deletion, etc...
DELETE_FILE_FUNC = "DELETE_FILE_FUNCTION"

# Special types for use by the deriving class.
BUFFER_REF_TYPE = "BUFFER_REF_TYPE"
INT_REF_TYPE = "INT_REF_TYPE"
SIZE_T_REF_TYPE = "SIZE_T_REF_TYPE"

# Callback types
WRITE_CALLBACK_TYPE = "WRITE_CALLBACK_TYPE"
READ_CALLBACK_TYPE = "READ_CALLBACK_TYPE"
CLOSE_CALLBACK_TYPE = "CLOSE_CALLBACK_TYPE"
CALLBACK_CTXT_TYPE = "CALLBACK_CTXT_TYPE"


class TestSuiteGeneratorBase(object):
    """
    This class is used for generating a test suite for language bindings
    to make sure that they work as expected. This base class simply walks
    a predetermined pseudo-AST-like structure and the deriving class
    implements functionality that executes those types of statements.
    """
    def __init__(self):
        self.outStrm = None
        self.indentLevel = 0
        self.indentText = "    "


    def getIndentStr(self):
        return self.indentText * self.indentLevel

    # Pure-virtual test functions
    def startTestFile(self):
        assert False, "Not Implemented!"

    def endTestFile(self):
        assert False, "Not Implemented!"

    # Generate the start and end of the class that handles 
    # all of the tests.
    def startTestClass(self, name):
        assert False, "Not Implemented!"

    def endTestClass(self):
        assert False, "Not Implemented!"


    # Test Function Generating Handler
    def startTestFuncGen(self, testTitle):
        assert False, "Not Implemented!"

    def endTestFuncGen(self):
        assert False, "Not Implemented!"

    # Test Generating parts without a start and end function pairs
    def genCall(self, assignResultTo, callName, variableNames):
        assert False, "Not Implemented!"

    def genTestStatement(self, assertionCmpType, actualVariable, expectedVariable, messageBase):
        assert False, "Not Implemented!"

    def genTestIOContext(self, ctxtVariable, didWrite, didRead, didClose, messageBase):
        assert False, "Not Implemented!"

    def genUnaryTestStatement(self, assertionCmpType, actualVariable, messageBase):
        assert False, "Not Implemented!"

    def genVariableDecl(self, variableNativeType, variableName, variableInitializerValue):
        assert False, "Not Implemented!"

    def genLineComment(self, str):
        assert False, "Not Implemented!"

    def genIncrementVariableExpr(self, variableName, incrementBy = 1):
        assert False, "Not Implemented!"        

    # more complex pseudo-AST functionality.
    def startWhileLoop(self, conditionVariable, testForValue):
        assert False, "Not Implemented!"

    def endWhileLoop(self):
        assert False, "Not Implemented!"



    # Helper Wrapper Functions
    def gen_startTestFile(self):
        self.outStrm.write(self.startTestFile() + "\n")

    def gen_endTestFile(self):
        self.outStrm.write(self.endTestFile() + "\n")

    # Generate the start and end of the class that handles 
    # all of the tests.
    def gen_startTestClass(self, name):
        self.outStrm.write(self.getIndentStr() + self.startTestClass(name) + "\n")
        self.indentLevel += 1

    def gen_endTestClass(self):
        self.indentLevel -= 1
        self.outStrm.write(self.getIndentStr() + self.endTestClass() + "\n")

    def gen_startTestFuncGen(self, testTitle):
        self.outStrm.write(self.getIndentStr() + self.startTestFuncGen(testTitle) + "\n")
        self.indentLevel += 1

    def gen_endTestFuncGen(self):
        self.indentLevel -= 1
        self.outStrm.write(self.getIndentStr() + self.endTestFuncGen() + "\n")

    def gen_genCall(self, assignResultTo, callName, parameters):
        self.outStrm.write(self.getIndentStr() + self.genCall(assignResultTo, callName, parameters) + "\n")

    def gen_genTestStatement(self, assertionCmpType, actualVariable, expectedVariable, messageBase):
        self.outStrm.write(self.getIndentStr() + self.genTestStatement(assertionCmpType, expectedVariable, actualVariable, messageBase) + "\n")

    def gen_genTestIOContext(self, ctxtVariable, didWrite, didRead, didClose, messageBase):
        self.outStrm.write(self.getIndentStr() + self.genTestIOContext(ctxtVariable, didWrite, didRead, didClose, messageBase) + "\n")


    def gen_genUnaryTestStatement(self, assertionCmpType, actualVariable, messageBase):
        self.outStrm.write(self.getIndentStr() + self.genUnaryTestStatement(assertionCmpType, actualVariable, messageBase) + "\n")

    def gen_genVariableDecl(self, variableNativeType, variableName, variableInitializerValue):
        self.outStrm.write(self.getIndentStr() + self.genVariableDecl(variableNativeType, variableName, variableInitializerValue) + "\n")

    def gen_genLineComment(self, stringValue):
        self.outStrm.write(self.getIndentStr() + self.genLineComment(stringValue) + "\n")

    def gen_genIncrementVariableExpr(self, variableName, incrementBy = 1):
        self.outStrm.write(self.getIndentStr() + self.genIncrementVariableExpr(variableName, incrementBy) + "\n")

    # more complex pseudo-AST functionality.
    def gen_startWhileLoop(self, conditionVariable, testForValue):
        self.outStrm.write(self.getIndentStr() + self.startWhileLoop(conditionVariable, testForValue) + "\n")
        self.indentLevel += 1

    def gen_endWhileLoop(self):
        self.outStrm.write(self.getIndentStr() + self.endWhileLoop() + "\n")
        self.indentLevel -= 1

    # Functions that implement the Pseudo-AST traversal.
    def run(self, outputTestFile):
        self.outStrm = open(outputTestFile, "w")
        self.gen_startTestFile()
        self.gen_startTestClass("archive_tests")

        # Generating archive tests.
        self.generateArchiveTests()

        self.gen_endTestClass()

        self.gen_endTestFile()
        self.outStrm.close()


    def _buildCreateArchive(self, varName):
        self.gen_genCall(varName, "srcml_archive_create", [])


    def _buildCleanUpArchive(self, varName):
        self.gen_genCall(None, "srcml_archive_free", [varName])

    def generateArchiveTests(self):

        # Test srcml_create_archve and srcml_free_archive functions.
        self.gen_startTestFuncGen("srcml_archive_create")
        self.gen_genVariableDecl("srcml_archive *", "archive", None)
        self._buildCreateArchive("archive")
        self.gen_genUnaryTestStatement(TEST_IS_NOT_NULL, "archive", "Created archive was null.")
        self._buildCleanUpArchive("archive")
        self.gen_endTestFuncGen()
            
        # Testing srcml_archive_check_extension
        self.gen_startTestFuncGen("srcml_archive_check_extension")
        self.gen_genVariableDecl("srcml_archive *", "archive", None)
        self.gen_genVariableDecl("const char *", "fileName", "file.cpp")
        self.gen_genVariableDecl("const char *", "actual", "")
        self._buildCreateArchive("archive")
        self.gen_genCall("actual", "srcml_archive_check_extension", ["archive", "fileName"])
        self.gen_genTestStatement(TEST_ARE_EQUAL, "actual", "srcml.LANGUAGE_CXX", "Didn't get expected result from file extension.")
        self._buildCleanUpArchive("archive")
        self.gen_endTestFuncGen()

        # # Testing srcml_write_open_filename and srcml_archive_close
        self.gen_startTestFuncGen("srcml_write_open_filename")
        self.gen_genVariableDecl("srcml_archive *", "oarchive", None)
        self.gen_genVariableDecl("const char *", "testFileName", "srcml_write_open_filename_test_file.cpp")
        self._buildCreateArchive("oarchive")
        self.gen_genCall(None, "srcml_archive_write_open_filename", ["oarchive", "testFileName", "0"])
        self.gen_genCall(None, "srcml_archive_close", ["oarchive"])
        self.gen_genUnaryTestStatement(TEST_FILE_EXISTS, "testFileName", "Didn't locate correct test file.")
        self._buildCleanUpArchive("oarchive")
        self.gen_genCall(None, DELETE_FILE_FUNC, ["testFileName"])
        self.gen_endTestFuncGen()

        # Testing srcml_write_open_memory
        self.gen_startTestFuncGen("srcml_write_open_memory")
        self.gen_genVariableDecl("srcml_archive *", "oarchive", None)
        self.gen_genVariableDecl(BUFFER_REF_TYPE, "buffer", None)
        self.gen_genVariableDecl(SIZE_T_REF_TYPE, "bufferSize", None)
        self._buildCreateArchive("oarchive")
        self.gen_genCall(None, "srcml_archive_write_open_memory", ["oarchive", "buffer", "bufferSize"])
        self.gen_genCall(None, "srcml_archive_close", ["oarchive"])
        self.gen_genUnaryTestStatement(TEST_BUFFER_HAS_CONTENT, "buffer", "Missing buffer content.")
        self.gen_genUnaryTestStatement(TEST_INT_HAS_CONTENT, "bufferSize", "Missing buffer size's content.")
        self._buildCleanUpArchive("oarchive")
        self.gen_endTestFuncGen()


        # Testing srcml_write_open_io
        self.gen_startTestFuncGen("srcml_write_open_io")
        self.gen_genVariableDecl("srcml_archive *", "oarchive", None)
        self.gen_genVariableDecl(WRITE_CALLBACK_TYPE, "writeCB", None)
        self.gen_genVariableDecl(CLOSE_CALLBACK_TYPE, "closeCB", None)
        self.gen_genVariableDecl(CALLBACK_CTXT_TYPE, "ctxt", None)
        self._buildCreateArchive("oarchive")
        self.gen_genCall(None, "srcml_archive_write_open_io", ["oarchive", "ctxt", "writeCB", "closeCB"])
        self.gen_genCall(None, "srcml_archive_close", ["oarchive"])
        self.gen_genTestIOContext("ctxt", True, False, True, "Failed to write or close")
        self._buildCleanUpArchive("oarchive")
        self.gen_endTestFuncGen()


        def genArchiveGetterSetterTest(propertyName, propNativeType, value, defaultValue):
            setter = "srcml_archive_set_" + propertyName
            getter = "srcml_archive_get_" + propertyName

            self.gen_startTestFuncGen("srcml_archive_set_" + propertyName + "___AND___srcml_archive_get_" + propertyName )
            self.gen_genVariableDecl("srcml_archive *", "archive", None)
            self.gen_genVariableDecl(propNativeType, "propNewValue", value)
            self.gen_genVariableDecl(propNativeType, "actual", defaultValue)
            self._buildCreateArchive("archive")
            self.gen_genCall(None, setter, ["archive", "propNewValue"])
            self.gen_genCall("actual", getter, ["archive"])
            self.gen_genTestStatement(TEST_ARE_EQUAL, "actual", "propNewValue", "Incorrect value returned from " + propertyName)
            self._buildCleanUpArchive("archive")
            self.gen_endTestFuncGen()

        genArchiveGetterSetterTest("xml_encoding", "char *", "banana", "")
        genArchiveGetterSetterTest("src_encoding", "char *", "banana", "")
        genArchiveGetterSetterTest("language", "char *", "C++", "")
        genArchiveGetterSetterTest("filename", "char *", "aardvark.cpp", "")
        genArchiveGetterSetterTest("url", "char *", "Zooooo/", "")
        genArchiveGetterSetterTest("version", "char *", "11111111111111", "")
        genArchiveGetterSetterTest("tabstop", "int", 7, 0)

        # Set Options Test
        # self.gen_startTestFuncGen("srcml_archive_set_options")
        # self.gen_genVariableDecl("srcml_archive *", "archive", None)
        # self.gen_genVariableDecl("unsigned long long", "expected", 123)
        # self.gen_genVariableDecl("unsigned long long", "actual", 0)
        # self._buildCreateArchive("archive")
        # self.gen_genCall(None, "srcml_archive_set_options", ["archive", "expected"])
        # self.gen_genCall("actual", "srcml_archive_get_options", ["archive"])
        # self.gen_genTestStatement(TEST_ARE_EQUAL, "actual", "expected", "Incorrect value returned from set/get options")
        # self._buildCleanUpArchive("archive")
        # self.gen_endTestFuncGen()

    def generateGlobalTest(self):
        assert False, "Not Implemented!"


# /* Clone the setup of an existing archive
#    Client is responsible for freeing memory using srcml_free_archive() */
# __LIBSRCML_DECL struct srcml_archive* srcml_clone_archive(const struct srcml_archive*);

# /* Setup options for srcml archive */

# __LIBSRCML_DECL int srcml_archive_set_options               (struct srcml_archive*, unsigned long long option);
# __LIBSRCML_DECL int srcml_archive_enable_option             (struct srcml_archive*, unsigned long long option);
# __LIBSRCML_DECL int srcml_archive_disable_option            (struct srcml_archive*, unsigned long long option);
# __LIBSRCML_DECL int srcml_archive_register_file_extension   (struct srcml_archive*, const char* extension, const char* language);
# __LIBSRCML_DECL int srcml_archive_register_namespace        (struct srcml_archive*, const char* prefix, const char* ns);
# __LIBSRCML_DECL int srcml_archive_set_processing_instruction(struct srcml_archive*, const char* target, const char* data); 
# __LIBSRCML_DECL int srcml_archive_register_macro            (struct srcml_archive*, const char* token, const char* type);  
# __LIBSRCML_DECL const char*        srcml_archive_get_revision                     (const struct srcml_archive*);
# /* Query of the options for srcml archive */

# __LIBSRCML_DECL unsigned long long srcml_archive_get_options                      (const struct srcml_archive*);
# __LIBSRCML_DECL int                srcml_archive_get_namespace_size               (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_namespace_prefix             (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char*        srcml_archive_get_prefix_from_uri              (const struct srcml_archive*, const char* namespace_uri);
# __LIBSRCML_DECL const char*        srcml_archive_get_namespace_uri                (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char*        srcml_archive_get_uri_from_prefix              (const struct srcml_archive*, const char* prefix);
# __LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_target(const struct srcml_archive*); 
# __LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_data  (const struct srcml_archive*); 
# __LIBSRCML_DECL int                srcml_archive_get_macro_list_size              (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_macro_token                  (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char*        srcml_archive_get_macro_token_type             (const struct srcml_archive*, const char* namespace_uri);
# __LIBSRCML_DECL const char*        srcml_archive_get_macro_type                   (const struct srcml_archive*, int pos);

# /* Create a new srcml unit.
#    Client is responsible for freeing memory using srcml_free_unit() */
# __LIBSRCML_DECL struct srcml_unit* srcml_create_unit(struct srcml_archive* archive);



# /* Append unit to an archive */
# __LIBSRCML_DECL int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);

# /* Iteratively build a unit */
# __LIBSRCML_DECL int srcml_write_start_unit(struct srcml_unit*);
# __LIBSRCML_DECL int srcml_write_end_unit(struct srcml_unit*);
# __LIBSRCML_DECL int srcml_write_start_element(struct srcml_unit*, const char * prefix, const char * name, const char * uri);
# __LIBSRCML_DECL int srcml_write_end_element(struct srcml_unit*);
# __LIBSRCML_DECL int srcml_write_namespace(struct srcml_unit*, const char * prefix, const char * uri);
# __LIBSRCML_DECL int srcml_write_attribute(struct srcml_unit*, const char * prefix, const char * name, const char * uri, const char * content);
# __LIBSRCML_DECL int srcml_write_string(struct srcml_unit*, const char * content);
 
# /* Free allocated unit */
# __LIBSRCML_DECL void srcml_free_unit(struct srcml_unit*);

# /* Close the srcML archive */
# 

# /* Open a srcML archive for reading */
# __LIBSRCML_DECL int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
# __LIBSRCML_DECL int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
# __LIBSRCML_DECL int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
# __LIBSRCML_DECL int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);
# __LIBSRCML_DECL int srcml_read_open_io      (struct srcml_archive*, void * context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context));

# /* Read the next unit from the archive
#    Returns 0 if there are no more units */
# __LIBSRCML_DECL struct srcml_unit* srcml_read_unit_header(struct srcml_archive*);
# __LIBSRCML_DECL struct srcml_unit* srcml_read_unit_xml(struct srcml_archive*);
# __LIBSRCML_DECL struct srcml_unit* srcml_read_unit(struct srcml_archive*);


# /* srcML XPath query and XSLT transform functions */
# __LIBSRCML_DECL int srcml_clear_transforms                 (struct srcml_archive*);
# __LIBSRCML_DECL int srcml_append_transform_xpath           (struct srcml_archive*, const char* xpath_string);
# __LIBSRCML_DECL int srcml_append_transform_xpath_attribute (struct srcml_archive*, const char* xpath_string,
#                                                             const char* prefix, const char* namespace_uri,
#                                                             const char* attr_name, const char* attr_value);
# __LIBSRCML_DECL int srcml_append_transform_xpath_element   (struct srcml_archive*, const char* xpath_string,
#                                                             const char* prefix, const char* namespace_uri,
#                                                             const char* element,
#                                                             const char* attr_prefix, const char* attr_namespace_uri,
#                                                             const char* attr_name, const char* attr_value);
# __LIBSRCML_DECL int srcml_append_transform_xslt_filename   (struct srcml_archive*, const char* xslt_filename);
# __LIBSRCML_DECL int srcml_append_transform_xslt_memory     (struct srcml_archive*, const char* xslt_buffer, size_t size);
# __LIBSRCML_DECL int srcml_append_transform_xslt_FILE       (struct srcml_archive*, FILE* xslt_file);
# __LIBSRCML_DECL int srcml_append_transform_xslt_fd         (struct srcml_archive*, int xslt_fd);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_filename(struct srcml_archive*, const char* relaxng_filename);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_memory  (struct srcml_archive*, const char* relaxng_buffer, size_t size);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_FILE    (struct srcml_archive*, FILE* relaxng_file);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_fd      (struct srcml_archive*, int relaxng_fd);
# __LIBSRCML_DECL int srcml_append_transform_param           (struct srcml_archive*, const char* param_name, const char* param_value);
# __LIBSRCML_DECL int srcml_append_transform_stringparam     (struct srcml_archive*, const char* param_name, const char* param_value);
# __LIBSRCML_DECL int srcml_apply_transforms                 (struct srcml_archive* iarchive, struct srcml_archive* oarchive);
