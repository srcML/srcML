/**
 * @file input_s3.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef LINKING_WITH_AWS_SDK

#include <string>
#include <input_s3.hpp>
#include <SRCMLStatus.hpp>
#include <srcml_pipe.hpp>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/config/AWSProfileConfigLoader.h>

// downloads s3 object into fd
int input_s3(srcml_input_src& input) {
    srcml_pipe(input, [](const srcml_request_t& srcml_request, const srcml_input_t& input_sources, const srcml_output_dest& destination) {
        std::string raw_url = input_sources[0].resource.substr(0, input_sources[0].resource.find('/'));
        std::string raw_bucket_name = raw_url.substr(0, raw_url.find('.'));
        std::string raw_object_name = input_sources[0].resource.substr(raw_url.length() + 1);

        Aws::SDKOptions options;
        Aws::InitAPI(options);

        Aws::Client::ClientConfiguration clientConfig;

        /*
            s3 client defaults to us-east-1
            sdk doesn't automatically pick up
            region config in ~/.aws/config
            options are to pass region into cli
            or use AWSProfileConfigLoader
        */
        // clientConfig.region = "us-east-1";

        const Aws::String bucket_name = raw_bucket_name.c_str();
        const Aws::String object_name = raw_object_name.c_str();

        Aws::S3::S3Client s3_client(clientConfig);

        Aws::S3::Model::GetObjectRequest object_request;
        object_request.SetBucket(bucket_name);
        object_request.SetKey(object_name);
        auto get_object_outcome = s3_client.GetObject(object_request);
        if (get_object_outcome.IsSuccess()) {
            auto &retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();
            char buffer[512] = { 0 };
            while (retrieved_file.getline(buffer, 511)) {
                // TODO: Can do this without creating an intermediate string
                std::string line(buffer);
                line += "\n";
                ssize_t result = write(*destination.fd, line.c_str(), line.size());
                if (result < 0) {
                    SRCMLstatus(WARNING_MSG, "srcml: Buffer error with s3 object: %s/%s",raw_bucket_name, raw_object_name);
                }
            }
        }
        else {
            auto error = get_object_outcome.GetError();
            SRCMLstatus(ERROR_MSG, "srcml: Error %s (%s)", error.GetExceptionName(), error.GetMessage());
        }

        Aws::ShutdownAPI(options);

        close(*destination.fd);
    });

    /*
        Need to tie this retrun to the success of the query
        similar to what input_curl is doing
    */
    return true;
}

#endif