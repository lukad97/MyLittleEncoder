#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

#include "global.h"

typedef enum {des_ecb, des_cbc, tdes_ecb, tdes_cbc, aes_ecb, aes_cbc} Algorithm;

int encryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);
int decryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);

#endif // _ENCRYPTION_H_
