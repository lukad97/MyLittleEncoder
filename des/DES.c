#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef unsigned char uc;

void charToBin(uc*, int);

uc* IP(uc* input)
{
    int permutation[] = {58, 50, 42, 34, 26, 18, 10, 2,
                         60, 52, 44, 36, 28, 20, 12, 4,
                         62, 54, 46, 38, 30, 22, 14, 6,
                         64, 56, 48, 40, 32, 24, 16, 8,
                         57, 49, 41, 33, 25, 17,  9, 1,
                         59, 51, 43, 35, 27, 19, 11, 3,
                         61, 53, 45, 37, 29, 21, 13, 5,
                         63, 55, 47, 39, 31, 23, 15, 7};
    int i;
    uc* output = calloc(8, sizeof(uc));

    for(i=0;i<64;i++)
    {
		output[i/8] |= (((0x80 >> ((permutation[i] - 1)%8)) & input[(permutation[i] - 1)/8]) << ((permutation[i] - 1)%8)) >> i%8;
    }
    return output;
}

uc* reverseIP(uc* input)
{
    int permutation[] =  {40,  8, 48, 16, 56, 24, 64, 32,
                          39,  7, 47, 15, 55, 23, 63, 31,
                          38,  6, 46, 14, 54, 22, 62, 30,
                          37,  5, 45, 13, 53, 21, 61, 29,
                          36,  4, 44, 12, 52, 20, 60, 28,
                          35,  3, 43, 11, 51, 19, 59, 27,
                          34,  2, 42, 10, 50, 18, 58, 26,
                          33,  1, 41,  9, 49, 17, 57, 25};
    int i;
    uc* output = calloc(8, sizeof(uc));

    for(i=0;i<64;i++)
		output[i/8] |= (((0x80 >> ((permutation[i] - 1)%8)) & input[(permutation[i] - 1)/8]) << ((permutation[i] - 1)%8)) >> i%8;

    return output;
}

void split(uc *input, uc *c, uc *d)
{
    int i;
    for(i=0;i<3;i++)
    {
        c[i] = input[i];
    }
    c[3] = input[3] & 0xF0;

    for (i=0; i<3; i++) {
		d[i] = (input[i+3] & 0x0F) << 4;
		d[i] |= (input[i+4] & 0xF0) >> 4;
	}

	d[3] = (input[6] & 0x0F) << 4;
}

void leftRotate(uc *input, int n)
{
    int i, j;
    uc bit1, bit2;
    for(i=0;i<n;i++)
    {
        bit1 = input[3]>>7;
        input[3]<<=1;
        for(j=2;j>=0;j--)
        {
            bit2 = input[j]>>7;
            input[j] <<= 1;
            input[j] |= bit1;
            bit1 = bit2;
        }
        input[3] |= bit1<<4;
    }
}

uc** keyGenerate(uc* key)
{
    uc** subKeys;

    int leftShifts[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    int i, j;
    uc *output = calloc(8, sizeof(uc));

    subKeys = malloc(16*sizeof(uc*));

    int PC1[] = {57, 49,  41, 33,  25,  17,  9,
                 1, 58,  50, 42,  34,  26, 18,
                10,  2,  59, 51,  43,  35, 27,
                19, 11,   3, 60,  52,  44, 36,
                63, 55,  47, 39,  31,  23, 15,
                 7, 62,  54, 46,  38,  30, 22,
                14,  6,  61, 53,  45,  37, 29,
                21, 13,   5, 28,  20,  12,  4};
    int PC2[] = {14, 17, 11, 24,  1,  5,
                 3, 28, 15,  6, 21, 10,
                23, 19, 12,  4, 26,  8,
                16,  7, 27, 20, 13,  2,
                41, 52, 31, 37, 47, 55,
                30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53,
                46, 42, 50, 36, 29, 32};

    uc *C, *D;
    C = calloc(4, sizeof(uc));
    D = calloc(4, sizeof(uc));

    for(i=0;i<56;i++)
        output[i/8] |= (((0x80 >> ((PC1[i] - 1)%8)) & key[(PC1[i] - 1)/8]) << ((PC1[i] - 1)%8)) >> i%8;

    split(output, C, D);

    for(i=0;i<16;i++)
    {
        leftRotate(C, leftShifts[i]);
        leftRotate(D, leftShifts[i]);
        subKeys[i] = calloc(6, sizeof(uc));
        for(j=0;j<48;j++)
        {
            if(PC2[j] < 29)
                subKeys[i][j/8] |= (((0x80 >> ((PC2[j] - 1)%8)) & C[(PC2[j] - 1)/8]) << ((PC2[j] - 1)%8)) >> j%8;
            else
                subKeys[i][j/8] |= (((0x80 >> ((PC2[j] - 29)%8)) & D[(PC2[j] - 29)/8]) << ((PC2[j] - 29)%8)) >> j%8;
        }
    }
    return subKeys;
}

void charToBin(uc* input, int n)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<8;j++)
            if(input[i] & (1<<(7-j)))
                printf("1");
            else
                printf("0");
//        printf(" ");
    }
    printf("\n");
}

int main()
{
    uc *key = calloc(8, sizeof(uc)), *output = calloc(8, sizeof(uc));
    uc **subKeys;
    int i;

    key[0] = 0x3B;
    key[1] = 0x38;
    key[2] = 0x98;
    key[3] = 0x37;
    key[4] = 0x15;
    key[5] = 0x20;
    key[6] = 0xF7;
    key[7] = 0x5E;
//    3b 38 98 37 15 20 f7 5e
//    input = IP(input);
//    ucToBin(input, 8);
//    input = reverseIP(input);
//    ucToBin(input, 8);

    charToBin(key, 8);
    subKeys = keyGenerate(key);

    for(i=0;i<16;i++)
        charToBin(subKeys[i], 6);
    printf("000100010111110010000001110101111110000101001110\n");
    return 0;
}
