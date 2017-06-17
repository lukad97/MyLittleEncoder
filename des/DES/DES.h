#ifndef _DES_H_
#define _DES_H_

#include <stdlib.h>
#include "FILEHEADER.h"

#define ALLOC_CHECK(p)  if (!(p)) exit(1)
#define FILE_CHECK(p)   if (!(p)) return(2)

#define ALLOC_ERR 1
#define FILE_ERR  2

typedef unsigned char uc;

typedef enum {des_ebc, tdes_ebc} Algorithm;

void charToBin(uc*, int);

// mode 1 for decrypt, mode 0 for encrypt
// name takes in path of file to be encoded
int desEncryptFileECB(char *name, uc* key);
int desDecryptFileECB(char *name, uc* key);

//int desEncryptFileCBC(char *name, uc* key, uc* IV);
//fileheader_t desDecryptFileCBC(char *name, uc* key, uc* IV);

int tdesEncodeBlock(uc *input, uc **key1, uc **key2, uc **key3, int mode, uc *output);

int tdesEncryptFileECB(char *name, uc* key1, uc* key2, uc* key3);
int tdesDecryptFileECB(char *name, uc* key1, uc* key2, uc* key3);


//int tdesEncryptFileCBC(char *name, uc* key1, uc* key2, uc* key3, uc* IV);
//int tdesDecryptFileCBC(char *name, uc* key1, uc* key2, uc* key3, uc* IV);

#endif // _DES_H_

