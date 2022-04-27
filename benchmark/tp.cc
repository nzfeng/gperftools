#include <cstdint>
#include <cstdlib>

#include <pthread.h>
#include <unistd.h>

// run_benchmark.h *has* to come before num_iterations.h
#include "run_benchmark.h"

#include "num_iterations.h"

static void bench_fastpath_throughput(long rep, long iterations,
                                      uintptr_t param) {
  size_t sz = 32;
  for (; iterations > 0; iterations--) {
    void *p = malloc(sz);
    if (!p) {
      abort();
    }
    free(p);
    // this makes next iteration use different free list. So
    // subsequent iterations may actually overlap in time.
    sz = (sz & 511) + 16;
  }
}

struct my_struct {
  long rep;
  long iterations;
  uintptr_t param;
};

static void *per_thread_throughput(void *vargp) {

  struct my_struct *ms = (struct my_struct *)vargp;
  long iterations = ms->iterations;

  // Don't leak the memory
  free(ms);

  size_t sz = 32;
  for (; iterations > 0; iterations--) {
    void *p = malloc(sz);
    if (!p) {
      abort();
    }
    free(p);
    // this makes next iteration use different free list. So
    // subsequent iterations may actually overlap in time.
    sz = (sz & 511) + 16;
  }
}

static void multithreaded_bench_fastpath_throughput(long rep, long iterations,
                                                    uintptr_t param) {

  my_struct ms;
  ms.rep = rep;
  ms.iterations = iterations;
  ms.param = param;
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, per_thread_throughput, (void *)&ms);
  pthread_join(thread_id, NULL);
}

int main(void) {
  // report_benchmark("bench_fastpath_throughput", bench_fastpath_throughput,
  //                  nullptr, 0);
  report_benchmark("multithreaded_bench_fastpath_throughput",
                   multithreaded_bench_fastpath_throughput, nullptr, 0);
  return 0;
}
