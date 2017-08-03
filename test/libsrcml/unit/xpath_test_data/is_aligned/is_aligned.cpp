alignas(32) int foo1;

struct alignas(16) sse_t
{
  float sse_data[4];
};
