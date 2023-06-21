# Kernel

We may need to swith to UEFI compatibility: [tutorial](https://wiki.osdev.org/User:Xenos/UEFI_Bare_Bones)

## Build commands

```bash
i686-elf-as boot.s -o boot.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs
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
i686-elf-as boot.s -o boot.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs
```

```bash
mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
```

## RUUUUUUN

```bash
qemu-system-i386 -cdrom myos.iso

# OR

qemu-system-i386 -kernel myos.bin
```