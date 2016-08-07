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

std::vector<void*> allocated[REPEATS];
std::vector<size_t> free_ind[REPEATS];

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

void init(long rep) {
  allocated[rep].insert(allocated[rep].begin(), ITERATIONS, nullptr);
  free_ind[rep].reserve(ITERATIONS);

  std::random_device rd;
  std::default_random_engine re(rd());
  for (size_t i = 0; i < ITERATIONS; i++) {
    free_ind[rep][i] = i;
  }
  std::shuffle(std::begin(free_ind[rep]), std::end(free_ind[rep]), re);
}

static void consumer(long rep) {
  set_affinity(1);
  size_t freed = 0;
  size_t i = 0;
  while (freed < ITERATIONS) {
    void*& curr = allocated[rep][free_ind[rep][i]];
    if (curr) {
        freed++;
        free(curr);
        curr = nullptr;
    }
    i = (i + 1) % ITERATIONS;
  }
}

static void producer(long rep, long iterations, uintptr_t param) {
  size_t sz = 32;
  set_affinity(0);
  std::thread t(consumer, rep);
  for (size_t i = 0; i < ITERATIONS; i++) {
    void *p = malloc(sz);
    if (!p) {
      abort();
    }
    allocated[rep][i] = p;
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
