#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define ALLOC_CHECK(p)  if (!(p)) exit(1)
#define FILE_CHECK(p)   if (!(p)) return(2)

#define ALLOC_ERR    1
#define FILE_ERR     2
#define CRC_MISMATCH 3
#define UNKNOWN_ALG  4

typedef unsigned char uc;

#define TEST_DEF
#undef TEST_DEF

#endif // _GLOBAL_H_
