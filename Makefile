all: qemu_launch

qemu_launch: os.bin
	qemu-system-i386 -drive format=raw,file=$<,index=0,if=floppy
os.bin: boot.bin kernel.bin
	cat $^ > $@
boot.bin: boot.asm
	nasm $< -f bin -o $@
kernel.bin: kernel-entry.o kernel.o
	ld -m elf_i386 -s -o $@ -Ttext 0x1000 $^ --oformat binary
kernel-entry.o: kernel-entry.elf
		nasm $< -f elf -o $@
kernel.o: kernel.c
	gcc -fno-pie -m32 -ffreestanding -c $< -o $@
clean:
	$(RM) *.o *.bin
