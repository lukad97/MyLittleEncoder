#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include "../global.h"

void getRoundKeys(uc *key, uc roundKeys[11][16]);
void encryptBlockRoundKeys(uc *state, uc roundKeys[11][16]);
void encryptBlock(uc *state, uc *key);

void getInvRoundKeys(uc *key, uc invRoundKeys[11][16]);
void decryptBlockRoundKeys(uc *state, uc invRoundKeys[11][16]);
void decryptBlock(uc *state, uc *key);

#endif // _AES_H_
