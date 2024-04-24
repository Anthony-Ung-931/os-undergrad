C_FILES=./kernel.c ./shell/console.c
O_FILES = ${C_FILES:.c=.o}

all: qemu_launch

qemu_launch: os.bin
	qemu-system-i386 -drive format=raw,file=$<,index=0,if=floppy

# Concatenate boot.bin and kernel.bin into os.bin
# Bootloader is 1st 512 bytes
# 	Check: 511th byte - Magic bytes "0x55AA"
os.bin: boot.bin kernel.bin
	cat $^ > $@

# assemble boot.asm and turn it into boot.bin
boot.bin: boot.asm
	nasm $< -f bin -o $@

# elf_i386 - Emulate on an i386 machine
# -s - Strip symbol table
kernel.bin: kernel-entry.o ${O_FILES}
	ld -m elf_i386 -s -o $@ -Ttext 0x1000 $^ --oformat binary

# elf - Assemble in an ELF format and name it kernel-entry.o
kernel-entry.o: kernel-entry.elf
	nasm $< -f elf -o $@

# Creates a Kernel object file
# gcc - Compiler
# -fno-pie - Don't treat this as ... executable
#  -m32 - Use 32-bit architecture
#  -ffreestanding - Supposed to be freestanding code.
#  	Don't include standard libraries.
#  -c - Compile but don't link
#  $< Output compiled file as kernel.o
${O_FILES}: kernel.c
	gcc -Iinclude -fno-pie -m32 -ffreestanding -c ${@:.o=.c} -o $@

clean:
	$(RM) *.o *.bin
	find . -name \*.o | xargs --no-run-if-empty rm
