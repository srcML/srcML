/**
 * @file src_input_remote.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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

/*
  src_input_remote assigns remote files and archives to the srcml parsing queue
*/

#include <src_input_remote.hpp>
#include <vector>
#include <curl/curl.h>
#include <archive.h>
#include <archive_entry.h>

#ifdef __MACH__
#include <sys/time.h>
#define CLOCK_REALTIME 0
//clock_gettime is not implemented on OSX
int clock_gettime(int /*clk_id*/, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

#define CURL_BUFFER_SIZE 16000

struct curl {
    CURL *handle;
    CURLM *multi_handle;
    CURLMsg *msg;
    int msgs_left;
    int still_running;
    size_t data_len;
    char *data_buffer;
    std::string source;
};

size_t curl_cb(void *buffer, size_t len, size_t nmemb, void *data) {
    curl *curling = (curl*) data;
    curling->data_len = len * nmemb;
    curling->data_buffer = (char *)buffer;
    return curling->data_len;
}

curl* curl_create(const std::string& source) {
    curl *curling = new curl();
    curling->source = source;
    return curling;
}

int arch_my_open(archive *, void *client_data) {
    curl *curling = (curl*) client_data;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curling->handle = curl_easy_init();
    curl_easy_setopt(curling->handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curling->handle, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curling->handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
    curl_easy_setopt(curling->handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curling->handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curling->handle, CURLOPT_WRITEFUNCTION, curl_cb);
    curl_easy_setopt(curling->handle, CURLOPT_WRITEDATA, curling);
    curl_easy_setopt(curling->handle, CURLOPT_URL, curling->source.c_str());

    curling->multi_handle = curl_multi_init();
    curl_multi_add_handle(curling->multi_handle, curling->handle);
    curl_multi_perform(curling->multi_handle, &curling->still_running);

    // TODO: SOMETHING HERE TO MAKE SURE THE FILE IS ACTUALLY PRESENT
    return ARCHIVE_OK;
}

void curl_messages(curl *curling) {
    while ((curling->msg = curl_multi_info_read(curling->multi_handle, &curling->msgs_left))) {
        if (curling->msg->msg == CURLMSG_DONE) {
            if (curling->msg->data.result == 0) {
//                std::cerr << "Download Complete!\n";
            }
            else {
                std::cerr << "Download status: " << curling->msg->data.result << "\n";
            }
            break;
        }
    }
}

void curl_cleanup(curl *curling) {
    curl_multi_cleanup(curling->multi_handle);
    curl_easy_cleanup(curling->handle);
}

ssize_t arch_my_read(archive *, void *client_data, const void **buff)
{
    curl *mydata = (curl*) client_data;

    mydata->data_len = 0;

    while(mydata->data_len == 0 && mydata->still_running) {
        curl_multi_perform(mydata->multi_handle, &mydata->still_running);
    }

    *buff = mydata->data_buffer;
    return mydata->data_len;
}

int arch_my_close(archive *, void *client_data)
{
    curl *mydata = (curl*) client_data;
    curl_messages(mydata);
    curl_cleanup(mydata);
    delete mydata;
    return 0;
}

void src_input_remote(ParseQueue& queue,
                      srcml_archive* srcml_arch,
                      const std::string& remote_uri,
                      const boost::optional<std::string>& option_language,
                      const boost::optional<std::string>& option_filename,
                      const boost::optional<std::string>& option_directory,
                      const boost::optional<std::string>& option_version) {
    // READ ARCHIVE

    archive *arch;
    archive_entry *entry;
    arch = archive_read_new();
    archive_read_support_format_ar(arch);
    archive_read_support_format_cpio(arch);
    archive_read_support_format_gnutar(arch);
    archive_read_support_format_iso9660(arch);
    archive_read_support_format_mtree(arch);
    archive_read_support_format_tar(arch);
    archive_read_support_format_xar(arch);
    archive_read_support_format_zip(arch);
    archive_read_support_format_raw(arch);
    archive_read_support_format_empty(arch);

    /* Check libarchive version enable version specific features/syntax */
#if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(arch);
#else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(arch);
    archive_read_support_format_cab(arch);
    archive_read_support_format_lha(arch);
    archive_read_support_format_rar(arch);

    // Compressions
    archive_read_support_filter_all(arch);
#endif

    int status = archive_read_open(arch, curl_create(remote_uri), arch_my_open, arch_my_read, arch_my_close);
    if (status == ARCHIVE_OK) {

        /* In general, go through this once for each time the header can be read
           Exception: if empty, go through the loop exactly once */
        int count = 0;
        int status = ARCHIVE_OK;
        while (status == ARCHIVE_OK &&
               (((status = archive_read_next_header(arch, &entry)) == ARCHIVE_OK) ||
                (status == ARCHIVE_EOF && !count))) {

            // skip any directories
            if (status == ARCHIVE_OK && archive_entry_filetype(entry) == AE_IFDIR)
                continue;

            // default is filename from archive entry (if not empty)
            std::string filename = status == ARCHIVE_OK ? archive_entry_pathname(entry) : "";

            if (count && filename != "data")
                srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);

            // archive entry filename for non-archive input is "data"
            if (filename.empty() || filename == "data") {
                filename = remote_uri;
            }

            if (option_filename)
                filename = *option_filename;

            // language may have been explicitly set
            std::string language;

            if (option_language)
                language = *option_language;

            // if not explicitly set, language comes from extension
            // we have to do this ourselves, since libsrcml won't for memory
            if (language.empty())
                if (const char* l = srcml_archive_check_extension(srcml_arch, filename.c_str()))
                    language = l;

            // form the parsing request
            ParseRequest request;
            if (option_filename || filename != "-")
                request.filename = filename;
            request.directory = option_directory;
            request.version = option_version;
            request.srcml_arch = srcml_arch;
            request.language = language;
            request.status = !language.empty() ? 0 : SRCML_STATUS_UNSET_LANGUAGE;

            // fill up the parse request buffer
            request.buffer.clear();
            if (!status) {
                const char* buffer;
                size_t size;
                int64_t offset;
                while (status == ARCHIVE_OK && archive_read_data_block(arch, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
                    request.buffer.insert(request.buffer.end(), buffer, buffer + size);
            }

            // Hand request off to the processing queue
            queue.push(request);

            ++count;
        }
        archive_read_finish(arch);
    }
}
