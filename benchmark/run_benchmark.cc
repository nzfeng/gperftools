// -*- Mode: C; c-basic-offset: 2; indent-tabs-mode: nil -*-
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "run_benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern "C" void xiosim_roi_begin() __attribute__ ((noinline));
extern "C" void xiosim_roi_end() __attribute__ ((noinline));

void xiosim_roi_begin() { __asm__ __volatile__ ("":::"memory"); }
void xiosim_roi_end() { __asm__ __volatile__ ("":::"memory"); }

struct internal_bench {
  bench_body body;
  uintptr_t param;
  init_body init_fn;
};

static void run_body(struct internal_bench *b, long rep, long iterations)
{
  b->body(rep, iterations, b->param);
}

static double measure_once(struct internal_bench *b, long rep, long iterations)
{
  struct timeval tv_before, tv_after;
  int rv;
  double time;

  rv = gettimeofday(&tv_before, NULL);
  if (rv) {
    perror("gettimeofday");
    abort();
  }

  run_body(b, rep, iterations);

  rv = gettimeofday(&tv_after, NULL);
  if (rv) {
    perror("gettimeofday");
    abort();
  }
  tv_after.tv_sec -= tv_before.tv_sec;
  time = tv_after.tv_sec * 1E6 + tv_after.tv_usec;
  time -= tv_before.tv_usec;
  time *= 1000;
  return time;
}

static double run_benchmark(struct internal_bench *b, long rep)
{
  double nsec;
  nsec = measure_once(b, rep, ITERATIONS);
  return nsec / ITERATIONS;
}

void report_benchmark(const char *name, bench_body body, init_body init_fn, uintptr_t param)
{
  struct internal_bench b = {.body = body, .param = param, .init_fn = init_fn};
  for (size_t i = 0; i < REPEATS; i++)
    if(b.init_fn)
      b.init_fn(i);

  for (size_t i = 0; i < REPEATS; i++) {
    if (i == 1)
      xiosim_roi_begin();

    double nsec = run_benchmark(&b, i);
    int slen;
    int padding_size;

    slen = printf("Benchmark: %s", name);
    if (param && name[strlen(name)-1] != ')') {
      slen += printf("(%lld)", (long long)param);
    }
    padding_size = 60 - slen;
    if (padding_size < 1) {
      padding_size = 1;
    }
    printf("%*c%f nsec\n", padding_size, ' ', nsec);
    fflush(stdout);
  }

  xiosim_roi_end();
}
