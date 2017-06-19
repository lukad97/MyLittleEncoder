#ifndef _DES_H_
#define _DES_H_

#include "../global.h"

int desDecryptFileCBC(char *name, uc* key);
int desDecryptFileECB(char *name, uc* key);
int desEncryptFileCBC(char *name, uc* key);
int desEncryptFileECB(char *name, uc* key);

int tdesDecryptFileCBC(char *name, uc* key1, uc* key2, uc* key3);
int tdesDecryptFileECB(char *name, uc* key1, uc* key2, uc* key3);
int tdesEncryptFileCBC(char *name, uc* key1, uc* key2, uc* key3);
int tdesEncryptFileECB(char *name, uc* key1, uc* key2, uc* key3);

#endif // _DES_H_

