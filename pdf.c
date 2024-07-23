#include <stdio.h>
#include "pdf.h"
#include "document.h"

HPDF_Doc pdf;
document_config_t doc_config;

int page_width;
int page_height;

void handle_error(HPDF_STATUS status, const char *message)
{
    if (status != HPDF_OK)
    {
        fprintf(stderr, "Error: %s\n", message);
        exit(1);
    }
}

void initialize_pdf_document()
{
    pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        fprintf(stderr, "Error: Cannot create PDF object\n");
        exit(1);
    }

    HPDF_UseUTFEncodings(pdf);
    HPDF_SetCurrentEncoder(pdf, "UTF-8");
}

void set_main_font_and_size(char *truetype_font_path, int font_size)
{
    const char *font_name = HPDF_LoadTTFontFromFile(pdf, truetype_font_path, HPDF_TRUE);
    doc_config.main_font = HPDF_GetFont(pdf, font_name, "UTF-8");
    if (!doc_config.main_font)
    {
        fprintf(stderr, "Error: Cannot load font\n");
        exit(1);
    }
    doc_config.main_font_size = font_size;
}

void finalize_pdf(const char *filename)
{
    HPDF_SaveToFile(pdf, filename);
    HPDF_Free(pdf);
}

HPDF_Page new_page()
{
    HPDF_Page page = HPDF_AddPage(pdf);

    // print main font and main font size
    printf("Main font: %s\n", HPDF_Font_GetFontName(doc_config.main_font));
    printf("Main font size: %d\n", doc_config.main_font_size);

    handle_error(HPDF_Page_SetFontAndSize(page, doc_config.main_font, doc_config.main_font_size), "Cannot set font and size");
    handle_error(HPDF_Page_SetSize(page, doc_config.page_size, HPDF_PAGE_PORTRAIT), "Cannot set page size");
    handle_error(HPDF_Page_SetTextLeading(page, doc_config.text_leading), "Cannot set text leading");

    page_width = HPDF_Page_GetWidth(page);
    page_height = HPDF_Page_GetHeight(page);

    return page;
}

void initialize_first_page()
{
    HPDF_Page page = new_page();

    HPDF_Page_BeginText(page);

    // Set page dimensions
    page_width = HPDF_Page_GetWidth(page);
    page_height = HPDF_Page_GetHeight(page);

    // Thai title size
    HPDF_Page_SetFontAndSize(page, doc_config.main_font, 20);
    float th_title_width = HPDF_Page_TextWidth(page, doc.title_th);

    float th_title_x = (page_width - th_title_width) / 2;
    HPDF_Page_MoveTextPos(page, th_title_x, page_height - 50);
    HPDF_Page_ShowText(page, doc.title_th);

    // Move to second line
    HPDF_Page_MoveTextPos(page, -th_title_x, -doc_config.main_font_size - 10);

    // English title size
    HPDF_Page_SetFontAndSize(page, doc_config.main_font, 18);
    float en_title_width = HPDF_Page_TextWidth(page, doc.title_en);

    float en_title_x = (page_width - en_title_width) / 2;
    HPDF_Page_MoveTextPos(page, en_title_x, 0);
    HPDF_Page_ShowText(page, doc.title_en);

    HPDF_Page_EndText(page);
}