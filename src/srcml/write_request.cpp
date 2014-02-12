#include <write_request.hpp>

bool operator<(const WriteRequest& n1, const WriteRequest& n2) {
    return n1.position < n2.position;
}

