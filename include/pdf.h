#pragma once

#ifndef PDF_H
#define PDF_H

#include <hpdf.h>

typedef struct {
    HPDF_Font main_font;
    HPDF_Font main_font_bold;
    HPDF_Font main_font_italics;
    HPDF_Font main_font_bold_italics;
    HPDF_PageSizes page_size;
    int main_font_size;
    int text_leading;
} document_config_t;

void initialize_pdf_document();
void set_main_font_and_size(char* truetype_font_path, int font_size);
void set_main_bold_font();
void set_main_italics_font();
void set_main_bold_italics_font();
void finalize_pdf(const char* filename);
void set_global_page_size(char* page_size_text);
HPDF_Page new_page();
void initialize_first_page();
void initialize_pdf_lib();

#endif