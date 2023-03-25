#include "calc.h"

void calc_sqrt(const VALUE *v, VALUE *dest)
{
	FLOAT f;

	switch(v->type)
	{
	case VTYPE_INT:
		f = v->i;
		break;
	case VTYPE_FLOAT:
		f = v->f;
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = csqrtl(v->c);
		return;
	}
	if(f < 0.0)
	{
		dest->type = VTYPE_COMPLEX;
		dest->c = CMPLXL(0.0, sqrtl(-f));
	}
	else
	{
		dest->type = VTYPE_FLOAT;
		dest->f = sqrtl(f);
	}
}

void calc_sin(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = sinl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = sinl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = csinl(v->c);
		break;
	}
}

void calc_cos(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = cosl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = cosl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = ccosl(v->c);
		break;
	}
}

void calc_tan(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = tanl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = tanl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = ctanl(v->c);
		break;
	}
}

void calc_asin(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = asinl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = asinl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = casinl(v->c);
		break;
	}
}

void calc_acos(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = acosl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = cacosl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = cacosl(v->c);
		break;
	}
}

void calc_atan(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = atanl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = atanl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = catanl(v->c);
		break;
	}
}

void calc_sinh(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = sinhl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = sinhl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = csinhl(v->c);
		break;
	}
}

void calc_cosh(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = coshl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = coshl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = ccoshl(v->c);
		break;
	}
}

void calc_tanh(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = tanhl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = tanhl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = ctanhl(v->c);
		break;
	}
}

void calc_asinh(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = asinhl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = asinhl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = casinhl(v->c);
		break;
	}
}


void calc_acosh(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = acoshl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = acoshl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = cacoshl(v->c);
		break;
	}
}

void calc_atanh(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = atanhl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = atanhl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = catanhl(v->c);
		break;
	}
}

void calc_exp(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = expl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = expl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = cexpl(v->c);
		break;
	}
}

void calc_ln(const VALUE *v, VALUE *dest)
{
	switch(v->type)
	{
	case VTYPE_INT:
		dest->type = VTYPE_FLOAT;
		dest->f = logl(v->i);
		break;
	case VTYPE_FLOAT:
		dest->type = VTYPE_FLOAT;
		dest->f = logl(v->f);
		break;
	case VTYPE_COMPLEX:
		dest->type = VTYPE_COMPLEX;
		dest->c = clogl(v->c);
		break;
	}
}

void calc_int(const VALUE *v, VALUE *dest)
{
	dest->type = VTYPE_INT;
	switch(v->type)
	{
	case VTYPE_INT:
		dest->i = v->i;
		break;
	case VTYPE_FLOAT:
	case VTYPE_COMPLEX:
		dest->i = v->f;
		break;
	}
}

static void __attribute__((constructor))
init(void)
{
	const struct {
		char name[NAME_MAX];
		FLOAT f;
	} builtin_vars[] = {
		{ "pi", M_PI },
		{ "e", M_E },
		{ "inf", 1.0 / 0.0 },
		{ "nan", 0.0 / 0.0 },
		// + ans
	};
	const struct {
		char name[NAME_MAX];
		U32 nParams;
		void *callback;
	} builtin_funcs[] = {
		{ "sqrt", 1, calc_sqrt },
		{ "sin", 1, calc_sin },
		{ "cos", 1, calc_cos },
		{ "tan", 1, calc_tan },
		{ "asin", 1, calc_asin },
		{ "acos", 1, calc_acos },
		{ "atan", 1, calc_atan },
		{ "sinhl", 1, calc_sinh },
		{ "coshl", 1, calc_cosh },
		{ "tanhl", 1, calc_tanh },
		{ "exp", 1, calc_exp },
		{ "ln", 1, calc_ln },
		{ "int", 1, calc_int },
	};
	N_Variables = Cap_Variables = ARRLEN(builtin_vars) + 1;
	Variables = malloc(sizeof(*Variables) * Cap_Variables);
	for(U32 i = 0; i < ARRLEN(builtin_vars); i++)
	{
		GROUP *group;

		group = malloc(sizeof(U32) + sizeof(FLOAT));
		group->type = GFLOAT;
		group->f = builtin_vars[i].f;
		strcpy(Variables[i].name, builtin_vars[i].name);
		Variables[i].group = group;
	}
	strcpy(Variables[ARRLEN(builtin_vars)].name, "ans");
	Variables[ARRLEN(builtin_vars)].group = malloc(sizeof(U32));
	*(U32*) Variables[ARRLEN(builtin_vars)].group = GLASTREG;

	N_Functions = Cap_Functions = ARRLEN(builtin_funcs);
	Functions = malloc(sizeof(*Functions) * Cap_Functions);
	for(U32 i = 0; i < ARRLEN(builtin_funcs); i++)
	{
		strcpy(Functions[i].name, builtin_funcs[i].name);
		Functions[i].group = NULL;
		Functions[i].nParams = builtin_funcs[i].nParams;
		Functions[i].callback = builtin_funcs[i].callback;
	}
}

static void __attribute__((destructor))
uninit(void)
{
	for(U32 i = 0; i < N_Functions; i++)
		free(Functions[i].group);
	free(Functions);
	for(U32 i = 0; i < N_Variables; i++)
		free(Variables[i].group);
	free(Variables);
}
