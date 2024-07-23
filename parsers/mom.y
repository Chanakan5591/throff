%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document.h"

void yyerror(const char *s);
int yylex(void);

document_t doc;

%}

%union {
    char *str;
}

%token <str> T_STRING T_IDENTIFIER T_NEWLINE
%token T_TITLE T_AUTHOR

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
    T_TITLE T_STRING T_STRING {
        doc.title_th = strdup($2);
        doc.title_en = strdup($3);
        printf("Thai Title: %s\n", $2);
        printf("English Title: %s\n", $3);
        free($2);
        free($3);
    }

author_statement:
    T_AUTHOR T_STRING T_STRING {
        doc.author_th = strdup($2);
        doc.author_en = strdup($3);
        printf("Thai Author: %s\n", $2);
        printf("English Author: %s\n", $3);
        free($2);
        free($3);
    }

content_statement:
    T_IDENTIFIER {
        if (doc.content == NULL) {
            doc.content = strdup($1);
        } else {
            size_t len = strlen(doc.content) + strlen($1) + 2;
            doc.content = (char*) realloc(doc.content, len);
            if (doc.content == NULL) {
                fprintf(stderr, "Error: Cannot allocate enough memories for this document\n");
                exit(1);
            }
            strcat(doc.content, " ");
            strcat(doc.content, $1);
        }
        free($1);
    }
    | content_statement T_IDENTIFIER {
        size_t len = strlen(doc.content) + strlen($2) + 2;
        doc.content = (char*) realloc(doc.content, len);
        if (doc.content == NULL) {
            fprintf(stderr, "Error: Cannot allocate enough memories for this document\n");
            exit(1);
        }
        strcat(doc.content, " ");
        strcat(doc.content, $2);
        free($2);
    }
    | T_NEWLINE {
        if (doc.content == NULL) {
            doc.content = strdup($1);
        } else {
            size_t len = strlen(doc.content) + 1;
            doc.content = (char*) realloc(doc.content, len + 1);
            if(doc.content == NULL) {
                fprintf(stderr, "Error: Cannot allocate enough memories for this document\n");
                exit(1);
            }
            strcat(doc.content, "\n");
        }
        free($1);
    }
    ;

%%

void init_document() {
    doc.title_th = NULL;
    doc.title_en = NULL;
    doc.author_th = NULL;
    doc.author_en = NULL;
    doc.content = NULL;
}

void free_document() {
    free(doc.title_th);
    free(doc.title_en);
    free(doc.author_th);
    free(doc.author_en);
    free(doc.content);
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}