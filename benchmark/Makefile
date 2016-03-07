MALLOC_LIB=../install/lib/libtcmalloc.a

FRAMEWORK=run_benchmark.c run_benchmark.h
FRAMEWORK_SRC=run_benchmark.c

CFLAGS=-g -O3 -std=c++1y
LDFLAGS=-lunwind -pthread

UBS=malloc_bench tp tp_dep gauss gauss_free
UBS_NATIVE=$(addsuffix _native, ${UBS})
all: ${UBS} ${UBS_NATIVE}

define UBENCH
$(1): $(1).cc ${FRAMEWORK} ${MALLOC_LIB}
	g++ ${CFLAGS} -o $(1) $(1).cc ${FRAMEWORK_SRC} ${MALLOC_LIB} ${LDFLAGS}

$(1)_native: $(1).cc ${FRAMEWORK} ${MALLOC_LIB}
	g++ ${CFLAGS} -DNATIVE_FACTOR=64 -o $(1)_native $(1).cc ${FRAMEWORK_SRC} ${MALLOC_LIB} ${LDFLAGS}
endef

$(foreach ub,${UBS},$(eval $(call UBENCH,${ub})))
