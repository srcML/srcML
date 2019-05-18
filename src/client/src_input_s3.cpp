/**
 * @file src_input_s3.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <src_input_s3.hpp>
#include <srcml_options.hpp>
#include <src_input_libarchive.hpp>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/client/ClientConfiguration.h>
#include <string>

// Convert input to a ParseRequest and assign request to the processing queue
int src_input_s3(ParseQueue& queue,
                    srcml_archive* srcml_arch,
                    const srcml_request_t& srcml_request,
                    const srcml_input_src& input) {

    std::string bucketname;
    std::string objectname;
    bucketname = input.resource.substr(0, input.resource.find('/'));
    objectname = input.resource.substr(bucketname.length() + 1);
    
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {

      Aws::Client::ClientConfiguration clientConfig;
      clientConfig.region = Aws::Region::US_EAST_1;

      // Assign these values before running the program
      const Aws::String bucket_name = bucketname.c_str();
      const Aws::String object_name = objectname.c_str();

      // Set up the request
      Aws::S3::S3Client s3_client(clientConfig);
      Aws::S3::Model::GetObjectRequest object_request;
      object_request.SetBucket(bucket_name);
      object_request.SetKey(object_name);

      // Get the object
      auto get_object_outcome = s3_client.GetObject(object_request);
      if (get_object_outcome.IsSuccess())
      {
          // Get an Aws::IOStream reference to the retrieved file
          auto &retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();

          std::string file = "";
          char file_data[512] = { 0 };
          while (retrieved_file.getline(file_data, 511)) {
            std::string line(file_data);
            file.append(line+"\n");
          }

          fprintf(stderr, "%s", file.c_str());

      }
      else
      {
          auto error = get_object_outcome.GetError();
          std::cout << "ERROR: " << error.GetExceptionName() << ": " << error.GetMessage() << std::endl;
      }

    }
    Aws::ShutdownAPI(options);

    return 1;
}