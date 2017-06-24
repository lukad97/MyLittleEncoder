#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include "../global.h"

#define BLOCK_SIZE 16

typedef enum {REGULAR, INVERSE} keyExpandMode;

void getRoundKeys(uc *key, uc roundKeys[][16], int Nk, keyExpandMode inverse);

void encryptBlockRoundKeys(uc *state, uc roundKeys[][16], int Nr);
void encryptBlock(uc *state, uc *key, int Nk);

void decryptBlockRoundKeys(uc *state, uc invRoundKeys[][16], int Nr);
void decryptBlock(uc *state, uc *key, int Nk);

#endif // _AES_H_
