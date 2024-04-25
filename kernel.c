
#include "console.h"
#include "stdint.h"
#include "keyboard.h"
#include "portmap.h"

void main() {
	
	clear_terminal();
	
	uint8_t byte;
	while(1) {
		while ( byte = scan()) {
			print_character(charmap[byte]);
		}
	}
	return;
}

