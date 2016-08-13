#include <cstdlib>
#include <cstdint>
#include <random>
#include <vector>

#include "run_benchmark.h"
#include "num_iterations.h"

// Structs of increasing sizes to enable the compiler to invoke sized delete.
struct size4 {
  size4() {}
  uint32_t elems[1];
};

struct size32 {
  size32() {}
  uint32_t elems[8];
};

struct size64 {
  size64() {}
  uint32_t elems[16];
};

struct size96 {
  size96() {}
  uint32_t elems[24];
};

struct size128 {
  size128() {}
  uint32_t elems[32];
};

struct size160 {
  size160(){}
  uint32_t elems[40];
};

struct size192 {
  size192() {}
  uint32_t elems[48];
};

struct size224 {
  size224() {}
  uint32_t elems[56];
};

struct size256 {
  size256() {}
  uint32_t elems[64];
};

static void bench_sized_deletes(long rep, long iterations, uintptr_t param)
{
  for (; iterations>0; iterations-=1) {
    size32 *p32 = new size32();
    if (!p32) abort();
    delete p32;

    size64 *p64 = new size64();
    if (!p64) abort();
    delete p64;

    size96 *p96 = new size96();
    if (!p96) abort();
    delete p96;

    size128 *p128 = new size128();
    if (!p128) abort();
    delete p128;

    size160 *p160 = new size160();
    if (!p160) abort();
    delete p160;

    size192 *p192 = new size192();
    if (!p192) abort();
    delete p192;

    size224 *p224 = new size224();
    if (!p224) abort();
    delete p224;

    size256 *p256 = new size256();
    if (!p256) abort();
    delete p256;
  }
}

int main(void) {
  report_benchmark("bench_sized_deletes", bench_sized_deletes, NULL, 0);
  return 0;
}
