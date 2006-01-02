#include "srcMLException.h"

#ifndef STACK_H
#define STACK_H

template <class Type, int N>
class SimpleStack {
 public:
  SimpleStack()
    : topindex(-1)
    {}

  Type& top() {
    if (topindex < 0)
      throw Segmentation_Fault();

    return ar[topindex];
  }

  const Type& top() const {
    if (topindex < 0)
      throw Segmentation_Fault();

    return ar[topindex];
  }

  void pop() {
    --topindex;
  }

  void push(const Type& d) {
    ar[++topindex] = d;
  }

  int size() const {
    return topindex + 1;
  }

  bool empty() const {
    return size() == 0;
  }

 private:
  Type ar[N];
  int topindex;
};

#endif
