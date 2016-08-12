#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <random>
#include <vector>

#include "run_benchmark.h"
#include "num_iterations.h"

size_t sizes[REPEATS][ITERATIONS / BASE_ITERATIONS][BASE_ITERATIONS];
void *blocks[REPEATS][ITERATIONS / BASE_ITERATIONS][BASE_ITERATIONS];
size_t free_ind[REPEATS][ITERATIONS / BASE_ITERATIONS][BASE_ITERATIONS];

static void init_sizes(long rep) {
  std::random_device rd;
  std::default_random_engine re(rd());

  std::vector<double> intervals{ 16, 64, 256, 512 };
  std::vector<double> weights{ 9, 0, 1 };
  std::piecewise_constant_distribution<> dist(intervals.begin(),
                                              intervals.end(), weights.begin());

  for (size_t r = 0; r < ITERATIONS / BASE_ITERATIONS; r++) {
    for (size_t i = 0; i < BASE_ITERATIONS; i++) {
      sizes[rep][r][i] = dist(re);
    }
  }

  std::default_random_engine re2(rd());
  std::uniform_int_distribution<size_t> d(0, BASE_ITERATIONS - 1);

  for (size_t r = 0; r < ITERATIONS / BASE_ITERATIONS; r++) {
    free_ind[rep][r][0] = 0;
    for (size_t i = 1; i < BASE_ITERATIONS; i++) {
      free_ind[rep][r][i] = static_cast<size_t>(d(re2)) % i;
    }
  }
}

static void bench_fastpath_gauss_free(long rep, long iterations,
                                      uintptr_t param) {
  for (size_t r = 0; r < iterations / BASE_ITERATIONS; r++) {
    for (long i = 1; i < BASE_ITERATIONS; i++) {
      void *p = malloc(sizes[rep][r][i]);
      if (!p) {
        abort();
      }
      blocks[rep][r][i] = p;

      size_t to_free = free_ind[rep][r][i];
      assert(to_free < i);
      if (blocks[rep][r][to_free] != nullptr) {
        free(blocks[rep][r][to_free]);
        blocks[rep][r][to_free] = nullptr;
      }
    }
  }
}

int main(void) {
  report_benchmark("bench_fastpath_gauss_free", bench_fastpath_gauss_free,
                   init_sizes, 0);
  return 0;
}
