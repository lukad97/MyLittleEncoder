/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Zaglavlje za rad sa dvostruko povezanom ulancanom listom.
*/

#ifndef _LIST_H
#define _LIST_H

/**
* @brief Struktura jednog elementa liste. Sadrzi pokazivace na sledeci
* i prethodni element, kao i void pokazivac na podatak koji se cuva u
* elementu, kako bi lista mogla da se koristi za cuvanje razlicitih podataka.
*/
typedef struct ListElement {
    struct ListElement *next, *prev;
    void *info;
} ListElement;

/**
* @brief Struktura liste. Sadrzi pokazivac na prvi i poslednji element liste i
* broj elemenata (duzinu) liste.
*/
typedef struct List {
    int length;
    ListElement *head, *tail;
} List;


/**
* @brief Funkcija za inicijalizaciju nove liste.
* @return Pokazivac na strukturu List koja predstavlja trenutno inicijalizovanu listu
*/
List* init_list();

/**
* @brief Funkcija za dodavanje novog elementa na kraj liste.
* @param[in] list Pokazivac na listu u koju treba dodati element
* @param[in] info Pokazivac na element koji se dodaje u listu
*/
void tail_add(List *list, void *info);

/**
* @brief Funkcija koja uklanja zadati element iz liste.
* @param[in] list Pokazivac na listu iz koje treba ukloniti element
* @param[in] curr Pokazivac na element koji treba ukloniti
*/
void remove_element(List *list, ListElement *curr);

/**
* @brief Funkcija koja uklanja sve elemente liste.
* @param[in] list Pokazivac na listu koju treba ukloniti
*/
void remove_list(List *list);

/**
* @brief Funkcija za pronalazenje k-tog pretka elementa liste.
* @param[in] elem Pokazivac na element liste za kojeg treba naci pretka
* @param[in] k Predstavlja kog pretka zadatog elementa treba naci
*/
ListElement* find_kth_prev(ListElement *elem, int k);

#endif // _LIST_H
