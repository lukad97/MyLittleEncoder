#ifndef _DES_H_
#define _DES_H_

typedef unsigned char uc;

void charToBin(uc*, int);
void encodeBlock(uc *input, uc *key, int mode, uc *output); // mode 1 for decrypt, mode 0 for encrypt

#endif // _DES_H_
