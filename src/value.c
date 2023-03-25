#include "calc.h"

struct gaplist {
	void *d;
	U32 n, cap;
	U32 *gaps;
	U32 nGaps, capGaps;
};

static struct {
	U64 *d;
	U32 n, cap;
	U32 *gaps;
	U32 nGaps, capGaps;
} IntValues;

static struct {
	FLOAT *d;
	U32 n, cap;
	U32 *gaps;
	U32 nGaps, capGaps;
} Values;

static struct {
	COMPLEX *d;
	U32 n, cap;
	U32 *gaps;
	U32 nGaps, capGaps;
} ComplexValues;

static void * const Lists[] = {
	NULL, &IntValues, &Values, &ComplexValues,
};

static const U32 SZ_Lists[] = {
	0, sizeof(U64), sizeof(FLOAT), sizeof(COMPLEX),
};

static void __attribute__((destructor))
uninit(void)
{
	free(IntValues.d);
	free(Values.d);
	free(ComplexValues.d);
	free(IntValues.gaps);
	free(Values.gaps);
	free(ComplexValues.gaps);
}

U32
allocvalue(const VALUE *value)
{
	U32 t;
	struct gaplist *list;
	U32 insPos;

	t = value->type;
	list = Lists[t];
	if(list->nGaps)
	{
		insPos = list->gaps[--list->nGaps];
	}
	else
	{
		insPos = list->n++;
		if(list->n > list->cap)
		{
			list->cap = (list->cap + 1) * 2;
			list->d = realloc(list->d, SZ_Lists[t] * list->cap);
		}
	}
	memcpy(list->d + SZ_Lists[t] * insPos, (void*) &value->i, SZ_Lists[t]);
	return t | (insPos << VTYPESH);
}

void
freevalue(U32 v)
{
	U32 i = 0;
	struct gaplist *list;

	list = Lists[v & VTYPE];
	if(list->nGaps + 1 > list->capGaps)
	{
		list->capGaps = (list->capGaps + 1) * 2;
		list->gaps = realloc(list->gaps, SZ_Lists[i] * list->capGaps);
	}
	list->gaps[list->nGaps++] = v >> VTYPESH;
}

void
vset(U32 v, VALUE *value)
{
	U32 t = 0;
	struct gaplist *list;
	void *dest;

	t = v & VTYPE;
	list = Lists[t];
	dest = list->d + SZ_Lists[t] * (v >> VTYPESH);
	memcpy(dest, &value->i, SZ_Lists[t]);
}

void
vget(U32 v, VALUE *value)
{
	U32 t = 0;
	struct gaplist *list;

	t = v & VTYPE;
	list = Lists[t];
	value->type = t;
	memcpy(&value->i, list->d + SZ_Lists[t] * (v >> VTYPESH), SZ_Lists[t]);
}

