#include "calc.h"
#include <math.h>

int calc_init(void)
{
	initscr();
	nocbreak();
	return OK;
}

static void parseline(char *buf);

enum {
	PRECEDENCE_NONE,
	PRECEDENCE_SEP,
	PRECEDENCE_PLUS,
	PRECEDENCE_MUL,
	PRECEDENCE_POW,
	PRECEDENCE_MAX,
};
struct {
	int opr;
	int precedence;
} oprPrecedence[] = {
	{ ',', PRECEDENCE_NONE },
	{ ';', PRECEDENCE_NONE },
	{ '+', PRECEDENCE_PLUS },
	{ '-', PRECEDENCE_PLUS },
	{ '*', PRECEDENCE_MUL },
	{ '/', PRECEDENCE_MUL },
	{ '^', PRECEDENCE_POW },
};
int precedenceof(int opr)
{
	for(int i = 0; i < ARRLEN(oprPrecedence); i++)
		if(oprPrecedence[i].opr == opr)
			return oprPrecedence[i].precedence;
	return PRECEDENCE_MAX;
}
static int compute(tok_t *toks, int nToks, int precedence, double *result);

int calc_run(void)
{
	char *buf;
	int capBuf;

	buf = malloc(capBuf = 512);
	while(1)
	{
		int c;
		int nBuf = 0;

		do
		{
			c = getch();
			if(c == 'c' - ('a' - 1))
				goto done;
			ARRAY_ADD(buf, nBuf, capBuf, c);
		} while(c != '\n');
		parseline(buf);
	}
done:
	free(buf);
	endwin();
	return OK;
}

static void parseline(char *buf)
{
	tok_t *toks, *curTok;
	int nToks, capToks;
	tok_t tok;
	bool push = 0;
	bool dot = 0;
	int n;
	double val;
	double e;
	
	capToks = 12;
	toks = malloc(sizeof(*toks) * capToks);
	nToks = 0;

	tok.id = TOK_NULL;
	while(1)
	{
		switch(tok.id)
		{
		case TOK_NULL:
			switch(*buf)
			{
			case '\n':
				goto done;
			case ' ': case '\t': case '\f': case '\v': case '\r':
				break;
			case '0' ... '9':
				tok.id = TOK_NUMBER;
				tok.d = *buf - '0';
				break;
			case 'A' ... 'Z':
			case 'a' ... 'z':
			case '$':
				tok.id = TOK_NAME;
				tok.name = buf;
				tok.nName = 1;
				break;
			default:
				tok.id = TOK_OPERATOR;
				tok.opr = *buf;
				push = 1;
				buf++;
			}
			break;
		case TOK_NAME:
			switch(*buf)
			{
			case 'A' ... 'Z':
			case 'a' ... 'z':
			case '0' ... '9':
			case '_':
				tok.nName++;
				break;
			default:
				push = 1;
			}
			break;
		case TOK_NUMBER:
			switch(*buf)
			{
			case '0' ... '9':
				if(dot)
				{
					e *= 1e-1;
					tok.d += (*buf - '0') * e;
				}
				else
				{
					tok.d *= 10.0;
					tok.d += *buf - '0';
				}
				break;
			case '.':
				e = 1.0;
				dot = 1;
				break;
			default:
				push = 1;
				dot = 0;
			}
		}
		if(push)
		{
			ARRAY_ADD(toks, nToks, capToks, tok);
			tok.id = TOK_NULL;
			push = 0;
		}
		else
			buf++;
	}
done:
	while((n = compute(toks, nToks, PRECEDENCE_SEP, &val)) > 0)
	{
		toks += nToks - n;
		nToks = n;
		printw("%lf, ", val);
	}
	if(n < 0)
		printw("syntax error\n");
	else
		printw("%lf\n", val);
	free(toks);
}

int compute(tok_t *toks, int nToks, int precedence, double *result)
{
	double val = 0.0, otherVal;
	int n;
	var_t *var;

	for(; nToks; nToks--, toks++)
	{
		switch(toks->id)
		{
		case TOK_NUMBER:
			val = toks->d;
			break;
		case TOK_OPERATOR:
			if(precedenceof(toks->opr) <= precedence)
				goto done;
			switch(toks->opr)
			{
			case '%': val *= 1e-2; break;
			case '!': val = tgamma(val + 1.0); break;
			default:
				if((n = compute(toks + 1, nToks - 1, precedence + 1, &otherVal)) < 0)
					return -1;
				switch(toks->opr)
				{
				case '+': val += otherVal; break;
				case '-': val -= otherVal; break;
				case '*': val *= otherVal; break;
				case '/': val /= otherVal; break;
				case '^': val = pow(val, otherVal); break;
				}
				toks += nToks - n;
				nToks = n;
				if(!nToks)
					goto done;
			}
			break;
		}
	}
done:
	*result = val;
	return nToks;
}
