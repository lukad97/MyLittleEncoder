#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aes_file_handler.h"
#include "aes.h"
#include "..\file_header\file_header.h"

// -------------- FILE ENCRYPTION ---------------------

int aesEncryptFile(char *path, uc* key, int Nk, modeOfOperation mode)
{
	FILE *in, *out;
	fileheader_t header;
	char outPath[FILENAME_LEN_MAX+4];
	uc state[BLOCK_SIZE], IV[BLOCK_SIZE];
	int i = 0, Nr = Nk + 6;

    uc roundKeys[Nr+1][BLOCK_SIZE];
    getRoundKeys(key, roundKeys, Nk, REGULAR);

	in = fopen(path, "rb");
	FILE_CHECK(in);

	strcpy(outPath, path);
	strcat(outPath, ".dat");

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	header = headerCreate(in, get_filename_from_path(path));

    for (i = 0; i < BLOCK_SIZE; ++i)
        IV[i] = header.IV[i];

    uc *p = (uc*) &header;

	for (i = 0; i < sizeof(header)/BLOCK_SIZE; i++) {
        encryptBlockRoundKeys(p, roundKeys, Nr);
        fwrite(p, sizeof(uc), BLOCK_SIZE, out);
        p += BLOCK_SIZE;
	}

    while ((i = fread(state, sizeof(uc), BLOCK_SIZE, in)))
    {
        for (; i<BLOCK_SIZE; i++)
            state[i] = 0;

        if (mode == EBC)
        {
            encryptBlockRoundKeys(state, roundKeys, Nr);
        }
        else if (mode == CBC)
        {
            for (i = 0; i < BLOCK_SIZE; ++i) {
                state[i] ^= IV[i];
            }
            encryptBlockRoundKeys(state, roundKeys, Nr);
            for (i = 0; i < BLOCK_SIZE; ++i) {
                IV[i] = state[i];
            }
        }

        fwrite(state, sizeof(uc), BLOCK_SIZE, out);
    }

	fclose(in);
	fclose(out);
	return 0;
}

int aesDecryptFile(char *path, uc* key, int Nk, modeOfOperation mode)
{
	FILE *in, *out;
	fileheader_t header;
	char outPath[FILENAME_MAX+4];
	uc state[BLOCK_SIZE], temp_state[BLOCK_SIZE];
	int i = 0, Nr = Nk + 6;

    uc invRoundKeys[Nr+1][BLOCK_SIZE];
    getRoundKeys(key, invRoundKeys, Nk, INVERSE);

	in = fopen(path, "rb");
	FILE_CHECK(in);

	uc *p = (uc*) &header;
	for (int i = 0; i < sizeof(header)/BLOCK_SIZE; i++) {
	    fread(p, sizeof(uc), BLOCK_SIZE, in);
        decryptBlockRoundKeys(p, invRoundKeys, Nr);
        p += BLOCK_SIZE;
	}

    uint64_t len = header.byteLength;

    strcpy(outPath, path);
    strcpy(get_filename_from_path(outPath), (char*) header.fileName);

    out = fopen(outPath, "rb");
    if (out != NULL) {
        fclose(out);
        srand(time(NULL));
        sprintf(get_filename_from_path(outPath), "%d\0", rand());
        strcat(outPath, (char*) header.fileName);
    }

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	while ((i = fread(state, sizeof(uc), BLOCK_SIZE, in)))
    {
        if (mode == EBC)
        {
            decryptBlockRoundKeys(state, invRoundKeys, Nr);
        }
        else if (mode == CBC)
        {
            for (i = 0; i < BLOCK_SIZE; ++i) {
                temp_state[i] = state[i];
            }
            decryptBlockRoundKeys(state, invRoundKeys, Nr);
            for (i = 0; i < BLOCK_SIZE; ++i) {
                state[i] ^= header.IV[i];
                header.IV[i] = temp_state[i];
            }
        }

        fwrite(state, sizeof(uc), len > BLOCK_SIZE ? BLOCK_SIZE : len, out);
        len -= BLOCK_SIZE;
    }
	fclose(in);
	fclose(out);

	in = fopen(outPath, "rb");
	FILE_CHECK(in);

    i = headerCheck(in, &header) ? CRC_MISMATCH : 0;

    fclose(in);

	return i;
}
