#ifndef _KEYS_H
#define _KEYS_H

#include "list.h"

#define KEYS_FILE "keys.txt"

#define SEPARATOR ':'
#define SEPARATOR_STRING ":\n"

#define MAX_KEY_ROW_LEN 256
#define MAX_KEY_NAME_LEN 15
#define KEY_PRINT_FORMAT "%-5s %-5s %-15s %-16s %-16s %-16s"

#define AES_KEY_LEN 16
#define DES_KEY_LEN 7

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
    char key_name[20];
    unsigned char key[3][20];
} Key;

int read_keys(char*, List**);
int write_keys(List*, char*);

void add_key(List*, Key*);
void remove_key(List*, Key*);
void remove_keys(List*);
Key* find_key_with_name(List*, char*);

int check_correct_key(Key*, char*);

#endif // _KEYS_H
