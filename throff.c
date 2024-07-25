#include <stdio.h>
#include "document.h"
#include "build/mom.tab.h"
#include "pdf.h"

extern int yyparse(void); // parser from bison generated file
extern FILE *yyin; // input file
extern void init_document(void);
extern void free_document(void);

void print_banner()
{
  printf("Throff v0.0.1 by Chanakan\n\n");
  printf("TTTTTTTTTTTTTTTTTTTTTTThhhhhhh                                                    ffffffffffffffff    ffffffffffffffff\n");
  printf("T:::::::::::::::::::::Th:::::h                                                   f::::::::::::::::f  f::::::::::::::::f \n");
  printf("T:::::::::::::::::::::Th:::::h                                                  f::::::::::::::::::ff::::::::::::::::::f\n");
  printf("T:::::TT:::::::TT:::::Th:::::h                                                  f::::::fffffff:::::ff::::::fffffff:::::f\n");
  printf("TTTTTT  T:::::T  TTTTTT h::::h hhhhh      rrrrr   rrrrrrrrr      ooooooooooo    f:::::f       fffffff:::::f       ffffff\n");
  printf("        T:::::T         h::::hh:::::hhh   r::::rrr:::::::::r   oo:::::::::::oo  f:::::f             f:::::f             \n");
  printf("        T:::::T         h::::::::::::::hh r:::::::::::::::::r o:::::::::::::::of:::::::ffffff      f:::::::ffffff       \n");
  printf("        T:::::T         h:::::::hhh::::::hrr::::::rrrrr::::::ro:::::ooooo:::::of::::::::::::f      f::::::::::::f       \n");
  printf("        T:::::T         h::::::h   h::::::hr:::::r     r:::::ro::::o     o::::of::::::::::::f      f::::::::::::f       \n");
  printf("        T:::::T         h:::::h     h:::::hr:::::r     rrrrrrro::::o     o::::of:::::::ffffff      f:::::::ffffff       \n");
  printf("        T:::::T         h:::::h     h:::::hr:::::r            o::::o     o::::o f:::::f             f:::::f             \n");
  printf("        T:::::T         h:::::h     h:::::hr:::::r            o::::o     o::::o f:::::f             f:::::f             \n");
  printf("      TT:::::::TT       h:::::h     h:::::hr:::::r            o:::::ooooo:::::of:::::::f           f:::::::f            \n");
  printf("      T:::::::::T       h:::::h     h:::::hr:::::r            o:::::::::::::::of:::::::f           f:::::::f            \n");
  printf("      T:::::::::T       h:::::h     h:::::hr:::::r             oo:::::::::::oo f:::::::f           f:::::::f            \n");
  printf("      TTTTTTTTTTT       hhhhhhh     hhhhhhhrrrrrrr               ooooooooooo   fffffffff           fffffffff            \n");
  printf("                                                                                                                        \n");
}

int main(int argc, char** argv)
{
  print_banner();
  init_document();

  if(argc < 2) {
    fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
    return 1;
  }

  yyin = fopen(argv[1], "r");
  if(yyin == NULL) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    return 1;
  }

  int result = yyparse();
  printf("\n");
  printf("Document Author: %s\n", doc.author_th);
  printf("Title: %s\n\n", doc.title_en);
  printf("Content: %s\n\n", doc.content);
  fclose(yyin);
  // finished bison parsing, everything is in doc now, later, maybe utilize bison for setting every configurations required to typeset pdf

  initialize_pdf_lib();
  initialize_pdf_document();
  set_main_font_and_size("fonts/THSarabunNew.ttf", 16);
  set_main_bold_font("fonts/THSarabunNew Bold.ttf");
  set_main_italics_font("fonts/THSarabunNew Italic.ttf");
  set_main_bold_italics_font("fonts/THSarabunNew BoldItalic.ttf");
  set_global_page_size(doc.paper_type);
  initialize_first_page();

  finalize_pdf("output.pdf");

  free_document();
  return result;
}

