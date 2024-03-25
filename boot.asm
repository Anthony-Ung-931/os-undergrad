[bits 16] 	/ 16-bit mode
[org 0x7c00] 	/ Bootloader is put in 0x7c00

KERNEL_ADDRESS equ 0x1000	; Kernel loaded on 2nd page
mov [BOOT_DRIVE], DL		; 
mov bp, 0x9000
mov sp, bp

call kernel_load
mov BX, DISK_SUCCESS_MESSAGE
call print_string
jmp switch_to_32bit
jmp done

%include "gdt.asm"

kernel_load:
	pusha
	mov AH, 0x02		; Read mode
	mov AL, [NUM_SECTORS]	; Loads NUM_SECTORS into AL
	mov BX, KERNEL_ADDRESS	; Loads kernel address into BX
	mov CH, 0x00		; CH - Cylinder
	mov CL, 0x02		; Sector 2
	mov DH, 0x00		; Head
	mov DL, [BOOT_DRIVE]	; (???)
	int 13h			; (???)
	jc disk_read_error	; If there is a flag, jump to disk_read_error
	cmp AL, [NUM_SECTORS]	; Compare number of sectors requested
				; 	to number of sectors read
	jne disk_sector_error
	popa			
	ret
disk_read_error:
	mov BX, DISK_READ_ERROR_MESSAGE
	call print_string
	jmp done			; Keeps jumping to the same address
disk_sector_error:
	mov BX, DISK_SECTOR_ERROR_MESSAGE
	call print_string
	jmp done


print_string:	; Prints characters one by one
	pusha
	mov AH, 0x0e
print_loop:
	mov AL, [BX]
	cmp AL, 0x00
	je done_print
	int 10h
	add BX, 1
	jmp print_loop
done_print:
	popa
	ret

done:
	jmp $


switch_to_32bit:	; Initializes GDT
			; 64 individual bits that need to be set
	cli
	lgdt [gdt]
			; Sets control register
	mov EAX, CR0
	or eax, 0x01	; Turns on bit 1 to mark 32-bit mode.
	mov CR0, EAX
			;Cannot manupulate CR0 directly

	jmp 0x08:begin_32bit	; Far jump

[bits 32] / 32-bit mode
begin_32bit:
	mov AX, 0x10
	mov SS, AX
	mov DS, AX
	mov ES, AX
	mov FS, AX
	mov GS, AX
	mov ebp, 0x90000	; Higher stack
	mov esp, ebp
	jmp KERNEL_ADDRESS	; Jumps to kernel address in kernel-entry.elf

NUM_SECTORS db 0x09
DISK_SUCCESS_MESSAGE db "Read succeeded, continuing", 0
DISK_READ_ERROR_MESSAGE db "Problem with disk read", 0
DISK_SECTOR_ERROR_MESSAGE db "Read and requested sectors differ", 0
BOOT_DRIVE db 0
times 510 - ($-$$) db 0	; Pads unused bytes up to 510th
dw 0xaa55		; Magic bytes

