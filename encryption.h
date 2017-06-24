#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

#include "global.h"

typedef enum {des_ecb, des_cbc, tdes_ecb, tdes_cbc,
              aes128_ecb, aes128_cbc, aes192_ecb, aes192_cbc,
              aes256_ecb, aes256_cbc} Algorithm;

int encryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);
int decryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);

#endif // _ENCRYPTION_H_
