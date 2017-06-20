#ifndef _PROCESS_H
#define _PROCESS_H

#include "keys.h"
#include <stdio.h>

int encrypt_file(char *file_path, Key *key, char *error_msg);
int decrypt_file(char *file_path, Key *key, char *error_msg);

int encrypt_more_files(char*, Key*, FILE*);
int decrypt_more_files(char*, Key*, FILE*);

void encrypt_regex_files(char*, Key*);
void decrypt_regex_files(char*, Key*);


char* get_one_string_input(char*, char*, int);
char** get_multiple_string_input(int, char**, char**, int);

#endif // _PROCESS_H
