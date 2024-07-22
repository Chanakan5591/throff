%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);

// Structure for holding parsed data, representing one pdf file
typedef struct {
    char *title;
    char *author;
    char *content;
} document_t;

document_t doc;

%}

%union {
    char *str;
}

%token <str> T_STRING T_IDENTIFIER
%token T_TITLE T_AUTHOR T_NEWLINE

%%

document:
    statements
    ;

statements:
    | statements statement T_NEWLINE
    ;

statement:
    title_statement
    | author_statement
    | content_statement
    ;

title_statement:
    T_TITLE T_STRING {
        printf("Title: %s\n", $2);
        free($2);
    }

author_statement:
    T_AUTHOR T_STRING {
        printf("Author: %s\n", $2);
        free($2);
    }

content_statement:
    T_IDENTIFIER {
        doc.content = strdup($1);
        free($1);
    }
    | content_statement T_IDENTIFIER {
        size_t len = strlen(doc.content) + strlen($2) + 2;
        doc.content = (char*) realloc(doc.content, len);
        strcat(doc.content, " ");
        strcat(doc.content, $2);
        printf("Full Content: %s\n", doc.content);
        free($2);
    }
    ;

%%

void init_document() {
    doc.title = NULL;
    doc.author = NULL;
    doc.content = NULL;
}

void free_document() {
    free(doc.title);
    free(doc.author);
    free(doc.content);
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}