#include "process.h"
#include "keys.h"
#include "gui.h"
#include <stdio.h>

/*********************** EXTERNAL FUNCTIONS ***********************/
void encrypt_file(char *file_path, Key *key) {
    /*** TODO ***/
}

void decrypt_file(char *file_path, Key *key) {
    /*** TODO ***/
}

void encrypt_more_files(char *file_path, Key *key) {
    char file[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");

    if (f) {
        while (fscanf(f, "%s", file) != EOF)
            encrypt_file(file, key);
        fclose(f);
    }
    else
        error_message("Unable to open file!", 1);
}

void decrypt_more_files(char *file_path, Key *key) {
    char file[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");

    if (f) {
        while (fscanf(f, "%s", file) != EOF)
            decrypt_file(file, key);
        fclose(f);
    }
    else
        error_message("Unable to open file!", 1);
}

void encrypt_regex_files(char *regex_pattern, Key *key) {
    /*** TODO ***/
}

void decrypt_regex_files(char *regex_pattern, Key *key) {
    /*** TODO ***/
}
