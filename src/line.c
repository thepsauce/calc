#include "calc.h"

char *Line;
U32 N_Line, Cap_Line;

void readline(void)
{ 
	int c;
	U32 i = 0;
	while(isspace(c = getchar()));
	do
	{
		if(i == Cap_Line)
		{
			Cap_Line = (Cap_Line + 1) * 2;
			Line = realloc(Line, sizeof(*Line) * Cap_Line);
		}
		Line[i++] = c;
	} while((c = getchar()) != '\n');
	if(i == Cap_Line)
	{
		Cap_Line = (Cap_Line + 1) * 2;
		Line = realloc(Line, sizeof(*Line) * Cap_Line);
	}
	Line[i] = 0;
	N_Line = i;
}

void setline(const char *str)
{
	free(Line);
	Line = strdup(str);
	N_Line = Cap_Line = strlen(str);
}
