/*
    runtest.cpp

    Runs the srcML parser tests.
*/
#include <srcml.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <deque>
#include <vector>
#include <archive.h>
#include <archive_entry.h>

const int FIELD_WIDTH_LANGUAGE = 15;
const int FIELD_WIDTH_URL = 30;

int main(int argc, char* argv[]) {

    std::string language;

    if (argc > 1)
        language = argv[1];

    // get a list of files (including directories) from the current directory
    std::vector<std::string> files;

    // start at the root of the tree
    auto darchive = archive_read_disk_new();
#if ARCHIVE_VERSION_NUMBER >= 3002003
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_ACL | ARCHIVE_READDISK_NO_XATTR | ARCHIVE_READDISK_NO_FFLAGS);
#elif ARCHIVE_VERSION_NUMBER >= 3002000
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_XATTR);
#endif
    archive_read_disk_open(darchive, "testsuite");
    archive_entry* entry = nullptr;
    bool first = true;
    int status = ARCHIVE_OK;
    while ((status = archive_read_next_header(darchive, &entry)) == ARCHIVE_OK) {

        archive_read_disk_descend(darchive);
        if (first) {
            first = false;
            continue;
        }

        std::string filename = archive_entry_pathname(entry);
        if (filename.substr(filename.find_last_of(".") + 1) != "xml")
            continue;

        if (archive_entry_filetype(entry) != AE_IFREG)
            continue;

        files.push_back(archive_entry_pathname(entry));
    }
    archive_read_close(darchive);

    std::sort(files.begin(), files.end());

    std::cerr << "FILES: " << files[1] << '\n';

    srcml_archive* outarchive = srcml_archive_create();
    char* outbuffer = 0;
    size_t outsize = 0;
    srcml_archive_write_open_memory(outarchive, &outbuffer, &outsize);
    srcml_unit* outunit = srcml_unit_create(outarchive);
    int line_count = 0;
    for (auto& filename : files)
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, filename.c_str());
        bool first = true;
        int count = 0;
        std::string url = srcml_archive_get_url(archive);
        while (srcml_unit* unit = srcml_archive_read_unit(archive)) {

            if (first) {
                std::string unit_language = srcml_unit_get_language(unit);
                if (!language.empty() && language != unit_language)
                    break;
                std::cout << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << srcml_unit_get_language(unit);
                std::cout << std::setw(FIELD_WIDTH_URL) << std::left << url;
                line_count = 0;
            }
            first = false;
            ++count;

            // get the src
            char* buffer = 0;
            size_t size = 0;
            srcml_unit_unparse_memory(unit, &buffer, &size);
            std::string s(buffer, size);
            //fprintf(stderr, "DEBUG:  %s %s %d s: %s\n", __FILE__,  __FUNCTION__, __LINE__,  s.c_str());

            // get the srcml
            std::string xml = srcml_unit_get_srcml(unit);

            //fprintf(stderr, "DEBUG:  %s %s %d s: %s\n", __FILE__,  __FUNCTION__, __LINE__,  s.c_str());
            //fprintf(stderr, "DEBUG:  %s %s %d xml: %s\n", __FILE__,  __FUNCTION__, __LINE__,  xml.c_str());

            // parse the source
            srcml_unit_set_language(outunit, srcml_unit_get_language(unit));
            int n = srcml_unit_parse_memory(outunit, buffer, size);
            //fprintf(stderr, "DEBUG:  %s %s %d n: %d\n", __FILE__,  __FUNCTION__, __LINE__,  (int) n);

            std::string sout = srcml_unit_get_srcml(outunit);
            //fprintf(stderr, "DEBUG:  %s %s %d sout: %s\n", __FILE__,  __FUNCTION__, __LINE__, sout);

            if (line_count >= 75) {
                std::cout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << " " << std::setw(FIELD_WIDTH_URL) << std::left << "...";
                line_count = 0;
            }

            if (count > 99)
                line_count += 3;
            else if (count > 9)
                line_count += 2;
            else
                line_count += 1;

            if (sout == xml) {
                std::cout << "\033[0;31m" << count << "\033[0m";
            } else {
                std::cout << "\033[0;33m" << count << "\033[0m";
            }
            std::cout << " ";

            srcml_unit_free(unit);
        }
        std::cout << '\n';

        srcml_archive_close(archive);

    }

    srcml_archive_close(outarchive);

    return 0;
}