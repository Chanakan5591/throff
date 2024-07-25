%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document.h"

void yyerror(const char *s);
int yylex(void);

int document_start = 0; // 1 when .START is found

document_t doc;

%}

%union {
    char *str;
}

%token <str> T_STRING T_IDENTIFIER T_NEWLINE T_PAPER_TYPE
%token T_TITLE T_AUTHOR T_AFFLIATIONS T_PAPER

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
    | affliation_statement
    | paper_type_statement
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

affliation_statement:
    T_AFFLIATIONS T_STRING T_STRING {
        doc.affliation_th = strdup($2);
        doc.affliation_en = strdup($3);
        printf("Thai Aff: %s\n", $2);
        printf("English Aff: %s\n", $3);
        free($2);
        free($3);
    }

paper_type_statement:
    T_PAPER T_PAPER_TYPE {
        doc.paper_type = strdup($2);
        printf("This paper is %s\n", $2);
        free($2);
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
    memset(&doc, 0, sizeof(doc)); // initialize everything in document struct
}

void free_document() {
    free(doc.paper_type);
    free(doc.title_th);
    free(doc.title_en);
    free(doc.author_th);
    free(doc.author_en);
    free(doc.content);
    free(doc.affliation_en);
    free(doc.affliation_th);
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}