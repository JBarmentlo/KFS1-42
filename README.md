# KFS1-42

# Setup

## KVM

[source website](https://ubuntu.com/blog/kvm-hyphervisor)

```bash
sudo apt -y install bridge-utils cpu-checker libvirt-clients libvirt-daemon qemu qemu-kvm xorriso mtools
sudo kvm-ok

# Should output
# INFO: /dev/kvm exists
# KVM acceleration can be used
```

You will have to enable virtualisation in your bios if `kvm-ok` says nope.

# KFS

[Source](https://wiki.osdev.org/Bare_Bones)

## 1. Making a cross compiler

[source website](https://wiki.osdev.org/GCC_Cross-Compiler)

### Env

```bash
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export INSTALL_NUM_CORES=8
```

### Dependencies

#### APT

```bash
sudo apt install bison flex libgmp3-dev libmpc-dev libmpfr-dev  texinfo
```

#### Download & Extract GCC and binutils

```bash
mkdir CrossCompiler
cd    CrossCompiler

wget https://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.gz
tar -xf binutils-2.34.tar.gz
rm      binutils-2.34.tar.gz

wget https://ftp.gnu.org/gnu/gcc/gcc-9.4.0/gcc-9.4.0.tar.gz
tar -xf gcc-9.4.0.tar.gz
rm      gcc-9.4.0.tar.gz

wget https://ftp.gnu.org/gnu/gdb/gdb-9.2.tar.gz
tar -xf gdb-9.2.tar.gz
rm gdb-9.2.tar.gz
```

#### Build

##### Binutils

```bash
cd CrossCompiler
 
mkdir build-binutils
cd build-binutils

../binutils-2.34/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make         -j $INSTALL_NUM_CORES
make install -j $INSTALL_NUM_CORES
```

##### GDB

Maybe gdb useless ?

```bash
cd CrossCompiler
cd build-binutils

../gdb.9.2/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
make all-gdb     -j $INSTALL_NUM_CORES
make install-gdb -j $INSTALL_NUM_CORES
```

##### GCC

The $PREFIX/bin dir **MUST** be in the PATH. We did that above.

```bash
cd CrossCompiler
 
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-9.4.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc               -j $INSTALL_NUM_CORES
make all-target-libgcc     -j $INSTALL_NUM_CORES
make install-gcc           -j $INSTALL_NUM_CORES
make install-target-libgcc -j $INSTALL_NUM_CORES
```

Test

```bash
❯ $HOME/opt/cross/bin/$TARGET-gcc --version
------------------------------------------------------------------------------
i686-elf-gcc (GCC) 9.4.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
````

Add to `PATH`

```bash
export PATH="$HOME/opt/cross/bin:$PATH"

❯ which i686-elf-gcc
/home/joep/opt/cross/bin/i686-elf-gcc
```