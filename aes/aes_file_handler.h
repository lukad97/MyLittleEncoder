/**
 * @file
 * @author  Kosta Bizetic
 * @brief   AES enkripcija fajlova
 * @details Ovaj fajl sadrzi prototipe funkcija za enkripciju fajla AES algoritmom.
            Podrzani modovi su ECB i CBC.
 */

#ifndef _AES_FILE_HANDLER_
#define _AES_FILE_HANDLER_R

#include <stdint.h>
#include "../global.h"

typedef enum {ECB, CBC} modeOfOperation;

/**
 * @brief     Funkcija za enkripciju fajlova AES algoritmom
 * @param[in] filePath  Put do fajla
 * @param[in] key       Kluc za enkripciju
 * @param[in] Nk        Broj 4-bitnih reci u kljucu [4, 6 ili 8]
 * @param[in] mode      Zeljeni mod enkripcije [ECB ili CBC]
 * @details   Funkcija enkriptuje fajl zadat putem filePath i napravi novi enkriptovani fajl sa dodatom .dat ekstenzijom.
 * @return    Prilikom korektne enkripcije vraca nulu.
 *            U slucaju greske vraca jedan od signala definisanih u global.h
 */
int aesEncryptFile(char *filePath, uc* key, int Nk, modeOfOperation mode);

/**
 * @brief     Funkcija za dekripciju fajlova AES algoritmom
 * @param[in] filePath  Put do fajla
 * @param[in] key       Kluc za dekripciju
 * @param[in] Nk        Broj 4-bitnih reci u kljucu [4, 6 ili 8]
 * @param[in] mode      Zeljeni mod dekripcije [ECB ili CBC]
 * @details   Funkcija dekriptuje fajl zadat putem filePath i pokusa da napravi fajl u istom direktorijumu sa orginalnim imenom.
              Ukoliko fajl sa originalnim imenom postoji pravi se novi fajl sa slucajnim brojem dodatim na pocetak imena.
 * @return    Prilikom korektne dekripcije vraca nulu.
 *            U slucaju greske vraca jedan od signala definisanih u global.h
 */
int aesDecryptFile(char *filePath, uc* key, int Nk, modeOfOperation mode);

#endif // _AES_FILE_HANDLER_R
