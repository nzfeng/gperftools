#!/bin/bash
#
# Configure tcmalloc.
#
# Usage:
#   ./do_configure [fdo=no] [magic=no]
#
#   FDO option:
#      no - don't build for FDO. Default.
#      instrument - add instrumentation to collect profile data
#      optimize - use profiled data to optimize the build.
#
#   Magic option: enable the special mallacc instructions. This will only run
#   in simulation and requires GCC 6.1 or later.
#      no - Build without any magic instructions (this will run natively). Default.
#      list_magic - Insert linked list push/pop/prefetch instructions
#      sample_magic - Remove sampling instructions (assume existence of perf counter).
#      all_magic - Apply all optimizations.
#
# gperftools is installed to /home/user/gperftools.

FDO=${1:-no}
MAGIC=${2:-no}

USAGE="Usage: $0 [no|instrument|optimize] [no|all_magic|list_magic|sample_magic]"
if [ "$FDO" != "no" ]  && [ "$FDO" != "instrument" ] && [ "$FDO" != "optimize" ]
then
    echo ${USAGE}
    exit 1
fi

if [ "$MAGIC" != "no" ]  && [ "$MAGIC" != "list_magic" ] && [ "$MAGIC" != "sample_magic" ] && [ "$MAGIC" != "all_magic" ]
then
    echo ${USAGE}
    exit 1
fi

MAGIC_FLAGS=""
INSTALL_SUFFIX=""
if [ "$MAGIC" == "all_magic" ]
then
    MAGIC_FLAGS="--enable-list-pop-magic --enable-list-push-magic --enable-size-class-magic --enable-sample-magic"
    INSTALL_SUFFIX="_all_magic"
elif [ "$MAGIC" == "list_magic" ]
then
    MAGIC_FLAGS="--enable-list-pop-magic --enable-list-push-magic"
    INSTALL_SUFFIX="_list_magic"
elif [ "$MAGIC" == "sample_magic" ]
then
    MAGIC_FLAGS="--enable-sample-magic"
    INSTALL_SUFFIX="_sample_magic"
fi

../configure --prefix="/home/${USER}/gperftools/install${INSTALL_SUFFIX}" \
             --disable-shared \
             --enable-sized-delete \
             --enable-fdo=${FDO} \
             ${MAGIC_FLAGS} \
             CC=/group/vlsiarch/skanev/toolchain_6_1/bin/gcc \
             CXX=/group/vlsiarch/skanev/toolchain_6_1/bin/g++ \
             CFLAGS="-I/home/skanev/libunwind/install/include -g -O3" \
             CXXFLAGS="-I/home/skanev/libunwind/install/include -g -std=c++1y -O3 -fabi-version=2 -D_GLIBCXX_USE_CXX11_ABI=0" \
             CPPFLAGS=-I/home/skanev/libunwind/install/include \
             LDFLAGS="-L/home/skanev/libunwind/install/lib -O3"
