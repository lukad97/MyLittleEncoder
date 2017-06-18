#ifndef _DES_H_
#define _DES_H_

#include <stdlib.h>
#include "../file_header/file_header.h"

typedef unsigned char uc;

typedef enum {des_ecb, des_cbc, tdes_ecb, tdes_cbc} Algorithm;

int encryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);
int decryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);

#endif // _DES_H_

