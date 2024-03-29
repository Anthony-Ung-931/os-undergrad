
#include "console.h"

void retest();

void main() {
	
	clear_terminal();
	print_string("HELLO");
	print_line("WORLD");
	print_string("TODAY");
	
	retest();

	return;
}

/* 	A call to retest() tests the behavior of clear_terminal() and confirms
 * 		that it does indeed reset the terminal position as intended.
 * */
void retest() {
	clear_terminal();
	print_string("HELLO");
	print_line("WORLD");
	print_string("TODAY");
	return;
}
