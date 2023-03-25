#include "calc.h"

TOKEN *Tokens;
U32 N_Tokens, Cap_Tokens;

INT getdecimal(U32 token)
{
	INT i = 0;
	char *str;
   
	str = Line + Tokens[token].start;
	while(isdigit(*str))
	{
		i *= 10;
		i += *(str++) - '0';
	}
	return i;
}

INT
getbinary(U32 token)
{
	INT i = 0;
	char *str;
   
	str = Line + Tokens[token].start + 2;
	while(*str == '0' || *str == '1')
	{
		i <<= 1;
		i += *(str++) - '0';
		while(isspace(*str))
			str++;
	}
	return i;
}

INT
getoctal(U32 token)
{
	INT i = 0;
	char *str;
   
	str = Line + Tokens[token].start + 1;
	while(*str >= '0' && *str <= '7')
	{
		i <<= 3;
		i += *(str++) - '0';
		while(isspace(*str))
			str++;
	}
	return i;
}

INT
gethexadecimal(U32 token)
{
	INT i = 0;
	char *str;
   
	str = Line + Tokens[token].start + 2;
	while(isxdigit(*str))
	{
		i <<= 4;
		i += isdigit(*str) ? *str - '0' : *str > 'a' ? *str - 'a' + 10 : *str - 'A' + 10;
		str++;
		while(isspace(*str))
			str++;
	}
	return i;
}

FLOAT
getfloat(U32 token)
{
	char *str;
	FLOAT f = 0;
	FLOAT e = 1e-1;

	str = Line + Tokens[token].start;
	while(isdigit(*str))
	{
		f *= 1e1;
		f += *(str++) - '0';
	}
	str++; // skip over .
	while(isdigit(*str))
	{
		f += (*str - '0') * e;
		e *= 1e-1;
		str++;
	}	
	return f;
}

U32
getwordlen(U32 token)
{
	U32 l = 1;
	char *str;

	str = Line + Tokens[token].start;
	while(isalnum(str[l]) || str[l] == '_')
		l++;
	return l;
}

void
tokenize(void)
{
	// THE BEST AND MOST INTELEGENT TOKENIZER YOU'VE EVER SEEN
	static const U32 map[0x100] = {
		[' '] = ESPACE,
		['\t'] = ESPACE,

		['0'] = Z0,
		['1'] = Z1,
		['2' ... '7'] = Z27,
		['8' ... '9'] = Z89,
		['a' ... 'f'] = ZAF,
		['A' ... 'F'] = ZAF,
		['g' ... 'z'] = ZGZ,
		['G' ... 'Z'] = ZGZ,
		['_'] = ZGZ,

		// -Woverride-init
		['b'] = ZB,
		['B'] = ZB,
		['x'] = ZX,
		['X'] = ZX,

		['#'] = TCMD,
		['$'] = TLASTREG,
		[':'] = TCOLON,
		['.'] = TDOT,
		[','] = TCOMMA,
		['('] = TROPEN,
		[')'] = TRCLOSE,
		['['] = TCOPEN,
		[']'] = TCCLOSE,
		['%'] = TPERCENT,
		['!'] = TEXCLAM,
		['+'] = TPLUS,
		['-'] = TMINUS,
		['*'] = TMUL,
		['/'] = TDIV,
		['^'] = TPOW,
		['&'] = TAND,
		['|'] = TOR,
		['<'] = TLSS,
		['>'] = TGTR,
		['='] = TAGN,
	};
	// connections of the tokens
	static const struct {
		U32 tok1, tok2;
		U32 resTok;
	} fusions[] = {
		{ TLASTREG, Z0, TREG }, // $0
		{ TLASTREG, Z1, TREG },
		{ TLASTREG, Z27, TREG },
		{ TLASTREG, Z89, TREG },
		{ TREG, Z0, TREG }, // $00
		{ TREG, Z1, TREG },
		{ TREG, Z27, TREG },
		{ TREG, Z89, TREG },
		// word
		{ ZB, ZB, TWORD },
		{ ZB, ZX, TWORD },
		{ ZB, ZAF, TWORD },
		{ ZB, ZGZ, TWORD },
		{ ZB, Z0, TWORD },
		{ ZB, Z1, TWORD },
		{ ZB, Z27, TWORD },
		{ ZB, Z89, TWORD },

		{ ZX, ZB, TWORD },
		{ ZX, ZX, TWORD },
		{ ZX, ZAF, TWORD },
		{ ZX, ZGZ, TWORD },
		{ ZX, Z0, TWORD },
		{ ZX, Z1, TWORD },
		{ ZX, Z27, TWORD },
		{ ZX, Z89, TWORD },

		{ ZAF, ZB, TWORD },
		{ ZAF, ZX, TWORD },
		{ ZAF, ZAF, TWORD },
		{ ZAF, ZGZ, TWORD },
		{ ZAF, Z0, TWORD },
		{ ZAF, Z1, TWORD },
		{ ZAF, Z27, TWORD },
		{ ZAF, Z89, TWORD },

		{ ZGZ, ZB, TWORD },
		{ ZGZ, ZX, TWORD },
		{ ZGZ, ZAF, TWORD },
		{ ZGZ, ZGZ, TWORD },
		{ ZGZ, Z0, TWORD },
		{ ZGZ, Z1, TWORD },
		{ ZGZ, Z27, TWORD },
		{ ZGZ, Z89, TWORD },

		{ TWORD, ZB, TWORD },
		{ TWORD, ZX, TWORD },
		{ TWORD, ZAF, TWORD },
		{ TWORD, ZGZ, TWORD },

		{ TWORD, Z0, TWORD },
		{ TWORD, Z1, TWORD },
		{ TWORD, Z27, TWORD },
		{ TWORD, Z89, TWORD },
		// numbers
		// float
		{ Z0, TDOT, TFLOAT },
		{ Z1, TDOT, TFLOAT },
		{ Z27, TDOT, TFLOAT },
		{ Z89, TDOT, TFLOAT },
		{ TDECIMAL, TDOT, TFLOAT },
		{ TDOT, Z0, TFLOAT },
		{ TDOT, Z1, TFLOAT },
		{ TDOT, Z27, TFLOAT },
		{ TDOT, Z89, TFLOAT },
		{ TFLOAT, Z0, TFLOAT },
		{ TFLOAT, Z1, TFLOAT },
		{ TFLOAT, Z27, TFLOAT },
		{ TFLOAT, Z89, TFLOAT },
		// binary
		{ Z0, ZB, Z0B }, // 0b 0B
		{ Z0B, Z0, TBINARY },
		{ Z0B, Z1, TBINARY },
		{ TBINARY, Z0, TBINARY },
		{ TBINARY, Z1, TBINARY },
		{ TBINARY, ESPACE, TBINARY },
		// decimal
		{ Z1, Z0, TDECIMAL },
		{ Z1, Z1, TDECIMAL },
		{ Z1, Z27, TDECIMAL },
		{ Z1, Z89, TDECIMAL },
		{ Z27, Z0, TDECIMAL },
		{ Z27, Z1, TDECIMAL },
		{ Z27, Z27, TDECIMAL },
		{ Z27, Z89, TDECIMAL },
		{ Z89, Z0, TDECIMAL },
		{ Z89, Z1, TDECIMAL },
		{ Z89, Z27, TDECIMAL },
		{ Z89, Z89, TDECIMAL },
		{ TDECIMAL, Z0, TDECIMAL },
		{ TDECIMAL, Z1, TDECIMAL },
		{ TDECIMAL, Z27, TDECIMAL },
		{ TDECIMAL, Z89, TDECIMAL },
		// octal
		{ Z0, Z0, TOCTAL },
		{ Z0, Z1, TOCTAL },
		{ Z0, Z27, TOCTAL },
		{ TOCTAL, Z0, TOCTAL },
		{ TOCTAL, Z1, TOCTAL },
		{ TOCTAL, Z27, TOCTAL },
		{ TOCTAL, ESPACE, TOCTAL },
		// hexadecimal
		{ Z0, ZX, Z0X }, // 0x 0X
		{ Z0X, Z0, THEXADECIMAL },
		{ Z0X, Z1, THEXADECIMAL },
		{ Z0X, Z27, THEXADECIMAL },
		{ Z0X, Z89, THEXADECIMAL },
		{ Z0X, ZAF, THEXADECIMAL },
		{ THEXADECIMAL, Z0, THEXADECIMAL },
		{ THEXADECIMAL, Z1, THEXADECIMAL },
		{ THEXADECIMAL, Z27, THEXADECIMAL },
		{ THEXADECIMAL, Z89, THEXADECIMAL },
		{ THEXADECIMAL, ZAF, THEXADECIMAL },
		{ THEXADECIMAL, ESPACE, THEXADECIMAL },
		// special characters
		{ TCOLON, TAGN, TCAGN }, // :=
		{ TAGN, TAGN, TEQU }, // ==
		{ TEXCLAM, TAGN, TNEQ }, // !=
		{ TGTR, TAGN, TGEQ }, // >=
		{ TLSS, TAGN, TLEQ }, // <=
		{ TGTR, TGTR, TRSH }, // >>
		{ TLSS, TLSS, TLSH }, // <<
		{ TAND, TAND, TLGAND }, // &&
		{ TOR, TOR, TLGOR }, // ||
	};
	// transformations
	static const U32 trans[ZMAX] = {
		[Z0] = TDECIMAL,
		[Z1] = TDECIMAL,
		[Z27] = TDECIMAL,
		[Z89] = TDECIMAL,
		[ZX] = TWORD,
		[ZB] = TWORD,
		[Z0B] = TERROR,
		[Z0X] = TERROR,
		[ZAF] = TWORD,
		[ZGZ] = TWORD,
	};
	U32 ti = 0;
	U32 last = 0;

	for(U32 i = 0; i < N_Line; i++)
	{
		U32 m;
		
		m = map[(int) Line[i]];
		//printf("%d\n", cases[c].type);
		for(U32 f = 0; f < ARRLEN(fusions); f++)
			if(last == fusions[f].tok1 &&
				m == fusions[f].tok2)
			{
				if((last = fusions[f].resTok))
					Tokens[ti - 1].type = fusions[f].resTok;
				goto made_fusion;
			}
		if(m > EMAX)
		{
			if(ti == Cap_Tokens)
			{
				Cap_Tokens = (Cap_Tokens + 1) * 2;
				Tokens = realloc(Tokens, sizeof(*Tokens) * Cap_Tokens);
			}
			Tokens[ti++] = (TOKEN) { m, i };
		}
		last = m;
	made_fusion:;
	}
	// transform tokens
	for(U32 i = 0; i < ti; i++)
		if(Tokens[i].type < ZMAX)
		{
			U32 to;

			to = trans[Tokens[i].type];
			if(!to)
				adderror(true, "invalid token");
			Tokens[i].type = to;
		}
	N_Tokens = ti;
}
