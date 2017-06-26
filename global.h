/**
* @file
* @author Luka Dojcilovic (l.dojcilovic@gmail.com)
* @author Kosta Bizetic (kosta.bizetic@gmail.com)
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Konstante koriscene u celoj aplikaciji.
*/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/**
* @brief Marko za proveru uspesnosti alokacije memorije.
*/
#define ALLOC_CHECK(p)  if (!(p)) exit(1)

/**
* @brief Makro za proveru uspesnoti otvaranja fajla.
*/
#define FILE_CHECK(p)   if (!(p)) return(2)


/**
* @brief Greska pri alokaciji memorije.
*/
#define ALLOC_ERR    1

/**
* @brief Greska pri otvaranju fajla.
*/
#define FILE_ERR     2

/**
* @brief Ne slaze se CRC dekriptovanog fajla sa CRC-om u njegovom hederu.
*/
#define CRC_MISMATCH 3

/**
* @brief Nepoznat alkoritam koriscen za enkripciju/dekripciju.
*/
#define UNKNOWN_ALG  4

typedef unsigned char uc;

#endif // _GLOBAL_H_
