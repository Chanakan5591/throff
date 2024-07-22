%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);

%}

%union {
    char *str;
}

%token <str> T_MACRO
%token <str> T_STRING

%%

document:
    /* empty */
    | document macro
    ;

macro:
    T_MACRO T_STRING
    {
        printf("Parsed macro: %s with argument: %s\n", $1, $2);
        free($1);
        free($2);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}