/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Zaglavlje sa deklaracijama funkcija koje se pozivaju prilikom enkripcije ili dekripcije fajlova i
* konstantama koje se koriste u toku rada.
*/

#ifndef _PROCESS_H
#define _PROCESS_H

#include "keys.h"
#include <stdio.h>

/**
* @brief Pomocni fajl u koji se ispisuju informacije potrebne za izvrsavanje naredbe
* ukoliko je izabrana opcija enkriptovanja/dekriptovanja preko regularnih izraza.
*/
#define REGEX_TMP_FILE "regex_tmp.txt"

/**
* @brief Regularni izraz koji predstavlja nula ili vise pojavljivanja jednog karaktera.
*/
#define MULTICHAR_REGEX "[a-zA-Z0-9]*"
/**
* @brief Duzina string MULTICHAR_REGEX
*/
#define MULTICHAR_REGEX_LEN 12
/**
* @brief Regularni izraz koji predstavlja tacno jedno pojavljivanje nekog karaktera.
*/
#define SINGLECHAR_REGEX "[a-zA-Z0-9]"
/**
* @brief Duzina string SINGLECHAR_REGEX
*/
#define SINGLECHAR_REGEX_LEN 11

/**
* @brief Funkcija za enkripciju jednog fajla zadatim kljucem.
* @param[in] file_path Putanja do fajla koji treba enkriptovati
* @param[in] key Pokazivac na kljuc koji treba koristiti prilikom enkripcije
* @param[out] error_msg String u koji ce biti upisana poruka o gresci ukoliko enkripcija nije uspesna
* @return 0 ako je enkripcija uspesna, broj razlicit od 0 u suprotnom
*/
int encrypt_file(char *file_path, Key *key, char *error_msg);

/**
* @brief Funkcija za dekripciju jednog fajla zadatim kljucem.
* @param[in] file_path Putanja do fajla koji treba dekriptovati
* @param[in] key Pokazivac na kljuc koji treba koristiti prilikom dekripcije
* @param[out] error_msg String u koji ce biti upisana poruka o gresci ukoliko dekripcija nije uspesna
* @return 0 ako je dekripcija uspesna, broj razlicit od 0 u suprotnom
*/
int decrypt_file(char *file_path, Key *key, char *error_msg);

/**
* @brief Funkcija za enkripciju vise fajlova zadatim kljucem.
* @param[in] file_path Putanja do fajla u kome se nalaze nazivi fajlova koje treba enkriptovati
* @param[in] key Pokazivac na kljuc koji treba koristiti prilikom enkripcije
* @param[out] log Pokazivac na fajl u koji treba ispisivati poruke o ishodu enkripcije svakog fajla
* @return 0 ako je moguce otvoriti zadati fajl (file_path), 1 u suprotnom
*/
int encrypt_more_files(char *file_path, Key *key, FILE *log);

/**
* @brief Funkcija za dekripciju vise fajlova zadatim kljucem.
* @param[in] file_path Putanja do fajla u kome se nalaze nazivi fajlova koje treba dekriptovati
* @param[in] key Pokazivac na kljuc koji treba koristiti prilikom dekripcije
* @param[out] log Pokazivac na fajl u koji treba ispisivati poruke o ishodu dekripcije svakog fajla
* @return 0 ako je moguce otvoriti zadati fajl (file_path), 1 u suprotnom
*/
int decrypt_more_files(char *file_path, Key *key, FILE *log);

/**
* @brief Funkcija za enkripciju vise fajlova zadatim kljucem.
* @param[in] file_path Regularni izraz kojem treba da odgovaraju fajlovi koje treba enkriptovati
* @param[in] key Pokazivac na kljuc koji treba koristiti prilikom enkripcije
* @param[out] error_msg String u koji ce biti upisana poruka o gresci
* @param[out] log Pokazivac na fajl u koji treba ispisivati poruke o ishodima enkripcije svakog fajla
* @return 0 ako nije doslo do greske, 1 u suprotnom
*/
int encrypt_regex_files(char *file_path, Key *key, char *error_msg, FILE *log);

/**
* @brief Funkcija za dekripciju vise fajlova zadatim kljucem.
* @param[in] file_path Regularni izraz kojem treba da odgovaraju fajlovi koje treba dekriptovati
* @param[in] key Pokazivac na kljuc koji treba koristiti prilikom dekripcije
* @param[out] error_msg String u koji ce biti upisana poruka o gresci
* @param[out] log Pokazivac na fajl u koji treba ispisivati poruke o ishodima dekripcije svakog fajla
* @return 0 ako nije doslo do greske, 1 u suprotnom
*/
int decrypt_regex_files(char *file_path, Key *key, char *error_msg, FILE *log);

#endif // _PROCESS_H
