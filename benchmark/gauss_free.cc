#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <random>
#include <vector>

#include "run_benchmark.h"

size_t sizes[ITERATIONS];
bool free_outcomes[ITERATIONS];
void* blocks[ITERATIONS];
size_t free_ind[ITERATIONS];

static void init_sizes() {
    std::random_device rd;
    std::default_random_engine re(rd());

    std::vector<double> intervals{16, 64, 256, 512};
    std::vector<double> weights{9, 0, 1};
    std::piecewise_constant_distribution<> dist(intervals.begin(), intervals.end(), weights.begin());

    for (size_t i = 0; i < ITERATIONS; i++) {
        sizes[i] = dist(re);
    }

    std::default_random_engine re2(rd());
    std::uniform_int_distribution<size_t> d(0, ITERATIONS);
    free_ind[0] = 0;
    for (size_t i = 1; i < ITERATIONS; i++) {
        free_ind[i] = static_cast<size_t>(d(re2)) % i;
    }
}

static void bench_fastpath_gauss(long iterations, uintptr_t param)
{
  for (long i = 1; i < iterations; i++) {
    void *p = malloc(sizes[i]);
    if (!p) {
      abort();
    }
    blocks[i] = p;

    size_t to_free = free_ind[i];
    assert(to_free < i);
    if (blocks[to_free] != nullptr) {
        free(blocks[to_free]);
        blocks[to_free] = nullptr;
    }
  }
}

int main(void) {
  report_benchmark("bench_fastpath_gauss", bench_fastpath_gauss, init_sizes, 0);
  return 0;
}
