#include <stdio.h>

#define BUFFER_MAX 128

char *next(char *line) {
	
}

int main() {
	char buffer[BUFFER_MAX] = { 0 };
	char *buffer_ptr = &buffer[0];
	size_t buffer_size = BUFFER_MAX;
	for(;;) {
		printf("> ");
		getline(&buffer_ptr, &buffer_size, stdin);
		printf("  %s", buffer);
		
	}
	
	return 0;
}
