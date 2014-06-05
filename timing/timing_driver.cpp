#include <iostream>
#include <string>
#include <fstream>

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

    size_t line_count = 0;

    bool is_text = false;
    while(in) {

        char c;
        in.get(c);

        if(c == '\n') {

            ++line_count;
            is_text = false;

        } else {

            is_text = true;

        }

    }

    if(line_count && !is_text) line_count--;
    if(is_text) ++line_count;

    std::string str_command = "srcml " + input_file + " -o " + output_file;

    //std::cout << "Command: " << str_command << " Line count: " << line_count << '\n';

    const char * command = str_command.c_str();

    boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::universal_time();

    FILE * sub_process = popen(command, "r");
    pclose(sub_process);

    std::string time = to_simple_string(boost::posix_time::microsec_clock::universal_time() - start_time);

    std::cout << time << '\n';

    return 0;
}
