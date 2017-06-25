/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Funkcije za rad sa kljucevima i njihovu obradu.
*/

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
            else
                (key->key)[1][0] = '\0';
            if (pntr = strtok(NULL, SEPARATOR_STRING))
                strcpy((key->key)[2], pntr);
            else
                (key->key)[2][0] = '\0';

            tail_add(*list, (void*)key);
        }
        fclose(in_file);
        return 0;
    }

    return 1;
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

void add_key(List *list, Key *key) {
    tail_add(list, (void*)key);
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

Key* find_key_with_name(List *list, char *key_name) {
    ListElement *curr;

    for (curr = list->head; curr; curr = curr->next)
        if (!strcmp(((Key*)curr->info)->key_name, key_name))
            return (Key*)curr->info;

    return NULL;
}

int check_correct_key(Key *key, char *error_msg) {
    char *encr_types[] = {AES16_STR, AES24_STR, AES32_STR, DES_STR, TDES_STR, 0};
    char *encr_modes[] = {MODE_ECB_STR, MODE_CBC_STR, 0};
    char **pntr;
    int is_aes16 = 0, is_aes24 = 0, is_aes32 = 0, is_des = 0, is_3des = 0;
    int is_ecb = 0, is_cbc = 0;

    if (!strcmp(key->type, encr_types[0]))
        is_aes16 = 1;
    else if (!strcmp(key->type, encr_types[1]))
        is_aes24 = 1;
    else if (!strcmp(key->type, encr_types[2]))
        is_aes32 = 1;
    else if (!strcmp(key->type, encr_types[3]))
        is_des = 1;
    else if (!strcmp(key->type, encr_types[4]))
        is_3des = 1;

    if (!(is_aes16 || is_aes24 || is_aes32 || is_des || is_3des)) {
        strcpy(error_msg, "Incorrect key data! (type = aes16|aes24|aes32|des|3des)");
        return 1;
    }

    if (!strcmp(key->mode, encr_modes[0]))
        is_ecb = 1;
    else if (!strcmp(key->mode, encr_modes[1]))
        is_cbc = 1;

    if (!(is_ecb || is_cbc)) {
        strcpy(error_msg, "Incorrect key data! (mode = ecb|cbc)");
        return 1;
    }

    if ((is_aes16 && strlen(key->key[0]) != AES16_KEY_LEN) || (is_aes24 && strlen(key->key[0]) != AES24_KEY_LEN) ||
        (is_aes32 && strlen(key->key[0]) != AES32_KEY_LEN) || (is_des && strlen(key->key[0]) != DES_KEY_LEN) ||
        (is_3des && (strlen(key->key[0]) != DES_KEY_LEN || strlen(key->key[1]) != DES_KEY_LEN || strlen(key->key[2]) != DES_KEY_LEN))) {
        strcpy(error_msg, "Incorrect key data! (key length: aes16(16)|aes24(24)|aes32(32)|des(7))");
        return 1;
    }
    return 0;
}
