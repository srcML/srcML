_Alignas 32 int foo1;
_Alignas(32) int foo1;

struct _Alignas(16) sse_t
{
  float sse_data[4];
};

struct _Alignas 16 sse_t
{
  float sse_data[4];
};
