# Kernel

We may need to swith to UEFI compatibility: [tutorial](https://wiki.osdev.org/User:Xenos/UEFI_Bare_Bones)

## Build commands

```bash
i686-elf-as boot.s -o boot.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
```

## Check multiboot header

```bash
if grub-file --is-x86-multiboot myos.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi
```

## Make the .ISO

Dependency to build `.iso`

```bash
sudo apt install xorriso
```

```bash
mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
```
