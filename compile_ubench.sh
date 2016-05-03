#!/bin/bash

UB="tp tp_dep gauss gauss_free prod"
mkdir -p output

# yeah, this is ugly, but seems to be the only way
# to pass the right profiles for libtcmalloc, not just
# the bmk wrapper

for BMK in ${UB}
do
    echo "Compiling for ${BMK}..."
    make distclean
    ./do_configure.sh instrument
    make -j24
    ./${BMK}_native
    make distclean
    ./do_configure.sh optimize
    make -j24
    cp ${BMK} output/
    cp ${BMK}_native output/
done
