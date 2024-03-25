
/* Kernel terminal is 80x25. */
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

void main() {
	
	/* 60 times a second, we pull the information from memory
	 * 	and paint it on the screen. */
	char* const VGA_BUFFER = (char*) 0xb8000;
	char* str = "Hello World";
	
	/* Basically how you print a string. */
	for (int i=0; str[i] != '\0'; i++) {
		VGA_BUFFER[i*2] = str[i];
	}

	return;
}

