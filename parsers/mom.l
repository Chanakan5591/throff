%{
#include "mom.tab.h"	
%}

%%
\.[A-Z][A-z0-9_]*	{ 
	fprintf(stderr, "Lex: T_MACRO recognized: %s\n", yytext);
	yylval.str = strdup(yytext);
	return T_MACRO;
}

\"[^\"]*\"			{
	fprintf(stderr, "Lex: T_STRING recognized: %s\n", yytext);
	yylval.str = strdup(yytext);
	return T_STRING;
}

[ \t\n]+			{ /* Ignore Whitespaces */ }
.					{ /* Ignore Unknown Characters */ }
%%

int yywrap() {
	return 1;
}

