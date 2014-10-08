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

using namespace std;

int main() {

    string inputFileName = "xpath_test_data/has_return/has_return.cpp";
    string xpathExpression = "//src:unit";
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
    

    cout.write(xpathResultArchiveBuffer, xpathResultArchiveBufferSize);
    cout << endl;

    free(xpathResultArchiveBuffer);
    free(archiveBuffer);
    // srcml_archive* oarchive;
    // srcml_read_open_filename(iarchive, "project.xml");
    // oarchive = srcml_clone_archive(iarchive);
    // srcml_write_open_filename(oarchive, "xpath.xml");

    // srcml_append_transform_xpath(iarchive, "//src:unit");

    // srcml_apply_transforms(iarchive, oarchive);
/*    
    srcml_close_archive(iarchive);
    srcml_close_archive(oarchive);

    srcml_free_archive(iarchive);
    srcml_free_archive(oarchive);


    int i;
    struct srcml_archive* archive;
    struct srcml_unit* unit;
    char * s;
    int size;
    int srcml_input;

    // create a new srcml archive structure
    archive = srcml_create_archive();

    //open a srcML archive for output
    srcml_write_open_memory(archive, &s, &size);

    //add all the files to the archive
    for (i = 1; i < argc; ++i) {

        unit = srcml_create_unit(archive);

        // Translate to srcml and append to the archive
        char buffer[256];
        srcml_input = OPEN(argv[i], O_RDONLY, 0);
        int num_read = READ(srcml_input, buffer, 256);
        CLOSE(srcml_input);
        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));

        srcml_parse_unit_memory(unit, buffer, num_read);

        // Translate to srcml and append to the archive
        srcml_write_unit(archive, unit);

        srcml_free_unit(unit);
    }

    // close the srcML archive 
    srcml_close_archive(archive);

    // free the srcML archive data
    srcml_free_archive(archive);

    // now dump the contents of the archive
    puts(s);
    free(s);
*/
    return 0;
}

