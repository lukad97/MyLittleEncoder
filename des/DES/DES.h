#ifndef _DES_H_
#define _DES_H_

#include <stdlib.h>
#include "FILEHEADER.h"

#define ALLOC_CHECK(p)  if (!(p)) exit(1)
#define FILE_CHECK(p)   if (!(p)) return(2)

#define ALLOC_ERR    1
#define FILE_ERR     2
#define CRC_MISMATCH 3

typedef unsigned char uc;

typedef enum {des_ebc, tdes_ebc} Algorithm;

int encryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);
int decryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);

//int tdesEncryptFileCBC(char *name, uc* key1, uc* key2, uc* key3, uc* IV);
//int tdesDecryptFileCBC(char *name, uc* key1, uc* key2, uc* key3, uc* IV);

#endif // _DES_H_

