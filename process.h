#ifndef _PROCESS_H
#define _PROCESS_H

#include "keys.h"
#include <stdio.h>

#define REGEX_TMP_FILE "regex_tmp.txt"

#define MULTICHAR_REGEX "[a-zA-Z0-9]*"
#define MULTICHAR_REGEX_LEN 12
#define SINGLECHAR_REGEX "[a-zA-Z0-9]"
#define SINGLECHAR_REGEX_LEN 11

int encrypt_file(char *file_path, Key *key, char *error_msg);
int decrypt_file(char *file_path, Key *key, char *error_msg);

int encrypt_more_files(char*, Key*, FILE*);
int decrypt_more_files(char*, Key*, FILE*);

int encrypt_regex_files(char*, Key*, char*, FILE*);
int decrypt_regex_files(char*, Key*, char*, FILE*);


char* get_one_string_input(char*, char*, int);
char** get_multiple_string_input(int, char**, char**, int);

#endif // _PROCESS_H
