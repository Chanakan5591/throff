%{
#include "mom.tab.h"	
%}


%%
\"[^\"]*\" {
    // Get the length of the matched text
    int len = strlen(yytext);

    // Check if the first and last characters are quotes
    if (len > 1 && (yytext[0] == '"' || yytext[0] == '\'') && (yytext[len - 1] == '"' || yytext[len - 1] == '\'')) {
        // Allocate space for the new string without quotes
        char *stripped = malloc(len - 1);
        if (stripped == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        // Copy the content without the surrounding quotes
        strncpy(stripped, yytext + 1, len - 2);
        stripped[len - 2] = '\0'; // Null-terminate the string

        // Print the recognized string
        fprintf(stderr, "Lex: T_STRING recognized: %s\n", stripped);

        // Set yylval to the new string without quotes
        yylval.str = stripped;
    } else {
        // Handle the case where quotes are not found (fallback)
        yylval.str = strdup(yytext);
        fprintf(stderr, "Lex: T_STRING recognized (fallback): %s\n", yylval.str);
    }

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

.AFFLIATION {
    fprintf(stderr, "Lex: T_AFFLIATIONS recognized: %s\n", yytext);
    return T_AFFLIATIONS;
}

.AF {
    fprintf(stderr, "Lex: T_AFFLIATIONS recognized: %s\n", yytext);
    return T_AFFLIATIONS;
}

.PAPER {
    fprintf(stderr, "Lex: T_PAPER_TYPE recognized: %s\n", yytext);
    return T_PAPER;
}

[Aa][0-9]+|[Bb][0-9]+|[Ll]etter|[Ll]egal {
    fprintf(stderr, "Lex: T_PAPER_TYPE (values) recognized: %s\n", yytext);
    yylval.str = strdup(yytext);
    return T_PAPER_TYPE;
}

^[^\.\n][^\n]* { // match everything except macros (starting with a dot)
	yylval.str = strdup(yytext);
	return T_IDENTIFIER;
}

[ \t]+ { /* Ignore Whitespaces */ }

\n {
	yylval.str = strdup("\n");
	return T_NEWLINE;
}
. { /* Ignore Unknown Characters */ }
%%

int yywrap() {
	return 1;
}

