#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <random>
#include <vector>

#include "run_benchmark.h"
#include "num_iterations.h"

extern "C" void xiosim_trash_cache() __attribute__ ((noinline));
void xiosim_trash_cache() { __asm__ __volatile__ ("":::"memory"); }

size_t sizes[REPEATS][ITERATIONS];
bool free_outcomes[REPEATS][ITERATIONS];
void* blocks[REPEATS][ITERATIONS];
size_t free_ind[REPEATS][ITERATIONS];

static void init_sizes(long rep) {
    std::random_device rd;
    std::default_random_engine re(rd());

    std::vector<double> intervals{16, 64, 256, 512};
    std::vector<double> weights{9, 0, 1};
    std::piecewise_constant_distribution<> dist(intervals.begin(), intervals.end(), weights.begin());

    for (size_t i = 0; i < ITERATIONS; i++) {
        sizes[rep][i] = dist(re);
    }

    std::default_random_engine re2(rd());
    std::uniform_int_distribution<size_t> d(0, ITERATIONS);
    free_ind[rep][0] = 0;
    for (size_t i = 1; i < ITERATIONS; i++) {
        free_ind[rep][i] = static_cast<size_t>(d(re2)) % i;
    }
}

static void bench_fastpath_gauss(long rep, long iterations, uintptr_t param)
{
  for (long i = 1; i < iterations; i++) {
    void *p = malloc(sizes[rep][i]);
    if (!p) {
      abort();
    }
    blocks[rep][i] = p;

    size_t to_free = free_ind[rep][i];
    assert(to_free < i);
    if (blocks[rep][to_free] != nullptr) {
        free(blocks[rep][to_free]);
        blocks[rep][to_free] = nullptr;
    }
    if (iterations % 8 == 0)
        xiosim_trash_cache();

  }
}


int main(void) {
  report_benchmark("bench_antagonist", bench_fastpath_gauss, init_sizes, 0);
  return 0;
}
