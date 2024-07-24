#pragma once

#ifndef DOCUMENT_H
#define DOCUMENT_H

typedef struct {
    char *title_th;
    char *title_en;
    char *author_th;
    char *author_en;
    char *affliation_th;
    char *affliation_en;
    char *content;
} document_t;


extern document_t doc;

#endif