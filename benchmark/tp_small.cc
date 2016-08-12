#include <cstdlib>
#include <cstdint>

#include "run_benchmark.h"
#include "num_iterations.h"

// Only use five different size classes to represent the best case for the size
// class cache.
static void bench_fastpath_throughput(long rep,
                                      long iterations,
                                      uintptr_t param)
{
  size_t sz = 8;
  for (; iterations>0; iterations--) {
    void *p = malloc(sz);
    if (!p) {
      abort();
    }
    free(p);
    // this makes next iteration use different free list. So
    // subsequent iterations may actually overlap in time.
    sz = (sz & 127) + 32;
  }
}

int main(void) {
  report_benchmark("bench_fastpath_throughput", bench_fastpath_throughput, nullptr, 0);
  return 0;
}
