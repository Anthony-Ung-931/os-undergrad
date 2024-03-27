#include "console.h"

/* The magic address for the VGA Buffer */
char* const VGA_BUFFER_POS = (char*) 0xb8000;
char const SPACE = (char) 0x20;

/* The console owns a global variable for its terminal position.
 * I will make the caller of get_address() responsible for incrementing
 * 	this global variable. */
int terminal_pos = 0;

/* Helper function declarations */
char* get_address();
int get_next_line();

/* Places a space character in all the addresses that correspond to 
 * 	text that would be painted on the screen. */
void clear_terminal() {
	char* const VGA_BUFFER = VGA_BUFFER_POS;
	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		VGA_BUFFER[i * VGA_BYTES_PER_CHARACTER] = SPACE;
	}
	return;
}

/* Puts a single character on the screen. */
void print_character(char c) {
	char* address = get_address();

	/* Suggested to use a switch statement to find the newline character
	 * 	so I can extend this switch statement to give special treatment
	 * 	to other characters. */
	switch (c) {
		case '\n': 
			terminal_pos = get_next_line();
			break;
		default: 
			/* Check if a character is printable. 
			 * I avoid use of >= to avoid the compiler checking
			 * 	for two different condition codes. */
			if((c > (char)(0x1f)) && (c < (char)(0x7f))) {
				*address = c;
			}
			else {
				/* Nonprintable characters get a space
				 * 	printed in their place. */
				*address = SPACE;
			}
			terminal_pos++;
			break;
	}
	return;
}

/* Puts a character on the screen. */
void print_string(char* str) {
	char* current_pos;
	for(int i = 0; str[i] != '\0'; i++) {
		current_pos = get_address();
		*current_pos = str[i];
		terminal_pos++;
	}
	return;
}

/* Prints the given string to the screen and then sets the cursor to the next
 * 	line. */
void print_line(char* str) {
	print_string(str);
	print_character('\n');
}

/* Gets the address of the character to be written to. 
 * In the old days, programmers used Macros to write small functions. 
 * 	This could produce specific buggy behaviours. 
 * Now, modern compilers automatically do inline substitutions of small
 * 	functions so code can be more readable for the user without any
 * 	performance loss. */
char* get_address() {
	/* If the terminal position is out of bounds, I clear the terminal
	 * 	and reset the terminal position to 0. */
	if((terminal_pos < 0) || 
		terminal_pos > ((VGA_WIDTH * VGA_HEIGHT) - 1)) {
		clear_terminal();
		terminal_pos = 0;
	}
	return (char*) (VGA_BUFFER_POS + terminal_pos * 
				VGA_BYTES_PER_CHARACTER);
}

/* Gets the next multiple of 80 from the current terminal position. */
int get_next_line() {
	/* Uses integer division to get the next higher multiple of 80. */
	return (terminal_pos + 80) / 80 * 80;
}
