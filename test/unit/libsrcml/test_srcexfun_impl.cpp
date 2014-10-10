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
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<xmlDoc> xml_doc_shared_ptr_t; 
using namespace std;
/*  Builds the xml document for libxml2 so that the results can be more
 *  easily inspected during testing.
 */
xml_doc_shared_ptr_t run_xpath(std::string const& inputFileName, std::string const& xpathExpression) {

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

    const char* ext = srcml_archive_check_extension(processedArchive, inputFileName.c_str());
    if(!ext) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed get file extension.");   
    }

    rc = srcml_unit_set_language(unit, ext);
    if(rc != SRCML_STATUS_OK) {
        cout << "Last Error string: " << srcml_error_string() << endl;
        throw std::runtime_error("Failed to set language.");
    }

    rc = srcml_parse_unit_filename(unit, inputFileName.c_str());
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
    rc = srcml_append_transform_xpath(processedArchive, xpathExpression.c_str());
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

    free(archiveBuffer);

    // Turning XPath document into libxml2 xmlDoc.
    xmlDocPtr ret = xmlParseMemory(xpathResultArchiveBuffer, xpathResultArchiveBufferSize);
    srcml_free_archive(processedArchive);
    srcml_free_archive(xpathResultArchive);
    free(xpathResultArchiveBuffer);
    return xml_doc_shared_ptr_t(ret, xmlFreeDoc);
}

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

int main() {
    xmlInitParser();
    xml_doc_shared_ptr_t doc = run_xpath("xpath_test_data/has_return/has_return.cpp", "//src:unit[src:has_return()] | //src:function[src:has_return()] | //src:if[src:has_return()] ");
    dbgDoc(doc);

    xml_doc_shared_ptr_t doc2 = run_xpath("xpath_test_data/has_return/has_return_fail.cpp", "//cpp:define[src:has_return()]");
    dbgDoc(doc2);
    xmlCleanupParser();
    return 0;
}

