/**
 * @file test_srcexfunc_impl.c
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <srcml.h>
#include <srcmlns.hpp>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <boost/shared_ptr.hpp>
#include <libxml/xpathInternals.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <fstream>


typedef boost::shared_ptr<xmlDoc> xml_doc_shared_ptr_t; 

typedef const char* ConstCharPtr;
using namespace std;


void dbgDoc(xml_doc_shared_ptr_t docPtr) {
    xmlChar* buf = 0;
    int bufSize = 0;
    xmlDocDumpFormatMemory(docPtr.get(), &buf, &bufSize, 0);
    if(buf) {
        typedef char* CharPtr;
        cout.write(CharPtr(buf), bufSize);
        cout << endl;
        xmlFree(buf);
    }
}

struct TestResult {
    string fileName;
    int actual;
    int expected;
    bool status;
};
typedef vector<TestResult> TestResultsContainer;
TestResultsContainer testResults;

void run_xpath_test(std::string const& testFile, std::string const& xpathToTest, int expectedResultCount) {

    cout << "Testing: " << testFile << endl;

    {

        std::ifstream in_file(SRCML_XPATH_EXFUN_TEST_DIR + string("/") + testFile);
        assert(in_file.good());

    }

    char* archiveBuffer = 0;
    size_t archiveBufferSize = 0;
    int rc = 0;

    // Create an archive to handle the data being read from a file.
    srcml_archive* processedArchive = srcml_archive_create();
    if(!processedArchive) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to create processed archive.");
    }

    rc = srcml_archive_write_open_memory(processedArchive, &archiveBuffer, &archiveBufferSize);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed open memory for writing.");
    }

    srcml_unit* unit = srcml_unit_create(processedArchive);
    if(!unit) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed create unit.");
    }

    const char* ext = srcml_archive_check_extension(processedArchive, testFile.c_str());
    if(!ext) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed get file extension.");   
    }

    rc = srcml_unit_set_language(unit, ext);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to set language.");
    }
    string fullTestFilePath = SRCML_XPATH_EXFUN_TEST_DIR + string("/") + testFile;
    rc = srcml_unit_parse_filename(unit, fullTestFilePath.c_str());
    if(rc != SRCML_STATUS_OK) {
        cout << rc << endl;
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to parse unit from file name.");
    }

    rc = srcml_write_unit(processedArchive, unit);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to parse unit from file name.");
    }

    srcml_unit_free(unit);
    srcml_archive_close(processedArchive);
    srcml_archive_free(processedArchive);

    processedArchive = 0;
    processedArchive = srcml_archive_create();
    if(!processedArchive) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed creating processed archive for 2nd time..");
    }

    rc = srcml_archive_read_open_memory(processedArchive, archiveBuffer, archiveBufferSize);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed open memory for reading.");
    }

    // Processing xpath from archive.
    char* xpathResultArchiveBuffer = 0;
    size_t xpathResultArchiveBufferSize = 0;

    srcml_archive* xpathResultArchive = srcml_archive_create();
    if(!xpathResultArchive) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to create xpathResultArchive.");
    }

    // Handling xpath archive.
    rc = srcml_archive_write_open_memory(xpathResultArchive, &xpathResultArchiveBuffer, &xpathResultArchiveBufferSize);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed open memory for writing to xpathResultArchive.");
    }

    // Setting up the transformation.
    rc = srcml_append_transform_xpath(processedArchive, xpathToTest.c_str());
    if(rc != SRCML_STATUS_OK) {
        cout << rc << endl;
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed on srcml_append_transform_xpath.");
    }

    // Applying the transformation and transferring the results
    // to the XPath archive.
    rc = srcml_apply_transforms(processedArchive, xpathResultArchive);
    if(rc != SRCML_STATUS_OK) {
        cout << rc << endl;
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed on srcml_apply_transforms.");
    }
    srcml_archive_close(xpathResultArchive);

    // free(archiveBuffer);

    // Turning XPath document into libxml2 xmlDoc.
    xmlDocPtr ret = xmlParseMemory(xpathResultArchiveBuffer, (int)xpathResultArchiveBufferSize);
    srcml_archive_free(processedArchive);
    srcml_archive_free(xpathResultArchive);
    free(xpathResultArchiveBuffer);
    xml_doc_shared_ptr_t doc(ret, xmlFreeDoc);

    xmlXPathContextPtr xpathCtx; 
    xpathCtx = xmlXPathNewContext(doc.get());
    if(!xpathCtx) {
        cout << "Failed to create XPath context" << endl;
        throw std::runtime_error("Failed to create XPath context.");
    }

    // Registering namespaces.
    if(xmlXPathRegisterNs(xpathCtx, BAD_CAST "src", BAD_CAST SRCML_SRC_NS_URI) != 0) {
        xmlXPathFreeContext(xpathCtx); 
        cout << "Failed to register namespace. : src" << endl;
        throw std::runtime_error("Failed to register namespace: src");
    }

    if(xmlXPathRegisterNs(xpathCtx, BAD_CAST "cpp", BAD_CAST SRCML_CPP_NS_URI) != 0) {
        xmlXPathFreeContext(xpathCtx); 
        cout << "Failed to register namespace: cpp" << endl;
        throw std::runtime_error("Failed to register namespace: cpp");
    }

    xmlXPathObjectPtr xpathObj;
    xpathObj = xmlXPathEvalExpression(BAD_CAST "/src:unit/*", xpathCtx);
    if(!xpathObj) {
        throw std::runtime_error("Didn't receive xpathObject");
    }

    if(!xpathObj->nodesetval) {
        cout.write(archiveBuffer, archiveBufferSize);
        cout << endl << endl << endl;
        throw std::runtime_error("Didn't receive nodesetval");
    }
    int totalResultCount = xpathObj->nodesetval->nodeNr;
    // Recording test status.
    TestResult testResult;
    testResult.fileName = testFile;
    testResult.actual = xpathObj->nodesetval->nodeNr;
    testResult.expected = expectedResultCount;
    if(totalResultCount != expectedResultCount) {
        testResult.status = false;
        
        cout << "Failed" << endl;
        cout << "Number of Nodes located: "<< totalResultCount << endl;
        cout << "Expected # of nodes: " << expectedResultCount << endl;
        cout << "Queried Document: " << endl;
        cout.write(archiveBuffer, archiveBufferSize);
        cout << endl << endl << endl;
        cout << "Results Doc:" << endl;
        dbgDoc(doc);
        cout << endl << endl << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "                   Results" << endl;
        cout << "---------------------------------------------------" << endl;
        for(int resultIndex = 0; resultIndex < xpathObj->nodesetval->nodeNr; ++resultIndex) {

            xmlBufferPtr buff = xmlBufferCreate();
            xmlNodeDump(buff, doc.get(),  xmlFirstElementChild(xpathObj->nodesetval->nodeTab[resultIndex]), 0, xmlKeepBlanksDefault(0));
            cout << "Result: " << (resultIndex + 1) << " " << endl;
            cout.write(ConstCharPtr(buff->content), xmlBufferLength(buff));
            cout << std::endl;
            xmlBufferFree(buff);
            cout << "---------------------------------------------------" << endl << endl;
        }
        cout << endl;
        // for xpathObj

    } else {
        testResult.status = true;
        cout << "Passed" << endl;
    }
    testResults.push_back(testResult);
    // Freeing previously used buffer because it's no longer
    // needed it maybe output if the test failed so it's freed after test
    // is finished.
    free(archiveBuffer);
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx); 

}


int summaryAndResultsDisplay() {
    bool hasFailed = false;
    for (TestResultsContainer::iterator iter = testResults.begin();
        iter != testResults.end();
        ++iter)

    {
        if (!iter->status){
            hasFailed = true;
            break;
        }
    }

    if(hasFailed){
        cout << endl << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "                   Failing Tests" << endl;
        cout << "---------------------------------------------------" << endl;
        for (TestResultsContainer::iterator iter = testResults.begin();
            iter != testResults.end();
            ++iter)
        {
            if (!iter->status){
                cout << "    " << iter->fileName << " " << iter->actual << " " << iter->expected << endl;
            }
        }
        return -1;
    }
    return 0;
}

int main() {
    cout << "---------------------------------------------------" << endl;
    cout << "                   Running XPath Tests" << endl;
    cout << "---------------------------------------------------" << endl;
    xmlInitParser();

    // has_return()
    run_xpath_test(
        "has_return/has_return.cpp",
        "//src:unit[src:has_return()] "
        "| //src:function[src:has_return()] "
        "| //src:destructor[src:has_return()]"
        "| //src:if[src:has_return()] "
        "| //src:else[src:has_return()] "
        "| //src:elseif[src:has_return()] "
        "| //src:then[src:has_return()] "
        "| //src:switch[src:has_return()]",
        12);

    string hasReturnXPath = "//src:block[src:has_return()]"
        "| //src:function[src:has_return()]"
        "| //src:constructor[src:has_return()]"
        "| //src:destructor[src:has_return()]"
        "| //src:while[src:has_return()]"
        "| //src:if[src:has_return()]"
        "| //src:then[src:has_return()]"
        "| //src:elseif[src:has_return()]"
        "| //src:else[src:has_return()]"
        "| //src:try[src:has_return()]"
        "| //src:catch[src:has_return()]"
        "| //src:finally[src:has_return()]"
        "| //src:do[src:has_return()]"
        "| //src:for[src:has_return()]"
        "| //src:foreach[src:has_return()]"
        "| //src:switch[src:has_return()]"
        "| //src:lambda[src:has_return()]"
        "| //src:delegate[src:has_return()]"
        "| //src:using_stmt[src:has_return()]"
        "| //src:fixed[src:has_return()]"
        "| //src:lock[src:has_return()]"
        "| //src:synchronized[src:has_return()]"
        "| //src:fixed[src:has_return()]"
        "| //src:unsafe[src:has_return()]"
        "| //src:static[src:has_return()]"
        "| //src:checked[src:has_return()]"
        "| //src:unchecked[src:has_return()]";

    run_xpath_test(
        "has_return/has_return_scope_test.cpp",
        hasReturnXPath,
        6
    );

    run_xpath_test(
        "has_return/has_return.cs",
        hasReturnXPath,
        8
    );

    run_xpath_test(
        "has_return/has_return.java",
        hasReturnXPath,
        4
    );

    // has_throw
    string hasThrowXPath = "//src:block[src:has_throw()]"
        "| //src:function[src:has_throw()]"
        "| //src:constructor[src:has_throw()]"
        "| //src:destructor[src:has_throw()]"
        "| //src:while[src:has_throw()]"
        "| //src:if[src:has_throw()]"
        "| //src:then[src:has_throw()]"
        "| //src:elseif[src:has_throw()]"
        "| //src:else[src:has_throw()]"
        "| //src:try[src:has_throw()]"
        "| //src:catch[src:has_throw()]"
        "| //src:finally[src:has_throw()]"
        "| //src:do[src:has_throw()]"
        "| //src:for[src:has_throw()]"
        "| //src:foreach[src:has_throw()]"
        "| //src:switch[src:has_throw()]"
        "| //src:lambda[src:has_throw()]"
        "| //src:delegate[src:has_throw()]"
        "| //src:using_stmt[src:has_throw()]"
        "| //src:fixed[src:has_throw()]"
        "| //src:lock[src:has_throw()]"
        "| //src:synchronized[src:has_throw()]"
        "| //src:fixed[src:has_throw()]"
        "| //src:unsafe[src:has_throw()]"
        "| //src:static[src:has_throw()]"
        "| //src:checked[src:has_throw()]"
        "| //src:unchecked[src:has_throw()]";

    run_xpath_test(
        "has_throw/has_throw.cpp",
        hasThrowXPath,
        19
    );

    run_xpath_test(
        "has_throw/has_throw.cs",
        hasThrowXPath,
        8
    );

    run_xpath_test(
        "has_throw/has_throw.java",
        hasThrowXPath,
        4
    );

    // is_nested()
    run_xpath_test(
        "is_nested/is_nested.cpp",
        "//src:asm[src:is_nested()]"
        "| //src:typedef[src:is_nested()]"
        "| //src:using[src:is_nested()]"
        "| //src:function[src:is_nested()]"
        "| //src:function_decl[src:is_nested()]"
        "| //src:constructor[src:is_nested()]"
        "| //src:constructor_decl[src:is_nested()]"
        "| //src:destructor[src:is_nested()]"
        "| //src:destructor_decl[src:is_nested()]"
        "| //src:property[src:is_nested()]"
        "| //src:event[src:is_nested()]"
        "| //src:union[src:is_nested()]"
        "| //src:union_decl[src:is_nested()]"
        "| //src:class[src:is_nested()]"
        "| //src:class_decl[src:is_nested()]"
        "| //src:struct[src:is_nested()]"
        "| //src:struct_decl[src:is_nested()]"
        "| //src:enum[src:is_nested()]"
        "| //src:namespace[src:is_nested()]",
        11
    );


    // is_class_template_partial_specialization()
    run_xpath_test(
        "is_class_template_partial_specialization/is_class_template_partial_specialization.cpp",
        "//src:union[src:is_class_template_partial_specialization()]"
        "| //src:union_decl[src:is_class_template_partial_specialization()]"
        "| //src:class[src:is_class_template_partial_specialization()]"
        "| //src:class_decl[src:is_class_template_partial_specialization()]"
        "| //src:struct[src:is_class_template_partial_specialization()]"
        "| //src:struct_decl[src:is_class_template_partial_specialization()]",
        2
    );



    // has_init()
    run_xpath_test(
        "has_init/has_init.cpp",
        "//src:decl[src:has_init()]"
        "| //src:decl_stmt[src:has_init()]"
        "| //src:param[src:has_init()]"
        "| //src:using_stmt[src:has_init()]"
        "| //src:using[src:has_init()]"
        "| //src:try[src:has_init()]"
        "| //src:synchronized[src:has_init()]"
        "| //src:fixed[src:has_init()]"
        "| //src:lock[src:has_init()]",
        7
    );

    run_xpath_test(
        "has_init/has_init.cs",
        "//src:decl[src:has_init()]"
        "| //src:decl_stmt[src:has_init()]"
        "| //src:param[src:has_init()]"
        "| //src:using_stmt[src:has_init()]"
        "| //src:using[src:has_init()]"
        "| //src:try[src:has_init()]"
        "| //src:synchronized[src:has_init()]"
        "| //src:fixed[src:has_init()]"
        "| //src:lock[src:has_init()]",
        5
    );

    run_xpath_test(
        "has_init/has_init.java",
        "//src:decl[src:has_init()]"
        "| //src:decl_stmt[src:has_init()]"
        "| //src:param[src:has_init()]"
        "| //src:using_stmt[src:has_init()]"
        "| //src:using[src:has_init()]"
        "| //src:try[src:has_init()]"
        "| //src:synchronized[src:has_init()]"
        "| //src:fixed[src:has_init()]"
        "| //src:lock[src:has_init()]",
        3
    );


    // has_break()
    string hasBreakXPath = "//src:while[src:has_break()]"
        "| //src:block[src:has_break()]"
        "| //src:if[src:has_break()]"
        "| //src:then[src:has_break()]"
        "| //src:elseif[src:has_break()]"
        "| //src:else[src:has_break()]"
        "| //src:try[src:has_break()]"
        "| //src:catch[src:has_break()]"
        "| //src:finally[src:has_break()]"
        "| //src:do[src:has_break()]"
        "| //src:for[src:has_break()]"
        "| //src:foreach[src:has_break()]"
        "| //src:switch[src:has_break()]"
        "| //src:using_stmt[src:has_break()]"
        "| //src:fixed[src:has_break()]"
        "| //src:lock[src:has_break()]"
        "| //src:synchronized[src:has_break()]"
        "| //src:fixed[src:has_break()]"
        "| //src:unsafe[src:has_break()]"
        "| //src:static[src:has_break()]"
        "| //src:checked[src:has_break()]"
        "| //src:unchecked[src:has_break()]";


    run_xpath_test(
        "has_break/has_break.cpp",
        hasBreakXPath,
        9
    );

    run_xpath_test(
        "has_break/has_break.cs",
        hasBreakXPath,
        11
    );

    run_xpath_test(
        "has_break/has_break.java",
        hasBreakXPath,
        11
    );


    // is_unsafe()
    string isUnsafeXPath = "//src:while[src:is_unsafe()]"
        "| //src:block[src:is_unsafe()]"
        "| //src:if[src:is_unsafe()]"
        "| //src:then[src:is_unsafe()]"
        "| //src:elseif[src:is_unsafe()]"
        "| //src:else[src:is_unsafe()]"
        "| //src:try[src:is_unsafe()]"
        "| //src:catch[src:is_unsafe()]"
        "| //src:finally[src:is_unsafe()]"
        "| //src:do[src:is_unsafe()]"
        "| //src:for[src:is_unsafe()]"
        "| //src:foreach[src:is_unsafe()]"
        "| //src:switch[src:is_unsafe()]"
        "| //src:using_stmt[src:is_unsafe()]"
        "| //src:fixed[src:is_unsafe()]"
        "| //src:lock[src:is_unsafe()]"
        "| //src:synchronized[src:is_unsafe()]"
        "| //src:fixed[src:is_unsafe()]"
        "| //src:unsafe[src:is_unsafe()]"
        "| //src:static[src:is_unsafe()]"
        "| //src:checked[src:is_unsafe()]"
        "| //src:unchecked[src:is_unsafe()]"
        "| //src:expr_stmt[src:is_unsafe()]"
        "| //src:decl_stmt[src:is_unsafe()]";

    run_xpath_test(
        "is_unsafe/is_unsafe.cs",
        isUnsafeXPath,
        3
    );

    // is_mutually_exclusive
    string isMutuallyExclusiveXPath =
        "//src:while[src:is_mutually_exclusive()]"
        "| //src:if[src:is_mutually_exclusive()]"
        "| //src:then[src:is_mutually_exclusive()]"
        "| //src:elseif[src:is_mutually_exclusive()]"
        "| //src:else[src:is_mutually_exclusive()]"
        "| //src:try[src:is_mutually_exclusive()]"
        "| //src:catch[src:is_mutually_exclusive()]"
        "| //src:finally[src:is_mutually_exclusive()]"
        "| //src:do[src:is_mutually_exclusive()]"
        "| //src:for[src:is_mutually_exclusive()]"
        "| //src:foreach[src:is_mutually_exclusive()]"
        "| //src:switch[src:is_mutually_exclusive()]"
        "| //src:using_stmt[src:is_mutually_exclusive()]"
        "| //src:fixed[src:is_mutually_exclusive()]"
        "| //src:synchronized[src:is_mutually_exclusive()]"
        "| //src:fixed[src:is_mutually_exclusive()]"
        "| //src:unsafe[src:is_mutually_exclusive()]"
        "| //src:static[src:is_mutually_exclusive()]"
        "| //src:checked[src:is_mutually_exclusive()]"
        "| //src:unchecked[src:is_mutually_exclusive()]"
        "| //src:expr_stmt[src:is_mutually_exclusive()]"
        "| //src:decl_stmt[src:is_mutually_exclusive()]";

    run_xpath_test(
        "is_mutually_exclusive/is_mutually_exclusive.cs",
        isMutuallyExclusiveXPath,
        2
    );

    run_xpath_test(
        "is_mutually_exclusive/is_mutually_exclusive.java",
        isMutuallyExclusiveXPath,
        4
    );


    // is_static()
    string isStaticXPath = 
        "//src:decl_stmt[ src:is_static() ]"
        "| //src:function_decl[src:is_static()]"
        "| //src:function[src:is_static()]"
        "| //src:event[src:is_static()]"
        "| //src:property[src:is_static()]"
        "| //src:class[src:is_static()]"
        "| //src:interface[src:is_static()]"
        "| //src:struct[src:is_static()]"
        "| //src:constructor[src:is_static()]"
        "| //src:constructor_decl[src:is_static()]"
    ;

    run_xpath_test("is_static/is_static.c", isStaticXPath, 3);
    run_xpath_test("is_static/is_static.cs", isStaticXPath, 9);
    run_xpath_test("is_static/is_static.java", isStaticXPath, 4);

    // block_is_static()
    string blockIsStaticXPath = 
        "//src:block[ src:block_is_static() ]"
    ;

    run_xpath_test("block_is_static/block_is_static.java", blockIsStaticXPath, 1);    

    // is_abstract()
    string isAbstractXPath = 
        "//src:function_decl[src:is_abstract()]"
        "| //src:function[src:is_abstract()]"
        "| //src:event[src:is_abstract()]"
        "| //src:property[src:is_abstract()]"
        "| //src:class[src:is_abstract()]"
        "| //src:struct[src:is_abstract()]"
        "| //src:destructor_decl[src:is_abstract()]"
        "| //src:constructor_decl[src:is_abstract()]"
        "| //src:interface[src:is_abstract()]"
    ;

    run_xpath_test("is_abstract/is_abstract.cpp", isAbstractXPath, 3);
    run_xpath_test("is_abstract/is_abstract.cs", isAbstractXPath, 18);
    run_xpath_test("is_abstract/is_abstract.java", isAbstractXPath, 4);


    // has_default_impl()
    string hasDefaultImplXPath = 
        "//src:function_decl[src:has_default_impl()]"
        "| //src:function[src:has_default_impl()]"
        "| //src:destructor_decl[src:has_default_impl()]"
        "| //src:constructor_decl[src:has_default_impl()]"
    ;

    run_xpath_test("has_default_impl/has_default_impl.cpp", hasDefaultImplXPath, 3);
    run_xpath_test("has_default_impl/has_default_impl.java", hasDefaultImplXPath, 1);

    // has_default_property_value()
    run_xpath_test("has_default_property_value/has_default_property_value.java", "//src:function_decl[src:has_default_property_value()]", 1);


    // is_final()
    string tempXPath = 
        "//src:function_decl[src:is_final()]"
        "| //src:function[src:is_final()]"
        "| //src:decl_stmt[src:is_final()]"
        "| //src:event[src:is_final()]"
        "| //src:property[src:is_final()]"
        "| //src:class[src:is_final()]"
        "| //src:struct[src:is_final()]"
    ;

    run_xpath_test("is_final/is_final.cpp", tempXPath, 1);
    run_xpath_test("is_final/is_final.java", tempXPath, 4);


    // is_synchronized
    tempXPath = 
        "//src:function_decl[src:is_synchronized()]"
        "| //src:function[src:is_synchronized()]"
    ;

    run_xpath_test("is_synchronized/is_synchronized.java", tempXPath, 2);

    // is_transient
    tempXPath = 
        "//src:decl_stmt[src:is_transient()]"
        "| //src:decl[src:is_transient()]"
    ;

    run_xpath_test("is_transient/is_transient.java", tempXPath, 2);

    // is_native
    tempXPath = "//src:function_decl[src:is_native()]";

    run_xpath_test("is_native/is_native.cs", tempXPath, 2);
    run_xpath_test("is_native/is_native.java", tempXPath, 1);


    // is_private
    tempXPath =
        "//src:function_decl[src:is_private()]"
        "| //src:function[src:is_private()]"
        "| //src:constructor[src:is_private()]"
        "| //src:constructor_decl[src:is_private()]"
        "| //src:destructor[src:is_private()]"
        "| //src:destructor_decl[src:is_private()]"
        "| //src:decl_stmt[src:is_private()]"
        "| //src:class[src:is_private()]"
        "| //src:class_decl[src:is_private()]"
        "| //src:struct[src:is_private()]"
        "| //src:struct_decl[src:is_private()]"
        "| //src:enum[src:is_private()]"
        "| //src:property[src:is_private()]"
        "| //src:event[src:is_private()]"
        "| //src:using[src:is_private()]"
        "| //src:typedef[src:is_private()]"
        "| //src:interface[src:is_private()]"
    ;
    run_xpath_test("is_private/is_private.cpp", tempXPath, 32);
    run_xpath_test("is_private/is_private.cs", tempXPath, 14);
    run_xpath_test("is_private/is_private.java", tempXPath, 6);


    // is_protected
    tempXPath =
        "//src:function_decl[src:is_protected()]"
        "| //src:function[src:is_protected()]"
        "| //src:constructor[src:is_protected()]"
        "| //src:constructor_decl[src:is_protected()]"
        "| //src:destructor[src:is_protected()]"
        "| //src:destructor_decl[src:is_protected()]"
        "| //src:decl_stmt[src:is_protected()]"
        "| //src:class[src:is_protected()]"
        "| //src:class_decl[src:is_protected()]"
        "| //src:struct[src:is_protected()]"
        "| //src:struct_decl[src:is_protected()]"
        "| //src:enum[src:is_protected()]"
        "| //src:property[src:is_protected()]"
        "| //src:event[src:is_protected()]"
        "| //src:using[src:is_protected()]"
        "| //src:typedef[src:is_protected()]"
        "| //src:interface[src:is_protected()]"
    ;
    run_xpath_test("is_protected/is_protected.cpp", tempXPath, 32);
    run_xpath_test("is_protected/is_protected.cs", tempXPath, 12);
    run_xpath_test("is_protected/is_protected.java", tempXPath, 6);

    // is_public
    tempXPath =
        "//src:function_decl[src:is_public()]"
        "| //src:function[src:is_public()]"
        "| //src:constructor[src:is_public()]"
        "| //src:constructor_decl[src:is_public()]"
        "| //src:destructor[src:is_public()]"
        "| //src:destructor_decl[src:is_public()]"
        "| //src:decl_stmt[src:is_public()]"
        "| //src:class[src:is_public()]"
        "| //src:class_decl[src:is_public()]"
        "| //src:struct[src:is_public()]"
        "| //src:struct_decl[src:is_public()]"
        "| //src:enum[src:is_public()]"
        "| //src:property[src:is_public()]"
        "| //src:event[src:is_public()]"
        "| //src:using[src:is_public()]"
        "| //src:typedef[src:is_public()]"
        "| //src:interface[src:is_public()]"
    ;
    run_xpath_test("is_public/is_public.cpp", tempXPath, 32);
    run_xpath_test("is_public/is_public.cs", tempXPath, 17);
    run_xpath_test("is_public/is_public.java", tempXPath, 8);

    // is_internal
    tempXPath =
        "//src:function_decl[src:is_internal()]"
        "| //src:function[src:is_internal()]"
        "| //src:constructor[src:is_internal()]"
        "| //src:constructor_decl[src:is_internal()]"
        "| //src:destructor[src:is_internal()]"
        "| //src:destructor_decl[src:is_internal()]"
        "| //src:decl_stmt[src:is_internal()]"
        "| //src:class[src:is_internal()]"
        "| //src:class_decl[src:is_internal()]"
        "| //src:struct[src:is_internal()]"
        "| //src:struct_decl[src:is_internal()]"
        "| //src:enum[src:is_internal()]"
        "| //src:property[src:is_internal()]"
        "| //src:event[src:is_internal()]"
        "| //src:using[src:is_internal()]"
        "| //src:interface[src:is_internal()]"
    ;
    run_xpath_test("is_internal/is_internal.cs", tempXPath, 13);

    // is_volatile
    tempXPath =
        "//src:function_decl[src:is_volatile()]"
        "| //src:function[src:is_volatile()]"
        "| //src:decl_stmt[src:is_volatile()]"
        "| //src:decl[src:is_volatile()]"
        "| //src:asm[src:is_volatile()]"
        "| //src:using[src:is_volatile()]"
        "| //src:type[src:is_volatile()]"
        "| //src:typedef[src:is_volatile()]"
    ;
    run_xpath_test("is_volatile/is_volatile.c", tempXPath, 3);
    run_xpath_test("is_volatile/is_volatile.cpp", tempXPath, 9);
    run_xpath_test("is_volatile/is_volatile.cs", tempXPath, 4);
    run_xpath_test("is_volatile/is_volatile.java", tempXPath, 3);

    // is_const
    tempXPath =
        "//src:function_decl[src:is_const()]"
        "| //src:function[src:is_const()]"
        "| //src:decl_stmt[src:is_const()]"
        "| //src:decl[src:is_const()]"
        "| //src:asm[src:is_const()]"
        "| //src:using[src:is_const()]"
        "| //src:type[src:is_const()]"
        "| //src:typedef[src:is_const()]"
    ;
    run_xpath_test("is_const/is_const.c", tempXPath, 3);
    run_xpath_test("is_const/is_const.cpp", tempXPath, 8);
    run_xpath_test("is_const/is_const.cs", tempXPath, 4);

    // is_inline
    tempXPath =
        "//src:function_decl[src:is_inline()]"
        "| //src:function[src:is_inline()]"
        "| //src:namespace[src:is_inline()]"
    ;
    run_xpath_test("is_inline/is_inline.c", tempXPath, 2);
    run_xpath_test("is_inline/is_inline.cpp", tempXPath, 1);

    // is_restrict
    tempXPath =
        "//src:decl[src:is_restrict()]"
        "| //src:decl_stmt[src:is_restrict()]"
    ;
    run_xpath_test("is_restrict/is_restrict.c", tempXPath, 2);
    run_xpath_test("is_restrict/is_restrict.cpp", tempXPath, 2);

    // is_register
    tempXPath =
        "//src:decl[src:is_register()]"
        "| //src:decl_stmt[src:is_register()]"
    ;
    run_xpath_test("is_register/is_register.c", tempXPath, 2);
    run_xpath_test("is_register/is_register.cpp", tempXPath, 2);

    // is_mutable
    tempXPath =
        "//src:decl[src:is_mutable()]"
        "| //src:decl_stmt[src:is_mutable()]"
        "| //src:lambda[src:is_mutable()]"
    ;
    run_xpath_test("is_mutable/is_mutable.cpp", tempXPath, 3);

    // is_thread_local
    tempXPath =
        "//src:decl[src:is_thread_local()]"
        "| //src:decl_stmt[src:is_thread_local()]"
        "| //src:lambda[src:is_thread_local()]"
    ;
    run_xpath_test("is_thread_local/is_thread_local.c", tempXPath, 2);
    run_xpath_test("is_thread_local/is_thread_local.cpp", tempXPath, 2);

    // is_constexpr
    tempXPath =
        "//src:function[src:is_constexpr()]"
        "| //src:function_decl[src:is_constexpr()]"
    ;
    run_xpath_test("is_constexpr/is_constexpr.cpp", tempXPath, 2);

    // is_override
    tempXPath =
        "//src:function[src:is_override()]"
        "| //src:function_decl[src:is_override()]"
        "| //src:event[src:is_override()]"
        "| //src:property[src:is_override()]"
    ;
    run_xpath_test("is_override/is_override.cpp", tempXPath, 2);
    run_xpath_test("is_override/is_override.cs", tempXPath, 1);
    run_xpath_test("is_override/is_override.java", tempXPath, 1);

    // is_virtual
    tempXPath =
        "//src:function[src:is_virtual()]"
        "| //src:function_decl[src:is_virtual()]"
        "| //src:destructor[src:is_virtual()]"
        "| //src:destructor_decl[src:is_virtual()]"
        "| //src:event[src:is_virtual()]"
        "| //src:property[src:is_virtual()]"
    ;
    run_xpath_test("is_virtual/is_virtual.cpp", tempXPath, 8);
    run_xpath_test("is_virtual/is_virtual.cs", tempXPath, 31);
    run_xpath_test("is_virtual/is_virtual.java", tempXPath, 2);

    // is_overridable
    tempXPath =
        "//src:function[src:is_overridable()]"
        "| //src:function_decl[src:is_overridable()]"
        "| //src:event[src:is_overridable()]"
        "| //src:property[src:is_overridable()]"
    ;
    run_xpath_test("is_overridable/is_overridable.cpp", tempXPath, 4);
    run_xpath_test("is_overridable/is_overridable.cs", tempXPath, 25);
    run_xpath_test("is_overridable/is_overridable.java", tempXPath, 2);

    // has_auto_specifier
    tempXPath =
        "//src:decl[src:has_auto_specifier()]"
        "| //src:decl_stmt[src:has_auto_specifier()]"
    ;
    run_xpath_test("has_auto_specifier/has_auto_specifier.cpp", tempXPath, 2);

    // is_extern_block
    tempXPath =
        "//src:block[src:is_extern_block()]"
    ;
    run_xpath_test("is_extern_block/is_extern_block.cpp", tempXPath, 1);

    // is_extern
    tempXPath =
        "//src:function[src:is_extern()]"
        "| //src:function_decl[src:is_extern()]"
        "| //src:decl_stmt[src:is_extern()]"
    ;
    run_xpath_test("is_extern/is_extern.cpp", tempXPath, 1);
    run_xpath_test("is_extern/is_extern.cs", tempXPath, 2);

    // has_deduced_type
    tempXPath =
        "//src:function[src:has_deduced_type()]"
        "| //src:function_decl[src:has_deduced_type()]"
        "| //src:decl_stmt[src:has_deduced_type()]"
        "| //src:decl[src:has_deduced_type()]"
        "| //src:lambda[src:has_deduced_type()]"
    ;
    run_xpath_test("has_deduced_type/has_deduced_type.cpp", tempXPath, 7);
    run_xpath_test("has_deduced_type/has_deduced_type.cs", tempXPath, 2);

    // is_deleted
    tempXPath =
        "//src:function_decl[src:is_deleted()]"
        "| //src:constructor_decl[src:is_deleted()]"
        "| //src:destructor_decl[src:is_deleted()]"
    ;
    run_xpath_test("is_deleted/is_deleted.cpp", tempXPath, 3);

    // is_explicit
    tempXPath =
        "//src:function[src:is_explicit()]"
        "| //src:function_decl[src:is_explicit()]"
        "| //src:constructor[src:is_explicit()]"
        "| //src:constructor_decl[src:is_explicit()]"
    ;
    run_xpath_test("is_explicit/is_explicit.cpp", tempXPath, 3);
    run_xpath_test("is_explicit/is_explicit.cs", tempXPath, 2);

    // is_bitfield
    tempXPath =
        "//src:decl_stmt[src:is_bitfield()]"
        "| //src:decl[src:is_bitfield()]"
    ;
    run_xpath_test("is_bitfield/is_bitfield.cpp", tempXPath, 4);

    // is_member
    tempXPath =
        "//src:typedef[src:is_member()]"
        "| //src:function[src:is_member()]"
        "| //src:function_decl[src:is_member()]"
        "| //src:using[src:is_member()]"
        "| //src:decl_stmt[src:is_member()]"
        "| //src:decl[src:is_member()]"
        "| //src:class[src:is_member()]"
        "| //src:struct[src:is_member()]"
        "| //src:enum[src:is_member()]"
        "| //src:friend[src:is_member()]"
        "| //src:class_decl[src:is_member()]"
        "| //src:struct_decl[src:is_member()]"
        "| //src:union[src:is_member()]"
        "| //src:union_decl[src:is_member()]"
        "| //src:public[src:is_member()]"
        "| //src:private[src:is_member()]"
        "| //src:protected[src:is_member()]"
        "| //src:constructor[src:is_member()]"
        "| //src:constructor_decl[src:is_member()]"
        "| //src:destructor[src:is_member()]"
        "| //src:destructor_decl[src:is_member()]"
        "| //src:event[src:is_member()]"
        "| //src:property[src:is_member()]"
        "| //src:interface[src:is_member()]"
    ;
    run_xpath_test("is_member/is_member.c", tempXPath, 6);
    run_xpath_test("is_member/is_member.cpp", tempXPath, 21);
    run_xpath_test("is_member/is_member.cs", tempXPath, 15);
    run_xpath_test("is_member/is_member.java", tempXPath, 5);


    // is_default
    tempXPath =
        "//src:function_decl[src:is_default()]"
        "| //src:function[src:is_default()]"
        "| //src:constructor[src:is_default()]"
        "| //src:constructor_decl[src:is_default()]"
        "| //src:destructor_decl[src:is_default()]"
    ;
    run_xpath_test("is_default/is_default.cpp", tempXPath, 3);
    run_xpath_test("is_default/is_default.java", tempXPath, 2);

    // is_pointer
    tempXPath =
        "//src:function_decl[src:is_pointer()]"
        "| //src:function[src:is_pointer()]"
        "| //src:decl_stmt[src:is_pointer()]"
        "| //src:decl[src:is_pointer()]"
        "| //src:type[src:is_pointer()]"
        "| //src:param[src:is_pointer()]"
        "| //src:argument[src:is_pointer()]"
    ;
    run_xpath_test("is_pointer/is_pointer.cpp", tempXPath, 18);


    // is_parameter_pack
    tempXPath =
        "//src:type[src:is_parameter_pack()]"
        "| //src:decl[src:is_parameter_pack()]"
        "| //src:param[src:is_parameter_pack()]"
        "| //src:argument[src:is_parameter_pack()]"
    ;
    run_xpath_test("is_parameter_pack/is_parameter_pack.cpp", tempXPath, 12);

    // is_generic
    tempXPath = 
        "//src:using[src:is_generic()]"
        "| //src:function_decl[src:is_generic()]"
        "| //src:function[src:is_generic()]"
        "| //src:union[src:is_generic()]"
        "| //src:union_decl[src:is_generic()]"
        "| //src:class[src:is_generic()]"
        "| //src:class_decl[src:is_generic()]"
        "| //src:struct[src:is_generic()]"
        "| //src:struct_decl[src:is_generic()]"
        "| //src:constructor[src:is_generic()]"
        "| //src:constructor_decl[src:is_generic()]"
        "| //src:destructor[src:is_generic()]"
        "| //src:destructor_decl[src:is_generic()]"
        "| //src:interface[src:is_generic()]"
        "| //src:annotation_defn[src:is_generic()]"
    ;
    run_xpath_test("is_generic/is_generic.cpp", tempXPath, 9);
    run_xpath_test("is_generic/is_generic.cs", tempXPath, 8);
    run_xpath_test("is_generic/is_generic.java", tempXPath, 5);

    // is_noexcept
    tempXPath = 
        "//src:function_decl[src:is_noexcept()]"
        "| //src:function[src:is_noexcept()]"
    ;
    run_xpath_test("is_noexcept/is_noexcept.cpp", tempXPath, 4);

    // is_reference
    tempXPath = 
        "//src:argument[src:is_reference()]"
        "| //src:param[src:is_reference()]"
        "| //src:decl[src:is_reference()]"
        "| //src:decl_stmt[src:is_reference()]"
        "| //src:type[src:is_reference()]"
    ;
    run_xpath_test("is_reference/is_reference.cpp", tempXPath, 7);
    run_xpath_test("is_reference/is_reference.cs", tempXPath, 6);

    // is_rvalue_reference
    tempXPath = 
        "//src:argument[src:is_rvalue_reference()]"
        "| //src:param[src:is_rvalue_reference()]"
        "| //src:decl[src:is_rvalue_reference()]"
        "| //src:decl_stmt[src:is_rvalue_reference()]"
        "| //src:type[src:is_rvalue_reference()]"
    ;
    run_xpath_test("is_rvalue_reference/is_rvalue_reference.cpp", tempXPath, 7);

    // is_noreturn
    tempXPath = 
        "//src:function[src:is_noreturn()]"
        "| //src:function_decl[src:is_noreturn()]"
    ;
    run_xpath_test("is_noreturn/is_noreturn.cpp", tempXPath, 2);
    run_xpath_test("is_noreturn/is_noreturn.c", tempXPath, 2);

    // is_readonly
    tempXPath = 
        "//src:decl[src:is_readonly()]"
        "| //src:decl_stmt[src:is_readonly()]"
        "| //src:type[src:is_readonly()]"
    ;
    run_xpath_test("is_readonly/is_readonly.cs", tempXPath, 3);

    // is_protected_internal
    tempXPath = 
        "//src:class[src:is_protected_internal()]"
        "| //src:class_decl[src:is_protected_internal()]"
        "| //src:struct[src:is_protected_internal()]"
        "| //src:struct_decl[src:is_protected_internal()]"
        "| //src:function[src:is_protected_internal()]"
        "| //src:function_decl[src:is_protected_internal()]"
        "| //src:decl_stmt[src:is_protected_internal()]"
        "| //src:decl[src:is_protected_internal()]"
        "| //src:constructor[src:is_protected_internal()]"
        "| //src:constructor_decl[src:is_protected_internal()]"
        "| //src:destructor[src:is_protected_internal()]"
        "| //src:destructor_decl[src:is_protected_internal()]"
        "| //src:event[src:is_protected_internal()]"
        "| //src:property[src:is_protected_internal()]"
    ;
    run_xpath_test("is_protected_internal/is_protected_internal.cs", tempXPath, 7);

    // is_out_param
    tempXPath = 
        "//src:param[src:is_out_param()]"
        "| //src:decl[src:is_out_param()]"
        "| //src:type[src:is_out_param()]"
        "| //src:argument[src:is_out_param()]"
        "| //src:expr[src:is_out_param()]"
    ;
    run_xpath_test("is_out_param/is_out_param.cs", tempXPath, 4);


    // is_variadic
    tempXPath = 
        "//src:function[src:is_variadic()]"
        "| //src:function_decl[src:is_variadic()]"
        "| //src:constructor[src:is_variadic()]"
        "| //src:constructor_decl[src:is_variadic()]"
        "| //src:class[src:is_variadic()]"
        "| //src:class_decl[src:is_variadic()]"
        "| //src:struct[src:is_variadic()]"
        "| //src:struct_decl[src:is_variadic()]"
        "| //src:union[src:is_variadic()]"
        "| //src:union_decl[src:is_variadic()]"
        "| //src:argument_list[@type='template'][src:is_variadic()]"
        "| //src:delegate[src:is_variadic()]"
        "| //src:parameter_list[src:is_variadic()]"
        "| //src:template[src:is_variadic()]"
        "| //src:using[src:is_variadic()]"
    ;
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments.cs", tempXPath, 12);
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments_1.cpp", tempXPath, 7);
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments_2.cpp", tempXPath, 8);
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments_3.cpp", tempXPath, 18);
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments_4.cpp", tempXPath, 8);
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments_5.cpp", tempXPath, 4);
    run_xpath_test("accepts_variable_arguments/accepts_variable_arguments.java", tempXPath, 4);


    // is_new_override
    tempXPath = 
        "//src:function[src:is_new_override()]"
        "| //src:function_decl[src:is_new_override()]"
    ;
    run_xpath_test("is_new_override/is_new_override.cs", tempXPath, 2);


    // is_sealed
    tempXPath = 
        "//src:function[src:is_sealed()]"
        "| //src:function_decl[src:is_sealed()]"
        "| //src:class[src:is_sealed()]"
        "| //src:class_decl[src:is_sealed()]"
        "| //src:struct[src:is_sealed()]"
        "| //src:struct_decl[src:is_sealed()]"
        "| //src:event[src:is_sealed()]"
        "| //src:property[src:is_sealed()]"
    ;
    run_xpath_test("is_sealed/is_sealed.cs", tempXPath, 7);

    // is_async
    tempXPath = 
        "//src:function[src:is_async()]"
        "| //src:function_decl[src:is_async()]"
    ;
    run_xpath_test("is_async/is_async.cs", tempXPath, 2);

    // is_implicit
    tempXPath = 
        "//src:function[src:is_implicit()]"
        "| //src:function_decl[src:is_implicit()]"
    ;
    run_xpath_test("is_implicit/is_implicit.cs", tempXPath, 4);

    // is_yield
    tempXPath = 
        "//src:return[src:is_yield()]"
        "| //src:break[src:is_yield()]"
    ;
    run_xpath_test("is_yield/is_yield.cs", tempXPath, 2);


    // is_extension_method
    tempXPath = 
        "//src:function[src:is_extension_method()]"
        "| //src:function_decl[src:is_extension_method()]"
    ;
    run_xpath_test("is_extension_method/is_extension_method.cs", tempXPath, 2);

    // is_main
    tempXPath = 
        "//src:function[src:is_main()]"
        "| //src:function_decl[src:is_main()]"
    ;
    run_xpath_test("is_main/is_main.cpp", tempXPath, 6);
    run_xpath_test("is_main/is_main.cs", tempXPath, 2);
    run_xpath_test("is_main/is_main.java", tempXPath, 1);

    // is_extern_c
    tempXPath = 
        "//src:function[src:is_extern_c()]"
        "| //src:function_decl[src:is_extern_c()]"
        "| //src:decl_stmt[src:is_extern_c()]"
    ;
    run_xpath_test("is_extern_c/is_extern_c.cpp", tempXPath, 5);

    // is_global
    tempXPath = 
        "//src:function[src:is_global()]"
        "| //src:function_decl[src:is_global()]"
        "| //src:decl_stmt[src:is_global()]"
        "| //src:union[src:is_global()]"
        "| //src:union_decl[src:is_global()]"
        "| //src:class[src:is_global()]"
        "| //src:class_decl[src:is_global()]"
        "| //src:struct[src:is_global()]"
        "| //src:struct_decl[src:is_global()]"
        "| //src:namespace[src:is_global()]"
        "| //src:using[src:is_global()]"
        "| //src:typedef[src:is_global()]"
        "| //src:enum[src:is_global()]"
    ;
    run_xpath_test("is_global/is_global.cpp", tempXPath, 25);

    // is_operator
    tempXPath = 
        "//src:function[src:is_operator()]"
        "| //src:function_decl[src:is_operator()]"
    ;
    run_xpath_test("is_operator/is_operator.cs", tempXPath, 2);
    run_xpath_test("is_operator/is_operator.cpp", tempXPath, 2);

    // has_ref_qualifier
    tempXPath = 
        "//src:function[src:has_ref_qualifier()]"
        "| //src:function_decl[src:has_ref_qualifier()]"
    ;
    run_xpath_test("has_ref_qualifier/has_ref_qualifier.cpp", tempXPath, 6);

    // has_trailing_return_type
    tempXPath = 
        "//src:function[src:has_trailing_return_type()]"
        "| //src:function_decl[src:has_trailing_return_type()]"
    ;
    run_xpath_test("has_trailing_return_type/has_trailing_return_type.cpp", tempXPath, 3);

    // is_empty_return
    tempXPath = 
        "//src:return[src:is_empty_return()]"
    ;
    run_xpath_test("is_empty_return/is_empty_return.cpp", tempXPath, 1);

    // has_attributes
    tempXPath = 
        "//src:unit[src:has_attributes()]"
        "| //src:class[src:has_attributes()]"
        "| //src:class_decl[src:has_attributes()]"
        "| //src:struct[src:has_attributes()]"
        "| //src:struct_decl[src:has_attributes()]"
        "| //src:union[src:has_attributes()]"
        "| //src:union_decl[src:has_attributes()]"
        "| //src:function[src:has_attributes()]"
        "| //src:function_decl[src:has_attributes()]"
        "| //src:constructor[src:has_attributes()]"
        "| //src:constructor_decl[src:has_attributes()]"
        "| //src:destructor[src:has_attributes()]"
        "| //src:destructor_decl[src:has_attributes()]"
        "| //src:property[src:has_attributes()]"
        "| //src:event[src:has_attributes()]"
        "| //src:param[src:has_attributes()]"
        "| //src:decl_stmt[src:has_attributes()]"
        "| //src:decl[src:has_attributes()]"
    ;
    run_xpath_test("has_attributes/has_attributes.cs", tempXPath, 9);
    run_xpath_test("has_attributes/has_attributes.cpp", tempXPath, 2);
    run_xpath_test("has_attributes/has_attributes.java", tempXPath, 8);

    // has_function_try
    tempXPath = 
        "//src:function[src:has_function_try()]"
        "| //src:constructor[src:has_function_try()]"
        "| //src:destructor[src:has_function_try()]"
    ;
    run_xpath_test("has_function_try/has_function_try.cpp", tempXPath, 3);

    // is_KnR_C
    tempXPath = 
        "//src:function[src:is_KnR_C()]"
    ;
    run_xpath_test("is_KnR_C/is_KnR_C.c", tempXPath, 1);

    // is_template_parameter
    tempXPath = 
        "//src:param[src:is_template_parameter()]"
        "| //src:argument[src:is_template_parameter()]"
    ;
    run_xpath_test("is_template_parameter/is_template_parameter.cpp", tempXPath, 7);
    run_xpath_test("is_template_parameter/is_template_parameter.cs", tempXPath, 5);
    run_xpath_test("is_template_parameter/is_template_parameter.java", tempXPath, 4);

    // is_template_parameter_pack
    tempXPath = 
        "//src:param[src:is_template_parameter_pack()]"
    ;
    run_xpath_test("is_template_parameter_pack/is_template_parameter_pack.cpp", tempXPath, 3);

    // is_template_template_parameter
    tempXPath = 
        "//src:param[src:is_template_template_parameter()]"
    ;
    run_xpath_test("is_template_template_parameter/is_template_template_parameter.cpp", tempXPath, 1);

    // is_template
    tempXPath = 
        "//src:class[src:is_template()]"
        "| //src:class_decl[src:is_template()]"
        "| //src:union[src:is_template()]"
        "| //src:union_decl[src:is_template()]"
        "| //src:struct[src:is_template()]"
        "| //src:struct_decl[src:is_template()]"
        "| //src:decl[src:is_template()]"
        "| //src:decl_stmt[src:is_template()]"
        "| //src:function[src:is_template()]"
        "| //src:function_decl[src:is_template()]"
        "| //src:constructor[src:is_template()]"
        "| //src:constructor_decl[src:is_template()]"
        "| //src:destructor[src:is_template()]"
        "| //src:destructor_decl[src:is_template()]"
        "| //src:call[src:is_template()]"
        "| //src:name[src:is_template()]"
        "| //src:type[src:is_template()]"
        "| //src:param[src:is_template()]"
        "| //src:argument[src:is_template()]"
    ;
    run_xpath_test("is_template/is_template.cpp", tempXPath, 11);
    run_xpath_test("is_template/is_template.cs", tempXPath, 12);
    run_xpath_test("is_template/is_template.java", tempXPath, 7);

    // is_default_parameter
    tempXPath = 
        "//src:param[src:is_default_parameter()]"
    ;
    run_xpath_test("is_default_parameter/is_default_parameter.c", tempXPath, 2);
    run_xpath_test("is_default_parameter/is_default_parameter.cpp", tempXPath, 1);
    run_xpath_test("is_default_parameter/is_default_parameter.cs", tempXPath, 0);

    // is_in_param
    tempXPath = 
        "//src:argument[src:is_in_param()]"
    ;
    run_xpath_test("is_in_param/is_in_param.cs", tempXPath, 1);

    // is_constrained
    tempXPath = 
        "//src:function[src:is_constrained()]"
        "| //src:function_decl[src:is_constrained()]"
        "| //src:class[src:is_constrained()]"
        "| //src:class_decl[src:is_constrained()]"
        "| //src:struct[src:is_constrained()]"
        "| //src:struct_decl[src:is_constrained()]"
        "| //src:decl[src:is_constrained()]"
        "| //src:type[src:is_constrained()]"
        "| //src:param[src:is_constrained()]"
        "| //src:argument_list[src:is_constrained()]"
        "| //src:argument[src:is_constrained()]"
    ;
    // TODO: AWaiting fix!
    // run_xpath_test("is_constrained/is_constrained.cs", tempXPath, 4);
    run_xpath_test("is_constrained/is_constrained.java", tempXPath, 16);

    // is_bound
    tempXPath = 
        "//src:argument[src:is_bound()]"
    ;
    run_xpath_test("is_bound/is_bound.java", tempXPath, 4);

    // is_unbound
    tempXPath = 
        "//src:argument[src:is_unbound()]"
    ;
    run_xpath_test("is_unbound/is_unbound.java", tempXPath, 1);

    // is_upper_bound
    tempXPath = 
        "//src:argument[src:is_upper_bound()]"
    ;
    run_xpath_test("is_upper_bound/is_upper_bound.java", tempXPath, 3);

    // is_wildcard
    tempXPath = 
        "//src:argument[src:is_wildcard()]"
    ;
    run_xpath_test("is_wildcard/is_wildcard.java", tempXPath, 3);

    // is_lower_bound
    tempXPath = 
        "//src:argument[src:is_lower_bound()]"
    ;
    run_xpath_test("is_lower_bound/is_lower_bound.java", tempXPath, 1);


    // is_anonymous
    tempXPath = 
        "//src:namespace[src:is_anonymous()]"
        "| //src:class[src:is_anonymous()]"
        "| //src:struct[src:is_anonymous()]"
        "| //src:union[src:is_anonymous()]"
        "| //src:enum[src:is_anonymous()]"
        "| //src:decl[src:is_anonymous()]"
        "| //src:param[src:is_anonymous()]"
        "| //src:argument[src:is_anonymous()]"
    ;
    run_xpath_test("is_anonymous/is_anonymous.c", tempXPath, 7);
    run_xpath_test("is_anonymous/is_anonymous.cpp", tempXPath, 3);
    run_xpath_test("is_anonymous/is_anonymous.java", tempXPath, 3);

    // enum_specifies_integer_type
    tempXPath = 
        "//src:enum[src:enum_specifies_integer_type()]"
    ;
    run_xpath_test("enum_specifies_integer_type/enum_specifies_integer_type.cpp", tempXPath, 1);
    run_xpath_test("enum_specifies_integer_type/enum_specifies_integer_type.cs", tempXPath, 1);

    // is_complete
    tempXPath = 
        "//src:enum[src:is_complete()]"
        "| //src:class[src:is_complete()]"
        "| //src:class_decl[src:is_complete()]"
        "| //src:struct[src:is_complete()]"
        "| //src:struct_decl[src:is_complete()]"
        "| //src:union[src:is_complete()]"
        "| //src:union_decl[src:is_complete()]"
        "| //src:function[src:is_complete()]"
        "| //src:function_decl[src:is_complete()]"
        "| //src:constructor[src:is_complete()]"
        "| //src:constructor_decl[src:is_complete()]"
        "| //src:destructor[src:is_complete()]"
        "| //src:destructor_decl[src:is_complete()]"
    ;
    run_xpath_test("is_complete/is_complete.c", tempXPath, 4);
    run_xpath_test("is_complete/is_complete.cpp", tempXPath, 2);
    run_xpath_test("is_complete/is_complete.cs", tempXPath, 2);

    // enum_is_scoped
    tempXPath = 
        "//src:enum[src:enum_is_scoped()]"
    ;
    run_xpath_test("enum_is_scoped/enum_is_scoped.cpp", tempXPath, 8);

    // is_using_access_decl
    tempXPath = 
        "//src:using[src:is_using_access_decl()]"
    ;
    run_xpath_test("is_using_access_decl/is_using_access_decl.cpp", tempXPath, 2);

    // is_type_alias
    tempXPath = 
        "//src:using[src:is_type_alias()]"
    ;
    run_xpath_test("is_type_alias/is_type_alias.cpp", tempXPath, 3);

    // is_using_namespace
    tempXPath = 
        "//src:using[src:is_using_namespace()]"
    ;
    run_xpath_test("is_using_namespace/is_using_namespace.cpp", tempXPath, 2);

    // has_initializers
    tempXPath = 
        "//src:constructor[src:has_initializers()]"
    ;
    run_xpath_test("has_initializers/has_initializers.cpp", tempXPath, 2);


    // is_converting_constructor
    tempXPath = 
        "//src:constructor[src:is_converting_constructor()]"
        "| //src:constructor_decl[src:is_converting_constructor()]"
    ;
    run_xpath_test("is_converting_constructor/is_converting_constructor.cpp", tempXPath, 4);

    // has_local_storage
    tempXPath = 
        "//src:decl_stmt[src:has_local_storage()]"
        "| //src:decl[src:has_local_storage()]"
        "| //src:type[src:has_local_storage()]"
        "| //src:param[src:has_local_storage()]"
    ;
    run_xpath_test("has_local_storage/has_local_storage.c", tempXPath, 9);
    run_xpath_test("has_local_storage/has_local_storage.cpp", tempXPath, 9);

    // is_scoped_to_file
    tempXPath = 
        "//src:decl_stmt[src:is_scoped_to_file()]"
        "| //src:using[src:is_scoped_to_file()]"
        "| //src:class[src:is_scoped_to_file()]"
        "| //src:class_decl[src:is_scoped_to_file()]"
        "| //src:struct[src:is_scoped_to_file()]"
        "| //src:struct_decl[src:is_scoped_to_file()]"
        "| //src:union[src:is_scoped_to_file()]"
        "| //src:union_decl[src:is_scoped_to_file()]"
        "| //src:function[src:is_scoped_to_file()]"
        "| //src:function_decl[src:is_scoped_to_file()]"
    ;
    run_xpath_test("is_scoped_to_file/is_scoped_to_file.c", tempXPath, 3);
    run_xpath_test("is_scoped_to_file/is_scoped_to_file.cpp", tempXPath, 8);

    // is_single_decl
    tempXPath = 
        "//src:decl_stmt[src:is_single_decl()]"
    ;
    run_xpath_test("is_single_decl/is_single_decl.c", tempXPath, 1);
    run_xpath_test("is_single_decl/is_single_decl.cpp", tempXPath, 1);
    run_xpath_test("is_single_decl/is_single_decl.cs", tempXPath, 1);
    run_xpath_test("is_single_decl/is_single_decl.java", tempXPath, 1);

    // is_multi_decl
    tempXPath = 
        "//src:decl_stmt[src:is_multi_decl()]"
    ;
    run_xpath_test("is_multi_decl/is_multi_decl.c", tempXPath, 1);
    run_xpath_test("is_multi_decl/is_multi_decl.cpp", tempXPath, 1);
    run_xpath_test("is_multi_decl/is_multi_decl.cs", tempXPath, 1);
    run_xpath_test("is_multi_decl/is_multi_decl.java", tempXPath, 1);

    // is_array
    tempXPath = 
        "//src:argument[src:is_array()]"
        "| //src:param[src:is_array()]"
        "| //src:type[src:is_array()]"
        "| //src:decl_stmt[src:is_array()]"
        "| //src:decl[src:is_array()]"
    ;
    run_xpath_test("is_array/is_array.c", tempXPath, 6);
    run_xpath_test("is_array/is_array.cpp", tempXPath, 8);
    run_xpath_test("is_array/is_array.cs", tempXPath, 6);
    run_xpath_test("is_array/is_array.java", tempXPath, 7);

    // is_event_function
    tempXPath = 
        "//src:function[src:is_event_function()]"
    ;
    run_xpath_test("is_event_function/is_event_function.cs", tempXPath, 2);

    // is_property_function
    tempXPath = 
        "//src:function[src:is_property_function()]"
        "| //src:function_decl[src:is_property_function()]"
    ;
    run_xpath_test("is_property_function/is_property_function.cs", tempXPath, 4);

    // defined_event_function
    tempXPath = 
        "//src:event[src:defined_event_function()]"
    ;
    run_xpath_test("defined_event_function/defined_event_function.cs", tempXPath, 3);

    // has_add
    tempXPath = 
        "//src:event[src:has_add()]"
    ;
    run_xpath_test("has_add/has_add.cs", tempXPath, 2);

    // has_remove
    tempXPath = 
        "//src:event[src:has_remove()]"
    ;
    run_xpath_test("has_remove/has_remove.cs", tempXPath, 2);

    // defined_property_function
    tempXPath = 
        "//src:property[src:defined_property_function()]"
    ;
    run_xpath_test("defined_property_function/defined_property_function.cs", tempXPath, 3);

    // has_get
    tempXPath = 
        "//src:property[src:has_get()]"
    ;
    run_xpath_test("has_get/has_get.cs", tempXPath, 2);

    // has_set
    tempXPath = 
        "//src:property[src:has_set()]"
    ;
    run_xpath_test("has_set/has_set.cs", tempXPath, 2);



    // is_indexer
    tempXPath = 
        "//src:property[src:is_indexer()]"
    ;
    run_xpath_test("is_indexer/is_indexer.cs", tempXPath, 3);

    // is_closure
    tempXPath = 
        "//src:lambda[src:is_closure()]"
        "| //src:delegate[src:is_closure()]"
    ;
    run_xpath_test("is_closure/is_closure.cpp", tempXPath, 4);
    run_xpath_test("is_closure/is_closure.cs", tempXPath, 2);

    // is_capture_all_by_value
    tempXPath = 
        "//src:lambda[src:is_capture_all_by_value()]"
        "| //src:capture[src:is_capture_all_by_value()]"
        "| //src:argument[src:is_capture_all_by_value()]"
    ;
    run_xpath_test("is_capture_all_by_value/is_capture_all_by_value.cpp", tempXPath, 4);

    // captures_named_variable
    tempXPath = 
        "//src:lambda[src:captures_named_variable()]"
        "| //src:capture[src:captures_named_variable()]"
        "| //src:argument[src:captures_named_variable()]"
    ;
    run_xpath_test("captures_named_variable/captures_named_variable.cpp", tempXPath, 14);

    // is_function_try
    tempXPath = 
        "//src:try[src:is_function_try()]"
    ;
    run_xpath_test("is_function_try/is_function_try.cpp", tempXPath, 3);

    // has_finally
    tempXPath = 
        "//src:catch[src:has_finally()]"
        "| //src:try[src:has_finally()]"
    ;
    run_xpath_test("has_finally/has_finally.cs", tempXPath, 2);
    run_xpath_test("has_finally/has_finally.java", tempXPath, 2);

    // is_resource_try
    tempXPath = 
        "//src:try[src:is_resource_try()]"
    ;
    run_xpath_test("is_resource_try/is_resource_try.java", tempXPath, 1);

    // has_indexing_expr
    tempXPath = 
        "//src:expr[src:has_indexing_expr()]"
        "| //src:expr_stmt[src:has_indexing_expr()]"
        "| //src:init[src:has_indexing_expr()]"
        "| //src:decl[src:has_indexing_expr()]"
        "| //src:decl_stmt[src:has_indexing_expr()]"
        "| //src:argument[src:has_indexing_expr()]"
        "| //src:param[src:has_indexing_expr()]"
        "| //src:condition[src:has_indexing_expr()]"
        "| //src:incr[src:has_indexing_expr()]"
    ;
    run_xpath_test("has_indexing_expr/has_indexing_expr.c", tempXPath, 20);
    run_xpath_test("has_indexing_expr/has_indexing_expr.cpp", tempXPath, 20);
    run_xpath_test("has_indexing_expr/has_indexing_expr.cs", tempXPath, 22);
    run_xpath_test("has_indexing_expr/has_indexing_expr.java", tempXPath, 16);

    // has_call
    tempXPath = 
        "//src:expr[src:has_call()]"
        "| //src:expr_stmt[src:has_call()]"
        "| //src:argument[src:has_call()]"
        "| //src:param[src:has_call()]"
        "| //src:decl[src:has_call()]"
        "| //src:decl_stmt[src:has_call()]"
        "| //src:init[src:has_call()]"
        "| //src:condition[src:has_call()]"
        "| //src:incr[src:has_call()]"
    ;
    run_xpath_test("has_call/has_call.c", tempXPath, 22);
    run_xpath_test("has_call/has_call.cpp", tempXPath, 22);
    run_xpath_test("has_call/has_call.cs", tempXPath, 22);
    run_xpath_test("has_call/has_call.java", tempXPath, 18);

    // is_member_operator
    tempXPath = 
        "//src:operator[src:is_member_operator()]"
    ;
    run_xpath_test("is_member_operator/is_member_operator.c", tempXPath, 2);
    run_xpath_test("is_member_operator/is_member_operator.cpp", tempXPath, 5);
    run_xpath_test("is_member_operator/is_member_operator.cs", tempXPath, 3);
    run_xpath_test("is_member_operator/is_member_operator.java", tempXPath, 2);

    // is_global_scope_operator
    tempXPath = 
        "//src:operator[src:is_global_scope_operator()]"
    ;
    run_xpath_test("is_global_scope_operator/is_global_scope_operator.cpp", tempXPath, 2);

    // is_pointer_member_operator
    tempXPath = 
        "//src:operator[src:is_pointer_member_operator()]"
    ;
    run_xpath_test("is_pointer_member_operator/is_pointer_member_operator.c", tempXPath, 1);
    run_xpath_test("is_pointer_member_operator/is_pointer_member_operator.cpp", tempXPath, 2);
    run_xpath_test("is_pointer_member_operator/is_pointer_member_operator.cs", tempXPath, 1);

    // is_bitwise_operator
    tempXPath = 
        "//src:operator[src:is_bitwise_operator()]"
    ;
    run_xpath_test("is_bitwise_operator/is_bitwise_operator.c", tempXPath, 11);
    run_xpath_test("is_bitwise_operator/is_bitwise_operator.cpp", tempXPath, 11);
    run_xpath_test("is_bitwise_operator/is_bitwise_operator.cs", tempXPath, 11);
    run_xpath_test("is_bitwise_operator/is_bitwise_operator.java", tempXPath, 13);

    // is_arithmetic_operator
    tempXPath = 
        "//src:operator[src:is_arithmetic_operator()]"
    ;
    run_xpath_test("is_arithmetic_operator/is_arithmetic_operator.c", tempXPath, 10);
    run_xpath_test("is_arithmetic_operator/is_arithmetic_operator.cpp", tempXPath, 10);
    run_xpath_test("is_arithmetic_operator/is_arithmetic_operator.cs", tempXPath, 10);
    run_xpath_test("is_arithmetic_operator/is_arithmetic_operator.java", tempXPath, 10);

    // is_multiplicative_operator
    tempXPath = 
        "//src:operator[src:is_multiplicative_operator()]"
    ;
    run_xpath_test("is_multiplicative_operator/is_multiplicative_operator.c", tempXPath, 2);
    run_xpath_test("is_multiplicative_operator/is_multiplicative_operator.cpp", tempXPath, 2);
    run_xpath_test("is_multiplicative_operator/is_multiplicative_operator.cs", tempXPath, 2);
    run_xpath_test("is_multiplicative_operator/is_multiplicative_operator.java", tempXPath, 2);

    // is_divisible_operator
    tempXPath = 
        "//src:operator[src:is_divisible_operator()]"
    ;
    run_xpath_test("is_divisible_operator/is_divisible_operator.c", tempXPath, 2);
    run_xpath_test("is_divisible_operator/is_divisible_operator.cpp", tempXPath, 2);
    run_xpath_test("is_divisible_operator/is_divisible_operator.cs", tempXPath, 2);
    run_xpath_test("is_divisible_operator/is_divisible_operator.java", tempXPath, 2);

    // is_additive_operator
    tempXPath = 
        "//src:operator[src:is_additive_operator()]"
    ;
    run_xpath_test("is_additive_operator/is_additive_operator.c", tempXPath, 2);
    run_xpath_test("is_additive_operator/is_additive_operator.cpp", tempXPath, 2);
    run_xpath_test("is_additive_operator/is_additive_operator.cs", tempXPath, 2);
    run_xpath_test("is_additive_operator/is_additive_operator.java", tempXPath, 2);

    // is_subtraction_operator
    tempXPath = 
        "//src:operator[src:is_subtraction_operator()]"
    ;
    run_xpath_test("is_subtraction_operator/is_subtraction_operator.c", tempXPath, 2);
    run_xpath_test("is_subtraction_operator/is_subtraction_operator.cpp", tempXPath, 2);
    run_xpath_test("is_subtraction_operator/is_subtraction_operator.cs", tempXPath, 2);
    run_xpath_test("is_subtraction_operator/is_subtraction_operator.java", tempXPath, 2);

    // is_shift_operator
    tempXPath = 
        "//src:operator[src:is_shift_operator()]"
    ;
    run_xpath_test("is_shift_operator/is_shift_operator.c", tempXPath, 4);
    run_xpath_test("is_shift_operator/is_shift_operator.cpp", tempXPath, 4);
    run_xpath_test("is_shift_operator/is_shift_operator.cs", tempXPath, 4);
    run_xpath_test("is_shift_operator/is_shift_operator.java", tempXPath, 6);

    // is_relational_operator
    tempXPath = 
        "//src:operator[src:is_relational_operator()]"
    ;
    run_xpath_test("is_relational_operator/is_relational_operator.c", tempXPath, 6);
    run_xpath_test("is_relational_operator/is_relational_operator.cpp", tempXPath, 6);
    run_xpath_test("is_relational_operator/is_relational_operator.cs", tempXPath, 6);
    run_xpath_test("is_relational_operator/is_relational_operator.java", tempXPath, 6);

    // is_equality_operator
    tempXPath = 
        "//src:operator[src:is_equality_operator()]"
    ;
    run_xpath_test("is_equality_operator/is_equality_operator.c", tempXPath, 2);
    run_xpath_test("is_equality_operator/is_equality_operator.cpp", tempXPath, 2);
    run_xpath_test("is_equality_operator/is_equality_operator.cs", tempXPath, 2);
    run_xpath_test("is_equality_operator/is_equality_operator.java", tempXPath, 2);

    // is_logical_operator
    tempXPath = 
        "//src:operator[src:is_logical_operator()]"
    ;
    run_xpath_test("is_logical_operator/is_logical_operator.c", tempXPath, 3);
    run_xpath_test("is_logical_operator/is_logical_operator.cpp", tempXPath, 3);
    run_xpath_test("is_logical_operator/is_logical_operator.cs", tempXPath, 3);
    run_xpath_test("is_logical_operator/is_logical_operator.java", tempXPath, 3);

    // is_assignment_operator
    tempXPath = 
        "//src:operator[src:is_assignment_operator()]"
    ;
    run_xpath_test("is_assignment_operator/is_assignment_operator.c", tempXPath, 10);
    run_xpath_test("is_assignment_operator/is_assignment_operator.cpp", tempXPath, 10);
    run_xpath_test("is_assignment_operator/is_assignment_operator.cs", tempXPath, 10);
    run_xpath_test("is_assignment_operator/is_assignment_operator.java", tempXPath, 11);

    // is_compound_assignment
    tempXPath = 
        "//src:expr[src:is_compound_assignment()]"
        "| //src:expr_stmt[src:is_compound_assignment()]"
    ;
    run_xpath_test("is_compound_assignment/is_compound_assignment.c", tempXPath, 2);
    run_xpath_test("is_compound_assignment/is_compound_assignment.cpp", tempXPath, 2);
    run_xpath_test("is_compound_assignment/is_compound_assignment.cs", tempXPath, 2);
    run_xpath_test("is_compound_assignment/is_compound_assignment.java", tempXPath, 2);

    // is_shift_assign_operator
    tempXPath = 
        "//src:operator[src:is_shift_assign_operator()]"
    ;
    run_xpath_test("is_shift_assign_operator/is_shift_assign_operator.c", tempXPath, 2);
    run_xpath_test("is_shift_assign_operator/is_shift_assign_operator.cpp", tempXPath, 2);
    run_xpath_test("is_shift_assign_operator/is_shift_assign_operator.cs", tempXPath, 2);
    run_xpath_test("is_shift_assign_operator/is_shift_assign_operator.java", tempXPath, 3);

    // is_unsigned_shift_operator
    tempXPath = 
        "//src:operator[src:is_unsigned_shift_operator()]"
    ;
    run_xpath_test("is_unsigned_shift_operator/is_unsigned_shift_operator.java", tempXPath, 2);

    // is_ranged_for
    tempXPath = 
        "//src:for[src:is_ranged_for()]"
        "| //src:foreach[src:is_ranged_for()]"
    ;
    run_xpath_test("is_ranged_for/is_ranged_for.cpp", tempXPath, 1);
    run_xpath_test("is_ranged_for/is_ranged_for.java", tempXPath, 1);

    // has_default_case
    tempXPath = 
        "//src:switch[src:has_default_case()]"
    ;
    run_xpath_test("has_default_case/has_default_case.c", tempXPath, 1);
    run_xpath_test("has_default_case/has_default_case.cpp", tempXPath, 1);
    run_xpath_test("has_default_case/has_default_case.cs", tempXPath, 1);
    run_xpath_test("has_default_case/has_default_case.java", tempXPath, 1);

    // is_continue_to_label
    tempXPath = 
        "//src:continue[src:is_continue_to_label()]"
    ;
    run_xpath_test("is_continue_to_label/is_continue_to_label.java", tempXPath, 1);

    // is_break_to_label
    tempXPath = 
        "//src:break[src:is_break_to_label()]"
    ;
    run_xpath_test("is_break_to_label/is_break_to_label.java", tempXPath, 1);

    // is_goto_case
    tempXPath = 
        "//src:goto[src:is_goto_case()]"
    ;
    run_xpath_test("is_goto_case/is_goto_case.cs", tempXPath, 1);

    // is_rethrow
    tempXPath = 
        "//src:throw[src:is_rethrow()]"
    ;
    run_xpath_test("is_rethrow/is_rethrow.cpp", tempXPath, 1);
    run_xpath_test("is_rethrow/is_rethrow.cs", tempXPath, 1);
    run_xpath_test("is_rethrow/is_rethrow.java", tempXPath, 1);



    // is_nullable
    tempXPath = 
        "//src:decl[src:is_nullable()]"
        "| //src:param[src:is_nullable()]"
        "| //src:type[src:is_nullable()]"
        "| //src:decl_stmt[src:is_nullable()]"
    ;
    run_xpath_test("is_nullable/is_nullable.cs", tempXPath, 6);

    // is_type_definition
    tempXPath = 
        "//src:*[src:is_type_definition()]"
    ;
    run_xpath_test("is_type_definition/is_type_definition.c", tempXPath, 3);
    run_xpath_test("is_type_definition/is_type_definition.cpp", tempXPath, 5);
    run_xpath_test("is_type_definition/is_type_definition.cs", tempXPath, 2);
    run_xpath_test("is_type_definition/is_type_definition.java", tempXPath, 2);

    // is_class
    tempXPath = 
        "//src:*[src:is_class()]"
    ;
    run_xpath_test("is_class/is_class.cpp", tempXPath, 2);
    run_xpath_test("is_class/is_class.cs", tempXPath, 2);
    run_xpath_test("is_class/is_class.java", tempXPath, 2);

    // is_enum
    tempXPath = 
        "//src:*[src:is_enum()]"
    ;
    run_xpath_test("is_enum/is_enum.c", tempXPath, 2);
    run_xpath_test("is_enum/is_enum.cpp", tempXPath, 4);
    run_xpath_test("is_enum/is_enum.cs", tempXPath, 2);
    run_xpath_test("is_enum/is_enum.java", tempXPath, 1);

    // is_enum_class
    tempXPath = 
        "//src:*[src:is_enum_class()]"
    ;
    run_xpath_test("is_enum_class/is_enum_class.cpp", tempXPath, 2);

    // is_interface
    tempXPath = 
        "//src:*[src:is_interface()]"
    ;
    run_xpath_test("is_interface/is_interface.cs", tempXPath, 1);
    run_xpath_test("is_interface/is_interface.java", tempXPath, 1);

    // is_partial
    tempXPath = 
        "//src:*[src:is_partial()]"
    ;
    run_xpath_test("is_partial/is_partial.cs", tempXPath, 2);

    // is_struct
    tempXPath = 
        "//src:*[src:is_struct()]"
    ;
    run_xpath_test("is_struct/is_struct.c", tempXPath, 2);
    run_xpath_test("is_struct/is_struct.cpp", tempXPath, 2);
    run_xpath_test("is_struct/is_struct.cs", tempXPath, 2);

    // is_union
    tempXPath = 
        "//src:*[src:is_union()]"
    ;
    run_xpath_test("is_union/is_union.c", tempXPath, 2);
    run_xpath_test("is_union/is_union.cpp", tempXPath, 2);

    // is_delegate_type
    tempXPath = 
        "//src:*[src:is_delegate_type()]"
    ;
    run_xpath_test("is_delegate_type/is_delegate_type.cs", tempXPath, 1);

    // is_annotation
    tempXPath = 
        "//src:*[src:is_annotation()]"
    ;
    run_xpath_test("is_annotation/is_annotation.java", tempXPath, 1);


    // Not implemented yet!
    // // has_friends
    // tempXPath = 
    //     "//src:union[src:has_friends()]"
    //     "| //src:class[src:has_friends()]"
    //     "| //src:struct[src:has_friends()]"
    //     "| //src:public[src:has_friends()]"
    //     "| //src:private[src:has_friends()]"
    //     "| //src:protected[src:has_friends()]"
    // ;
    // run_xpath_test("has_friends/has_friends.cpp", tempXPath, 6);
    // 
    // // is_cuda_kernel_call
    // tempXPath = 
    //     "//src:call[src:is_cuda_kernel_call()]"
    // ;
    // run_xpath_test("is_cuda_kernel_call/is_cuda_kernel_call.c", tempXPath, 50);
    // run_xpath_test("is_cuda_kernel_call/is_cuda_kernel_call.cpp", tempXPath, 50);



    xmlCleanupParser();
    // Checking for and displaying failed tests.
    return summaryAndResultsDisplay();
}

