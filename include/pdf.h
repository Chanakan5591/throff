#pragma once

#ifndef PDF_H
#define PDF_H

#include <hpdf.h>

typedef struct {
    HPDF_Font main_font;
    HPDF_PageSizes page_size;
    int main_font_size;
    int text_leading;
} document_config_t;

void initialize_pdf_document();
void set_main_font_and_size(char* truetype_font_path, int font_size);
void finalize_pdf(const char* filename);
HPDF_Page new_page();
void initialize_first_page();

#endif