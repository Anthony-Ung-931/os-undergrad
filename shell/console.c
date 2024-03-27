#include "console.h"

char* const VGA_BUFFER_POS = (char*) 0xb8000;
char const SPACE = (char) 0x20;

int terminal_pos = 0;

char* get_address();
int get_next_line();

void clear_terminal() {
	char* const VGA_BUFFER = VGA_BUFFER_POS;
	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		VGA_BUFFER[i * VGA_BYTES_PER_CHARACTER] = SPACE;
	}
	return;
}

void print_character(char c) {
	char* address = get_address();
	/* Suggested to use a switch statement to find the newline character. */
	switch (c) {
		case '\n': 
			terminal_pos = get_next_line();
			break;
		default: 
			/* Check if a character is printable. */
			if((c > (char)(0x1f)) && (c < (char)(0x7f))) {
				*address = c;
			}	
			terminal_pos++; 
	}
	return;
}

void print_string(char* str) {
	char* current_pos;
	for(int i = 0; str[i] != '\0'; i++) {
		current_pos = get_address();
		*current_pos = str[i];
		terminal_pos++;
	}
	return;
}

void print_line(char* str) {
	print_string(str);
	print_character('\n');
}

/* Gets the address of the character to be written to. */
char* get_address() {
	return (char*) (VGA_BUFFER_POS + terminal_pos * 
				VGA_BYTES_PER_CHARACTER);
}

/* Gets the terminal position for the next line. */
int get_next_line() {
	/* Uses integer division to get the next higher multiple of 80. */
	return (terminal_pos + 80) / 80 * 80;
}
