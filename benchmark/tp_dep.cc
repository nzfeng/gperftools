#include <cstdlib>
#include <cstdint>

#include "run_benchmark.h"

static void bench_fastpath_dependent(long rep,
                                     long iterations,
                                     uintptr_t param)
{
  size_t sz = 32;
  for (; iterations>0; iterations--) {
    void *p = malloc(sz);
    if (!p) {
      abort();
    }
    free(p);
    // this makes next iteration depend on current iteration. But this
    // iteration's free may still overlap with next iteration's malloc
    sz = ((sz | reinterpret_cast<size_t>(p)) & 511) + 16;
  }
}

int main(void) {
  report_benchmark("bench_fastpath_dependent", bench_fastpath_dependent, nullptr, 0);
  return 0;
}
