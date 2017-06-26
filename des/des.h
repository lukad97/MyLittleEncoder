/**
* @file
* @author Luka Dojcilovic (l.dojcilovic@gmail.com)
* @brief Funkcije za enkripciju i dekripciju DES i tDES algoritmima.
*/

#ifndef _DES_H_
#define _DES_H_

#include "../global.h"

/**
* @brief Funkcija za enkripciju fajla DES algoritmom u ECB modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key 58-bitni kljuc za dekripciju.
* @return Greske
*/
int desEncryptFileCBC(char *name, uc* key);

/**
* @brief Funkcija za dekripciju fajla DES algoritmom u ECB modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key 58-bitni kljuc za dekripciju.
* @return Greske
*/
int desDecryptFileECB(char *name, uc* key);

/**
* @brief Funkcija za dekripciju fajla DES algoritmom u CBC modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key 58-bitni kljuc za dekripciju.
* @return Greske
*/
int desEncryptFileECB(char *name, uc* key);

/**
* @brief Funkcija za enkripciju fajla DES algoritmom u CBC modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key 58-bitni kljuc za dekripciju.
* @return Greske
*/
int desDecryptFileCBC(char *name, uc* key);


/**
* @brief Funkcija za enkripciju fajla tDES algoritmom u ECB modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @return Greske
*/
int tdesEncryptFileECB(char *name, uc* key1, uc* key2, uc* key3);

/**
* @brief Funkcija za dekripciju fajla tDES algoritmom u ECB modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @return Greske
*/
int tdesDecryptFileECB(char *name, uc* key1, uc* key2, uc* key3);

/**
* @brief Funkcija za enkripciju fajla tDES algoritmom u CBC modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @return Greske
*/
int tdesEncryptFileCBC(char *name, uc* key1, uc* key2, uc* key3);

/**
* @brief Funkcija za dekripciju fajla tDES algoritmom u CBC modu.
* @param[in] name Path fajla koji treba dekriptovati.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @param[in] key1 58-bitni kljuc za dekripciju.
* @return Greske
*/
int tdesDecryptFileCBC(char *name, uc* key1, uc* key2, uc* key3);

#endif // _DES_H_

