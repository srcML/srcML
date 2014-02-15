#include <src_input_stdin.hpp>

#include <stdio.h>
#include <unistd.h>
#include <iostream>

// check stdin for data
bool src_input_stdin() {

  // init file descriptor with stdin
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  /* 
   Need a timeout so the application doesn't
    hang waiting for input that never comes 
  */
  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  // use select to see if stdin has data
  int selectRetVal = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);

  if (selectRetVal == -1) {
    std::cerr << "SELECT FAILED!\n";
    return false;
  }
  if (selectRetVal == 0) {
    std::cerr << "NO DATA TO FETCH!\n";
    return false;
  }
  return true;
}
