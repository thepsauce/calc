#include "calc.h"

void *Groups;
static U32 SZ_Groups, SZ_Cap_Groups;
static U32 I_Token;

// returns offset
static U32 
addgroup(U32 type, void *value, U32 szValue)
{
	U32 oldSz;
	U32 newSz;
	GROUP *group;

	newSz = SZ_Groups + sizeof(U32) + szValue;
	if(newSz > SZ_Cap_Groups)
	{
		SZ_Cap_Groups = SZ_Cap_Groups * 2 + sizeof(U32) + szValue;
		Groups = realloc(Groups, SZ_Cap_Groups);
	}
	group = Groups + SZ_Groups;
	group->type = type;
	if(value)
		memcpy((void*) group + sizeof(group->type), value, szValue);
	oldSz = SZ_Groups;
	SZ_Groups = newSz;
	return oldSz;	
}

static GROUP *
creategroupfromvalue(VALUE *val)
{
	GROUP *group;

	switch(val->type)
	{
	case VTYPE_INT:
		group = malloc(sizeof(U32) + sizeof(INT));
		group->type = GINT;
		group->i = val->i;
		break;
	case VTYPE_FLOAT:
		group = malloc(sizeof(U32) + sizeof(FLOAT));
		group->type = GFLOAT;
		group->f = val->f;
		break;
	case VTYPE_COMPLEX:
		group = malloc(sizeof(U32) + sizeof(COMPLEX));
		group->type = GCOMPLEX;
		group->c = val->c;
		break;
	}
	return group;
}

static void
parsegroup(U32 precedence)
{
	static const struct {
		U32 type;
		U32 gType;
		U32 prec;
	} precs[] = {
		{ TWORD, GMUL, 2 },
		{ TROPEN, GMUL, 2 },
		{ TPERCENT, GPERCENT, 100 },
		{ TEXCLAM, GFACTORIAL, 100 },

		{ TPLUS, GADD, 1 },
		{ TMINUS, GSUB, 1 },

		{ TMUL, GMUL, 2 },
		{ TDIV, GDIV, 2 },

		{ TPOW, GPOW, 3 },

		{ TLSH, GLSH, 4 },
		{ TRSH, GRSH, 4 },

		{ TGTR, GGTR, 5 },
		{ TGEQ, GGEQ, 5 },
		{ TLSS, GLSS, 5 },
		{ TLEQ, GLEQ, 5 },

		{ TEQU, GEQU, 6 },
		{ TNEQ, GNEQ, 6 },

		{ TAND, GAND, 7 },

		{ TXOR, GXOR, 8 },

		{ TOR, GOR, 9 },

		{ TLGAND, GLGAND, 10 },

		{ TLGOR, GLGOR, 11 },
	};
	U32 iFirst;

	iFirst = SZ_Groups;
again:
	switch(Tokens[I_Token].type)
	{
	case TPLUS:
		I_Token++;
		goto again;
	case TMINUS:
		I_Token++;
		addgroup(GNEG, NULL, 0);
		goto again;
	case TROPEN:
		I_Token++;
		parsegroup(0);
		if(I_Token != N_Tokens && Tokens[I_Token].type == TRCLOSE)
			I_Token++;
		break;
	case TWORD:
	{
		U32 l;
		char *s;

		l = getwordlen(I_Token);
		if(l > NAME_MAX - 1)
		{
			adderror(true, "word is too long (maximum is %u)", NAME_MAX - 1);
			I_Token = N_Tokens;
			return;
		}
		s = Line + Tokens[I_Token++].start;
		if((I_Token != N_Tokens && Tokens[I_Token].type == TROPEN) ||
				getfunction(s, l, UINT_MAX) != UINT_MAX)
		{
			U32 group;

			group = addgroup(GFUNC, NULL, sizeof(U32) + NAME_MAX);
			memcpy(((GROUP*) (Groups + group))->funcName, s, l);
			((GROUP*) (Groups + group))->funcName[l] = 0;
			if(I_Token == N_Tokens)
			{
				adderror(true, "missing function argument for '%.*s'", l, s);
				*((U32*) (Groups + group)) = GNOP;
				SZ_Groups -= NAME_MAX; // remove name of function
				break;
			}
			if(Tokens[I_Token].type == TROPEN)
			{
				U32 a = 0;

				I_Token++;
				if(I_Token != N_Tokens && Tokens[I_Token].type == TRCLOSE)
					I_Token++;
				else while(1)
				{
					if(I_Token == N_Tokens)
						break;
					parsegroup(0);
					a++;
					if(Tokens[I_Token].type != TCOMMA)
						break;
					I_Token++;
				}
				if(I_Token != N_Tokens)
				{
					if(Tokens[I_Token].type == TRCLOSE)
						I_Token++;
					else
						adderror(true, "expecting closing paranthesis");
				}
				((GROUP*) (Groups + group))->nParams = a;
			}
			else
			{
				parsegroup(2);
				((GROUP*) (Groups + group))->nParams = 1;
			}
		}
		else
		{
			U32 group;

			group = addgroup(GVAR, NULL, NAME_MAX);
			memcpy(((GROUP*) (Groups + group))->name, s, l);
			((GROUP*) (Groups + group))->name[l] = 0;
		}
		break;
	}
	case TFLOAT:
		addgroup(GFLOAT, &(FLOAT) { getfloat(I_Token) }, sizeof(FLOAT));
		I_Token++;
		break;
	case TBINARY:
		addgroup(GINT, &(INT) { getbinary(I_Token) }, sizeof(INT));
		I_Token++;
		break;
	case TDECIMAL:
		addgroup(GINT, &(INT) { getdecimal(I_Token) }, sizeof(INT));
		I_Token++;
		break;
	case THEXADECIMAL:
		addgroup(GINT, &(INT) { gethexadecimal(I_Token) }, sizeof(INT));
		I_Token++;
		break;
	case TOCTAL:
		addgroup(GINT, &(INT) { getoctal(I_Token) }, sizeof(INT));
		I_Token++;
		break;
	case TLASTREG:
		I_Token++;
		addgroup(GLASTREG, NULL, 0);
		break;
	case TREG:
		Tokens[I_Token].start++; // ignore the '$'
		addgroup(GREG, &(INT) { getdecimal(I_Token) }, sizeof(INT));
		I_Token++;
		break;
	case TMUL: case TDIV: case TPOW:
		adderror(true, "invalid use of operator");
		I_Token = N_Tokens;
		return;
	}
next:
	if(I_Token == N_Tokens)
		return;
	for(U32 i = 0; i < ARRLEN(precs); i++)
		if(precs[i].type == Tokens[I_Token].type)
		{
			if(precs[i].prec <= precedence)
				return;
			// check if we have an operator without 2nd operand
			if(N_Tokens == I_Token + (i > 1) && precs[i].prec != 100)
			{
				adderror(true, "hanging operator");
				I_Token = N_Tokens;
				return;
			}
			if(SZ_Groups + sizeof(U32) > SZ_Cap_Groups)
			{
				SZ_Cap_Groups = SZ_Cap_Groups * 2 + sizeof(U32);
				Groups = realloc(Groups, sizeof(*Groups) * SZ_Cap_Groups);
			}
			memmove(Groups + iFirst + sizeof(U32), Groups + iFirst,
					sizeof(*Groups) * (SZ_Groups - iFirst));
			*((U32*) (Groups + iFirst)) = precs[i].gType;
			SZ_Groups += sizeof(U32);
			if(i > 1)
				I_Token++;
			if(precs[i].prec != 100)
				parsegroup(precs[i].prec);
			goto next;
		}
}

void parseconstassign(void)
{
	VARIABLE var;
	VALUE val;
	U32 l = 0;
	if(Tokens[0].type != TWORD)
	{
		adderror(true, "expected variable name before ':='");
		return;
	}
	l = getwordlen(0);
	if(l > ARRLEN(Variables->name) - 1)
	{
		adderror(true, "variable name(max %u) is too long", ARRLEN(Variables->name) - 1);
		return;
	}
	I_Token = 2;
	if(Tokens[1].type == TROPEN)
	{
		adderror(true, "can't assign a function with ':=' operator");
		return;
	}
	if(Tokens[1].type != TCAGN)
	{
		adderror(true, "assign operator(':=') must be right behind the variable name");
		return;
	}
	parse(2);
	if(poperrors())
		return;
	memcpy(var.name, Line, l);
	var.name[l] = 0;
	if(!SZ_Groups)
	{
		adderror(true, "need right side of assignment or it's invalid");
		return;
	}
	compute(&(struct context) { .groups = Groups, .iGroup = 0, .nLocals = 0 }, &val);
	if(poperrors())
	{
		adderror(true, "failed to compute right hand side of assignment");
		return;
	}
	var.group = creategroupfromvalue(&val);
	setvariable(var.name, var.group);
}

void parseassign(void)
{
	U32 l = 0;

	if(Tokens[0].type != TWORD)
	{
		adderror(true, "expected variable name before '='");
		return;
	}
	l = getwordlen(0);
	if(l > ARRLEN(Variables->name) - 1)
	{
		adderror(true, "variable name(max %u) is too long", ARRLEN(Variables->name) - 1);
		return;
	}
	I_Token = 2;
	if(Tokens[1].type == TROPEN)
	{
		FUNCTION func;

		memcpy(func.name, Line, l);
		func.name[l] = 0;
		func.nParams = 0;
		while(1)
		{
			switch(Tokens[I_Token].type)
			{
			case TWORD:
				if(func.nParams == ARRLEN(func.params))
				{
					adderror(true, "too many parameters(max is %u)", ARRLEN(func.params));
					return;
				}
				l = getwordlen(I_Token);
				if(l > ARRLEN(func.name) - 1)
				{
					adderror(true, "variable name(max %u) is too long", ARRLEN(func.name) - 1);
					return;
				}
				memcpy(func.params[func.nParams].name, Line + Tokens[I_Token].start, l);
				func.params[func.nParams].name[l] = 0;
				func.nParams++;
				I_Token++;
				break;
			case TCOMMA:
				I_Token++;
				break;
			case TRCLOSE:
				I_Token++;
				goto got_params;
			}
		}
	got_params:
		if(Tokens[I_Token].type != TAGN)
		{
			adderror(true, "expected assign operator =");
			return;
		}
		I_Token++;
		parse(I_Token);
		if(poperrors())
			return;
		if(!SZ_Groups)
		{
			func.group = malloc(sizeof(U32));
			*(U32*) func.group = GNOP;
			adderror(false, "empty function");
		}
		else
		{
			func.group = memcpy(malloc(SZ_Groups), Groups, SZ_Groups);
		}
		setfunction(func.name, func.nParams, func.params, func.group);
	}
	else
	{
		VARIABLE var;

		if(Tokens[1].type != TAGN)
		{
			adderror(true, "assign operator('=') must be right behind the variable name");
			return;
		}
		parse(2);
		memcpy(var.name, Line, l);
		var.name[l] = 0;
		if(!SZ_Groups)
		{
			var.group = malloc(sizeof(U32));
			*(U32*) var.group = GNOP;
		}
		else
		{
			var.group = memcpy(malloc(SZ_Groups), Groups, SZ_Groups);
		}
		setvariable(var.name, var.group);
	}
}

void parse(U32 firstToken)
{
	SZ_Groups = 0;
	I_Token = firstToken;
	parsegroup(0);
}
