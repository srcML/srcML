#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop

int main(int argc, char * argv[]) {

    if(argc < 3) {

        std::cerr << "timing_driver input_file output_file\n";
        exit(1);

    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

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

    std::string str_command = "srcml " + input_file + " -o " + output_file;

    //std::cout << "Command: " << str_command << " Line count: " << lines_of_code << '\n';

    const char * command = str_command.c_str();

    boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::universal_time();

    FILE * sub_process = popen(command, "r");
    pclose(sub_process);

    boost::posix_time::time_duration elapsed_time_duration = boost::posix_time::microsec_clock::universal_time() - start_time;

    long double elapsed_seconds = elapsed_time_duration.total_seconds() + (elapsed_time_duration.fractional_seconds() / pow((long double)10, boost::posix_time::time_duration::num_fractional_digits()));

    unsigned long long lines_of_code_per_second = (lines_of_code / elapsed_seconds) + 0.5;

    std::cout << lines_of_code_per_second / 1000 << ',' << (lines_of_code_per_second % 1000) << " LOC/sec\n";

    return 0;
}
