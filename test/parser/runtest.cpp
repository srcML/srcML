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
#include <map>
#include <string.h>
#include <archive.h>
#include <archive_entry.h>
#include <algorithm>

const int FIELD_WIDTH_LANGUAGE = 12;
const int FIELD_WIDTH_URL = 30;

int main(int argc, char* argv[]) {

    std::string language;

    if (argc > 1)
        language = argv[1];

    int unitnum = 0;
    if (argc > 2)
        unitnum = atoi(argv[2]);

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

        if (first) {
            archive_read_disk_descend(darchive);
            first = false;
            continue;
        }

        if (archive_entry_filetype(entry) != AE_IFREG)
            continue;

        std::string filename = archive_entry_pathname(entry);
        if (filename.substr(filename.find_last_of(".") + 1) != "xml")
            continue;

        files.push_back(std::move(filename));
    }
    archive_read_close(darchive);

    std::sort(files.begin(), files.end());

    std::map<std::string, int> ltotal;
    std::map<std::string, int> misses;

    std::vector<std::string> errors;

    int line_count = 0;
    int total = 0;
    int failed = 0;
    for (auto& filename : files) {

        bool found = true;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_filename(archive, filename.c_str());
        bool first = true;
        int count = 0;
        std::string url = srcml_archive_get_url(archive) ? srcml_archive_get_url(archive) : "";
        std::string unit_language;
        while (srcml_unit* unit = srcml_archive_read_unit(archive)) {

            if (first) {
                unit_language = srcml_unit_get_language(unit);
                if (!language.empty() && language != unit_language && language != url) {
                    found = false;
                    break;
                }
                std::cout << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << srcml_unit_get_language(unit);
                std::cout << std::setw(FIELD_WIDTH_URL) << std::left << url;
                line_count = 0;
                first = false;
            }
            ++count;
            ++total;

            if (unitnum > 0 && count > unitnum)
                break;

            ++ltotal[unit_language];

            // get the src
            char* buffer = 0;
            size_t size = 0;
            srcml_unit_set_src_encoding(unit, "UTF-8");
            srcml_unit_unparse_memory(unit, &buffer, &size);

            // get the srcml
            const char* xml = srcml_unit_get_srcml_raw(unit);

            srcml_unit* outunit = srcml_unit_clone(unit);
            srcml_unit_set_language(outunit, srcml_unit_get_language(unit));

            srcml_unit_parse_memory(outunit, buffer, size);

            const char* sout = srcml_unit_get_srcml_raw(outunit);

            if (line_count >= 75) {
                std::cout << '\n' << std::setw(FIELD_WIDTH_LANGUAGE) << " " << std::setw(FIELD_WIDTH_URL) << std::left << "...";
                line_count = 0;
            }

            line_count += std::to_string(count).size() + 1;

            if (strcmp(sout, xml) == 0) {
                std::cout << "\033[0;33m" << count << "\033[0m";
            } else {
                ++failed;
                ++misses[unit_language];

                std::string errreport = unit_language;
                errreport += '\t';
                errreport += url;
                errreport += '\t';
                errreport += std::to_string(count);
                errreport += '\n';
                errreport += "test:";
                errreport += xml;
                errreport += '\n';
                errreport += "srcml:";
                errreport += sout;
                errreport += '\n';
                errors.push_back(errreport);
                std::cout << "\033[0;31m" << count << "\033[0m";
            }
            std::cout << " ";

            free(buffer);
            srcml_unit_free(outunit);
            srcml_unit_free(unit);
        }
        if (found)
            std::cout << '\n';

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // error report
    std::cout << "Errors:\n";
    for (const auto& err : errors) {
        std::cout << err;
    }
    double percent = double(failed * 100) / total;
    std::cout << "\nCounts: " << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << "Total" << std::setw(6) << std::right << failed << std::setw(6) << std::right << total << '\t' << std::setprecision(2) << percent << "%" << '\n';

    for (auto& kv : ltotal) {
        double percent = double(misses[kv.first] * 100) / kv.second;
        std::cout << "        " << std::setw(FIELD_WIDTH_LANGUAGE) << std::left << kv.first << std::setw(6) << std::right << misses[kv.first] << std::setw(6) << std::right << kv.second << '\t' << std::setprecision(2) << percent << "%" << '\n';
    }

    return 0;
}