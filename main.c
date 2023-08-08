#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { NUM, STOP, IS_EQ, IS_MORE_EQ, IS_LESS_EQ, IS_NOT_EQ, SHL, SHR };
int token = 0;
int token_val = 0;
char *src = NULL;
char *buffer_ptr = NULL;
void next() {
	if(!(*src) || *src == -1) {
		token = STOP;
		return;
	}
	
	while(*src == ' ' || *src == '\t' || *src == '\n') {
		src++;
	}

	const int new_token[] = { IS_EQ, IS_MORE_EQ, IS_LESS_EQ, IS_NOT_EQ, SHL, SHR };
	const char *two_char_opers[] = {
		"=><!<>",
		"====<>"
	};
	
	for(int i = 0; two_char_opers[0][i] != 0; i++) {
		if(*src == two_char_opers[0][i] && src[1] == two_char_opers[1][i]) {
			token = new_token[i];
			src += 2;
			return;
		}
	}

	// for one char opers
	token = *(src++);
	if(token >= '0' && token <= '9') {
		token_val = token - '0';
		token = NUM;
		while(*src >= '0' && *src <= '9') {
			token_val = (token_val * 10) + (*src - '0');
			src++;
		}

		return;
	}
}

int expect(char x) {
	if(token != x) {
		fprintf(stderr, "expected %c:%d, but got %c:%d instead\n", x, x, token, token);
		exit(1);
	}

	next();
}

int expr();
int value() {
	int value = 0;
	if(token == STOP || *src == 0 || *src == -1) {
		fprintf(stderr, "can't find a number to get value of\n");
		exit(1);
	}
	
	if(token == '(') {
		expect('(');
		value = expr();
		expect(')');
	} else {
		value = token_val;
		expect(NUM);
	}

	return value;
}

int expr_tail(int left_val) {
	switch(token) {
		case '+': expect('+'); return expr_tail(left_val + value());
		case '-': expect('-'); return expr_tail(left_val - value());
		case '*': expect('*'); return expr_tail(left_val * value());
		case '/': 
			expect('/'); 
			int right_val = value();
			if(right_val == 0) {
				fprintf(stderr, "can't divide %d by 0\n", left_val);
				exit(1);
			}
			
			return expr_tail(left_val / right_val);
			break;
		
		case '%': expect('%'); return expr_tail(left_val % value());
		case '<': expect('<'); return expr_tail(left_val < value());
		case '>': expect('>'); return expr_tail(left_val > value());
		case '&': expect('&'); return expr_tail(left_val & value());
		case '|': expect('|'); return expr_tail(left_val | value());
		case ':': expect(':'); return expr_tail(left_val ^ value());
		case IS_EQ: expect(IS_EQ); return expr_tail(left_val == value());
		case IS_MORE_EQ: expect(IS_MORE_EQ); return expr_tail(left_val >= value());
		case IS_LESS_EQ: expect(IS_LESS_EQ); return expr_tail(left_val <= value());
		case IS_NOT_EQ: expect(IS_NOT_EQ); return expr_tail(left_val != value());
		case SHL: expect(SHL); return expr_tail(left_val << value());
		case SHR: expect(SHR); return expr_tail(left_val >> value());
	}

	return left_val;
}

int is_oper(char x) {
	const char *opers = "+-*/%";
	for(int i = 0; opers[i] != 0; i++) {
		if(opers[i] == x) {
			return 1;
		}
	}
	
	return 0;
}

int expr() {
	int left_val = value();
	return expr_tail(left_val);
}

#define BUFFER_MAX 128
int main() {
	char buffer[BUFFER_MAX] = { 0 };
	buffer_ptr = &buffer[0];
	size_t buffer_size = BUFFER_MAX;
	for(;;) {
		printf("> ");
		getline(&buffer_ptr, &buffer_size, stdin);
		src = buffer_ptr;
		next();
		while(token != STOP && *src && *src != -1) {
			printf("%d\n", expr());
		}

		memset(buffer, 0, sizeof(char) * BUFFER_MAX);
	}
	
	return 0;
}
