#ifndef _PROCESS_H
#define _PROCESS_H

#include "keys.h"

void encrypt_file(char *file_path, Key *key);
void decrypt_file(char *file_path, Key *key);

void encrypt_more_files(char*, Key*);
void decrypt_more_files(char*, Key*);

void encrypt_regex_files(char*, Key*);
void decrypt_regex_files(char*, Key*);

#endif // _PROCESS_H
