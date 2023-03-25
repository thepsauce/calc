#ifndef CALC_H
#define CALC_H

#define _GNU_SOURCE
#include <complex.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRLEN(a) (sizeof(a)/sizeof*(a))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define MAX(a, b) ((a)>(b)?(a):(b))

#define OK 0
#define ERR (-1)

typedef long double FLOAT;
typedef uint8_t U8;
typedef uint32_t U32;
typedef uint64_t U64;
typedef U64 INT;
typedef long double complex COMPLEX;

typedef struct error {
	char *msg;
} ERROR;

extern ERROR *Errors;
extern U32 N_Errors, Cap_Errors;
extern bool Severe;

void adderror(bool severe, const char *fmt, ...);
bool poperrors(void);

typedef enum {
	OUTPUT_HEX,
	OUTPUT_BIN,
	OUTPUT_OCT,
	OUTPUT_DEC,
} OUTPUT_MODE;

extern OUTPUT_MODE OutputMode;

#undef NAME_MAX
#define NAME_MAX 8
#define PARAM_MAX 6
#define REG_MAX 256

#define VTYPE 0xf
#define VTYPESH 4
enum {
	VTYPE_NULL,
	VTYPE_INT,
	VTYPE_FLOAT,
	VTYPE_COMPLEX,
};

typedef struct value {
	U32 type;
	union {
		INT i;
		FLOAT f;
		COMPLEX c;
	};
} VALUE;
U32 allocvalue(const VALUE *value);
void freevalue(U32 v);
void vset(U32 v, VALUE *value);
void vget(U32 v, VALUE *value);

extern U32 Registers[REG_MAX];
extern U32 NextRegister;
extern char *Line;
extern U32 N_Line;

void readline(void);
void trimline(void);
void setline(const char *str);

enum {
	TERROR,
	ESPACE, // ' ' \t
	EMAX,
// Token pieces
	Z0, Z1, Z27, Z89, // 0 1 [2-7] [89] 
	ZB, ZX, // [bB] [xX]
	Z0B, Z0X, // 0[bB] 0[xX]
	ZAF, // [a-fA-F]
	ZGZ, // [g-zG-Z_]
	ZMAX,
// Full tokens (which can also be pieces of larger tokens)
	TCMD, // #
	TREG, // $[0-9]
	TLASTREG, // $
	TWORD, // [a-zA-Z_][a-zA-Z0-9_]*
	TFLOAT, // [0-9]+\.[0-9]*
	TDECIMAL, // [0-9]+
	TBINARY, // 0b[01]+
	TOCTAL, // 0[0-7]+
	THEXADECIMAL, // 0x[a-fA-F0-9]+
	TDOT, TCOMMA, TCOLON, // \. , :
	TROPEN, TRCLOSE, TCOPEN, TCCLOSE, // \( \) \[ \]
	TPERCENT, TEXCLAM, // % !
	TPLUS, TMINUS, // \+ - 
	TMUL, TDIV, // \* /
	TPOW, // ^
	TLSH, TRSH, // << >> lsh rsh
	TGTR, TGEQ, TLSS, TLEQ, // > >= < <=
	TEQU, TNEQ, // == !=
	TAND, // & and
	TOR, // \| or
	TLGAND, // &&
	TLGOR, // \|\|
	TAGN, TCAGN, // = :=
// Branches (NOT USED, generally, all 'word' operators are not implemented)
	// TAND, // and
	// TOR, // or
	TXOR, // xor
	// TLSH, // lsh
	// TRSH, // rsh
	TROL, // rol
	TROR, // ror
};
typedef struct token {
	U32 type;
	U32 start;
} TOKEN;

INT getbinary(U32 token);
INT getdecimal(U32 token);
INT gethexadecimal(U32 token);
INT getoctal(U32 token);
FLOAT getfloat(U32 token);
U32 getwordlen(U32 token);

extern TOKEN *Tokens;
extern U32 N_Tokens;

void tokenize(void);

typedef struct command {
	char *name;
	void (*callback)(void);
} COMMAND;

void command(void);

#define GROUP_TYPE  0x000000ff
#define GROUP_VALUE 0xffffff00
#define GROUP_VALUESH 8
enum {
	GNOP,
	GLASTREG, GREG,
	GFUNC,
	GVAR,
	GINT, GFLOAT, GCOMPLEX,
	GPERCENT, 
	GFACTORIAL,
	GNEG,
	GADD, GSUB, GNOT,
	GMUL, GDIV,
	GPOW,
	GLSH, GRSH,
	GGTR, GGEQ, GLSS, GLEQ,
	GEQU, GNEQ,
	GAND, 
	GXOR,
	GOR,
	GLGAND,
	GLGOR,
};

typedef struct __attribute__((__packed__)) group {
	U32 type;
	union {
		INT i;	
		FLOAT f;
		COMPLEX c;
		char name[NAME_MAX];
		struct {
			U32 nParams;
			char funcName[NAME_MAX];
		};
	};	
} GROUP;

extern void *Groups;

typedef struct variable {
	char name[NAME_MAX];
	void *group;
} VARIABLE;

void setvariable(const char *name, void *group);
U32 getvariable(const char *name, U32 nName);

extern VARIABLE *Variables;
extern U32 N_Variables, Cap_Variables;

typedef struct function {
	char name[NAME_MAX];
	U32 nParams;
	union {
		struct param {
			char name[NAME_MAX];
		} params[PARAM_MAX];
		void *callback;
	};
	void *group;
} FUNCTION;

void setfunction(const char *name, U32 nParams, struct param *params, void *group);
U32 getfunction(const char *name, U32 nName, U32 nParams);

extern FUNCTION *Functions;
extern U32 N_Functions, Cap_Functions;

void parseassign(void);
void parseconstassign(void);
void parse(U32 firstToken);

struct context {
	void *groups;
	U32 iGroup;
	U32 nLocals;
	struct {
		char name[NAME_MAX];
		VALUE val;
	} locals[PARAM_MAX];
};

void printgroup(struct context *ctx);
void compute(struct context *ctx, VALUE *value);
#endif // CALC_H
