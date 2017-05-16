#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


//testing...

//Za sada ne radi :P
typedef struct int64
{
    unsigned long long data : 64;
}int64;

int64 IP(int64 input)
{
    int permutation[] = {58, 50, 42, 34, 26, 18, 10, 2, 60,\
                       52, 44, 36, 28, 20, 12, 4, 62, 54,\
                       46, 38, 30, 22, 14, 6, 64, 56, 48,\
                       40, 32, 24, 16, 8, 57, 49, 41, 33,\
                       25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,\
                       61, 53, 45, 37, 29, 21, 13, 5,\
                       63, 55, 47, 39, 31, 23, 15, 7}, i;
    int64 output;
    output.data = 0;
    for(i=0;i<64;i++)
    {
        if(input.data & (1<<(64-permutation[i])))
            output.data |= 1<<(63-i);
    }
    return output;
}

void print_binary(int64 input)
{
    int i;
    for(i=0;i<64;i++)
        if(input.data & (1<<(63-i)))
            printf("1");
        else
            printf("0");
    printf("\n");
}

int main()
{
    int64 test;
    test.data = 8;
    print_binary(test);
    print_binary(IP(test));
    return 0;
}
