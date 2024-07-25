#pragma once

#ifndef DOCUMENT_H
#define DOCUMENT_H

typedef struct {
    char *paper_type;
    char *title_th;
    char *title_en;
    char *author_th;
    char *author_en;
    char *affiliation_th;
    char *affiliation_en;
    char *content;
} document_t;


extern document_t doc;

#endif