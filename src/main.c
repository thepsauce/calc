#include "calc.h"

VARIABLE *Variables;
U32 N_Variables, Cap_Variables;

void
setvariable(const char *name, void *group)
{
	if(strlen(name) >= sizeof(Variables->name))
		return;
	for(U32 i = 0; i < N_Variables; i++)
		if(!strcmp(Variables[i].name, name))
		{
			free(Variables[i].group);
			Variables[i].group = group;
			return;
		}
	if(N_Variables + 1 > Cap_Variables)
	{
		Cap_Variables = (Cap_Variables + 1) * 2;
		Variables = realloc(Variables, sizeof(*Variables) * Cap_Variables);
	}
	strcpy(Variables[N_Variables].name, name);
	Variables[N_Variables++].group = group;
}

U32
getvariable(const char *name, U32 nName)
{
	/*U32 l, m, vl = 0, v = ERR;

	for(U32 i = 0; i < N_Variables; i++)
	{
		l = strlen(Variables[i].name);
		m = MIN(l, nName);
		if(l <= nName && l > vl
		&& !memcmp(Variables[i].name, name, m))
		{
			vl = l;
			v = i;
		}
	}*/
	for(U32 i = 0; i < N_Variables; i++)
		if(strlen(Variables[i].name) == nName && !memcmp(Variables[i].name, name, nName))
			return i;
	return UINT32_MAX;
}

FUNCTION *Functions;
U32 N_Functions, Cap_Functions;

void
setfunction(const char *name, U32 nParams, struct param *params, void *group)
{
	if(strlen(name) >= sizeof(Functions->name))
		return;
	for(U32 i = 0; i < N_Functions; i++)
		if(!strcmp(Functions[i].name, name) && Functions[i].nParams == nParams)
		{
			memcpy(Functions[i].params, params, sizeof(*params) * nParams);
			free(Functions[i].group);
			Functions[i].group = group;
			return;
		}
	if(N_Functions + 1 > Cap_Functions)
	{
		Cap_Functions = (Cap_Functions + 1) * 2;
		Functions = realloc(Functions, sizeof(*Functions) * Cap_Functions);
	}
	strcpy(Functions[N_Functions].name, name);
	Functions[N_Functions].nParams = nParams;
	memcpy(Functions[N_Functions].params, params, sizeof(*params) * nParams);
	Functions[N_Functions++].group = group;
}

U32
getfunction(const char *name, U32 nName, U32 nParams)
{
	for(U32 i = 0; i < N_Functions; i++)
		if((nParams == UINT32_MAX || Functions[i].nParams == nParams) && 
			strlen(Functions[i].name) == nName && !memcmp(Functions[i].name, name, nName))
			return i;
	return UINT32_MAX;
}

void
printgroup(struct context *ctx)
{
	GROUP *group;
	
	group = ctx->groups + ctx->iGroup;
	ctx->iGroup += sizeof(U32);
	switch(group->type)
	{
	case GNOP: printf("*NOP"); break;
	case GLASTREG: printf("$"); break;
	case GREG:
		ctx->iGroup += sizeof(group->i);
		printf("$%lu", group->i);
		break;
	case GVAR: 
		ctx->iGroup += sizeof(group->name);
		printf("%s", group->name);
		break;
	case GFUNC:
		ctx->iGroup += sizeof(group->nParams);
		ctx->iGroup += sizeof(group->funcName); 
		printf("%s(", group->funcName);
		if(group->nParams)
			printgroup(ctx); 
		for(U32 i = 1; i < group->nParams; i++) { printf(", "); printgroup(ctx); }
		printf(")");
		break;
	case GINT:
		ctx->iGroup += sizeof(group->i);
		printf("%ld", group->i);
		break;
	case GFLOAT:
		ctx->iGroup += sizeof(group->f);
		printf("%Lf", group->f);
		break;
	case GCOMPLEX:
		ctx->iGroup += sizeof(group->c);
		printf("%Lf + %Lfi", creall(group->c), cimagl(group->c));
		break;
	case GPERCENT: printgroup(ctx); printf("%%"); break;
	case GFACTORIAL: printgroup(ctx); printf("!"); break;
	case GNEG: printf("-"); printgroup(ctx); break;
	case GADD: printf("("); printgroup(ctx); printf(" + "); printgroup(ctx); printf(")"); break;
	case GSUB: printf("("); printgroup(ctx); printf(" - "); printgroup(ctx); printf(")"); break;
	case GMUL: printf("("); printgroup(ctx); printf(" * "); printgroup(ctx); printf(")"); break;
	case GDIV: printf("("); printgroup(ctx); printf(" / "); printgroup(ctx); printf(")"); break;
	case GPOW: printf("("); printgroup(ctx); printf(" ^ "); printgroup(ctx); printf(")"); break;
	case GAND: printf("("); printgroup(ctx); printf(" and "); printgroup(ctx); printf(")"); break;
	case GOR: printf("("); printgroup(ctx); printf(" or "); printgroup(ctx); printf(")"); break;
	case GXOR: printf("("); printgroup(ctx); printf(" xor "); printgroup(ctx); printf(")"); break;
	case GLGAND: printf("("); printgroup(ctx); printf(" && "); printgroup(ctx); printf(")"); break;
	case GLGOR: printf("("); printgroup(ctx); printf(" || "); printgroup(ctx); printf(")"); break;
	case GGTR: printf("("); printgroup(ctx); printf(" > "); printgroup(ctx); printf(")"); break;
	case GGEQ: printf("("); printgroup(ctx); printf(" >= "); printgroup(ctx); printf(")"); break;
	case GLSS: printf("("); printgroup(ctx); printf(" < "); printgroup(ctx); printf(")"); break;
	case GLEQ: printf("("); printgroup(ctx); printf(" <= "); printgroup(ctx); printf(")"); break;
	case GEQU: printf("("); printgroup(ctx); printf(" == "); printgroup(ctx); printf(")"); break;
	case GNEQ: printf("("); printgroup(ctx); printf(" != "); printgroup(ctx); printf(")"); break;
	}
}

U32 Registers[REG_MAX];
U32 NextRegister;
void addgroup(U32 type, void *value, U32 szValue);

ERROR *Errors;
U32 N_Errors, Cap_Errors;
bool Severe;

void adderror(bool severe, const char *fmt, ...)
{
	va_list l;
	char buf[256];

	va_start(l, fmt);
	vsnprintf(buf, sizeof(buf), fmt, l);
	va_end(l);	

	if(N_Errors + 1 > Cap_Errors)
	{
		Cap_Errors = Cap_Errors * 2 + 1;
		Errors = realloc(Errors, sizeof(*Errors) * Cap_Errors);
	}
	Errors[N_Errors++].msg = strdup(buf);
	Severe = severe;
}

bool poperrors(void)
{
	bool s;

	s = Severe;
	if(N_Errors)
	{
		for(U32 i = 0; i < N_Errors; i++)
		{
			printf("error: %s\n", Errors[i].msg);
			free(Errors[i].msg);
		}
		N_Errors = 0;
	}
	Severe = 0;
	return s;
}

OUTPUT_MODE OutputMode = OUTPUT_DEC;

static int 
handleline(void)
{
	VALUE v;
	U32 r;

	tokenize();
	if(poperrors())
		return ERR;
	
	// check for assigning
	for(U32 t = 0; t < N_Tokens; t++)
		if(Tokens[t].type == TAGN)
		{
			parseassign();
			return poperrors() ? ERR : OK;
		}
		else if(Tokens[t].type == TCAGN)
		{
			parseconstassign();
			return poperrors() ? ERR : OK;
		}
	parse(0);
	if(poperrors() || !Groups)
		return ERR;
	compute(&(struct context) { .groups = Groups, .iGroup = 0, .nLocals = 0 }, &v);
	if(poperrors())
		return ERR;
	r = allocvalue(&v);
	printf("$%u\t= ", NextRegister);
	switch(OutputMode)
	{
	case OUTPUT_HEX:
	{
		char buf[2 * sizeof(v.i) + sizeof(v.i)];
		U32 i = ARRLEN(buf) - 1;
		memset(buf, '0', i);
		for(U32 j = 1; j < sizeof(v.i); j++)	
			buf[j * 2 + j - 1] = ' ';
		buf[i] = 0;	
		while(v.i)
		{
			i--;
			if(buf[i] == ' ')
				i--;
			if(v.i & 0xf)
				buf[i] = (v.i & 0xf) < 0xa ? ((v.i & 0xf) + '0') : ((v.i & 0xf) + 'a' - 0xa);
			v.i >>= 4;
		}
		printf("%s", buf);
		break;
	}
	case OUTPUT_BIN:
	{
		char buf[CHAR_BIT * sizeof(v.i) + sizeof(v.i)];
		U32 i = ARRLEN(buf) - 1;
		memset(buf, '0', i);
		for(U32 j = 1; j < sizeof(v.i); j++)	
			buf[j * CHAR_BIT + j - 1] = ' ';
		buf[i] = 0;	
		while(v.i)
		{
			i--;
			if(buf[i] == ' ')
				i--;
			if(v.i & 1)
				buf[i] = '0' + (v.i & 1);
			v.i >>= 1;
		}
		printf("%s", buf);
		break;
	}
	case OUTPUT_OCT:
	{
		char buf[3 * sizeof(v.i) + sizeof(v.i)];
		U32 i = ARRLEN(buf) - 1;
		memset(buf, '0', i);
		for(U32 j = 1; j < sizeof(v.i); j++)	
			buf[j * 3 + j - 1] = ' ';
		buf[i] = 0;	
		while(v.i)
		{
			i--;
			if(buf[i] == ' ')
				i--;
			if(v.i & 0x7)
				buf[i] = '0' + (v.i & 0x7);
			v.i >>= 3;
		}
		printf("%s", buf);
		break;
	}
	case OUTPUT_DEC:
		switch(v.type)
		{
		case VTYPE_INT: printf("%ld", v.i); break;
		case VTYPE_FLOAT: printf("%Lf", v.f); break;
		case VTYPE_COMPLEX: printf("%Lf + %Lfi", creall(v.c), cimagl(v.c)); break;
		}
		break;
	}
	printf("\n");
	Registers[NextRegister++] = r;
	return OK;
}

int
main(int argc, char **argv)
{
	argc--;
	argv++;
	if(argc > 2)
	{
		printf("error: too many arguments (expected 0 or 1 or 2)\n");
		return ERR;
	}
	if(argc && argv[0][0] == '-')
	{
		switch(argv[0][1])
		{
		case 'h': OutputMode = OUTPUT_HEX; break;
		case 'b': OutputMode = OUTPUT_BIN; break;
		case 'o': OutputMode = OUTPUT_OCT; break;
		case 'd': OutputMode = OUTPUT_DEC; break;
		}
		argv++;
		argc--;
	}
	if(argc > 0)
	{
		setline(argv[0]);
		return handleline();
	}

	printf("Calculator unstable version 1.\n");
	printf("Use '#help' to get help\n\n");

	while(1)
	{
		Severe = 0;
		readline();
		// printf("%s\n", Line);
		if(Line[0] == '#')
		{
			command();
			poperrors();
			continue;
		}
		handleline();
	}	
	return OK;
}

static void __attribute__((destructor))
uninit(void)
{
	poperrors();
	free(Errors);
}
