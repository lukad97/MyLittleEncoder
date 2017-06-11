#ifndef _DES_H_
#define _DES_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ALLOC_CHECK(p)  if (!(p)) printf("Memory allocation unsuccessful.\n"), exit(1)
#define FILE_CHECK(p)   if (!(p)) printf("Opening file unsuccessful.\n"), exit(2)

typedef unsigned char uc;

void charToBin(uc*, int);

// mode 1 for decrypt, mode 0 for encrypt
// name takes in path of file to be encoded
void desEncodeBlock(uc *input, uc *key, int mode, uc *output);

void desEncodeFileECB(char *name, uc* key, int mode);

void desEncryptFileCBC(char *name, uc* key, uc* IV);
void desDecryptFileCBC(char *name, uc* key, uc* IV);

void tdesEncodeBlock(uc *input, uc *key1, uc *key2, uc *key3, int mode, uc *output);

void tdesEncodeFileECB(char *name, uc* key1, uc* key2, uc* key3, int mode);

void tdesEncryptFileCBC(char *name, uc* key1, uc* key2, uc* key3, uc* IV);
void tdesDecryptFileCBC(char *name, uc* key1, uc* key2, uc* key3, uc* IV);

#endif // _DES_H_
