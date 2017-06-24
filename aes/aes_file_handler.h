#ifndef _AES_FILE_HANDLER_
#define _AES_FILE_HANDLER_R

#include <stdint.h>
#include "../global.h"

typedef enum {EBC, CBC} modeOfOperation;

int aesEncryptFile(char *name, uc* key, int Nk, modeOfOperation mode);
int aesDecryptFile(char *name, uc* key, int Nk, modeOfOperation mode);

#endif // _AES_FILE_HANDLER_R
