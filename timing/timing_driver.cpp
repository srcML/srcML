#include <iostream>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop

int main(int argc, char * argv[]) {

    if(argc < 2) {

        std::cerr << "timing_driver command args ...\n";
        exit(1);

    }

    std::string str_command = "";
    while(*(++argv)) {

        str_command += argv[0];
        str_command += " ";

    }

    //std::cout << str_command << '\n';

    const char * command = str_command.c_str();

    boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::universal_time();

    FILE * sub_process = popen(command, "r");
    pclose(sub_process);

    std::string time = to_simple_string(boost::posix_time::microsec_clock::universal_time() - start_time);

    std::cout << time << '\n';

    return 0;
}
