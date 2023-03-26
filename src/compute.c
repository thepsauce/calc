#include "calc.h"

void iperc(const VALUE *v, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = (FLOAT) v->i * 1e-2;
}

void fperc(const VALUE *v, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v->f * 1e-2;
}

void iiadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	if(v1->i > (UINT64_MAX - v2->i))
		adderror(false, "integer overflow!");
	dest->i = v1->i + v2->i;
}

void fiadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f + v2->i;
}

void ifadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->i + v2->f;
}

void ffadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f + v2->f;
}

void ciadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c + v2->i;
}

void icadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->i + v2->c;
}

void cfadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c + v2->f;
}

void fcadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->f + v2->c;
}

void ccadd(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c + v2->c;
}

void iisub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->i = v1->i - v2->i;
}

void fisub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f - v2->i;
}

void ifsub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->i - v2->f;
}

void ffsub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f - v2->f;
}

void cisub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c - v2->i;
}

void icsub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->i - v2->c;
}

void cfsub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c - v2->f;
}

void fcsub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->f - v2->c;
}

void ccsub(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c - v2->c;
}

void iimul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	U64 x;

	x = v1->i * v2->i;
	if(v1->i && x / v1->i != v2->i)
		adderror(false, "integer overflow (mul) !");
	dest->type = VTYPE_INT;
	dest->i = v1->i * v2->i;
}

void fimul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f * v2->i;
}

void ifmul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->i * v2->f;
}

void ffmul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f * v2->f;
}

void cimul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c * v2->i;
}

void icmul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->i * v2->c;
}

void cfmul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c * v2->f;
}

void fcmul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->f * v2->c;
}

void ccmul(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	// (a + bi) (c + di) = ac - bd + adi + bci
	// dest->c.real = v1->c.real * v2->c.real - v1->c.imag * v2->c.imag;
	// dest->c.imag = v1->c.real * v2->c.imag + v1->c.imag * v2->c.real;
	dest->c = v1->c * v2->c;
}

void iidiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	if(!v2->i)
	{
		adderror(UINT_MAX, "integer division by 0!");
		return;		
	}
	dest->i = v1->i / v2->i;
}

void fidiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f / v2->i;
}

void ifdiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->i / v2->f;
}

void ffdiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = v1->f / v2->f;
}

void cidiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c / v2->i;
}

void icdiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	// a / (c + di) = (ac - adi) / (c * c + d * d)
	// FLOAT d = 1.0 / (v2->c.real * v2->c.real + v2->c.imag * v2->c.imag);
	// dest->c.real = v1->i * v2->c.real * d;
	// dest->c.imag = -v1->c.real * v2->c.imag * d;
	dest->c = v1->i / v2->c;
}

void cfdiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = v1->c / v2->f;
}

void fcdiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	// a / (c + di) = (ac - adi) / (c * c + d * d)
	// FLOAT d = 1.0 / (v2->c.real * v2->c.real + v2->c.imag * v2->c.imag);
	// dest->c.real = v1->f * v2->c.real * d;
	// dest->c.imag = -v1->c.real * v2->c.imag * d;
	dest->c = v1->f / v2->c;
}

void ccdiv(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	// (a + bi) / (c + di) = ((a + bi) (c - di)) / (c * c + d * d)
	// = (ac + bd - adi + bci) / (c * c + d * d)
	// FLOAT d = 1.0 / (v2->c.real * v2->c.real + v2->c.imag * v2->c.imag);
	// dest->c.real = (v1->c.real * v2->c.real + v1->c.imag * v2->c.imag) * d;
	// dest->c.imag = (v1->c.imag * v2->c.real - v1->c.real * v2->c.imag) * d;
	dest->c = v1->c / v2->c;
}

void iipow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	U64 r = 1;
	U64 base, exp;

	base = v1->i;
	exp = v2->i;
	while(1)
	{
		if(exp & 1)
			r *= base;
		exp >>= 1;
		if(!exp)
			break;
		base *= base;
	}
	dest->type = VTYPE_INT;
	dest->i = r;
}

void fipow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = powl(v1->f, v2->i);
}

void ifpow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = powl(v1->i, v2->f);
}

void ffpow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->f = powl(v1->f, v2->f);
}

void cipow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = cpowl(v1->c, v2->i);
}

void icpow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = cpowl(v1->i, v2->c);
}

void cfpow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = cpowl(v1->c, v2->f);
}

void fcpow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = cpowl(v1->f, v2->c);
}

void ccpow(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = cpowl(v1->c, v2->c);
}

void iilsh(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->i = v1->i << v2->i;
}

void iirsh(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->i = v1->i >> v2->i;
}

void iirol(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	INT shift, value;

	dest->type = VTYPE_INT;
	value = v1->i;
	shift = v2->i;
	if(!(shift &= sizeof(INT) * 8 - 1))
		dest->i = value;
	else
		dest->i = (value << shift) | (value >> (sizeof(INT) * 8 - shift));
}

void iiror(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	INT shift, value;

	dest->type = VTYPE_INT;
	value = v1->i;
	shift = v2->i;
	if(!(shift &= sizeof(INT) * 8 - 1))
		dest->i = value;
	else
		dest->i = (value >> shift) | (value << (sizeof(INT) * 8 - shift));
}

void iiand(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->i = v1->i & v2->i;
}

void iior(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->i = v1->i | v2->i;
}

void iixor(const VALUE *v1, const VALUE *v2, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->i = v1->i ^ v2->i;
}

void ineg(const VALUE *v, VALUE *dest)
{
	dest->type = VTYPE_INT;
	dest->c = -v->i;
}

void fneg(const VALUE *v, VALUE *dest)
{
	dest->type = VTYPE_FLOAT;
	dest->c = -v->f;
}

void cneg(const VALUE *v, VALUE *dest)
{
	dest->type = VTYPE_COMPLEX;
	dest->c = -v->c;
}

void
compute(struct context *ctx, VALUE *value)
{
	static const struct combination {
		void *func;
		U32 t1, t2;
	} combinations[] = {
		{ iperc, VTYPE_INT, 0 },
		{ fperc, VTYPE_FLOAT, 0 },
		{ NULL, 0, 0 },
		{ iiadd, VTYPE_INT, VTYPE_INT },
		{ fiadd, VTYPE_FLOAT, VTYPE_INT }, 
		{ ifadd, VTYPE_INT, VTYPE_FLOAT }, 
		{ ffadd, VTYPE_FLOAT, VTYPE_FLOAT }, 
		{ ciadd, VTYPE_COMPLEX, VTYPE_INT }, 
		{ icadd, VTYPE_INT, VTYPE_COMPLEX }, 
		{ cfadd, VTYPE_COMPLEX, VTYPE_FLOAT }, 
		{ fcadd, VTYPE_FLOAT, VTYPE_COMPLEX }, 
		{ ccadd, VTYPE_COMPLEX, VTYPE_COMPLEX },
		{ NULL, 0, 0 },
		{ iisub, VTYPE_INT, VTYPE_INT },
		{ fisub, VTYPE_FLOAT, VTYPE_INT }, 
		{ ifsub, VTYPE_INT, VTYPE_FLOAT }, 
		{ ffsub, VTYPE_FLOAT, VTYPE_FLOAT }, 
		{ cisub, VTYPE_COMPLEX, VTYPE_INT }, 
		{ icsub, VTYPE_INT, VTYPE_COMPLEX }, 
		{ cfsub, VTYPE_COMPLEX, VTYPE_FLOAT }, 
		{ fcsub, VTYPE_FLOAT, VTYPE_COMPLEX }, 
		{ ccsub, VTYPE_COMPLEX, VTYPE_COMPLEX },
		{ NULL, 0, 0 },
		{ iimul, VTYPE_INT, VTYPE_INT },
		{ fimul, VTYPE_FLOAT, VTYPE_INT }, 
		{ ifmul, VTYPE_INT, VTYPE_FLOAT }, 
		{ ffmul, VTYPE_FLOAT, VTYPE_FLOAT }, 
		{ cimul, VTYPE_COMPLEX, VTYPE_INT }, 
		{ icmul, VTYPE_INT, VTYPE_COMPLEX }, 
		{ cfmul, VTYPE_COMPLEX, VTYPE_FLOAT }, 
		{ fcmul, VTYPE_FLOAT, VTYPE_COMPLEX }, 
		{ ccmul, VTYPE_COMPLEX, VTYPE_COMPLEX },
		{ NULL, 0, 0 },
		{ iidiv, VTYPE_INT, VTYPE_INT },
		{ fidiv, VTYPE_FLOAT, VTYPE_INT }, 
		{ ifdiv, VTYPE_INT, VTYPE_FLOAT }, 
		{ ffdiv, VTYPE_FLOAT, VTYPE_FLOAT }, 
		{ cidiv, VTYPE_COMPLEX, VTYPE_INT }, 
		{ icdiv, VTYPE_INT, VTYPE_COMPLEX }, 
		{ cfdiv, VTYPE_COMPLEX, VTYPE_FLOAT }, 
		{ fcdiv, VTYPE_FLOAT, VTYPE_COMPLEX }, 
		{ ccdiv, VTYPE_COMPLEX, VTYPE_COMPLEX },
		{ NULL, 0, 0 },
		{ iipow, VTYPE_INT, VTYPE_INT },
		{ fipow, VTYPE_FLOAT, VTYPE_INT }, 
		{ ifpow, VTYPE_INT, VTYPE_FLOAT }, 
		{ ffpow, VTYPE_FLOAT, VTYPE_FLOAT }, 
		{ cipow, VTYPE_COMPLEX, VTYPE_INT }, 
		{ icpow, VTYPE_INT, VTYPE_COMPLEX }, 
		{ cfpow, VTYPE_COMPLEX, VTYPE_FLOAT }, 
		{ fcpow, VTYPE_FLOAT, VTYPE_COMPLEX }, 
		{ ccpow, VTYPE_COMPLEX, VTYPE_COMPLEX },
		{ NULL, 0, 0 },
		{ iilsh, VTYPE_INT, VTYPE_INT },
		{ NULL, 0, 0 },
		{ iirsh, VTYPE_INT, VTYPE_INT },
		{ NULL, 0, 0 },
		{ iiand, VTYPE_INT, VTYPE_INT },
		{ NULL, 0, 0 },
		{ iior, VTYPE_INT, VTYPE_INT },
		{ NULL, 0, 0 },
		{ iixor, VTYPE_INT, VTYPE_INT },
		{ NULL, 0, 0 },
		{ ineg, VTYPE_INT, 0 },
		{ fneg, VTYPE_FLOAT, 0 },
		{ cneg, VTYPE_COMPLEX, 0 },
		{ NULL, 0, 0 },
	};
	U32 operations[] = {
		[GPERCENT] = 0,
		[GADD] = 3,
		[GSUB] = 13,
		[GMUL] = 23,
		[GDIV] = 33,
		[GPOW] = 43,
		[GLSH] = 53,
		[GRSH] = 55,
		[GAND] = 57,
		[GOR]  = 59,
		[GXOR] = 61,
		[GNEG] = 63,
	};
	GROUP *group;
	
	group = ctx->groups + ctx->iGroup;
	ctx->iGroup += sizeof(U32);
	switch(group->type)
	{
	case GLASTREG: 
	{
		U32 reg;

		if(!NextRegister)
		{
			value->type = VTYPE_FLOAT;
			value->f = 0.0/0.0;
			return;
		}
		reg = Registers[NextRegister - 1];
		vget(reg, value);
		break;
	}
	case GREG: 
	{
		U32 reg;

		ctx->iGroup += sizeof(group->i);
		if(!NextRegister)
		{
			value->type = VTYPE_FLOAT;
			value->f = 0.0/0.0;
			return;
		}
		reg = Registers[group->i];
		vget(reg, value);
		break;
	}
	case GFUNC:
	{
		U32 func;
		U32 l;
		U32 nParams;

		ctx->iGroup += sizeof(group->nParams);
		ctx->iGroup += sizeof(group->funcName);

		nParams = group->nParams;
		l = strlen(group->funcName);
		func = getfunction(group->funcName, l, nParams);
		if(func == UINT_MAX)
		{
			adderror(true, "function '%.*s #%u' doesn't exist", l, group->funcName, nParams); 
		}
		else if(Functions[func].group)
		{
			struct context inCtx;

			inCtx.groups = Functions[func].group;
			inCtx.iGroup = 0;
			inCtx.nLocals = Functions[func].nParams;
			for(U32 p = 0; p < Functions[func].nParams; p++)
			{
				strcpy(inCtx.locals[p].name, Functions[func].params[p].name);
				compute(ctx, &inCtx.locals[p].val);
			}
			compute(&inCtx, value);
		}
		else
		{
			switch(nParams)
			{
			case 0:
				((void (*)(VALUE*)) Functions[func].callback)(value);
				break;
			case 1:
			{
				VALUE v;

				compute(ctx, &v);
				((void (*)(const VALUE*, VALUE*)) Functions[func].callback)(&v, value);
				break;
			}
			case 2:
			{
				VALUE v1, v2;

				compute(ctx, &v1);
				compute(ctx, &v2);
				((void (*)(const VALUE*, const VALUE*, VALUE*)) Functions[func].callback)(&v1, &v2, value);
				break;
			}
			}
		}
		break;
	}
	case GVAR:
	{	
		U32 var;
		U32 l;

		ctx->iGroup += sizeof(group->name);

		l = strlen(group->name);
		for(U32 i = 0; i < ctx->nLocals; i++)
			if(!strncmp(ctx->locals[i].name, group->name, l))
			{
				*value = ctx->locals[i].val;
				return;
			}

		var = getvariable(group->name, l);
		if(var == UINT_MAX)
		{
			if(l == 1 && group->name[0] == 'i')
			{
				value->type = VTYPE_COMPLEX;
				value->c = CMPLX(0, 1);
			}
			else
				adderror(true, "variable %.*s doesn't exist", l, group->name);
		}
		else
			compute(&(struct context) { .groups = Variables[var].group, .iGroup = 0, .nLocals = 0 }, value);
		break;
	}
	case GINT:
		ctx->iGroup += sizeof(group->i);
		value->type = VTYPE_INT;
		value->i = group->i;
		break;
	case GFLOAT:
		ctx->iGroup += sizeof(group->f);
		value->type = VTYPE_FLOAT;
		value->f = group->f;
		break;
	case GCOMPLEX:
		ctx->iGroup += sizeof(group->c);
		value->type = VTYPE_COMPLEX;
		value->c = group->c;
		break;
	default:
	{
		VALUE v1, v2;
		const struct combination *comb;

		if(group->type >= ARRLEN(operations))
		{
			value->type = VTYPE_FLOAT;
			value->f = 0;
			adderror(true, "there is no support for this operator");
			return;
		}

		comb = combinations + operations[group->type];
		compute(ctx, &v1);
		if(comb->t2)
			compute(ctx, &v2);
		else
			v2.type = 0;
		while(comb->func)
		{
			if(comb->t1 == v1.type && comb->t2 == v2.type)
			{
				if(comb->t2)
					((void (*)(const VALUE *v1, const VALUE *v2, VALUE *dest)) comb->func)(&v1, &v2, value);
				else
					((void (*)(const VALUE *v, VALUE *dest)) comb->func)(&v1, value);
				return;
			}
			comb++;
		}
		value->type = VTYPE_FLOAT;
		value->f = 0;
		adderror(true, "invalid types for operation");
	}
	}
}

