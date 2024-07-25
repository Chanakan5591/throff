#include <stdio.h>
#include "pdf.h"
#include "document.h"

HPDF_Doc pdf;
document_config_t doc_config;

int page_width;
int page_height;
int DEV = 1;

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
        fprintf(stderr, "Error: Cannot load regular font\n");
        exit(1);
    }
    doc_config.main_font_size = font_size;
}

void set_main_bold_font(char *truetype_font_path)
{
    const char *font_name = HPDF_LoadTTFontFromFile(pdf, truetype_font_path, HPDF_TRUE);
    doc_config.main_font_bold = HPDF_GetFont(pdf, font_name, "UTF-8");
    if (!doc_config.main_font_bold)
    {
        fprintf(stderr, "Error: Cannot load bold font\n");
        exit(1);
    }
}

void set_main_italics_font(char *truetype_font_path)
{
    const char *font_name = HPDF_LoadTTFontFromFile(pdf, truetype_font_path, HPDF_TRUE);
    doc_config.main_font_italics = HPDF_GetFont(pdf, font_name, "UTF-8");
    if (!doc_config.main_font_italics)
    {
        fprintf(stderr, "Error: Cannot load italics font\n");
        exit(1);
    }
}

void set_main_bold_italics_font(char *truetype_font_path)
{
    const char *font_name = HPDF_LoadTTFontFromFile(pdf, truetype_font_path, HPDF_TRUE);
    doc_config.main_font_bold_italics = HPDF_GetFont(pdf, font_name, "UTF-8");
    if (!doc_config.main_font_bold_italics)
    {
        fprintf(stderr, "Error: Cannot load bold italics font\n");
        exit(1);
    }
}

void finalize_pdf(const char *filename)
{
    HPDF_SaveToFile(pdf, filename);
    HPDF_Free(pdf);
}

void set_global_page_size(char* page_size_text) {
    doc_config.page_size = NULL; // implement soon
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

void render_text(HPDF_Page page, HPDF_Font font, float font_size, const char* text, float page_width, float offset_x, float offset_y) {
    HPDF_Page_SetFontAndSize(page, font, font_size);
    float text_width = HPDF_Page_TextWidth(page, text);
    float text_x = (page_width - text_width) / 2 + offset_x;
    HPDF_Page_MoveTextPos(page, text_x, offset_y);
    HPDF_Page_ShowText(page, text);
}

void initialize_first_page()
{
    HPDF_Page page = new_page();
    const int th_title_font_size = 20;
    //const int en_title_font_size = 18;
    const int gap_between_titles = 8;

    const float margin = 72.0;

    // Get page size
    float page_width = HPDF_Page_GetWidth(page);
    float page_height = HPDF_Page_GetHeight(page);

    // Set content area (inside the margin)
    float content_left = margin;
    float content_right = page_width - margin;
    float content_top = page_height - margin;
    float content_bottom = margin;

    if(DEV) {
        HPDF_Page_Rectangle(page, content_left, content_bottom, content_right - content_left, content_top - content_bottom);
        HPDF_Page_Stroke(page);
    }

    HPDF_Page_BeginText(page);

    // Set page dimensions
    page_width = HPDF_Page_GetWidth(page);
    page_height = HPDF_Page_GetHeight(page);

    /* START Thai Title*/
    HPDF_Page_SetFontAndSize(page, doc_config.main_font_bold, 20);
    float th_title_width = HPDF_Page_TextWidth(page, doc.title_th);

    float th_title_x = (page_width - th_title_width) / 2;
    HPDF_Page_MoveTextPos(page, th_title_x, content_top - th_title_font_size);
    HPDF_Page_ShowText(page, doc.title_th);
    /* END Thai Title */

    // Move to second line
    HPDF_Page_MoveTextPos(page, -th_title_x, -doc_config.main_font_size - gap_between_titles);

    /* START English Title */
    HPDF_Page_SetFontAndSize(page, doc_config.main_font_bold, 18);
    float en_title_width = HPDF_Page_TextWidth(page, doc.title_en);

    float en_title_x = (page_width - en_title_width) / 2;
    HPDF_Page_MoveTextPos(page, en_title_x, 0);
    HPDF_Page_ShowText(page, doc.title_en);
    /* END English Title */

    // Drop to author line
    HPDF_Page_MoveTextPos(page, -en_title_x, -doc_config.main_font_size - 32);

    /* START Author TH */
    HPDF_Page_SetFontAndSize(page, doc_config.main_font_bold, 16);
    float author_th_width = HPDF_Page_TextWidth(page, doc.author_th);

    float th_author_x = (content_right - author_th_width) - 24;
    HPDF_Page_MoveTextPos(page, th_author_x, 0);
    HPDF_Page_ShowText(page, doc.author_th);
    /* END Author TH */

    // Second line
    HPDF_Page_MoveTextPos(page, -th_author_x, -doc_config.main_font_size - 2);

    /* START Author EN */
    HPDF_Page_SetFontAndSize(page, doc_config.main_font_bold, 16);
    float author_en_width = HPDF_Page_TextWidth(page, doc.author_en);

    float en_author_x = (content_right - author_en_width) - 24;
    HPDF_Page_MoveTextPos(page, en_author_x, 0);
    HPDF_Page_ShowText(page, doc.author_en);
    /* END Author EN */

    // Third line
    HPDF_Page_MoveTextPos(page, -en_author_x, -doc_config.main_font_size - 2);

    /* START Affliations TH */
    HPDF_Page_SetFontAndSize(page, doc_config.main_font, 14);
    float affiliation_th_width = HPDF_Page_TextWidth(page, doc.affliation_th);

    float th_affliation_x = (content_right - affiliation_th_width) - 24;
    HPDF_Page_MoveTextPos(page, th_affliation_x, 0);
    HPDF_Page_ShowText(page, doc.affliation_th);
    /* END Affliations TH */

     // Fourth line
    HPDF_Page_MoveTextPos(page, -th_affliation_x, -doc_config.main_font_size - 2);

    /* START Affliations EN */
    HPDF_Page_SetFontAndSize(page, doc_config.main_font, 14);
    float affiliation_en_width = HPDF_Page_TextWidth(page, doc.affliation_en);

    float en_affliation_x = (content_right - affiliation_en_width) - 24;
    HPDF_Page_MoveTextPos(page, en_affliation_x, 0);
    HPDF_Page_ShowText(page, doc.affliation_en);
    /* END Affliations EN */

    HPDF_Page_MoveTextPos(page, (-en_affliation_x + content_left), -doc_config.main_font_size - 32);
    // start any user content


    HPDF_Page_EndText(page);
}