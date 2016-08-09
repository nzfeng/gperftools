#include <cstdlib>
#include <cstdint>
#include <random>
#include <vector>

#include "run_benchmark.h"

// Structs of increasing sizes to enable the compiler to invoke sized delete.
struct size32 {
  uint32_t elems[1];
};

struct size64 {
  uint32_t elems[2];
};

struct size96 {
  uint32_t elems[3];
};

struct size128 {
  uint32_t elems[4];
};

struct size160 {
  uint32_t elems[5];
};

struct size192 {
  uint32_t elems[6];
};

struct size224 {
  uint32_t elems[7];
};

struct size256 {
  uint32_t elems[8];
};

static void bench_sized_deletes(long rep, long iterations, uintptr_t param)
{
  for (; iterations>0; iterations-=8) {
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
