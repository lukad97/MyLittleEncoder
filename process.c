/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Funkcije koje se pozivaju prilikom enkripcije ili dekripcije fajlova. Ove funkcije izvrsavaju
* proveru i obradu zadatih argumenata, a zatim pozivaju funkcije deklarisane u zaglavlju encryption.h.
*/

#include "process.h"
#include "keys.h"
#include "gui.h"
#include "encryption.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <regex.h>

/*********************** INTERNAL FUNCTIONS ***********************/
/**
* @brief Funkcija za ispitivanje koji algoritam treba koristiti za zadati kljuc.
* @param[in] key Kljuc za koji treba pronaci algoritam
* @return Algoritam koji treba koristiti
*/
static Algorithm select_algorithm(Key *key) {
    Algorithm algo;
    int aes16_flag = 0, aes24_flag = 0, aes32_flag = 0, des_flag = 0, tdes_flag = 0;
    int ecb_flag = 0, cbc_flag = 0;

    if (!strcmp(key->type, AES16_STR)) aes16_flag = 1;
    else if (!strcmp(key->type, AES24_STR)) aes24_flag = 1;
    else if (!strcmp(key->type, AES32_STR)) aes32_flag = 1;
    else if (!strcmp(key->type, DES_STR)) des_flag = 1;
    else tdes_flag = 1;

    if (!strcmp(key->mode, MODE_ECB_STR)) ecb_flag = 1;
    else cbc_flag = 1;

    if (aes16_flag) {
        if (ecb_flag) algo = aes128_ecb;
        else algo = aes128_cbc;
    }
    else if (aes24_flag) {
        if (ecb_flag) algo = aes192_ecb;
        else algo = aes192_cbc;
    }
    else if (aes32_flag) {
        if (ecb_flag) algo = aes256_ecb;
        else algo = aes256_cbc;
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

/**
* @brief Funkcija koja zadati obrazac prevodi u obrazac za koji je moguce pozivati funkcije iz zaglavlja regex.h.
* @param[in] pattern Obrazac koji treba izmeniti
*/
static void build_regexp_string(char *pattern) {
    char *filename_pntr = pattern + strlen(pattern) - 1;

    /// find first char of filename
    while (filename_pntr >= pattern && *filename_pntr != '/')
        filename_pntr--;
    filename_pntr++;

    /// replacing * and ?
    while (*filename_pntr) {
        if (*filename_pntr == '*') {
            memmove((void*)(filename_pntr + MULTICHAR_REGEX_LEN), (const void*)(filename_pntr + 1), strlen(filename_pntr + 1) + 1);
            strncpy(filename_pntr, MULTICHAR_REGEX, MULTICHAR_REGEX_LEN);
            filename_pntr += MULTICHAR_REGEX_LEN;
        }
        else if (*filename_pntr == '?') {
            memmove((void*)(filename_pntr + SINGLECHAR_REGEX_LEN), (const void*)(filename_pntr + 1), strlen(filename_pntr + 1) + 1);
            strncpy(filename_pntr, MULTICHAR_REGEX, SINGLECHAR_REGEX_LEN);
            filename_pntr += SINGLECHAR_REGEX_LEN;
        }
        else if (*filename_pntr == '.') {
            memmove((void*)(filename_pntr + 3), (const void*)(filename_pntr + 1), strlen(filename_pntr + 1) + 1);
            strncpy(filename_pntr, "[.]", 3);
            filename_pntr += 3;
        }
        else
            filename_pntr++;
    }
}

/**
* @brief Funkcija koja iz putanje uklanja naziv postavljanjem '\0' karaktera na odgovarajuce mesto u stringu.
* @param[in] path Putanja iz koje treba ukloniti naziv fajla
*/
static void remove_filename_from_path(char *path) {
    char *pntr = path + strlen(path) - 1;
    while (pntr >= path && *pntr != '/')
        pntr--;
    *(pntr + 1) = '\0';
}

/**
* @brief Funkcija koja u okviru zadate putanje pronalazi mesto na kome pocinje naziv fajla.
* @param[in] file_path Putanja do fajla
* @return Pokazivac na karakter gde pocinje naziv fajla
*/
static char* get_filename_from_path(char *file_path) {
    char *pntr;
    char *last_slash = file_path;

    for (pntr = file_path; *pntr; pntr++)
        if (*pntr == '/')
            last_slash = pntr;

    return last_slash == file_path ? file_path : last_slash + 1;
}

/**
* @brief Funkcija koja vrsi pripremu za enkripciju/dekripciju vise fajlova pomocu regularnog izraza.
* @param[in] file_path Putanja do fajla koji treba enkriptovati
* @param[out] error_msg String u koji ce biti upisana poruka o gresci
* @return 0 ako nije doslo do greske, 1 u suprotnom
*/
static int regex_preprocess(char *file_path, char *error_msg) {
    struct re_pattern_buffer pattern_buf;
    char regex_pattern[MAX_STR_LEN];
    DIR *dir;
    struct dirent *entry;
    struct stat stats;
    char stat_str[MAX_STR_LEN];
    FILE *out_file = fopen(REGEX_TMP_FILE, "w");

    /// getting pattern part from path
    strcpy(regex_pattern, get_filename_from_path(file_path));

    /// finding directory
    remove_filename_from_path(file_path);
    if (file_path[0] == '\0')
        strcpy(file_path, "./");

    ///open directory
    dir = opendir(file_path);
    if (!dir) {
        strcpy(error_msg, "Directory not found!");
        return 1;
    }

    /// compiling pattern
    re_syntax_options = RE_INTERVALS | RE_CHAR_CLASSES | RE_CONTEXT_INDEP_OPS;
    build_regexp_string(regex_pattern);
    pattern_buf.translate = pattern_buf.fastmap = pattern_buf.buffer = pattern_buf.allocated = 0;
    re_compile_pattern(regex_pattern, strlen(regex_pattern), &pattern_buf);

    /// going through directory entries
    while (entry = readdir(dir)) {
        strcpy(stat_str, file_path);
        strcat(stat_str, entry->d_name);

        /// only files are being compared to regex
        stat(stat_str, &stats);
        if (S_ISREG(stats.st_mode) && re_match(&pattern_buf, entry->d_name, strlen(entry->d_name), 0, 0) == strlen(entry->d_name) && out_file)
            fprintf(out_file, "%s\n", stat_str);
    }
    closedir(dir);
    fclose(out_file);
    return 0;
}

/*********************** EXTERNAL FUNCTIONS ***********************/
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

int encrypt_more_files(char *file_path, Key *key, FILE *log) {
    char file[MAX_STR_LEN];
    char error_msg[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");

    if (f) {
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
        return 0;
    }
    else
        return 1;
}

int decrypt_more_files(char *file_path, Key *key, FILE *log) {
    char file[MAX_STR_LEN];
    char error_msg[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");

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
        return 0;
    }
    else
        return 1;
}

int encrypt_regex_files(char *file_path, Key *key, char *error_msg, FILE *log) {
    int exit_code = regex_preprocess(file_path, error_msg);

    if (exit_code)
        return 1;

    if (encrypt_more_files(REGEX_TMP_FILE, key, log)) {
        strcpy(error_msg, "Unable to open regex temporary file");
        return 1;
    }
    return 0;
}

int decrypt_regex_files(char *file_path, Key *key, char *error_msg, FILE *log) {
    int exit_code = regex_preprocess(file_path, error_msg);

    if (exit_code)
        return 1;

    if (decrypt_more_files(REGEX_TMP_FILE, key, log)) {
        strcpy(error_msg, "Unable to open regex temporary file");
        return 1;
    }
    return 0;
}
