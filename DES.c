#include <stdlib.h>
#include <stdio.h>
#include <string.h>



char* IP(char* input)
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
    char* output = calloc(8, sizeof(char));

    for(i=0;i<64;i++)
    {
		output[i/8] |= (((0x80 >> ((permutation[i] - 1)%8)) & input[(permutation[i] - 1)/8]) << ((permutation[i] - 1)%8)) >> i%8;
    }
    return output;
}

void charToBin(char* input, int n)
{
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<8;j++)
            if(input[i] & (1<<(7-j)))
                printf("1");
            else
                printf("0");
    printf("\n");
}

int main()
{
    char test[8] = {};
    test[7] = 5;
    charToBin(test, 8);
    charToBin(IP(test), 8);
    return 0;
}
