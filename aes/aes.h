/**
 * @file
 * @author Kosta Bizetic
 * @brief AES enkripcija bloka
 * @details Ovaj fajl sadrzi prototipe funkcija za enkripciju bloka AES algoritmom.
 */

#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include "../global.h"

#define Nb 4            /**< Broj 4-bitnih reci u AES bloku */
#define BLOCK_SIZE 16   /**< Broj bajtova u jednom AES bloku */

/** @typedef
 *  @brief Mod operacije getRoundKeys() funkcije
 */
typedef enum {REGULAR, INVERSE} keyExpandMode;

/**
 * @brief      Funkcija za generisanje kljuceva za enkripciju/dekripciju
 * @param[in]  key          Pokazivac na kljuc
 * @param[out] roundKeys    Matrica u koju ce rezultujuci kljucevi biti upisani
 * @param[in]  Nk           Broj 4-bitnih reci u kljucu [4, 6 ili 8]
 * @param[in]  inverse      Da li se radi ekspanzija za enkriptovanje ili dekriptovanje
 * @details    Ova funkcija prvo prosiri pocetni kljuc, a zatim ga prepakuje u format pogodan za enkripciju/dekripciju.
 *             U slucaju dekripcije poziva se i dodatna funkcija invMixColumn() koja ubrzava proces dekripcije vise blokova istim kljucem.
 */
void getRoundKeys(uc *key, uc roundKeys[][BLOCK_SIZE], int Nk, keyExpandMode inverse);

/**
 * @brief Funkcija za enkriptovanje bloka
 * @param[in] state Pokazivac na blok koji se enkriptuje
 * @param[in] key Kljuc za ekspanziju
 * @param[in] Nk Broj 4-bitnih reci u kljucu [4, 6 ili 8]
 * @details Funkcija prosiri i prepakuje kljuc key, a zatim enkriptuje blok state.
 */
void encryptBlock(uc *state, uc *key, int Nk);

/**
 * @brief Funkcija za enkriptovanje bloka
 * @param[in] state Pokazivac na blok koji se enkriptuje
 * @param[in] roundKeys Matrica kljuceva za enkripciju
 * @param[in] Nr Broj rundi enkripcije [10, 12 ili 14]
 * @details Funkcija enkriptuje blok state sa vec prepakovanim klucevima roundKeys[][].
            Ova funkcija je korisna kada se istim klucem enkriptuje vise blokova zaredom prilikom
            enkripcije fajla.
 */
void encryptBlockRoundKeys(uc *state, uc roundKeys[][BLOCK_SIZE], int Nr);

/**
 * @brief Funkcija za dekriptovanje bloka
 * @param[in] state Pokazivac na blok koji se dekriptuje
 * @param[in] key Kljuc za ekspanziju
 * @param[in] Nk Broj 4-bitnih reci u kljucu [4, 6 ili 8]
 * @details Funkcija prosiri i prepakuje kljuc key, a zatim dekriptuje blok state.
 */
void decryptBlock(uc *state, uc *key, int Nk);

/**
 * @brief Funkcija za dekriptovanje bloka
 * @param[in] state Pokazivac na blok koji se dekriptuje
 * @param[in] invRoundKeys Matrica kljuceva za dekripciju
 * @param[in] Nr Broj rundi enkripcije [10, 12 ili 14]
 * @details Funkcija dekriptuje blok state sa vec prepakovanim klucevima roundKeys[][].
            Ova funkcija je korisna kada se istim klucem dekriptuje vise blokova zaredom prilikom
            dekripcije fajla.
 */
void decryptBlockRoundKeys(uc *state, uc invRoundKeys[][BLOCK_SIZE], int Nr);


#endif // _AES_H_
