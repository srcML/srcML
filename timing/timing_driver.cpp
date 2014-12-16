/**
 * @file timing_driver.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop

int main(int argc, char * argv[]) {

    if(argc < 3) {

        std::cerr << "timing_driver input_file log_file\n";
        exit(1);

    }

    std::string input_file = argv[1];

    std::ifstream in(input_file);

    size_t lines_of_code = 0;

    bool is_text = false;
    while(in) {

        char c;
        in.get(c);

        if(c == '\n') {

            ++lines_of_code;
            is_text = false;

        } else {

            is_text = true;

        }

    }

    if(lines_of_code && !is_text) lines_of_code--;
    if(is_text) ++lines_of_code;

    std::string extension = input_file.substr(input_file.rfind('.') + 1);

    std::string language = "";
    if(extension == "hpp" || extension == "cpp") language = "C++";
    if(extension == "h" || extension == "c") language = "C";
    if(extension == "cs") language = "C#";
    if(extension == "java") language = "Java";

    std::string str_command = "srcml " + input_file + " -o " + input_file + ".xml";

    //std::cout << "Command: " << str_command << " Line count: " << lines_of_code << '\n';

    const char * command = str_command.c_str();

    boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::universal_time();

    FILE * sub_process = popen(command, "r");
    pclose(sub_process);

    boost::posix_time::time_duration elapsed_time_duration = boost::posix_time::microsec_clock::universal_time() - start_time;

    long double elapsed_seconds = elapsed_time_duration.total_seconds() + (elapsed_time_duration.fractional_seconds() / pow((long double)10, boost::posix_time::time_duration::num_fractional_digits()));

    unsigned long long lines_of_code_per_second = (lines_of_code / elapsed_seconds) + 0.5;

    std::ostringstream lines_of_code_per_second_stream;
    if(lines_of_code_per_second > 1000)
        lines_of_code_per_second_stream << lines_of_code_per_second / 1000 << ',' << std::setfill('0') << std::setw(3) << (lines_of_code_per_second % 1000);
    else
        lines_of_code_per_second_stream << lines_of_code_per_second;

    std::cout << std::left << std::setw(9) << language << std::right << std::setw(6) << lines_of_code_per_second_stream.str() << " LOC/sec\n";

    std::string log_file_name = argv[2];

    std::ofstream log_file(log_file_name, std::ios_base::app);

    log_file << std::left << std::setw(9) << language << std::right << std::setw(6) << lines_of_code_per_second_stream.str() << " LOC/sec\n";

    return 0;
}
