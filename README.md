gperftools with Mallacc Support
-------------------------------

This is a version of gperftools with Mallacc support, forked from the main
branch at commit [050f2d2](https://github.com/gperftools/gperftools/commit/050f2d).
This README describes the additional steps needed to build this special version
of gperftools; the original README is available at [README_orig.txt](README_orig.txt).

# Installation #

## Building dependencies

Check [INSTALL](INSTALL) -- you will need to download a specific version of
[libunwind](http://download.savannah.gnu.org/releases/libunwind/libunwind-0.99-beta.tar.gz)
and build it like so (assuming at /home/user/libunwind):
```bash
CFLAGS=-D_FORTIFY_SOURCE=1 ./configure --prefix=/home/user/libunwind/install
make
make install
```

You'll also need to install libtool.

## Building the library ##

Run `./autogen.sh` to generate the `configure` script before moving on.

We have a `do_configure.sh` script containing our build options under the build
directory. This script controls where libtcmalloc is installed to, whether any
of the special Mallacc instructions are used (a compile-time decision), and
where the toolchain is located. See the script header for more information on
its arguments.

To build the baseline library, without any Mallacc instructions (which means
you can run this natively):

```bash
cd build
./do_configure.sh
make
make install
```
This will install gperftools to `/home/user/gperftools/install`.

To build the library with all the Mallacc instructions enabled:

```bash
cd build
./do_configure.sh no all_magic
make
make install
```

This will install gperftools to `/home/user/gperftools/install_all_magic`.

Linking a binary with this version of the library means you must run the binary
in simulation, as this binary contains placeholder instructions that the simulator
will replace with the real Mallacc instructions.

## Linker script ##

In order to link the Mallacc-enabled tcmalloc with a binary, you will also need
a custom linker script that ensures that various attributes of the
`google_malloc` section retain intact, because the simulator will look for a
section with a particular name and set of attributes when replacing placeholder
instructions. This linker script is located at [spec_linker.ld](spec_linker.ld).

Copy the linker script to your gperftools installation directory (e.g.
/home/user/gperftools/install/spec_linker.ld). Then, open up the linker
script and replace the path to the linker script in the line starting with
`google_malloc` with the correct path.

Although this is not strictly required for the native build of tcmalloc, we
recommend you do this for the native build as well, because later build scripts
expect this file to be there.

## Building the microbenchmarks ##

Go to the build directory and run the `compile_ubench.sh` script. By default,
the script builds the baseline version without Mallacc instructions. You can
change these settings by editing the compile script directly.  The benchmarks
are placed under `build/`. The microbenchmarks are:

  * tp
  * tp_small
  * tp_dep
  * gauss
  * gauss_free
  * sized_deletes
  * prod
  * antagonist (this only runs under XIOSim).

Microbenchmark source code is under `gperftools/benchmark`.
