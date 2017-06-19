#include <stdio.h>
#include <string.h>
#include "des.h"
#include "../file_header/file_header.h"
#include "../global.h"

uc bitPairity(uc k)
{
	int s = 0, i;

	for (i = 0; i<8; i++)
		s += (k >> i) & 0x01;

	if (s % 2)
		return 0x00;
	else
		return 0x01;
}

void desExpandKey(uc* key, uc *ekey)
{
	int i;
	memset(ekey, 0, sizeof(ekey));

	for (i = 0; i<8; i++)
	{
		ekey[i] = (key[i - 1] & (0xFF >> (8 - i))) << (8 - i);
		ekey[i] |= (key[i] & (0xFF << (i + 1))) >> i;
		ekey[i] |= bitPairity(ekey[i]);
	}
}

void IP(uc* input, uc *output)
{
	int permutation[] = { 58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7 };
	int i;
	for (i = 0; i<8; i++)
		output[i] = 0;
	for (i = 0; i<64; i++)
		output[i / 8] |= (((0x80 >> ((permutation[i] - 1) % 8)) & input[(permutation[i] - 1) / 8]) << ((permutation[i] - 1) % 8)) >> i % 8;
}

void reverseIP(uc* input, uc *output)
{
	int permutation[] = { 40,  8, 48, 16, 56, 24, 64, 32,
		39,  7, 47, 15, 55, 23, 63, 31,
		38,  6, 46, 14, 54, 22, 62, 30,
		37,  5, 45, 13, 53, 21, 61, 29,
		36,  4, 44, 12, 52, 20, 60, 28,
		35,  3, 43, 11, 51, 19, 59, 27,
		34,  2, 42, 10, 50, 18, 58, 26,
		33,  1, 41,  9, 49, 17, 57, 25 };
	int i;
	for (i = 0; i<8; i++)
		output[i] = 0;
	for (i = 0; i<64; i++)
		output[i / 8] |= (((0x80 >> ((permutation[i] - 1) % 8)) & input[(permutation[i] - 1) / 8]) << ((permutation[i] - 1) % 8)) >> i % 8;

}

void splitKey(uc *input, uc *c, uc *d)
{
	int i;
	for (i = 0; i<3; i++)
	{
		c[i] = input[i];
	}
	c[3] = input[3] & 0xF0;

	for (i = 0; i<3; i++) {
		d[i] = (input[i + 3] & 0x0F) << 4;
		d[i] |= (input[i + 4] & 0xF0) >> 4;
	}

	d[3] = (input[6] & 0x0F) << 4;
}

void splitMsg(uc *input, uc *L, uc *R)
{
	int i;
	for (i = 0; i<4; i++)
	{
		L[i] = input[i];
		R[i] = input[i + 4];
	}
}

void leftRotate(uc *input, int n)
{
	int i, j;
	uc bit1, bit2;
	for (i = 0; i<n; i++)
	{
		bit1 = input[3] >> 7;
		input[3] <<= 1;
		for (j = 2; j >= 0; j--)
		{
			bit2 = input[j] >> 7;
			input[j] <<= 1;
			input[j] |= bit1;
			bit1 = bit2;
		}
		input[3] |= bit1 << 4;
	}
}

uc** keyGenerate(uc* key)
{
	uc** subKeys;

	int leftShifts[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
	int i, j;
	uc output[8];
	memset(output, 0, 8);
	ALLOC_CHECK(output);

	subKeys = malloc(16 * sizeof(uc*));
	ALLOC_CHECK(subKeys);

	int PC1[] = { 57, 49,  41, 33,  25,  17,  9,
		1, 58,  50, 42,  34,  26, 18,
		10,  2,  59, 51,  43,  35, 27,
		19, 11,   3, 60,  52,  44, 36,
		63, 55,  47, 39,  31,  23, 15,
		7, 62,  54, 46,  38,  30, 22,
		14,  6,  61, 53,  45,  37, 29,
		21, 13,   5, 28,  20,  12,  4 };
	int PC2[] = { 14, 17, 11, 24,  1,  5,
		3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8,
		16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32 };

	uc C[4], D[4];
	memset(C, 0, 4);
	memset(D, 0, 4);
	for (i = 0; i<56; i++)
		output[i / 8] |= (((0x80 >> ((PC1[i] - 1) % 8)) & key[(PC1[i] - 1) / 8]) << ((PC1[i] - 1) % 8)) >> i % 8;

	splitKey(output, C, D);

	for (i = 0; i<16; i++)
	{
		leftRotate(C, leftShifts[i]);
		leftRotate(D, leftShifts[i]);
		subKeys[i] = calloc(6, sizeof(uc));
		ALLOC_CHECK(subKeys[i]);
		for (j = 0; j<48; j++)
		{
			if (PC2[j] < 29)
				subKeys[i][j / 8] |= (((0x80 >> ((PC2[j] - 1) % 8)) & C[(PC2[j] - 1) / 8]) << ((PC2[j] - 1) % 8)) >> j % 8;
			else
				subKeys[i][j / 8] |= (((0x80 >> ((PC2[j] - 29) % 8)) & D[(PC2[j] - 29) / 8]) << ((PC2[j] - 29) % 8)) >> j % 8;
		}
	}

	return subKeys;
}

void charToBin(uc* input, int n)
{
	int i, j;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<8; j++)
			if (input[i] & (1 << (7 - j)))
				printf("1");
			else
				printf("0");
		printf(" ");
	}
	printf("\n");
}

uc *shortenMsg(uc *expandedMsg)
{
	int S1[] = { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
		0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
		4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
		15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 };

	int S2[] = { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
		3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
		0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
		13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9 };

	int S3[] = { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
		13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
		13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
		1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 };

	int S4[] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
		13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
		10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
		3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 };

	int S5[] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
		14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
		4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
		11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 };

	int S6[] = { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
		10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
		9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
		4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 };

	int S7[] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
		13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
		1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
		6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 };

	int S8[] = { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
		1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
		7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
		2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 };

	int row, column;
	uc shortenedMsg[4];
	memset(shortenedMsg, 0, 4);

	row = column = 0;
	row |= ((expandedMsg[0] & 0x80) >> 6);
	row |= ((expandedMsg[0] & 0x04) >> 2);
	column |= ((expandedMsg[0] & 0x78) >> 3);

	shortenedMsg[0] |= ((uc)S1[row * 16 + column] << 4);

	row = column = 0;
	row |= (expandedMsg[0] & 0x02);
	row |= ((expandedMsg[1] & 0x10) >> 4);
	column |= ((expandedMsg[0] & 0x01) << 3);
	column |= ((expandedMsg[1] & 0xE0) >> 5);

	shortenedMsg[0] |= (uc)S2[row * 16 + column];

	row = column = 0;
	row |= ((expandedMsg[1] & 0x08) >> 2);
	row |= ((expandedMsg[2] & 0x40) >> 6);
	column |= ((expandedMsg[1] & 0x07) << 1);
	column |= ((expandedMsg[2] & 0x80) >> 7);

	shortenedMsg[1] |= ((uc)S3[row * 16 + column] << 4);

	row = column = 0;
	row |= ((expandedMsg[2] & 0x20) >> 4);
	row |= (expandedMsg[2] & 0x01);
	column |= ((expandedMsg[2] & 0x1E) >> 1);

	shortenedMsg[1] |= (uc)S4[row * 16 + column];

	row = column = 0;
	row |= ((expandedMsg[3] & 0x80) >> 6);
	row |= ((expandedMsg[3] & 0x04) >> 2);
	column |= ((expandedMsg[3] & 0x78) >> 3);

	shortenedMsg[2] |= ((uc)S5[row * 16 + column] << 4);

	row = column = 0;
	row |= (expandedMsg[3] & 0x02);
	row |= ((expandedMsg[4] & 0x10) >> 4);
	column |= ((expandedMsg[3] & 0x01) << 3);
	column |= ((expandedMsg[4] & 0xE0) >> 5);

	shortenedMsg[2] |= (uc)S6[row * 16 + column];

	row = column = 0;
	row |= ((expandedMsg[4] & 0x08) >> 2);
	row |= ((expandedMsg[5] & 0x40) >> 6);
	column |= ((expandedMsg[4] & 0x07) << 1);
	column |= ((expandedMsg[5] & 0x80) >> 7);

	shortenedMsg[3] |= ((uc)S7[row * 16 + column] << 4);

	row = column = 0;
	row |= ((expandedMsg[5] & 0x20) >> 4);
	row |= (expandedMsg[5] & 0x01);
	column |= ((expandedMsg[5] & 0x1E) >> 1);

	shortenedMsg[3] |= (uc)S8[row * 16 + column];

	return shortenedMsg;
}

void f(uc *R, uc *K, uc *output)
{
	int E[] = { 32,  1,  2,  3,  4,  5,
		4,  5,  6,  7,  8,  9,
		8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32,  1 };
	int P[] = { 16,  7, 20, 21,
		29, 12, 28, 17,
		1, 15, 23, 26,
		5, 18, 31, 10,
		2,  8, 24, 14,
		32, 27,  3,  9,
		19, 13, 30,  6,
		22, 11,  4, 25 };
	int i;
	uc expandedMsg[6], *shortenedMsg;
	memset(expandedMsg, 0, 6);

	for (i = 0; i<48; i++)
		expandedMsg[i / 8] |= (((0x80 >> (E[i] - 1) % 8) & R[(E[i] - 1) / 8]) << (E[i] - 1) % 8) >> i % 8;


	for (i = 0; i<6; i++)
		expandedMsg[i] ^= K[i];


	shortenedMsg = shortenMsg(expandedMsg);


	for (i = 0; i<4; i++)
		output[i] = 0;
	for (i = 0; i<32; i++)
		output[i / 8] |= (((0x80 >> (P[i] - 1) % 8) & shortenedMsg[(P[i] - 1) / 8]) << (P[i] - 1) % 8) >> i % 8;

}

void freeKeys(uc **keys)
{
	int i;
	for (i = 0; i<16; i++)
		free(keys[i]);
	free(keys);
}


void desEncodeBlock(uc *input, uc **subKeys, int mode, uc *output)
{
	uc R[4], L[4], tempL[4], tempR[4];
	uc pInput[8];
	int i, j, k;

	IP(input, pInput);

	splitMsg(pInput, L, R);

	for (i = 0; i<16; i++)
	{
		memcpy(tempL, R, 4);

		if (mode)
			k = 15 - i;
		else
			k = i;

		f(R, subKeys[k], tempR);
		for (j = 0; j<4; j++)
			tempR[j] ^= L[j];
		for (j = 0; j<4; j++)
		{
			L[j] = tempL[j];
			R[j] = tempR[j];
		}
	}


	for (i = 0; i<4; i++)
	{
		pInput[i] = R[i];
		pInput[i + 4] = L[i];
	}

	reverseIP(pInput, output);
}

int desEncryptFileECB(char *name, uc* key)
{
	FILE *in, *out;
	fileheader_t head;
	char outPath[256];
	uc msg[8], output[8], **subKeys, ekey[8];
	int i = 0;

	desExpandKey(key, ekey);
	subKeys = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	strcpy(outPath, name);
	strcat(outPath, ".dat");

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	head = headerCreate(in, get_filename_from_path(name));

	for (i = 0; i < 32; i++)
	{
		memcpy(msg, head.fileName + 8 * i, 8 * sizeof(uc));
		desEncodeBlock(msg, subKeys, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	memcpy(msg, &head.byteLength, sizeof(msg));
	desEncodeBlock(msg, subKeys, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	memcpy(msg, &head.crc, sizeof(head.crc));
	memcpy(msg + 4, &head.pad, sizeof(head.pad));
	desEncodeBlock(msg, subKeys, 0, output);
	fwrite(output, sizeof(uc), 8, out);


	while ((i = fread(msg, sizeof(uc), 8, in)) == 8)
	{
		desEncodeBlock(msg, subKeys, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}
	if (i)
	{
		for (; i<8; i++)
			msg[i] = 0;
		desEncodeBlock(msg, subKeys, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	freeKeys(subKeys);
	fclose(in);
	fclose(out);
	return 0;
}

int desDecryptFileECB(char *name, uc* key)
{
	FILE *in, *out;
	fileheader_t head;
	uc msg[8], output[8], c, **subKeys, ekey[8];
	int i = 0;
	uint64_t len;

	desExpandKey(key, ekey);
	subKeys = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	for (i = 0; i < 32; i++)
	{
		fread(output, sizeof(uc), 8, in);
		desEncodeBlock(output, subKeys, 1, msg);
		memcpy(head.fileName + 8 * i, msg, 8 * sizeof(uc));
	}

	fread(output, sizeof(uc), 8, in);
	desEncodeBlock(output, subKeys, 1, msg);
	memcpy(&head.byteLength, msg, sizeof(msg));

	fread(output, sizeof(uc), 8, in);
	desEncodeBlock(output, subKeys, 1, msg);
	memcpy(&head.crc, msg, sizeof(head.crc));
	memcpy(&head.pad, msg + 4, sizeof(head.pad));

	char outPath[256];
	strcpy(outPath, name);
	strcpy(get_filename_from_path(outPath), (char*)head.fileName);

	out = fopen(outPath, "rb");
	if (out != NULL) {
		fclose(out);
		srand(time(NULL));
		sprintf(get_filename_from_path(outPath), "%d\0", rand());
		strcat(outPath, (char*)head.fileName);
	}

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	len = head.byteLength;

	while ((fread(msg, sizeof(uc), 8, in)) == 8 )
	{
		desEncodeBlock(msg, subKeys, 1, output);
		fwrite(output, sizeof(uc), (len > 8) ? 8 : len, out);
		len -= 8;
	}

	freeKeys(subKeys);
	fclose(in);
	fclose(out);
	out = fopen(outPath, "rb");
	FILE_CHECK(out);

	if (headerCheck(out, &head))
	{
		fclose(out);
		return 3;
	}

	fclose(out);
	return 0;
}

int desEncryptFileCBC(char *name, uc* key)
{
	FILE *in, *out;
	char outPath[256];
	uc msg[8], output[8], c;
	fileheader_t head;
	int i = 0, flag = 0, j;
	char ekey[8], **subKeys;

	desExpandKey(key, ekey);
	subKeys = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	strcpy(outPath, name);
	strcat(outPath, ".dat");

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	head = headerCreate(in, get_filename_from_path(name));

	for (i = 0; i < 32; i++)
	{
		memcpy(msg, head.fileName + 8 * i, 8 * sizeof(uc));
		desEncodeBlock(msg, subKeys, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	memcpy(msg, &head.byteLength, sizeof(msg));
	desEncodeBlock(msg, subKeys, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	memcpy(msg, &head.crc, sizeof(head.crc));
	memcpy(msg + 4, &head.pad, sizeof(head.pad));
	desEncodeBlock(msg, subKeys, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	memcpy(msg, head.IV, sizeof(msg));
	desEncodeBlock(msg, subKeys, 0, output);
	fwrite(output, sizeof(uc), 8, out);


	while ((i = fread(msg, sizeof(uc), 8, in)) == 8)
	{
		if (!flag)
		{
			for (j = 0; j<8; j++)
				msg[j] ^= head.IV[j];
			flag = 1;
		}
		else
			for (j = 0; j<8; j++)
				msg[j] ^= output[j];
		desEncodeBlock(msg, subKeys, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}
	if (i)
	{
		for (; i<8; i++)
			msg[i] = 0;
		if (!flag)
		{
			for (j = 0; j<8; j++)
				msg[j] ^= head.IV[j];
			flag = 1;
		}
		else
			for (j = 0; j<8; j++)
				msg[j] ^= output[j];
		desEncodeBlock(msg, subKeys, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	freeKeys(subKeys);
	fclose(in);
	fclose(out);
	return 0;
}

int desDecryptFileCBC(char *name, uc* key)
{
	FILE *in, *out;
	uc msg[8], prevMsg[8], output[8], c;
	fileheader_t head;
	char **subKeys, ekey[8];
	int i = 0, flag = 0, j;
	uint64_t len;

	desExpandKey(key, ekey);
	subKeys = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	for (i = 0; i < 32; i++)
	{
		fread(output, sizeof(uc), 8, in);
		desEncodeBlock(output, subKeys, 1, msg);
		memcpy(head.fileName + 8 * i, msg, 8 * sizeof(uc));
	}

	fread(output, sizeof(uc), 8, in);
	desEncodeBlock(output, subKeys, 1, msg);
	memcpy(&head.byteLength, msg, sizeof(msg));

	fread(output, sizeof(uc), 8, in);
	desEncodeBlock(output, subKeys, 1, msg);
	memcpy(&head.crc, msg, sizeof(head.crc));
	memcpy(&head.pad, msg + 4, sizeof(head.pad));

	fread(output, sizeof(uc), 8, in);
	desEncodeBlock(output, subKeys, 1, msg);
	memcpy(head.IV, msg, sizeof(msg));

	char outPath[256];
	strcpy(outPath, name);
	strcpy(get_filename_from_path(outPath), (char*)head.fileName);

	out = fopen(outPath, "rb");
	if (out != NULL) {
		fclose(out);
		srand(time(NULL));
		sprintf(get_filename_from_path(outPath), "%d\0", rand());
		strcat(outPath, (char*)head.fileName);
	}

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	len = head.byteLength;

	while ((i = fread(msg, sizeof(uc), 8, in)) == 8)
	{
		desEncodeBlock(msg, subKeys, 1, output);
		if (!flag)
		{
			for (j = 0; j < 8; j++)
				output[j] ^= head.IV[j];
			flag = 1;
		}
		else
			for (j = 0; j < 8; j++)
				output[j] ^= prevMsg[j];
		fwrite(output, sizeof(uc), (len > 8) ? 8 : len, out);
		for (i = 0; i<8; i++)
			prevMsg[i] = msg[i];
		len -= 8;
	}

	freeKeys(subKeys);
	fclose(in);
	fclose(out);
	out = fopen(outPath, "rb");
	FILE_CHECK(out);

	if (headerCheck(out, &head))
	{
		fclose(out);
		return 3;
	}

	return 0;
}

int tdesEncodeBlock(uc *input, uc **subKeys1, uc **subKeys2, uc **subKeys3, int mode, uc *output)
{
	uc **Keys[3] = { subKeys1, subKeys2, subKeys3 };
	int i, k;
	for (i = 0; i<3; i++)
	{
		if (mode)
			k = i;
		else
			k = 2 - i;
		desEncodeBlock(input, Keys[k], mode, output);
		mode = 1 - mode;
		input = output;
	}
	return 0;
}

int tdesEncryptFileECB(char *name, uc* key1, uc* key2, uc* key3)
{
	FILE *in, *out;
	char outPath[256];
	uc msg[8], output[8], c, **subKeys1, **subKeys2, **subKeys3, ekey[8];
	fileheader_t head;
	int i = 0;

	desExpandKey(key1, ekey);
	subKeys1 = keyGenerate(ekey);

	desExpandKey(key2, ekey);
	subKeys2 = keyGenerate(ekey);

	desExpandKey(key3, ekey);
	subKeys3 = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	strcpy(outPath, name);
	strcat(outPath, ".dat");

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	head = headerCreate(in, get_filename_from_path(name));

	for (i = 0; i < 32; i++)
	{
		memcpy(msg, head.fileName + 8 * i, 8 * sizeof(uc));
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	memcpy(msg, &head.byteLength, sizeof(msg));
	tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	memcpy(msg, &head.crc, sizeof(head.crc));
	memcpy(msg + 4, &head.pad, sizeof(head.pad));
	tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
	fwrite(output, sizeof(uc), 8, out);


	while ((i = fread(msg, sizeof(uc), 8, in)) == 8)
	{
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}
	if (i)
	{
		for (; i<8; i++)
			msg[i] = 0;
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	freeKeys(subKeys1);
	freeKeys(subKeys2);
	freeKeys(subKeys3);

	fclose(in);
	fclose(out);
	return 0;
}

int tdesDecryptFileECB(char *name, uc* key1, uc* key2, uc* key3)
{
	FILE *in, *out;
	char *oname = malloc(strlen(name) + 3);
	uc msg[8], output[8], c, **subKeys1, **subKeys2, **subKeys3, ekey[8];
	fileheader_t head;
	int i = 0;
	uint64_t len;

	desExpandKey(key1, ekey);
	subKeys1 = keyGenerate(ekey);

	desExpandKey(key2, ekey);
	subKeys2 = keyGenerate(ekey);

	desExpandKey(key3, ekey);
	subKeys3 = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	for (i = 0; i < 32; i++)
	{
		fread(output, sizeof(uc), 8, in);
		tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
		memcpy(head.fileName + 8 * i, msg, 8 * sizeof(uc));
	}

	fread(output, sizeof(uc), 8, in);
	tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
	memcpy(&head.byteLength, msg, sizeof(msg));

	fread(output, sizeof(uc), 8, in);
	tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
	memcpy(&head.crc, msg, sizeof(head.crc));
	memcpy(&head.pad, msg + 4, sizeof(head.pad));

	char outPath[256];
	strcpy(outPath, name);
	strcpy(get_filename_from_path(outPath), (char*)head.fileName);

	out = fopen(outPath, "rb");
	if (out != NULL) {
		fclose(out);
		srand(time(NULL));
		sprintf(get_filename_from_path(outPath), "%d\0", rand());
		strcat(outPath, (char*)head.fileName);
	}

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	len = head.byteLength;

	while ((fread(msg, sizeof(uc), 8, in)) == 8)
	{
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 1, output);
		fwrite(output, sizeof(uc), (len > 8) ? 8 : len, out);
		len -= 8;
	}

	freeKeys(subKeys1);
	freeKeys(subKeys2);
	freeKeys(subKeys3);

	fclose(in);
	fclose(out);
	out = fopen(outPath, "rb");
	FILE_CHECK(out);

	if (headerCheck(out, &head))
	{
		fclose(out);
		return 3;
	}

	fclose(out);
	return 0;
}

int tdesEncryptFileCBC(char *name, uc* key1, uc* key2, uc* key3)
{
	FILE *in, *out;
	char outPath[256];
	fileheader_t head;
	uc msg[8], output[8], c, **subKeys1, **subKeys2, **subKeys3, ekey[8];
	int i, flag = 0, j;

	desExpandKey(key1, ekey);
	subKeys1 = keyGenerate(ekey);

	desExpandKey(key2, ekey);
	subKeys2 = keyGenerate(ekey);

	desExpandKey(key3, ekey);
	subKeys3 = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	strcpy(outPath, name);
	strcat(outPath, ".dat");

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	head = headerCreate(in, get_filename_from_path(name));

	for (i = 0; i < 32; i++)
	{
		memcpy(msg, head.fileName + 8 * i, 8 * sizeof(uc));
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	memcpy(msg, &head.byteLength, sizeof(msg));
	tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	memcpy(msg, &head.crc, sizeof(head.crc));
	memcpy(msg + 4, &head.pad, sizeof(head.pad));
	tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	memcpy(msg, &head.IV, sizeof(msg));
	tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
	fwrite(output, sizeof(uc), 8, out);

	while ((i = fread(msg, sizeof(uc), 8, in)) == 8)
	{
		if (!flag)
		{
			for (j = 0; j<8; j++)
				msg[j] ^= head.IV[j];
			flag = 1;
		}
		else
			for (j = 0; j<8; j++)
				msg[j] ^= output[j];
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
		fwrite(output, sizeof(uc), 8, out);
		i = 0;
	}
	if (i)
	{
		for (; i<8; i++)
			msg[i] = 0;
		if (!flag)
		{
			for (j = 0; j<8; j++)
				msg[j] ^= head.IV[j];
			flag = 1;
		}
		else
			for (j = 0; j<8; j++)
				msg[j] ^= output[j];
		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 0, output);
		fwrite(output, sizeof(uc), 8, out);
	}

	freeKeys(subKeys1);
	freeKeys(subKeys2);
	freeKeys(subKeys3);

	fclose(in);
	fclose(out);
	return 0;
}

int tdesDecryptFileCBC(char *name, uc* key1, uc* key2, uc* key3)
{
	FILE *in, *out;
	char *oname = malloc(strlen(name) + 3);
	uc msg[8], prevMsg[8], output[8], c, **subKeys1, **subKeys2, **subKeys3, ekey[8];
	fileheader_t head;
	int i = 0, flag = 0, j;
	uint64_t len;

	desExpandKey(key1, ekey);
	subKeys1 = keyGenerate(ekey);

	desExpandKey(key2, ekey);
	subKeys2 = keyGenerate(ekey);

	desExpandKey(key3, ekey);
	subKeys3 = keyGenerate(ekey);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	for (i = 0; i < 32; i++)
	{
		fread(output, sizeof(uc), 8, in);
		tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
		memcpy(head.fileName + 8 * i, msg, 8 * sizeof(uc));
	}

	fread(output, sizeof(uc), 8, in);
	tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
	memcpy(&head.byteLength, msg, sizeof(msg));

	fread(output, sizeof(uc), 8, in);
	tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
	memcpy(&head.crc, msg, sizeof(head.crc));
	memcpy(&head.pad, msg + 4, sizeof(head.pad));

	fread(output, sizeof(uc), 8, in);
	tdesEncodeBlock(output, subKeys1, subKeys2, subKeys3, 1, msg);
	memcpy(head.IV, msg, sizeof(msg));

	char outPath[256];
	strcpy(outPath, name);
	strcpy(get_filename_from_path(outPath), (char*)head.fileName);

	out = fopen(outPath, "rb");
	if (out != NULL) {
		fclose(out);
		srand(time(NULL));
		sprintf(get_filename_from_path(outPath), "%d\0", rand());
		strcat(outPath, (char*)head.fileName);
	}

	out = fopen(outPath, "wb");
	FILE_CHECK(out);

	len = head.byteLength;

	while ((i = fread(msg, sizeof(uc), 8, in)) == 8)
	{

		tdesEncodeBlock(msg, subKeys1, subKeys2, subKeys3, 1, output);
		if (!flag)
		{
			for (j = 0; j<8; j++)
				output[j] ^= head.IV[j];
			flag = 1;
		}
		else
			for (j = 0; j<8; j++)
				output[j] ^= prevMsg[j];
		fwrite(output, sizeof(uc), (len > 8) ? 8 : len, out);
		len -= 8;
		for (i = 0; i<8; i++)
			prevMsg[i] = msg[i];
	}


	freeKeys(subKeys1);
	freeKeys(subKeys2);
	freeKeys(subKeys3);

	fclose(in);
	fclose(out);
	out = fopen(outPath, "rb");
	FILE_CHECK(out);

	if (headerCheck(out, &head))
	{
		fclose(out);
		return 3;
	}

	fclose(out);
	return 0;
}
