/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Zaglavlje za obradu argumenata ukoliko se program pokrece iz komandne linije.
*/

#ifndef _CMD_LINE_H
#define _CMD_LINE_H

#include "list.h"

/**
* @brief String koji sadrzi znakove koji razdvajaju argumente. Potreban za koriscenje u funkciji strtok.
*/
#define ARGS_SEPARATORS " \n"
/**
* @brief String koji se ispisuje u opstem slucaju ukoliko je uneta nepravilna komanda.
*/
#define INVALID_COMMAND_STR "Invalid command! Type command with [- or /]h for help!\n"
/**
* @brief Maksimalna duzina stringa u okviru programa.
*/
#define MAX_STR_LEN 512

/**
* @brief Funkcija koja obradjuje argumente komandne linije i poziva odgovarajucu funkciju za dalju obradu.
* @param[in] argc Broj argumenata komandne linije
* @param[in] argv Argumenti komandne linije
* @param[in] key_list Pokazivac na listu trenutno ucitanih kljuceva
*/
void process_command(int argc, char *argv[], List *key_list);

/**
* @brief Makro za ispisivanje u fajl ukoliko fajl postoji.
*/
#define print_log(file, args...) if(file) fprintf(file, args)

#endif // _BATCH_H
