//#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "keys.h"
#include "gui.h"
#include "process.h"

/************************* PREDEFINITIONS *************************/
/// Menu
void menu0(), menu1(), menu2();
void menu0_add_key(), menu0_read_keys(), menu0_save_keys(), menu0_choose_active(), menu0_remove();
void remove_key_option(), remove_all_keys_option();
void show_encr_submenu(), show_decr_submenu();
void one_file_encr(), more_files_encr(), regex_encr();
void one_file_decr(), more_files_decr(), regex_decr();
/// Other
char* get_one_string_input(char *description, char *file_name, int field_size);


/***************************** STATIC *****************************/
static List *key_list = NULL;
static Key *active_key = NULL;
/// TREBACE JOS SIGURNO

Menu main_menu[] =
{
    {"Keys", menu0, "Keys options"},
    {"Encryption/Decryption", menu1, "Choose mode and file"},
    {"Exit", menu2, "Exit"},
    {"", (Func)NULL, ""}
};

Menu keys_menu[] =
{
    {"Add key", menu0_add_key, "Add new key to the list"},
    {"Read from file", menu0_read_keys, "Read keys from the file"},
    {"Save to file", menu0_save_keys, "Save keys from list to the file"},
    {"Choose active key", menu0_choose_active, "Choose key from the list to be used"},
    {"Remove key(s)", menu0_remove, "Remove key/keys from the list"},
    {"", (Func)NULL, ""}
};

Menu remove_keys_submenu[] =
{
    {"Remove key", remove_key_option, "Select key from the list to remove"},
    {"Remove all keys", remove_all_keys_option, "Remove all keys from the list"},
    {"", (Func)NULL, ""}
};

Menu encrypt_menu[] =
{
    {"Encryption", show_encr_submenu, "Choose mode and file to encrypt"},
    {"Decryption", show_decr_submenu, "Choose mode and file to decrypt"},
    {"", (Func)NULL, ""}
};

Menu encr_submenu[] =
{
    {"One file", one_file_encr, "Chose one file to encrypt"},
    {"More files", more_files_encr, "Choose more files to encrypt"},
    {"Regex", regex_encr, "Insert regex pattern for file names to encrypt"},
    {"", (Func)NULL, ""}
};

Menu decr_submenu[] =
{
    {"One file", one_file_decr, "Chose one file to decrypt"},
    {"More files", more_files_decr, "Choose more files to decrypt"},
    {"Regex", regex_decr, "Insert regex pattern for file names to decrypt"},
    {"", (Func)NULL, ""}
};

void menu0() {
    do_menu(keys_menu);
}

void menu1() {
    do_menu(encrypt_menu);
}

void menu2() {
    finish();
}

void menu0_add_key() {
    /** TODO **/
    /**
    char *field_descriptions[5];
    char *field_buf[4];
    char buf[4][MAX_STR_LEN];
    buf[0][0] = buf[1][0] = buf[2][0] = buf[3][0] = '\0';

    field_descriptions[0] = "Type";
    field_descriptions[1] = "Mode";
    field_descriptions[2] = "Key name";
    field_descriptions[3] = "Key";
    field_descriptions[4] = 0;
    field_buf[0] = buf[0];
    field_buf[1] = buf[1];
    field_buf[2] = buf[2];
    field_buf[3] = buf[3];

    FILE *f = fopen("DEBUG.txt", "a");
    fprintf(f, "lala\n");
    if (get_input(field_descriptions, field_buf, 50) == KEY_ESC)
        fprintf(f, "prccc\n");
    else
        fprintf(f, "%s %s %s %s\n", buf[0], buf[1], buf[2], buf[3]);
    fclose(f);
    **/
}

void menu0_read_keys() {
    if (read_keys(KEYS_FILE, &key_list)) {
        error_message("Unable to open file with keys! Keys not read...", 1);
        active_key = NULL;
    }
    else {
        active_key = key_list->head ? (Key*)key_list->head->info : NULL;
        if (!active_key)
            error_message("File with keys is empty!", 1);
    }
}

void menu0_save_keys() {
    if (write_keys(key_list, KEYS_FILE))
        error_message("Unable to open file with keys! Keys not written...", 1);
}

void menu0_choose_active() {
    Key *key = select_key(key_list);
    if (key) {
        char msg[MAX_KEY_ROW_LEN] = "Selected active key: ";
        active_key = key;
        error_message(strcat(msg, key->key_name), 0);
    }
}

void menu0_remove() {
    do_menu(remove_keys_submenu);
}

void remove_key_option() {
    Key *key = select_key(key_list);

    if (key) {
        char msg[MAX_KEY_ROW_LEN] = "Removed key: ";
        int flag = key == active_key;

        error_message(strcat(msg, key->key_name), 0);
        remove_key(key_list, key);

        active_key = key_list->head ? (Key*)key_list->head->info : NULL;
    }
}

void remove_all_keys_option() {
    remove_keys(key_list);
    error_message("All keys removed!", 0);
}

void show_encr_submenu() {
    do_menu(encr_submenu);
}

void show_decr_submenu() {
    do_menu(decr_submenu);
}

void one_file_encr() {
    /** TODO **/
    char file_name[MAX_STR_LEN];

    if (get_one_string_input("File path:", file_name, 60)) {
        FILE *file;
        if (file = fopen(file_name, "r")) {
            fclose(file);
            if (active_key)
                encrypt_file(file_name, active_key);
            else
                error_message("No active key selected!", 1);
        }
        else
            error_message("File does not exist!", 1);
    }
    else
        error_message("File path not inputed!", 1);
}

void more_files_encr() {
    /** TODO **/
}

void regex_encr() {
    /** TODO **/
}

void one_file_decr() {
    /** TODO **/
}

void more_files_decr() {
    /** TODO **/
}

void regex_decr() {
    /** TODO **/
}

char* get_one_string_input(char *description, char *file_name, int field_size) {
    char *field_name[2];
    char *field_buf[1];

    field_name[0] = description;
    field_name[1] = 0;
    field_buf[0] = file_name;

    return (get_input(field_name, field_buf, field_size) == KEY_ESC) ? NULL : file_name;
}

int main(int argc, char *argv[]) {
    /// initialization
    key_list = init_list();
    read_keys(KEYS_FILE, &key_list);
    if (key_list->head)
        active_key = (Key*)key_list->head->info;

    /// MAIN WORK
    if (argc == 1) /// gui
        start_menu(main_menu, "Encryption/Decryption SREDI OVAJ TITLE!");
    else { /// batch

    }

    /// free
    remove_list(key_list);
    free(key_list);
    key_list = active_key = NULL;

    return 0;
}
