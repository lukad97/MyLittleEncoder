#include "process.h"
#include "keys.h"
#include "gui.h"
#include "encryption.h"
#include "global.h"
#include <stdio.h>
#include <string.h>

/*********************** INTERNAL FUNCTIONS ***********************/
Algorithm select_algorithm(Key *key) {
    Algorithm algo;
    int aes_flag = 0, des_flag = 0, tdes_flag = 0;
    int ecb_flag = 0, cbc_flag = 0;

    if (!strcmp(key->type, "aes")) aes_flag = 1;
    else if (!strcmp(key->type, "des")) des_flag = 1;
    else tdes_flag = 1;

    if (!strcmp(key->mode, "ecb")) ecb_flag = 1;
    else cbc_flag = 1;

    if (aes_flag) {
        if (ecb_flag) algo = aes_ecb;
        else algo = aes_cbc;
    }
    else if (des_flag) {
        if (ecb_flag) algo = des_ecb;
        else algo = des_cbc;
    }
    else {
        if (ecb_flag) algo = tdes_ecb;
        else algo = tdes_cbc;
    }

    return algo;
}

/*********************** EXTERNAL FUNCTIONS ***********************/
/// ako je sve okej vrati 0 i error_msg[0] = '\0'
/// u suprotnom vraca 1 i poruku o gresci u error_msg
int encrypt_file(char *file_path, Key *key, char *error_msg) {
    Algorithm algo = select_algorithm(key);
    int exit_code = encryptFile(file_path, (key->key)[0], (key->key)[1], (key->key)[2], algo);

    if (exit_code) {
        switch(exit_code) {
        case ALLOC_ERR:
            strcpy(error_msg, "Bad allocation");
            break;
        case FILE_ERR:
            strcpy(error_msg, "Unable to open file");
            break;
        case UNKNOWN_ALG:
            strcpy(error_msg, "Unknown encryption algorithm");
            break;
        }

        return exit_code;
    }
    error_msg[0] = '\0';
    return 0;
}

int decrypt_file(char *file_path, Key *key, char *error_msg) {
    Algorithm algo = select_algorithm(key);
    int exit_code = decryptFile(file_path, (key->key)[0], (key->key)[1], (key->key)[2], algo);

    if (exit_code) {
        switch(exit_code) {
        case ALLOC_ERR:
            strcpy(error_msg, "Bad allocation");
            break;
        case FILE_ERR:
            strcpy(error_msg, "Unable to open file");
            break;
        case CRC_MISMATCH:
            strcpy(error_msg, "Decryption unsuccessful");
            break;
        case UNKNOWN_ALG:
            strcpy(error_msg, "Unknown encryption algorithm");
            break;
        }

        return exit_code;
    }
    error_msg[0] = '\0';
    return 0;
}

int encrypt_more_files(char *file_path, Key *key) {
    char file[MAX_STR_LEN];
    char error_msg[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");
    FILE *log = fopen("log.txt", "a");

    if (f) {
        if (log)
            fprintf(log, "---------------------\n");
        while (fscanf(f, "%s", file) != EOF) {
            if (encrypt_file(file, key, error_msg)) {
                if (log)
                    fprintf(log, "Error with file %s:%s\n", file, error_msg);
            }
            else {
                if (log)
                    fprintf(log, "File %s encrypted\n", file);
            }
        }
        fclose(f);
        if (log)
            fclose(log);
        return 0;
    }
    else
        return 1;
}

int decrypt_more_files(char *file_path, Key *key) {
    char file[MAX_STR_LEN];
    char error_msg[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");
    FILE *log = fopen("log.txt", "a");

    if (f) {
        while (fscanf(f, "%s", file) != EOF) {
            if (decrypt_file(file, key, error_msg)) {
                if (log)
                    fprintf(log, "Error with file %s:%s\n", file, error_msg);
            }
            else {
                if (log)
                    fprintf(log, "File %s decrypted\n", file);
            }
        }
        fclose(f);
        fclose(log);
        return 0;
    }
    else
        return 1;
}

/*** TODO ***/
void encrypt_regex_files(char *regex_pattern, Key *key) {
}

/*** TODO ***/
void decrypt_regex_files(char *regex_pattern, Key *key) {
}

char* get_one_string_input(char *description, char *buf, int field_size) {
    char *field_name[2];
    char *field_buf[1];
    int f_size[1] = {field_size};

    field_name[0] = description;
    field_name[1] = 0;
    field_buf[0] = buf;

    return (get_input(field_name, field_buf, f_size) == KEY_ESC) ? NULL : buf;
}
