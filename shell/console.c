#include "console.h"

/* The structure for each character: */
struct character {
	char value;
	char style;
};

/* The magic address for the VGA Buffer */
struct character* const VGA_BUFFER_POS = (struct character*) 0xb8000;
char const SPACE = (char) 0x20;

/* The console owns a global variable for its terminal position.
 * I will make the caller of get_address() responsible for incrementing
 * 	this global variable. */
static int terminal_pos = 0;

/* Globals for the background color */
static VGA_Color terminal_font_color = LIGHT_GRAY; 
static VGA_Color terminal_background_color = BLACK; 


/* Helper function declarations */
struct character* get_address();
int get_next_line();

/* Places a space character in all the addresses that correspond to 
 * 	text that would be painted on the screen. */
void clear_terminal() {
	terminal_pos = 0;
	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		print_character_with_color(SPACE, BLACK, LIGHT_GRAY);
	}
	terminal_pos = 0;
	return;
}


void print_character(char c) {
	print_character_with_color
		(c, terminal_background_color, terminal_font_color);
}

/* Puts a single character on the screen. */
void print_character_with_color
	(char c, VGA_Color bg_color, VGA_Color font_color) 
{
	struct character* address = get_address();

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
				address->value = c;
				address->style = (bg_color << 4) | font_color;
			}
			else {
				/* Nonprintable characters get a space
				 * 	printed in their place. */
				address->value = SPACE;
			}
			terminal_pos++;
			break;
	}
	return;
}

/* Puts a character on the screen. */
void print_string(char* str) {
	print_string_with_color
		(str, terminal_background_color, terminal_font_color);
}


void print_string_with_color(
	char* str, VGA_Color bg_color, VGA_Color font_color) 
{
	/* print_string does not need to do its own address calculations.
	 * print_character does that. */
	for(int i = 0; str[i] != '\0'; i++) {
		print_character_with_color(str[i], bg_color, font_color);
	}
	return;
}

/* Prints the given string to the screen and then sets the cursor to the next
 * 	line. */
void print_line(char* str) {
	print_line_with_color
		(str, terminal_background_color, terminal_font_color);
}


void print_line_with_color
	(char* str, VGA_Color bg_color, VGA_Color font_color) 
{
	print_string_with_color(str, bg_color, font_color);
	print_character_with_color('\n', bg_color, font_color);
}

/* Gets the address of the character to be written to. 
 * In the old days, programmers used Macros to write small functions. 
 * 	This could produce specific buggy behaviours. 
 * Now, modern compilers automatically do inline substitutions of small
 * 	functions so code can be more readable for the user without any
 * 	performance loss. */
struct character* get_address() {
	/* If the terminal position is out of bounds, I clear the terminal
	 * 	which also resets the terminal position to 0. */
	if((terminal_pos < 0) || 
		terminal_pos > ((VGA_WIDTH * VGA_HEIGHT) - 1)) {
		clear_terminal();
	}
	return (struct character*) (VGA_BUFFER_POS + terminal_pos);
}

/* Gets the next multiple of 80 from the current terminal position. */
int get_next_line() {
	/* Uses integer division to get the next higher multiple of 80. */
	return (terminal_pos + VGA_WIDTH) / VGA_WIDTH * VGA_WIDTH;
}

void set_terminal_font_color(VGA_Color col) {
	terminal_font_color = col;
}

void set_terminal_background_color(VGA_Color col) {
	terminal_background_color = col;
}
