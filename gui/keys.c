#include "keys.h"
#include "list.h"

#include <stdio.h>
#include <string.h>

int read_keys(char *filename, List **list) {
    FILE *in_file = fopen(filename, "r");
    Key *key;
    char row[MAX_KEY_ROW_LEN + 2];
    char *pntr;

    remove_list(*list);
    free(*list);
    *list = init_list();

    if (in_file) {
        while (fgets(row, MAX_KEY_ROW_LEN + 1, in_file)) {
            key = (Key*) malloc(sizeof(Key));

            pntr = strtok(row, SEPARATOR_STRING);
            strcpy(key->type, pntr);
            pntr = strtok(NULL, SEPARATOR_STRING);
            strcpy(key->mode, pntr);
            pntr = strtok(NULL, SEPARATOR_STRING);
            strcpy(key->key_name, pntr);
            pntr = strtok(NULL, SEPARATOR_STRING);
            strcpy((key->key)[0], pntr);
            if (pntr = strtok(NULL, SEPARATOR_STRING))
                strcpy((key->key)[1], pntr);
            if (pntr = strtok(NULL, SEPARATOR_STRING))
                strcpy((key->key)[2], pntr);

            tail_add(*list, (void*)key);
        }
        fclose(in_file);
        return 0;
    }

    return 1;
}

void add_key(List *list, Key *key) {
    tail_add(list, (void*)key);
}

int write_keys(List *list, char *filename) {
    FILE *out_file = fopen(filename, "w");

    if (out_file) {
        ListElement *curr;
        for (curr = list->head; curr; curr = curr->next) {
            Key *key = (Key*)curr->info;
            fprintf(out_file, "%s%c%s%c%s%c%s%c%s%c%s\n", key->type, SEPARATOR, key->mode, SEPARATOR,
                    key->key_name, SEPARATOR, key->key[0], SEPARATOR, key->key[1], SEPARATOR, key->key[2]);
        }
        fclose(out_file);
        return 0;
    }
    return 1;
}

void remove_key(List *list, Key *key) {
    ListElement *curr;

    for (curr = list->head; curr && (Key*)curr->info != key; curr = curr->next);

    if (curr)
        remove_element(list, curr);
}

void remove_keys(List *list) {
    remove_list(list);
}

/*
char* convert_key_to_string(Key *key) {
    static char buffer[MAX_KEY_ROW_LEN];
    int curr_pntr = 0;
    char *curr_char;

    for (curr_char = key->type; *curr_char; curr_char++)
        buffer[curr_pntr++] = *curr_char;
    buffer[curr_pntr++] = ' ';

    for (curr_char = key->mode; *curr_char; curr_char++)
        buffer[curr_pntr++] = *curr_char;
    buffer[curr_pntr++] = ' ';

    for (curr_char = key->key_name; *curr_char; curr_char++)
        buffer[curr_pntr++] = *curr_char;
    buffer[curr_pntr++] = ' ';

    for (curr_char = key->key; *curr_char; curr_char++)
        buffer[curr_pntr++] = *curr_char;
    buffer[curr_pntr++] = '\0';

    return buffer;
}
*/
