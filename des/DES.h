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
void desEncodeFile(char *name, uc* key, int mode);
void tdesEncodeBlock(uc *input, uc *key1, uc *key2, uc *key3, int mode, uc *output);
void tdesEncodeFile(char *name, uc* key1, uc* key2, uc* key3, int mode);


#endif // _DES_H_
