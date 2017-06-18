#ifndef _AES_FILE_HANDLER_
#define _AES_FILE_HANDLER_R

#include <stdint.h>

typedef uint8_t uc;

int encryptFileECB(char *name, uc* key);
int decryptFileECB(char *name, uc* key);

#endif // _AES_FILE_HANDLER_R
