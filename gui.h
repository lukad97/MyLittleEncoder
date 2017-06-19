#ifndef _GUI_H
#define _GUI_H

#include "list.h"
#include "keys.h"
#include <curses.h>

#define A_ATTR (A_ATTRIBUTES ^ A_COLOR)
#define KEY_ESC 0x1b

#define MAX_STR_LEN 256

typedef void (*Func)();

typedef struct Menu {
    char *name;
    Func func;
    char *description;
} Menu;


void finish();
void start_menu(Menu *mp, char *title);
void do_menu(Menu *mp);

void title_message(char*);
void status_message(char*);
void error_message(char*, int);
void remove_status_message();
void remove_error_message();

void clear_body();

bool key_pressed();
int get_key();
int wait_for_key();

WINDOW *winputbox(WINDOW*, int, int);
int weditstr(WINDOW*, char*, int);
void repaint_edit_box(WINDOW*, int, char*);
int get_input(char*[], char*[], int*);

#define mvwinputbox(w,y,x,l,c) (wmove(w,y,x)==ERR?w:winputbox(w,l,c))
#define mvweditstr(w,y,x,s,f) (wmove(w,y,x)==ERR?ERR:weditstr(w,s,f))


Key* select_key(List*);

#endif // _GUI_H
