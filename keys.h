/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Zaglavlje za rad sa kljucevima i njihovu obradu.
*/

#ifndef _KEYS_H
#define _KEYS_H

#include "list.h"

/**
* @brief Naziv fajla u kome se cuvaju kljucevi.
*/
#define KEYS_FILE "keys.txt"

/**
* @brief Karakter koji razdvaja informacije o kljucu unutar fajla.
* Koristi se prilikom ispisivanja kljuceva u fajl.
*/
#define SEPARATOR ':'
/**
* @brief Karakteri koji razdvajaju informacije o kljucu unutar fajla. Koristi se
* prilikom poziva strtok funkcije.
*/
#define SEPARATOR_STRING ":\n"

/**
* @brief Maksimalna duzina stringa u koji ce biti ucitan red iz fajla.
*/
#define MAX_KEY_ROW_LEN 256
/**
* @brief Maksimalna duzina naziva kljuca.
*/
#define MAX_KEY_NAME_LEN 15
/**
* @brief Format za ispisivanje kljuceva za Triple DES.
*/
#define KEY_PRINT_FORMAT_TDES "%-6s %-6s %-15s %-10s %-10s %-10s"
/**
* @brief Format za ispisivanje kljuceva koji nisu za Triple DES.
*/
#define KEY_PRINT_FORMAT_OTHERS "%-6s %-6s %-15s %-32s"

/**
* @brief Duzina kljuca za AES16 u bajtovima.
*/
#define AES16_KEY_LEN 16
/**
* @brief Duzina kljuca za AES24 u bajtovima.
*/
#define AES24_KEY_LEN 24
/**
* @brief Duzina kljuca za AES32 u bajtovima.
*/
#define AES32_KEY_LEN 32
/**
* @brief Duzina kljuca za DES i Triple DES u bajtovima.
*/
#define DES_KEY_LEN 7

/**
* @brief Jedinstveni string koji predstavlja AES16 tip enkripcije/dekripcije.
*/
#define AES16_STR "aes16"
/**
* @brief Jedinstveni string koji predstavlja AES24 tip enkripcije/dekripcije.
*/
#define AES24_STR "aes24"
/**
* @brief Jedinstveni string koji predstavlja AES32 tip enkripcije/dekripcije.
*/
#define AES32_STR "aes32"
/**
* @brief Jedinstveni string koji predstavlja DES tip enkripcije/dekripcije.
*/
#define DES_STR "des"
/**
* @brief Jedinstveni string koji predstavlja Triple DES tip enkripcije/dekripcije.
*/
#define TDES_STR "3des"

/**
* @brief Jedinstveni string koji predstavlja ECB mod enkripcije/dekripcije.
*/
#define MODE_ECB_STR "ecb"
/**
* @brief Jedinstveni string koji predstavlja CBC mod enkripcije/dekripcije.
*/
#define MODE_CBC_STR "cbc"

/**
* @brief Struktura jednog kljuca. Sadrzi polja koja odredjuju tip i mod enkripcije/dekripcije,
* naziv kljuca i same kljuceve predstavljene kao niz karaktera. U svakom trenutku se cuvaju
* tri kljuca nezavisno od tipa enkripcije/dekripcije, ali su drugi i treci validni samo ukoliko
* je za tip enkripcije/dekripcije izabran Triple DES.
*/
typedef struct Key {
    char type[10];
    char mode[10];
    char key_name[20];
    unsigned char key[3][35];
} Key;

/**
* @brief Funkcija za citanje kljuceva iz fajla u zadatu listu. Lista ce prvo biti obrisana, a potom u nju ucitani novi elementi.
* @param[in] filename Naziv fajla iz kog treba ucitati kljuceve
* @param[in] list Dvostruki pokazivac na listu u koju treba ucitati kljuceve, kako bi lista mogla da bude izmenjena
* @return 0 pri uspesnom citanju, 1 u suprotnom
*/
int read_keys(char *filename, List **list);

/**
* @brief Funkcija za ispisivanje kljuceva iz liste u zadati fajl.
* @param[in] list Pokazivac na listu kljuceva koju treba ispisati
* @param[in] filename Naziv fajla u koji treba upisati kljuceve.
* @return 0 pri uspesnom ispisivanju, 1 u suprotnom
*/
int write_keys(List *list, char *filename);

/**
* @brief Funkcija za dodavanje kljuca u listu.
* @param[in] list Pokazivac na listu u koju treba dodati kljuc
* @param[in] key Pokazivac na kljuc koji treba dodati
*/
void add_key(List *list, Key *key);

/**
* @brief Funkcija za uklanjanje kljuca iz liste.
* @param[in] list Pokazivac na listu iz koje treba ukloniti kljuc
* @param[in] key Pokazivac na kljuc koji treba ukloniti
*/
void remove_key(List *list, Key *key);

/**
* @brief Funkcija za uklanjanje svih kljuceva iz liste.
* @param[in] list Pokazivac na listu iz koje treba ukloniti kljuceve
*/
void remove_keys(List *list);

/**
* @brief Funkcija koja pronalazi kljuc u listi sa zadatim imenom.
* @param[in] list Pokazivac na listu u kojoj treba naci kljuc
* @param[in] key_name Ime kljuca koji treba naci
* @return Pokazivac na kljuc sa zadatim imenom, NULL ukoliko kljuc ne postoji u listi
*/
Key* find_key_with_name(List *list, char *key_name);

/**
* @brief Funkcija koja proverava da li je kljuc ispravan.
* @param[in] key Pokazivac na kljuc koji treba proveriti
* @param[out] error_msg String u koji ce biti upisana poruka o gresci ukoliko kljuc nije ispravan
* @return 0 ukoliko je kljuc ispravan, 1 u suprotnom
*/
int check_correct_key(Key *key, char *error_msg);

#endif // _KEYS_H
