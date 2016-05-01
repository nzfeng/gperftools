#!/bin/bash
./configure --prefix=/home/skanev/gperftools/install \
            --disable-shared \
            CC=/group/vlsiarch/skanev/toolchain/bin/gcc \
            CXX=/group/vlsiarch/skanev/toolchain/bin/g++ \
            CFLAGS="-I/home/skanev/libunwind/install/include -g" \
            CXXFLAGS="-I/home/skanev/libunwind/install/include -g -std=c++1y" \
            CPPFLAGS=-I/home/skanev/libunwind/install/include \
            LDFLAGS=-L/home/skanev/libunwind/install/lib
