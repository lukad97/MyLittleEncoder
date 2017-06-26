/**
 * @file
 * @author  Kosta Bizetic
 * @author  Luka Dojcilovic
 * @brief   Enkripcija/dekripcija fajlova
 * @details Ovaj fajl sadrzi prototipe funkcija koje objedinjuju sve algoritme za enkripciju i dekripciju fajlova.
            Koristi se tako sto funkcijama prosledi enum Algorithm zeljenog algortima.
 */

#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

#include "global.h"

typedef enum {des_ecb, des_cbc, tdes_ecb, tdes_cbc,
              aes128_ecb, aes128_cbc, aes192_ecb, aes192_cbc,
              aes256_ecb, aes256_cbc} Algorithm;


/**
 * @brief     Funkcija za enkripciju fajlova
 * @param[in] filePath  Put do fajla
 * @param[in] key1      Kluc za sve algoritme
 * @param[in] key2      Drugi kljuc u slucaju Triple-DES algoritma
 * @param[in] key3      Treci kljuc u slucaju Triple-DES algoritma
 * @param[in] mode      Flag zeljenog algorima
 * @details   Funkcija enkriptuje fajl zadat putem filePath i napravi novi enkriptovani fajl sa dodatom .dat ekstenzijom.
 * @return    Prilikom korektne enkripcije vraca nulu.
 *            U slucaju greske vraca jedan od signala definisanih u global.h
 *
 */
int encryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);

/**
 * @brief     Funkcija za dekripciju fajlova
 * @param[in] filePath  Put do fajla
 * @param[in] key1      Kluc za sve algoritme
 * @param[in] key2      Drugi kljuc u slucaju Triple-DES algoritma
 * @param[in] key3      Treci kljuc u slucaju Triple-DES algoritma
 * @param[in] mode      Flag zeljenog algorima
 * @details   Funkcija dekriptuje fajl zadat putem filePath i pokusa da napravi fajl u istom direktorijumu sa orginalnim imenom.
              Ukoliko fajl sa originalnim imenom postoji pravi se novi fajl sa slucajnim brojem dodatim na pocetak imena.
 * @return    Prilikom korektne dekripcije vraca nulu.
 *            U slucaju greske vraca jedan od signala definisanih u global.h
 */
int decryptFile(char *filePath, uc* key1, uc* key2, uc* key3, Algorithm mode);

#endif // _ENCRYPTION_H_
