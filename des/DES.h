#ifndef _DES_H_
#define _DES_H_



typedef unsigned char uc;

void charToBin(uc*, int);
void desEncodeBlock(uc *input, uc *key, int mode, uc *output); // mode 1 for decrypt, mode 0 for encrypt
void desEncodeFile(char *name, uc* key, int mode);


#endif // _DES_H_
