
/* Kernel terminal is 80x25. */
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;
char* const VGA_BUFFER_POS = (char*) 0xb8000;

/*
void clear_terminal();
*/

void main() {
	
	clear_terminal();
	print_string("HELLO");
	print_line("WORLD");
	print_string("TODAY");

	/*	
	clear_terminal();
	*/

	/* 60 times a second, we pull the information from memory
	 * 	and paint it on the screen. */
	/*
	char* const VGA_BUFFER = VGA_BUFFER_POS;
	char* str = "Hello World";
	*/

	/* Basically how you print a string. */
	/*
	for (int i=0; str[i] != '\0'; i++) {
		VGA_BUFFER[i*2] = str[i];
	}
	*/

	return;
}

/*
void clear_terminal() {
	char* const VGA_BUFFER = VGA_BUFFER_POS;
	for(int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
		VGA_BUFFER[i*2] = (char) 0x20;
	}
}
*/
