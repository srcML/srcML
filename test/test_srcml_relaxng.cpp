/*

  Test cases for srcml_xslt
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#include <srcml_sax2_utilities.hpp>
#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_relaxng
   */

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * relaxngs[2] = {"schema.rng", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_relaxng(buffer_input, relaxngs, fd, 0) == SRCML_STATUS_OK);
    std::ifstream in("project.xml");
    std::string output;
    std::string temp;
    while(in >> temp)
      output += temp;
    assert(output == "<?xmlversion=\"1.0\"encoding=\"\"standalone=\"yes\"?><unit>a;</unit>");
    unlink("input.xml");
    unlink("project.xml");
  }

  return 0;

}
