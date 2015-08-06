#!/bin/bash
set -e

echo "Assembling..."
nasm -O32 -f bin boot.asm -o boot.bin
nasm -O32 -f elf32 kernel.asm -o kernel.o
nasm -O32 -f elf32 idtList.asm -o idtList.o
nasm -O32 -f elf32 cpu.asm -o cpu.o
nasm -O32 -f elf32 flush.asm -o flush.o

echo "OK"
echo "Compiling..."
g++ -m32 -std=c++11 -fpermissive -fno-exceptions -fleading-underscore -fno-rtti -enable-__cxa_atexit  -nostdlib -nodefaultlibs -nostartfiles -lgpp -lgpl -lm -lpc -w -c @files.txt
echo "OK"
echo "linking"
ld -m elf_i386 -T kernel.ld @linker.txt
echo "OK"
echo "Finalizing..."
mv a.out ckernel.bin

cat boot.bin ckernel.bin > OS.bin

echo "OK"
exit 0
