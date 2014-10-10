/**
 * @file test_srcexfunc_impl.c
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <cstdlib>
#include <cassert>
#include <vector>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <boost/shared_ptr.hpp>
#include <libxml/xpathInternals.h>
#include <srcmlns.hpp>



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

    char* archiveBuffer = 0;
    int archiveBufferSize = 0;
    int rc = 0;

    // Create an archive to handle the data being read from a file.
    srcml_archive* processedArchive = srcml_create_archive();
    if(!processedArchive) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to create processed archive.");
    }

    rc = srcml_write_open_memory(processedArchive, &archiveBuffer, &archiveBufferSize);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed open memory for writing.");
    }

    srcml_unit* unit = srcml_create_unit(processedArchive);
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

    rc = srcml_parse_unit_filename(unit, testFile.c_str());
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

    srcml_free_unit(unit);
    srcml_close_archive(processedArchive);
    srcml_free_archive(processedArchive);

    processedArchive = 0;
    processedArchive = srcml_create_archive();
    if(!processedArchive) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed creating processed archive for 2nd time..");
    }

    rc = srcml_read_open_memory(processedArchive, archiveBuffer, archiveBufferSize);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed open memory for reading.");
    }

    // Processing xpath from archive.
    char* xpathResultArchiveBuffer = 0;
    int xpathResultArchiveBufferSize = 0;

    srcml_archive* xpathResultArchive = srcml_create_archive();
    if(!xpathResultArchive) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to create xpathResultArchive.");
    }

    // Handling xpath archive.
    rc = srcml_write_open_memory(xpathResultArchive, &xpathResultArchiveBuffer, &xpathResultArchiveBufferSize);
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
    srcml_close_archive(xpathResultArchive);

    // free(archiveBuffer);

    // Turning XPath document into libxml2 xmlDoc.
    xmlDocPtr ret = xmlParseMemory(xpathResultArchiveBuffer, xpathResultArchiveBufferSize);
    srcml_free_archive(processedArchive);
    srcml_free_archive(xpathResultArchive);
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
            cout << "Result: " << resultIndex << " " << endl;
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
        "xpath_test_data/has_return/has_return.cpp",
        "//src:unit[src:has_return()] "
        "| //src:function[src:has_return()] "
        "| //src:destructor[src:has_return()]"
        "| //src:if[src:has_return()] "
        "| //src:else[src:has_return()] "
        "| //src:elseif[src:has_return()] "
        "| //src:then[src:has_return()] "
        "| //src:switch[src:has_return()]",
        12);

    run_xpath_test(
        "xpath_test_data/has_return/has_return_scope_test.cpp",
        "//src:block[src:has_return()]"
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
        "| //src:unchecked[src:has_return()]",
        6
    );

    run_xpath_test(
        "xpath_test_data/has_return/has_return_return_stmt_test.cpp",
        "//src:return[src:has_return()] ",
        0
    );

    run_xpath_test(
        "xpath_test_data/has_return/has_return.cs",
        "//src:block[src:has_return()]"
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
        "| //src:unchecked[src:has_return()]",
        8
    );

    run_xpath_test(
        "xpath_test_data/has_return/has_return.java",
        "//src:block[src:has_return()]"
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
        "| //src:unchecked[src:has_return()]",
        4
    );

    xmlCleanupParser();


    // Checking for and displaying failed tests.

    return summaryAndResultsDisplay();
}

