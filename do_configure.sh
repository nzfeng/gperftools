#!/bin/bash
FDO=${1:-no}

if [ "$FDO" != "no" ]  && [ "$FDO" != "instrument" ] && [ "$FDO" != "optimize" ]
then
    echo "Usage: $0 [no|instrument|optimize]"
    exit 1
fi

./configure --prefix=/home/skanev/gperftools/install \
            --disable-shared \
            --enable-sized-delete \
            --enable-fdo=${FDO} \
            CC=/group/vlsiarch/skanev/toolchain_6_1/bin/gcc \
            CXX=/group/vlsiarch/skanev/toolchain_6_1/bin/g++ \
            CFLAGS="-I/home/skanev/libunwind/install/include -g -O3" \
            CXXFLAGS="-I/home/skanev/libunwind/install/include -g -std=c++1y -O3" \
            CPPFLAGS=-I/home/skanev/libunwind/install/include \
            LDFLAGS="-L/home/skanev/libunwind/install/lib -O3"
