#ifndef _KEYS_H
#define _KEYS_H

#include "list.h"

#define KEYS_FILE "keys.txt"
#define SEPARATOR ':'
#define SEPARATOR_STRING ":\n"

#define MAX_KEY_ROW_LEN 256

/*
typedef enum EncryptionType {
    DES, TRIPLE_DES, AES
} EncryptionType;

typedef enum EncryptionMode {

} EncryptionMode;
*/

typedef struct Key {
    char type[10];
    char mode[10];
    char key_name[200];
    char key[100];
} Key;

int read_keys(char*, List**);
void add_key(List*, Key*);
int write_keys(List*, char*);
void remove_key(List*, Key*);
void remove_keys(List*);

#endif // _KEYS_H
