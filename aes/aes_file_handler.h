#ifndef _AES_FILE_HANDLER_
#define _AES_FILE_HANDLER_R

#include <stdint.h>
#include "../global.h"

int aesEncryptFileECB(char *name, uc* key, int Nk);
int aesDecryptFileECB(char *name, uc* key, int Nk);

#endif // _AES_FILE_HANDLER_R
