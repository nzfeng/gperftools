#!/bin/bash

UB="tp tp_dep gauss gauss_free prod"
mkdir -p output

rm -rf fdo_build*
mkdir fdo_build
cd fdo_build

../build/do_configure.sh instrument
make -j24

cd ..

# yeah, this is ugly, but seems to be the only way
# to pass the right profiles for libtcmalloc, not just
# the bmk wrapper

for BMK in ${UB}
do
    BUILD_DIR=fdo_build_${BMK}
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    ln -s ../fdo_build/${BMK} ${BMK}
    ./${BMK}

    echo "Compiling opt for ${BMK}..."
    ../build/do_configure.sh optimize
    make -j24

    cp ${BMK} ../output/
    cp ${BMK}_native ../output/

    cd ..
done
