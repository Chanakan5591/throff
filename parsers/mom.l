%{
#include "mom.tab.h"	
%}


%%
\"[^\"]*\" {
	fprintf(stderr, "Lex: T_STRING recognized: %s\n", yytext);
	yylval.str = strdup(yytext);
	return T_STRING;
}

.TI {
	fprintf(stderr, "Lex: T_TITLE recognized: %s\n", yytext);
	return T_TITLE;
}

.TITLE {
	fprintf(stderr, "Lex: T_TITLE recognized: %s\n", yytext);
	return T_TITLE;
}

.AU {
	fprintf(stderr, "Lex: T_AUTHOR recognized: %s\n", yytext);
	return T_AUTHOR;
}

.AUTHOR {
	fprintf(stderr, "Lex: T_AUTHOR recognized: %s\n", yytext);
	return T_AUTHOR;
}

[A-Za-z0-9]+ {
	yylval.str = strdup(yytext);
	return T_IDENTIFIER;
}

[\u0E00-\u0E7F]+ {
	yylval.str = strdup(yytext);
	return T_IDENTIFIER;
}

[ \t]+ { /* Ignore Whitespaces */ }
\n { return T_NEWLINE; }
. { /* Ignore Unknown Characters */ }
%%

int yywrap() {
	return 1;
}

