#ifndef _KEYS_H
#define _KEYS_H

#include "list.h"

#define KEYS_FILE "keys.txt"

#define SEPARATOR ':'
#define SEPARATOR_STRING ":\n"

#define MAX_KEY_ROW_LEN 256
#define MAX_KEY_NAME_LEN 15
#define KEY_PRINT_FORMAT_TDES "%-6s %-6s %-15s %-10s %-10s %-10s"
#define KEY_PRINT_FORMAT_OTHERS "%-6s %-6s %-15s %-32s"

#define AES16_KEY_LEN 16
#define AES24_KEY_LEN 24
#define AES32_KEY_LEN 32
#define DES_KEY_LEN 7

#define AES16_STR "aes16"
#define AES24_STR "aes24"
#define AES32_STR "aes32"
#define DES_STR "des"
#define TDES_STR "3des"

#define MODE_ECB_STR "ecb"
#define MODE_CBC_STR "cbc"

typedef struct Key {
    char type[10];
    char mode[10];
    char key_name[20];
    unsigned char key[3][35];
} Key;

int read_keys(char*, List**);
int write_keys(List*, char*);

void add_key(List*, Key*);
void remove_key(List*, Key*);
void remove_keys(List*);
Key* find_key_with_name(List*, char*);

int check_correct_key(Key*, char*);

#endif // _KEYS_H
