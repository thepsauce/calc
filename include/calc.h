#ifndef CALC_H
#define CALC_H

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
			
#define ARRAY_ADD(arr, n, cap, item) ({ \
	if(n == cap) \
	{ \
		cap = (cap + 1) * 2; \
		arr = realloc(arr, sizeof(*arr) * cap); \
	} \
	arr[n++] = item; \
	arr; \
})

#define ARRLEN(arr) (sizeof(arr)/sizeof(*(arr)))

enum {
	TOK_NULL,
	TOK_NUMBER,
	TOK_OPERATOR,
	TOK_NAME,
};
typedef struct token {
	int id;
	union {
		double d;
		int opr;
		struct {
			char *name;
			int nName;
		};
	};
} tok_t;

typedef struct variable {
	char *name;
	tok_t *tokens;
	int nTokens;
	double lastValue;
} var_t;

int calc_init(void);
int calc_run(void);

#endif // CALC_H
