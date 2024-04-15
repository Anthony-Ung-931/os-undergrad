
#include "console.h"

void retest();

void main() {
	
	clear_terminal();
	print_string_with_color("hello", YELLOW, CYAN);
	print_line_with_color("World", MAGENTA, GREEN);
	print_string("Today");
	
	retest();

	return;
}

/* 	A call to retest() tests the behavior of clear_terminal() and confirms
 * 		that it does indeed reset the terminal position as intended.
 * */
void retest() {
	clear_terminal();
	print_string_with_color("hello", YELLOW, CYAN);
	print_line_with_color("World", MAGENTA, GREEN);
	print_string("Today");
	
	return;
}
