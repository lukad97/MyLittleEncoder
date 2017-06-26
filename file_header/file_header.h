/**
* @file
* @author Luka Dojcilovic (l.dojcilovic@gmail.com)
* @author Kosta Bizetic (kosta.bizetic@gmail.com)
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Funkcije za rad sa sa hederima fajlova.
*/

#ifndef _FILE_HEADER_H_
#define _FILE_HEADER_H_

#include <stdint.h>
#include <stdlib.h>
#include "../global.h"

/**
* @brief Maksimalna duzina imena fajla.
*/
#define FILENAME_LEN_MAX 256

/**
* @brief Struktura hedera fajla, sadrzi ime fajla, njegovu duzinu u 
* bajtovima, njegov CRC-32 i IV koji je neophodan za CBC mod.
*/
typedef struct
{
	int8_t fileName[FILENAME_LEN_MAX];
	uint64_t byteLength;
	uint32_t crc, pad;
	uint8_t IV[16];
} fileheader_t;

/**
* @brief Funkcija za kreiranje hedera fajla, IV je random generisan.
* @param[in] file File kome se kreira heder.
* @param[in] fileName Ime fajla kome se kreira heder.
* return Heder datog fajla.
*/
fileheader_t headerCreate(FILE *file, const char *fileName);

/**
* @brief Funkcija koja provera da li je CRC dekriptovanog
* fajla jedank CRC-u koji mu je bio zapisan u hederu.
* @param[in] file Dekriptovani fajl.
* @param[in] header Heder koji se nalazio u datom fajlu.
* return 1 ako su CRC-ovi razliciti, 0 ako su isti.
*/
int          headerCheck(FILE *file, fileheader_t *header);

/**
* @private
*/
void         headerPrint(fileheader_t *header);

/**
* @brief Funkcija koja iz path-a fajla nalazi njegovo ime.
* @param[in] file_path Path fajla.
* @return Pokazivac na string koji predstavlja ime fajla.
*/
char*        get_filename_from_path(char *file_path);

#endif // _FILE_HEADER_H_
