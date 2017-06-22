#include "gui.h"
#include "list.h"
#include "keys.h"
#include <curses.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/* color_pair indices */
#define TITLECOLOR          1
#define MAINMENUCOLOR       (2 | A_BOLD)
#define MAINMENUREVCOLOR    (3 | A_BOLD | A_REVERSE)
#define SUBMENUCOLOR        (4 | A_BOLD)
#define SUBMENUREVCOLOR     (5 | A_BOLD | A_REVERSE)
#define BODYCOLOR           6
#define BODYREVCOLOR        (7 | A_REVERSE)
#define STATUSCOLOR         (8 | A_BOLD)
#define INPUTBOXCOLOR       9
#define EDITBOXCOLOR        (10 | A_BOLD | A_REVERSE)

/* dimensions */
#define TH 1
#define MH 1
#define SH 2
#define BH (LINES - TH - MH - SH)
#define BW COLS

/* macro */
#define min(a,b) (a) < (b) ? (a) : (b)

/************************* PREDEFINITIONS *************************/
void status_message(char*);
void error_message(char*, int);
void remove_status_message();
void remove_error_message();
int wait_for_key();

/***************************** STATIC *****************************/
static WINDOW *wtitle, *wmain, *wbody, *wstatus;
static bool quit = FALSE;
static bool in_curses = FALSE;
static int key = ERR, ch = ERR;
static int menux, menuy;

/*********************** INTERNAL FUNCTIONS ***********************/
static void init_colors() {
    start_color();

    init_pair(TITLECOLOR       & ~A_ATTR, COLOR_BLACK, COLOR_CYAN);
    init_pair(MAINMENUCOLOR    & ~A_ATTR, COLOR_WHITE, COLOR_CYAN);
    init_pair(MAINMENUREVCOLOR & ~A_ATTR, COLOR_WHITE, COLOR_BLUE); /// COLOR_WHITE, COLOR_BLACK
    init_pair(SUBMENUCOLOR     & ~A_ATTR, COLOR_WHITE, COLOR_CYAN);
    init_pair(SUBMENUREVCOLOR  & ~A_ATTR, COLOR_WHITE, COLOR_BLUE); /// COLOR_WHITE, COLOR_BLACK
    init_pair(BODYCOLOR        & ~A_ATTR, COLOR_WHITE, COLOR_BLUE);
    init_pair(BODYREVCOLOR     & ~A_ATTR, COLOR_WHITE, COLOR_BLACK);
    init_pair(STATUSCOLOR      & ~A_ATTR, COLOR_WHITE, COLOR_CYAN);
    init_pair(INPUTBOXCOLOR    & ~A_ATTR, COLOR_BLACK, COLOR_CYAN);
    init_pair(EDITBOXCOLOR     & ~A_ATTR, COLOR_WHITE, COLOR_BLACK);
}

static void set_color(WINDOW *win, chtype color) {
    chtype attr = color & A_ATTR;
    attr &= ~A_REVERSE; // ignore reverse
    wattrset(win, COLOR_PAIR(color & A_CHARTEXT) | attr);
}

static void color_box(WINDOW *win, chtype color, int has_box) {
    chtype attr = color & A_ATTR;
    set_color(win, color);

    wbkgd(win, COLOR_PAIR(color & A_CHARTEXT) | (attr & ~A_REVERSE));
    werase(win);

    if (has_box && getmaxy(win) > 2)
        box(win, 0, 0);

    touchwin(win);
    wrefresh(win);
}

static char* pad_str_left(char *s, int length) {
    static char buffer[MAX_STR_LEN];
    char format[10];

    sprintf(format, strlen(s) > length ? "%%.%ds" : "%%-%ds", length);
    sprintf(buffer, format, s);

    return buffer;
}

/// assumes enough space
static char* str_add_space_left(char *s) {
    int len = strlen(s);
    int i;

    for (i = len; i; i--)
        s[i] = s[i - 1];
    s[len + 1] = '\0';
    s[0] = ' ';

    return s;
}

static char* convert_key_to_string(Key *key) {
    static char s[MAX_KEY_ROW_LEN];
    sprintf(s, KEY_PRINT_FORMAT, key->type, key->mode, key->key_name, (key->key)[0], (key->key)[1], (key->key)[2]);
    return s;
}

static void remove_line(WINDOW *win, int line) {
    mvwaddstr(win, line, 1, pad_str_left(" ", BW - 2));
    wrefresh(win);
}

static void idle() {
    char buffer[MAX_STR_LEN];
    time_t t;
    struct tm *tp;

    if (time(&t) == -1)
        return;

    tp = localtime(&t);
    sprintf(buffer, "%.2d-%.2d-%.4d  %.2d:%.2d:%.2d", tp->tm_mday, tp->tm_mon + 1,
            tp->tm_year + 1900, tp->tm_hour, tp->tm_min, tp->tm_sec);

    // mvwaddstr(wtitle, 0, BW - strlen(buffer) - 2, buffer);
    mvwaddstr(wtitle, 0, BW - 22, buffer);
    wrefresh(wtitle);
}

static void menu_dimensions(Menu *mp, int *lines, int *columns) {
    int i, len, max = 0;

    for (i = 0; mp->func; i++, mp++)
        if ((len = strlen(mp->name)) > max)
            max = len;

    *lines = i;
    *columns = max + 2;
}

static void set_menu_position(int y, int x) {
    menuy = y;
    menux = x;
}

static void get_menu_position(int *y, int *x) {
    *y = menuy;
    *x = menux;
}

static void repaint_menu(WINDOW *wmenu, Menu *mp) {
    int i;

    for (i = 0; mp->func; i++, mp++)
        mvwaddstr(wmenu, i + 1, 2, mp->name);

    touchwin(wmenu);
    wrefresh(wmenu);
}

static void repaint_main_menu(Menu *mp, int width) {
    int i;

    for (i = 0; mp->func; i++, mp++)
        mvwaddstr(wmain, 0, i * width, str_add_space_left(pad_str_left(mp->name, width - 1)));

    touchwin(wmain);
    wrefresh(wmain);
}

static void repaint_body_keys(ListElement *list_elem, int length, int pad_y, int pad_x) {
    int i;
    ListElement *curr = list_elem;
    char row[MAX_KEY_ROW_LEN];

    sprintf(row, KEY_PRINT_FORMAT, "Type", "Mode", "Key name", "Key 1", "Key 2", "Key 3");
    ///mvwaddstr(wbody, pad_y, pad_x, row);
    error_message(row, 0);

    for (i = 0; curr && i < length; i++, curr = curr->next)
        mvwaddstr(wbody, i + pad_y, pad_x, convert_key_to_string((Key*)curr->info));

    touchwin(wbody);
    wrefresh(wbody);
}

static void repaint_file_explorer(WINDOW *win, char names[][MAX_STR_LEN], int n_rows, int pady, int padx) {
    int i;
    for (i = 0; i < n_rows; i++)
        mvwaddstr(win, i + pady, padx, names[i]);

    touchwin(win);
    wrefresh(win);
}

static int count_dir_entries(DIR *dir, char *curr_path) {
    int cnt = 0;
    struct dirent *entry;
    char tmp_str[MAX_STR_LEN];
    struct stat tmp_stat;

    rewinddir(dir);
    while (entry = readdir(dir)) {
        if (!strcmp(entry->d_name, "."))
            continue;

        strcpy(tmp_str, curr_path);
        strcat(tmp_str, entry->d_name);

        stat(tmp_str, &tmp_stat);
        if (S_ISREG(tmp_stat.st_mode) || S_ISDIR(tmp_stat.st_mode))
            cnt++;
    }
    rewinddir(dir);

    return cnt;
}

static int find_dir_entries(DIR *dir, int start_idx, int max_entries, char *curr_path, char names[][MAX_STR_LEN]) {
    int i;
    struct dirent *entry;
    char tmp_str[MAX_STR_LEN];
    struct stat tmp_stat;

    rewinddir(dir);
    start_idx = (start_idx / max_entries) * max_entries;
    while (start_idx && (entry = readdir(dir))) {
        if (!strcmp(entry->d_name, "."))
            continue;

        strcpy(tmp_str, curr_path);
        strcat(tmp_str, entry->d_name);

        stat(tmp_str, &tmp_stat);
        if (S_ISREG(tmp_stat.st_mode) || S_ISDIR(tmp_stat.st_mode))
            start_idx--;
    }
    for (i = 0; i < max_entries && (entry = readdir(dir)); i++) {
        if (!strcmp(entry->d_name, ".")) {
            i--;
            continue;
        }

        strcpy(tmp_str, curr_path);
        strcat(tmp_str, entry->d_name);

        stat(tmp_str, &tmp_stat);
        if (S_ISREG(tmp_stat.st_mode))
            strcpy(names[i], entry->d_name);
        else if (S_ISDIR(tmp_stat.st_mode)) {
            int offset = 0;
            if (strcmp(entry->d_name, "..")) {
                names[i][0] = '/';
                offset = 1;
            }
            strcpy(names[i] + offset, entry->d_name);
        }
        else
            i--;
    }

    return i;
}

static void main_help() {
#ifdef ALT_X
    status_message("Use arrow keys and Enter to select (Alt-X to quit)");
#else
    status_message("Use arrow keys and Enter to select");
#endif // ALT_X
}

static void main_menu(Menu *mp) {
    int n_items, bar_len;
    int old_selection = -1, curr_selection = 0;
    int c;

    menu_dimensions(mp, &n_items, &bar_len);
    repaint_main_menu(mp, bar_len);

    while (!quit) {
        if (curr_selection != old_selection) {
            if (old_selection != -1) {
                mvwaddstr(wmain, 0, old_selection * bar_len, str_add_space_left(pad_str_left(mp[old_selection].name, bar_len - 1)));
                status_message(mp[curr_selection].description);
            }
            else
                main_help();

            set_color(wmain, MAINMENUREVCOLOR);
            mvwaddstr(wmain, 0, curr_selection * bar_len, str_add_space_left(pad_str_left(mp[curr_selection].name, bar_len - 1)));
            set_color(wmain, MAINMENUCOLOR);

            old_selection = curr_selection;
            wrefresh(wmain);
        }

        switch (c = (key != ERR ? key : wait_for_key())) {
        case KEY_DOWN:
        case '\n':
            touchwin(wbody);
            wrefresh(wbody);
            remove_error_message();
            set_menu_position(TH + MH, curr_selection * bar_len);
            curs_set(1);
            (mp[curr_selection].func)();
            curs_set(0);

            switch (key) {
            case KEY_LEFT:
                curr_selection = (curr_selection + n_items - 1) % n_items;
                key = ERR; /// key = '\n';
                break;
            case KEY_RIGHT:
                curr_selection = (curr_selection + 1) % n_items;
                key = ERR; /// key = '\n';
                break;
            default:
                key = ERR;
            }

            repaint_main_menu(mp, bar_len);
            old_selection = -1;

            break;
        case KEY_LEFT:
            curr_selection = (curr_selection + n_items - 1) % n_items;
            break;
        case KEY_RIGHT:
            curr_selection = (curr_selection + 1) % n_items;
            break;
        case KEY_ESC:
            main_help();
            break;
        }
    }

    remove_error_message();
    touchwin(wbody);
    wrefresh(wbody);
}

static void clean() {
    if (in_curses) {
        delwin(wtitle);
        delwin(wmain);
        delwin(wbody);
        delwin(wstatus);
        curs_set(1);
        endwin();
        in_curses = FALSE;
    }
}

/*********************** EXTERNAL FUNCTIONS ***********************/
void finish() {
    quit = TRUE;
}

void title_message(char *title) {
    mvwaddstr(wtitle, 0, 2, pad_str_left(title, BW - 3));
    wrefresh(wtitle);
}

void status_message(char *message) {
    mvwaddstr(wstatus, 1, 2, pad_str_left(message, BW - 3));
    wrefresh(wstatus);
}

void error_message(char *message, int sound_flag) {
    if (sound_flag)
        beep();
    mvwaddstr(wstatus, 0, 2, pad_str_left(message, BW - 3));
    wrefresh(wstatus);
}

void remove_status_message() {
    remove_line(wstatus, 1);
}

void remove_error_message() {
    remove_line(wstatus, 0);
}

void clear_body() {
    werase(wbody);
    wmove(wbody, 0, 0);
    touchwin(wbody);
    wrefresh(wbody);
}

void clear_file_explorer_subwin(WINDOW *win) {
    werase(win);
    wmove(win, 0, 0);
    color_box(win, BODYCOLOR, 1);
    touchwin(win);
    wrefresh(win);
}

bool key_pressed() {
    ch = wgetch(wbody);
    return ch != ERR;
}

int get_key() {
    int c = ch;
    ch = ERR;

#ifdef ALT_X
    quit = (c == ALT_X);
#endif // ALT_ESC

    return c;
}

int wait_for_key() {
    do
        idle();
    while(!key_pressed());
    return get_key();
}

Key* select_key(List *list) {
    int curr_selection = 1, old_selection = -1;
    ListElement *curr = list->head, *old = NULL;
    bool stop = FALSE;
    int rewrite_old_flag = 1;

    if (!list->head) {
        error_message("No keys in the list!", 1);
        return NULL;
    }

    curs_set(0);
    clear_body();
    repaint_body_keys(list->head, BH - 1, 1, 2);

    key = ERR;
    while (!stop && !quit) {
        if (curr_selection != old_selection) {
            if (old_selection != -1 && rewrite_old_flag)
                mvwaddstr(wbody, (old_selection - 1) % (BH - 1) + 1, 2, convert_key_to_string((Key*)old->info));

            set_color(wbody, BODYREVCOLOR);
            mvwaddstr(wbody, (curr_selection - 1) % (BH - 1) + 1, 2, convert_key_to_string((Key*)curr->info));
            set_color(wbody, BODYCOLOR);

            old_selection = curr_selection;
            old = curr;
            wrefresh(wbody);
        }

        switch (key = (key != ERR ? key : wait_for_key())) {
        case KEY_UP:
            curr_selection--;
            if (!curr_selection) {
                curr_selection = list->length;
                curr = list->tail;
                clear_body();
                repaint_body_keys(find_kth_prev(curr, list->length % (BH - 1) - 1), BH - 1, 1, 2);
                rewrite_old_flag = 0;
            }
            else {
                curr = curr->prev;
                if (curr_selection % (BH - 1) == 0) {
                    clear_body();
                    repaint_body_keys(find_kth_prev(curr, BH - 2), BH - 1, 1, 2);
                    rewrite_old_flag = 0;
                }
                else
                    rewrite_old_flag = 1;
            }
            key = ERR;
            break;
        case KEY_DOWN:
            curr_selection++;
            if (curr_selection > list->length) {
                curr_selection -= list->length;
                curr = list->head;
                clear_body();
                repaint_body_keys(curr, BH - 1, 1, 2);
                rewrite_old_flag = 0;
            }
            else {
                curr = curr->next;
                if ((curr_selection - 1) % (BH - 1) == 0) {
                    clear_body();
                    repaint_body_keys(curr, BH - 1, 1, 2);
                    rewrite_old_flag = 0;
                }
                else
                    rewrite_old_flag = 1;
            }
            key = ERR;
            break;
        case KEY_ESC:
            curr = NULL;
        case '\n':
            stop = TRUE;
            key = ERR;
            break;
        default:
            key = ERR;
        }
    }

    clear_body();

    if (curr)
        return (Key*)curr->info;
    error_message("No key selected!", 1);
    return NULL;
}

void do_menu(Menu *mp) {
    int y, x;
    int n_items, bar_len;
    int menu_width, menu_height;
    int old_selection = -1, curr_selection = 0;
    bool stop = FALSE;
    WINDOW *wmenu;

    curs_set(0);
    get_menu_position(&y, &x);
    menu_dimensions(mp, &n_items, &bar_len);
    menu_height = n_items + 2;
    menu_width = bar_len + 2;
    wmenu = newwin(menu_height, menu_width, y, x);
    color_box(wmenu, SUBMENUCOLOR, 1);
    repaint_menu(wmenu, mp);

    key = ERR;
    while (!stop && !quit) {
        if (curr_selection != old_selection) {
            if (old_selection != -1)
                mvwaddstr(wmenu, old_selection + 1, 1, str_add_space_left(pad_str_left(mp[old_selection].name, bar_len - 1)));

            set_color(wmenu, SUBMENUREVCOLOR);
            mvwaddstr(wmenu, curr_selection + 1, 1, str_add_space_left(pad_str_left(mp[curr_selection].name, bar_len - 1)));
            set_color(wmenu, SUBMENUCOLOR);
            status_message(mp[curr_selection].description);

            old_selection = curr_selection;
            wrefresh(wmenu);
        }

        switch (key = (key != ERR ? key : wait_for_key())) {
        case '\n':
            ///touchwin(wbody);
            ///wrefresh(wbody);
            ///set_menu_position(y + 1, x + 1);
            set_menu_position(y + curr_selection + 1, x + menu_width);
            remove_error_message();

            key = ERR;
            curs_set(1);
            (mp[curr_selection].func)();
            curs_set(0);

            stop = TRUE;
            /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            /// da se vrati na pocetak, samo moram da vidim kako da ne brisem error_message
            /// mislim da je dovoljno da se iskomentarise samo malo nize ~504 red
            repaint_menu(wmenu, mp);

            old_selection = -1;
            break;
        case KEY_UP:
            curr_selection = (curr_selection + n_items - 1) % n_items;
            key = ERR;
            break;
        case KEY_DOWN:
            curr_selection = (curr_selection + 1) % n_items;
            key = ERR;
            break;
        case KEY_ESC:
        case KEY_LEFT:
        case KEY_RIGHT:
            if (key == KEY_ESC)
                key = ERR;
            stop = TRUE;
            break;
        default:
            key = ERR;
        }
    }

    ///remove_error_message();
    delwin(wmenu);
    touchwin(wbody);
    wrefresh(wbody);
}

void start_menu(Menu *mp, char *title) {
    initscr();
    resize_term(LINES, COLS + 12);
    init_colors();
    in_curses = TRUE;

    wtitle = subwin(stdscr, TH, BW, 0, 0);
    wmain = subwin(stdscr, MH, BW, TH, 0);
    wbody = subwin(stdscr, BH, BW, TH + MH, 0);
    wstatus = subwin(stdscr, SH, BW, TH + MH + BH, 0);

    color_box(wtitle, TITLECOLOR, 0);
    color_box(wmain, MAINMENUCOLOR, 0);
    color_box(wbody, BODYCOLOR, 0);
    color_box(wstatus, STATUSCOLOR, 0);

    if (title)
        title_message(title);

    cbreak();
    noecho();
    curs_set(0);
    nodelay(wbody, TRUE);
    halfdelay(10);
    keypad(wbody, TRUE);
    scrollok(wbody, TRUE);

    leaveok(stdscr, TRUE);
    leaveok(wtitle, TRUE);
    leaveok(wmain, TRUE);
    leaveok(wstatus, TRUE);

    main_menu(mp);
    clean();
}

void repaint_edit_box(WINDOW *win, int x, char *buf) {
    int max_x = getmaxx(win);
    werase(win);
    mvwprintw(win, 0, 0, "%s", pad_str_left(buf, max_x));
    wmove(win, 0, x);
    wrefresh(win);
}

WINDOW *winputbox(WINDOW *win, int n_lines, int n_cols) {
    WINDOW *winp;
    int curr_x, curr_y, beg_x, beg_y;

    getyx(win, curr_y, curr_x);
    getbegyx(win, beg_y, beg_x);

    winp = newwin(n_lines, n_cols, beg_y + curr_y, beg_x + curr_x);
    color_box(winp, INPUTBOXCOLOR, 1);

    return winp;
}

int weditstr(WINDOW *win, char *buf, int field) {
    char original_string[MAX_STR_LEN];
    char *buf_pntr = buf;
    bool display_default_message = TRUE, stop = FALSE, insert_mode = FALSE;
    int curry, currx, begy, begx;
    ///int old_attr;
    WINDOW *wedit;
    int ch = 0;

    if ((field >= MAX_STR_LEN) || (buf == NULL) || strlen(buf) > field - 1)
        return ERR;

    strcpy(original_string, buf);

    wrefresh(win);
    getyx(win, curry, currx);
    getbegyx(win, begy, begx);

    wedit = subwin(win, 1, field, begy + curry, begx + currx);
    ///old_attr = wedit->_attrs;
    color_box(wedit, EDITBOXCOLOR, 0);

    keypad(wedit, TRUE);
    curs_set(1);

    while (!stop) {
        idle();
        repaint_edit_box(wedit, buf_pntr - buf, buf);

        switch (ch = wgetch(wedit)) {
        case ERR:
            break;
        case KEY_ESC:
            strcpy(buf, original_string);
            stop = TRUE;
            break;
        case '\n':
        case KEY_UP:
        case KEY_DOWN:
            stop = TRUE;
            break;
        case KEY_LEFT:
            if (buf_pntr > buf)
                buf_pntr--;
            break;
        case KEY_RIGHT:
            display_default_message = FALSE;
            if (buf_pntr - buf < strlen(buf))
                buf_pntr++;
            break;
        case '\t':
        case KEY_IC:
        case KEY_EIC:
            display_default_message = FALSE;
            insert_mode = !insert_mode;
            curs_set(insert_mode ? 2 : 1);
            break;
        default:
            if (ch == erasechar()) {
                if (buf_pntr > buf) {
                    memmove((void*)(buf_pntr - 1), (const void*)buf_pntr, strlen(buf_pntr) + 1);
                    buf_pntr--;
                }
            }
            else if (isprint(ch)) {
                if (display_default_message) {
                    buf_pntr = buf;
                    *buf_pntr = '\0';
                    display_default_message = FALSE;
                }

                if (insert_mode) {
                    if (strlen(buf) < field - 1) {
                        memmove((void*)(buf_pntr + 1), (const void*)buf_pntr, strlen(buf_pntr) + 1);
                        *buf_pntr++ = ch;
                    }
                }
                else if (buf_pntr - buf < field - 1) {
                    if (!*buf_pntr)
                        buf_pntr[1] = '\0';
                    *buf_pntr++ = ch;
                }
            }
        }
    }

    curs_set(0);
    ///wattrset(wedit, old_attr);
    color_box(wedit, INPUTBOXCOLOR, 0);
    repaint_edit_box(wedit, buf_pntr - buf, buf);
    delwin(wedit);

    return ch;
}

int get_input(char *description[], char *buf[], int *field) {
    WINDOW *winput;
    int oldy, oldx, maxy, maxx, n_lines, n_cols = 0;
    int i, n, len, max_desc_len = 0, max_field_len = 0;
    int ch = 0;
    bool stop = FALSE;

    for (n = 0; description[n]; n++) {
        if ((len = strlen(description[n])) > max_desc_len)
            max_desc_len = len;

        if (field[n] > max_field_len)
            max_field_len = field[n];
    }

    //n_lines = n + 2, n_cols = max_desc_len + field + 4;
    n_lines = n + 2, n_cols = max_desc_len + max_field_len + 4;
    getyx(wbody, oldy, oldx);
    getmaxyx(wbody, maxy, maxx);

    winput = mvwinputbox(wbody, (maxy - n_lines) / 2, (maxx - n_cols) / 2, n_lines, n_cols);

    for (i = 0; i < n; i++)
        mvwprintw(winput, i + 1, 2, "%s", description[i]);

    i = 0;
    while (!stop) {
        switch (ch = mvweditstr(winput, i + 1, max_desc_len + 3, buf[i], field[i])) {
        case KEY_ESC:
            stop = TRUE;
            break;
        case KEY_UP:
            i = (i + n - 1) % n;
            break;
        case '\n':
        case KEY_DOWN:
            if (++i == n)
                stop = TRUE;
        }
    }

    delwin(winput);
    touchwin(wbody);
    wmove(wbody, oldx, oldy);
    wrefresh(wbody);

    return ch;
}

int weditpath(WINDOW *win, char *path, int field) {
    char original_string[MAX_STR_LEN];
    char *path_pntr = path;
    bool display_default_message = TRUE, stop = FALSE, insert_mode = FALSE;
    int curry, currx, begy, begx;
    WINDOW *wedit;
    int ch = 0;

    if ((field >= MAX_STR_LEN) || (path == NULL) || strlen(path) > field)
        return ERR;

    strcpy(original_string, path);

    wrefresh(win);
    getyx(win, curry, currx);
    getbegyx(win, begy, begx);

    wedit = subwin(win, 1, field, begy + curry, begx + currx);
    color_box(wedit, EDITBOXCOLOR, 0);

    keypad(wedit, TRUE);
    curs_set(1);

    while (!stop && !quit) {
        idle();
        repaint_edit_box(wedit, path_pntr - path, path);

        switch (ch = wgetch(wedit)) {
        case ERR:
            break;
        case KEY_ESC:
            strcpy(path, original_string);
            stop = TRUE;
            break;
        case '\n':
        case '\t':
            stop = TRUE;
            break;
        case KEY_LEFT:
            if (path_pntr > path)
                path_pntr--;
            break;
        case KEY_RIGHT:
            display_default_message = FALSE;
            if (path_pntr - path < strlen(path))
                path_pntr++;
            break;
        case KEY_IC:
        case KEY_EIC:
            display_default_message = FALSE;
            insert_mode = !insert_mode;
            curs_set(insert_mode ? 2 : 1);
            break;
        default:
            if (ch == erasechar()) {
                if (path_pntr > path) {
                    memmove((void*)(path_pntr - 1), (const void*)path_pntr, strlen(path_pntr) + 1);
                    path_pntr--;
                }
            }
            else if (isprint(ch)) {
                if (display_default_message) {
                    path_pntr = path;
                    *path_pntr = '\0';
                    display_default_message = FALSE;
                }

                if (insert_mode) {
                    if (strlen(path) < field - 1) {
                        memmove((void*)(path_pntr + 1), (const void*)path_pntr, strlen(path_pntr) + 1);
                        *path_pntr++ = ch;
                    }
                }
                else if (path_pntr - path < field - 1) {
                    if (!*path_pntr)
                        path_pntr[1] = '\0';
                    *path_pntr++ = ch;
                }
            }
        }
    }

    curs_set(0);
    color_box(wedit, BODYCOLOR, 0);
    repaint_edit_box(wedit, path - path_pntr, path);
    delwin(wedit);

    return ch;
}

void file_explorer(WINDOW *win, DIR **dir, int *dir_entries, int n_lines, int *curr_selection, int *old_selection, int *rewrite_old_flag, char *curr_path) {
    int curr = *curr_selection, old = *old_selection;
    bool stop = FALSE;
    char names[n_lines][MAX_STR_LEN];
    struct dirent *entry;
    int i;
    char message[MAX_STR_LEN] = "Current path: ";
    int base_message_len = strlen(message);

    /// find entries to show
    int entries_found = find_dir_entries(*dir, curr, n_lines, curr_path, names);
//    seekdir(*dir, (curr / n_lines) * n_lines);
//    for (i = 0; i < n_lines && (entry = readdir(*dir)); i++)
//        strcpy(names[i], entry->d_name);

    /// setup
    curs_set(0);
    clear_file_explorer_subwin(win);
    repaint_file_explorer(win, names, entries_found < n_lines ? entries_found : n_lines, 1, 1);
    set_color(win, BODYREVCOLOR);
    mvwaddstr(win, 1 + curr % n_lines, 1, names[curr % n_lines]);
    set_color(win, BODYCOLOR);
    wrefresh(win);
    strcpy(message + base_message_len, curr_path);
    error_message(message, 0);

    /// do work
    key = ERR;
    while (!stop && !quit) {
        if (curr != old) {
            if (old != -1 && *rewrite_old_flag)
                mvwaddstr(win, 1 + old % n_lines, 1, names[old % n_lines]);

            set_color(win, BODYREVCOLOR);
            mvwaddstr(win, 1 + curr % n_lines, 1, names[curr % n_lines]);
            set_color(win, BODYCOLOR);

            old = curr;
            wrefresh(win);
        }

        switch (key = (key != ERR ? key : wait_for_key())) {
        case KEY_UP:
            if (!curr) {
                curr = *dir_entries - 1;
                /// find entries to show
                find_dir_entries(*dir, curr, n_lines, curr_path, names);
//                seekdir(*dir, (curr / n_lines) * n_lines);
//                for (i = 0; i < n_lines && (entry = readdir(*dir)); i++)
//                    strcpy(names[i], entry->d_name);
                clear_file_explorer_subwin(win);
                repaint_file_explorer(win, names, *dir_entries % n_lines == 0 ? n_lines : *dir_entries % n_lines, 1, 1);
                *rewrite_old_flag = 0;
            }
            else {
                curr--;
                if ((curr + 1) % n_lines == 0) {
                    clear_file_explorer_subwin(win);
                    /// find entries to show
                    find_dir_entries(*dir, curr, n_lines, curr_path, names);
//                    seekdir(*dir, (curr / n_lines) * n_lines);
//                    for (i = 0; i < n_lines && (entry = readdir(*dir)); i++)
//                        strcpy(names[i], entry->d_name);
                    repaint_file_explorer(win, names, n_lines, 1, 1);
                    *rewrite_old_flag = 0;
                }
                else
                    *rewrite_old_flag = 1;
            }
            key = ERR;
            break;
        case KEY_DOWN:
            curr++;
            if (curr == *dir_entries) {
                curr = 0;
                clear_file_explorer_subwin(win);
                /// find entries to show
                find_dir_entries(*dir, curr, n_lines, curr_path, names);
//                rewinddir(*dir);
//                for (i = 0; i < n_lines && (entry = readdir(*dir)); i++)
//                    strcpy(names[i], entry->d_name);
                repaint_file_explorer(win, names, *dir_entries < n_lines ? *dir_entries : n_lines, 1, 1);
                *rewrite_old_flag = 0;
            }
            else {
                if (curr % n_lines == 0) {
                    clear_file_explorer_subwin(win);
                    /// find entries to show
                    entries_found = find_dir_entries(*dir, curr, n_lines, curr_path, names);
//                    seekdir(*dir, (curr / n_lines) * n_lines);
//                    for (i = 0; i < n_lines && (entry = readdir(*dir)); i++)
//                        strcpy(names[i], entry->d_name);
                    repaint_file_explorer(win, names, entries_found < n_lines ? entries_found : n_lines, 1, 1);
                    *rewrite_old_flag = 0;
                }
                else
                    *rewrite_old_flag = 1;
            }
            key = ERR;
            break;
        case '\n':
            if (names[curr % n_lines][0] == '/' || !strcmp(names[curr % n_lines], "..")) {
                int offset = names[curr % n_lines][0] == '/';
                strcat(curr_path, names[curr % n_lines] + offset);
                strcat(curr_path, "/");

                closedir(*dir);
                *dir = opendir(curr_path);
                curr = 0, old = -1, *rewrite_old_flag = 1;
                *dir_entries = count_dir_entries(*dir, curr_path);
                find_dir_entries(*dir, curr, n_lines, curr_path, names);
                clear_file_explorer_subwin(win);
                repaint_file_explorer(win, names, *dir_entries < n_lines ? *dir_entries : n_lines, 1, 1);

                key = ERR;
            }
            else {
                strcat(curr_path, names[curr % n_lines]);
                stop = TRUE;
            }

            strcpy(message + base_message_len, curr_path);
            error_message(message, 0);
            break;
        case KEY_ESC:
        case '\t':
            stop = TRUE;
            break;
        default:
            key = ERR;
        }
    }

    *curr_selection = curr, *old_selection = old;
}

int get_filepath(char *path) {
    WINDOW *wexp, *wpath;
    bool stop = FALSE;
    int begy, begx;
    int curr_selection = 0, old_selection = -1, rewrite_old_flag = 1;
    char curr_path[MAX_STR_LEN] = "./"; /// da pamtim trenutnu putanju za file explorer
    DIR *curr_dir = opendir(".");
    struct dirent *entry;
    int dir_entries = count_dir_entries(curr_dir, curr_path);
    char *file_path_str = "File path:";
    int file_path_str_len = strlen(file_path_str);
    int choose_flag = 0;

    getbegyx(wbody, begy, begx);
    wpath = newwin(3, BW, begy, begx);
    wexp = newwin(BH - 3, BW, begy + 3, begx);
    color_box(wpath, BODYCOLOR, 1);
    color_box(wexp, BODYCOLOR, 1);
    mvwprintw(wpath, 1, 1, "%s", file_path_str);

    while (!stop && !quit) {
        if (choose_flag) {
            status_message("Use arrow keys and Enter to select (TAB to manually input file path)");
            file_explorer(wexp, &curr_dir, &dir_entries, BH - 5, &curr_selection,
                          &old_selection, &rewrite_old_flag, curr_path);
        }
        else {
            status_message("Manually input file path (TAB to switch to file explorer)");
            remove_error_message();
            key = mvweditpath(wpath, 1, file_path_str_len + 2, path, BW - 4 - file_path_str_len);
        }

        switch (key) {
        case KEY_ESC:
        case '\n':
            stop = TRUE;
            break;
        case '\t':
            choose_flag = !choose_flag;
            break;
        default:
            key = ERR;
        }
    }

    if (key == '\n' && choose_flag)
        strcpy(path, curr_path);

    remove_error_message();
    closedir(curr_dir);
    delwin(wpath);
    delwin(wexp);
    touchwin(wbody);
    wmove(wbody, 0, 0);
    wrefresh(wbody);

    return key;
}
