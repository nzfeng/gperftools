#!/bin/bash
FDO=${1:-no}
MAGIC=${2:-no}

USAGE="Usage: $0 [no|instrument|optimize] [no|list_magic]"
if [ "$FDO" != "no" ]  && [ "$FDO" != "instrument" ] && [ "$FDO" != "optimize" ]
then
    echo ${USAGE}
    exit 1
fi

if [ "$MAGIC" != "no" ]  && [ "$MAGIC" != "list_magic" ]
then
    echo ${USAGE}
    exit 1
fi

MAGIC_FLAGS=""
if [ "$MAGIC" == "list_magic" ]
then
    MAGIC_FLAGS="--enable-list-pop-magic"
# --enable-list-push-magic" for now, turns out fdo builds use the bt instruction
fi

../configure --prefix="/home/${USER}/gperftools/install" \
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
