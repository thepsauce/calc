#include "calc.h"
#include <dlfcn.h>

COMMAND *Commands;
U32 N_Commands, Cap_Commands;

void
quit(void)
{
	exit(0);
}

void
showfunctions(void)
{
	char *filter;

	if(N_Tokens <= 2 || Tokens[2].type != TWORD)
		filter = NULL;
	else
		filter = Line + Tokens[2].start;
	for(U32 i = 0; i < N_Functions; i++)
	{
		if(filter && strcmp(filter, Functions[i].name))
			continue;
		printf("%s(", Functions[i].name);
		if(Functions[i].group)
		{
			if(Functions[i].nParams)
				printf("%s", Functions[i].params[0].name);
			for(U32 p = 1; p < Functions[i].nParams; p++)
				printf(", %s", Functions[i].params[p].name);
			printf(") = ");
			printgroup(&(struct context) { .groups = Functions[i].group, .iGroup = 0, .nLocals = 0 });
		}
		else
		{
			if(Functions[i].nParams)
				printf("x");
			for(U32 c = 1; c < Functions[i].nParams; c++)
				printf(", %c", c + 'x');
			printf(")");
		}
		printf("\n");
		if(filter)
			return;
	}
	if(filter)
		adderror(UINT_MAX, "variable '%s' doesn't exist", filter);
}

void
showvariables(void)
{
	char *filter;

	if(N_Tokens <= 2 || Tokens[2].type != TWORD)
		filter = NULL;
	else
		filter = Line + Tokens[2].start;
	for(U32 i = 0; i < N_Variables; i++)
	{
		if(filter && strcmp(filter, Variables[i].name))
			continue;
		printf("%s = ", Variables[i].name);
		printgroup(&(struct context) { .groups = Variables[i].group, .iGroup = 0, .nLocals = 0 });
		printf("\n");
		if(filter)
			return;
	}
	if(filter)
		adderror(UINT_MAX, "variable '%s' doesn't exist", filter);
}

void
showtokens(void)
{
	static const char *names[0x100] = {
		[TLASTREG] "last",
		[TREG] "nth register",
		[TWORD] = "word",
		[TDECIMAL] = "decimal",
		[THEXADECIMAL] = "hexadecimal",
		[TBINARY] = "binary",
		[TOCTAL] = "octal",
	};
	tokenize();
	for(U32 t = 0; t < N_Tokens; t++)
		printf("%12s(%u): %.*s\n", names[Tokens[t].type] ? : "" ,
				Tokens[t].type, (t + 1 != N_Tokens ? Tokens[t + 1].start : N_Line) - Tokens[t].start,
				Line + Tokens[t].start);
}

void
showgroups(void)
{
	tokenize();
	parse(0);
	if(N_Errors)
		printf("%u errors occured\n", N_Errors);
	printgroup(&(struct context) { .groups = Groups, .iGroup = 0 });
	printf("\n");
}

void
command(void)
{
	U32 i = 1;
	bool possible[N_Commands];
	U32 p;
	int c;
	U32 iName = 0;

	while(isspace(Line[i]))
		i++;
	if(!isalpha(Line[i]))
	{
		adderror(true, "error: need command name after #");
		return;
	}
	N_Line -= i;
	memmove(Line, Line + i, N_Line);
	Line[N_Line] = 0;

	memset(possible, 0xff, sizeof(possible));
next:
	c = Line[iName];
	if(!isalpha(c))
	{
		if(p > 1)
		{
			printf("ambigouos command name '%.*s'\n", iName, Line);
			printf("choose either one of these:\n");
			for(U32 b = 0; b < N_Commands; b++)
				if(possible[b])
					printf("  %s\n", Commands[b].name);
		}
		else
		{
			for(U32 b = 0; b < N_Commands; b++)
				if(possible[b])
				{
					while(isspace(Line[iName]))
						iName++;
					N_Line -= iName;
					memmove(Line, Line + iName, N_Line);
					Line[N_Line] = 0;
					Commands[b].callback();
				}
		}
		return;
	}
	p = 0;
	for(U32 b = 0; b < N_Commands; b++)
	{
		if(possible[b] && Commands[b].name[iName] != c)
			possible[b] = false;
		if(possible[b])
			p++;
	}
	if(!p)
	{
		while(isalpha(Line[iName]))
			iName++;
		printf("command '%.*s' doesn't exist\n", iName, Line);
		return;
	}
	iName++;
	goto next;
}

static void
showtable(void)
{
}

static void
load(void)
{
}

static void
save(void)
{
}

static void
commandload(void)
{
	U32 i = 0;
	char name[256];
	U32 nName = 0;
	char *base;
	char *path;
	char *dot;
	void *handle;
	char *error;
	void (*callback)(void);

	while(isspace(Line[i]))
		i++;
	if(!isalpha(Line[i]))
	{
		printf("need file argument\n");
		return;
	}
	while(!isspace(Line[i]) && Line[i])
		name[nName++] = Line[i++];
	if(nName >= sizeof(name))
	{
		printf("max of filename is %lu\n", sizeof(name) - 1);
		return;
	}
	name[nName] = 0;
	path = realpath(name, NULL);
	if(!path)
	{
		printf("invalid path or file doesn't exists\n");
		return;
	}
	base = basename(path);

	handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
	if(!handle)
	{
		printf("%s\n", dlerror());
		free(path);
		return;
	}
	dlerror();
	dot = strchr(path, '.');
	if(dot)
		dot[0] = 0;
	callback = dlsym(handle, base);
	free(path);
	error = dlerror();
	if(error)
	{
		printf("%s\n", error);
		return;
	}
	(*callback)();
	dlclose(handle);
}

static void
setoutput(void)
{
	U32 l = 0;
	static const struct {
		const char *name;
		U32 id;
	} map[] = {
		{ "hex", OUTPUT_HEX },
		{ "bin", OUTPUT_BIN },
		{ "oct", OUTPUT_OCT },
		{ "dec", OUTPUT_DEC },
	};
	if(!isalpha(Line[0]))
		goto err;
	while(isalpha(Line[l]))
		l++;
	if(l != 3)
		goto err;
	for(U32 m = 0; m < ARRLEN(map); m++)
		if(!memcmp(map[m].name, Line, 3))
		{
			OutputMode = map[m].id;
			return;
		}
err:
	adderror(true, "expected one of the following options:\n\thex\n\tbin\n\toct\n\tdec");
}

static void
showhelp(void)
{
	printf("Calculator unstable version 1.\n");
	printf("The following commands are available:\n");
	for(U32 i = 0; i < N_Commands; i++)
		printf("\t%s\n", Commands[i].name);
	printf("Write any expression: 3 * 3 or 2 + 4\n");
	printf("Or declaration: f(x)=x^2 or a=88\n");
}

static void __attribute__((constructor))
init(void)
{
	static const struct {
		const char *name;
		void (*callback)(void);
	} builtin_commands[] = {
		{ "quit", quit },
		{ "variables", showvariables },
		{ "functions", showfunctions },
		{ "tokenize", showtokens },
		{ "groups", showgroups },
		{ "output", setoutput },
		{ "help", showhelp },
		//{ "table", showtable },
		//{ "load", load },
		//{ "save", save },
		//{ "commandload", commandload },
	};
	N_Commands = Cap_Commands = ARRLEN(builtin_commands);
	Commands = malloc(sizeof(*Commands) * Cap_Commands);
	memcpy(Commands, builtin_commands, sizeof(*Commands) * Cap_Commands);
}

static void __attribute((destructor))
uninit(void)
{
	free(Commands);
}
