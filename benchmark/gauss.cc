#include <cstdlib>
#include <cstdint>
#include <random>
#include <vector>

#include "run_benchmark.h"

size_t sizes[ITERATIONS];

static void init_sizes() {
    std::random_device rd;
    std::default_random_engine re(rd());

    std::vector<double> intervals{16, 64, 256, 512};
    std::vector<double> weights{9, 0, 1};
    std::piecewise_constant_distribution<> dist(intervals.begin(), intervals.end(), weights.begin());

    for (size_t i = 0; i < ITERATIONS; i++) {
        sizes[i] = dist(re);
    }
}

static void bench_fastpath_gauss(long iterations, uintptr_t param)
{
  for (; iterations>0; iterations--) {
    void *p = malloc(sizes[iterations]);
    if (!p) {
      abort();
    }
  }
}

int main(void) {
  report_benchmark("bench_fastpath_gauss", bench_fastpath_gauss, init_sizes, 0);
  return 0;
}
