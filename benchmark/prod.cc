#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <random>
#include <thread>
#include <vector>

#include <pthread.h>

#include "run_benchmark.h"

std::vector<void*> allocated(ITERATIONS, nullptr);
std::vector<size_t> free_ind(ITERATIONS);

static void set_affinity(int core_id) {
  pthread_t thread = pthread_self();

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  int res = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (res != 0) {
    fprintf(stderr, "%s\n", strerror(res));
    abort();
  }
}

void init() {
  std::random_device rd;
  std::default_random_engine re(rd());
  for (size_t i = 0; i < ITERATIONS; i++) {
    free_ind[i] = i;
  }
  std::shuffle(std::begin(free_ind), std::end(free_ind), re);
}

static void consumer() {
  set_affinity(1);
  size_t freed = 0;
  size_t i = 0;
  while (freed < ITERATIONS) {
    void*& curr = allocated[free_ind[i]];
    if (curr) {
        freed++;
        free(curr);
        curr = nullptr;
    }
    i = (i + 1) % ITERATIONS;
  }
}

static void producer(long iterations, uintptr_t param) {
  size_t sz = 32;
  set_affinity(0);
  std::thread t(consumer);
  for (size_t i = 0; i < ITERATIONS; i++) {
    void *p = malloc(sz);
    if (!p) {
      abort();
    }
    allocated[i] = p;
    // this makes next iteration use different free list. So
    // subsequent iterations may actually overlap in time.
    sz = (sz & 511) + 16;
  }

  t.join();
}

int main(void) {
  report_benchmark("bench_producer", producer, init, 0);
  return 0;
}
