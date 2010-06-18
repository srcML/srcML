#include "URIStream.h"
#include <iostream>
#include <vector>

int main() {

  std::vector<URIStream *> streams;
  URIStream * stream = new URIStream("http://www.cs.uakron.edu/~mjd52/test");
  streams.push_back(stream);

  char* line;
  while (!streams.empty())
  {
	  while(line = streams.back()->getline())
	   //std::cout <<line <<'\n';
		  if(std::strstr(line, "://") != NULL)
			  streams.push_back(new URIStream(line));
		  else
			  std::cout << line << '\n';
	  streams.pop_back();
  }
}
