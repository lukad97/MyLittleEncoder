/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Zaglavlje za rad sa vizuelnim okruzenjem.
*/

#ifndef _GUI_H
#define _GUI_H

#include "list.h"
#include "keys.h"
#include <curses.h>
#include <dirent.h>

/**
* @brief Maska koja iz chtype izvlaci atribute koji ne odgovaraju boji.
*/
#define A_ATTR (A_ATTRIBUTES ^ A_COLOR)
/**
* @brief Vrednost tastera ESC.
*/
#define KEY_ESC 0x1b

/**
* @brief Maksimalna duzina stringa u okviru programa.
*/
#define MAX_STR_LEN 512

/**
* @brief Typedef pokazivaca na void funkciju bez parametara.
*/
typedef void (*Func)();

/**
* @brief Struktura jedne opcije menija ili podmenija. Sadrzi ime opcije,
* opis opcije koji se ispisuje u statusnom delu prozora i pokazivac na
* funkciju koja se poziva ukoliko je opcija izabrana.
*/
typedef struct Menu {
    char *name;
    Func func;
    char *description;
} Menu;

/**
* @brief Funkcija koja se poziva da bi se zavrsio rad vizuelnog okruzenja.
* Postavlja internu promenljivu quit na TRUE.
*/
void finish();
/**
* @brief Funkcija koja zapocinje rad sa vizuelnim okruzenjem.
* @param[in] mp Struktura glavnog menija.
* @param[in] title Naslov programa.
*/
void start_menu(Menu *mp, char *title);
/**
* @brief Funkcija koja rukovodi radom podmenija.
* @param[in] mp Struktura podmenija.
*/
void do_menu(Menu *mp);

/**
* @brief Funkcija koja postavlja naslov programa.
* @param[in] title Naslov programa.
*/
void title_message(char *title);
/**
* @brief Funkcija koja postavlja statusnu poruku.
* @param[in] message Poruka.
*/
void status_message(char *message);
/**
* @brief Funkcija koja postavlja poruku o gresci, ali se moze koristiti i za bilo kakve poruke.
* @param[in] message Poruka.
* @param[in] sound_flag Ukoliko je poruka koju treba prikazati greska, sound_flag moze da se postavi na
* vrednost razlicitu od nule da bi se emitovao ton upozorenja.
*/
void error_message(char *message, int sound_flag);
/**
* @brief Funkcija koja uklanja statusnu poruku.
*/
void remove_status_message();
/**
* @brief Funkcija koja uklanja poruku o gresci.
*/
void remove_error_message();

/**
* @brief Funkcija koja cisti glavni deo programa i resetuje kursor na pocetnu poziciju.
*/
void clear_body();

/**
* @brief Funkcija koja odredjuje da li je u trenutku poziva pritisnut neki taster.
* @return TRUE ako je taster pritisnut, FALSE u suprotnom.
*/
bool key_pressed();
/**
* @brief Funkcija koja vraca vrednost poslednjeg pritisnutog karaktera.
* @return int vrednost poslednjeg pritisnutog tastera.
*/
int get_key();
/**
* @brief Funkcija koja ceka da se pritisne neki taster, a nakon sto je taster pritisnut vraca njegovu vrednost.
* @return int vrednost pritisnutog tastera.
*/
int wait_for_key();

/**
* @brief Funkcija koja kreira prozor u okviru datog prozora koji ce biti koriscen za unos informacija.
* Prozor se kreira na na mestu na kome se trenutno nalazi kursor datog prozora.
* @param[in] win Prozor u okviru kog treba kreirati novi prozor.
* @param[in] n_lines Broj linija novog prozora.
* @param[in] n_cols Broj kolona novog prozora.
* @return Pokazivac na novokreirani prozor.
*/
WINDOW *winputbox(WINDOW *win, int n_lines, int n_cols);
/**
* @brief Funkcija koja rukovodi unosom podataka u jedno polje u okviru datog prozora. Na osnovu trenutne pozicije
* kursora u datom prozoru pravi se potprozor u kome ce biti omogucen unos.
* @param[in] win Prozor u okviru kog treba realizovati unos.
* @param[out] buf U okviru ovog stringa bice vracena uneta vrednost. Na pocetku u buf treba da bude upisana trenutna
* vrednost stringa i ona ce biti inicijalno prikazana. Ukoliko ne treba prikazati inicijalnu vrednost postaviti buf[0] = '\0'.
* @param[in] field Velicina polja za unos
* @return int vrednost poslednjeg pritisnutog tastera.
*/
int weditstr(WINDOW *win, char *buf, int field);
/**
* @brief Funkcija koja ispisuje trenutni sadrzaj bafera u dati prozor dimenzije (1xN) i postavlja poziciju kursora na datu vrednost.
* @param[in] win Prozor u kome treba ispisati vrednost bafera.
* @param[in] x Pozicija na koju treba postaviti kursor.
* @param[in] buf String koji treba ispisati.
*/
void repaint_edit_box(WINDOW *win, int x, char *buf);
/**
* @brief Funkcija koja rukovodi unosom podataka u sva zadata polja.
* @param[in] description Niz naziva polja za podatke koje treba uneti. Poslednji pokazivac mora da bude NULL.
* @param[in] buf Trenutne vrednosti polja za unos. Broj trenutnih vrednosti mora da se poklapa sa brojem naziva polja.
* @param[in] field Niz velicina polja za unos za svako polje pojedinacno.
* @return int vrednost poslednjeg pritisnutog karaktera. Ukoliko je to '\n' znaci da je unos zavrsen, a ako je KEY_ESC
* znaci da je korisnik prekinuo unos.
*/
int get_input(char *description[], char *buf[], int *field);

/**
* @brief Makro koji pokusava da pomeri kursor u okviru prozora w. Ukoliko
* je pomeranje uspesno, u okviru prozora w bice napravljen prozor inputbox,
* u suprotnom prozor se ne menja.
*/
#define mvwinputbox(w,y,x,l,c) (wmove(w,y,x)==ERR?w:winputbox(w,l,c))
/**
* @brief Makro koji pokusava da pomeri kursor u okviru prozora w. Ukoliko
* je pomeranje uspesno, u okviru prozora w bice napravljen potrprozor za
* unos i funkcija weditstr ce rukovati unosom, u suprotnom bice vracena greska.
*/
#define mvweditstr(w,y,x,s,f) (wmove(w,y,x)==ERR?ERR:weditstr(w,s,f))

/**
* @brief Funkcija koja rukovodi odabirom jednog kljuca iz liste kljuceva.
* @param[in] list Pokazivac na listu koja sadrzi kljuceve.
* @return Pokazivac na izabrani kljuc ili NULL ukoliko je biranje prekinuto. Ukoliko je biranje prekinuto
* ispisuje se poruka o gresci.
*/
Key* select_key(List *list);
/**
* @brief Funkcija koja rukovodi unosom podataka o putanji fajla koji se bira.
* @param[in] win Prozor u okviru kog treba realizovati unos.
* @param[out] path U okviru ovog stringa bice vracena uneta vrednost. Na pocetku u path treba da bude upisana trenutna
* vrednost stringa i ona ce biti inicijalno prikazana. Ukoliko ne treba prikazati inicijalnu vrednost postaviti path[0] = '\0'.
* @param[in] field Velicina polja za unos
* @return int vrednost poslednjeg pritisnutog tastera.
*/
int weditpath(WINDOW *win, char *path, int field);
/**
* @brief Funkcija koja rukovodi radom file explorer-a.
* @param[in] win Prozor u okviru kog treba iscrtavati explorer.
* @param[in] dir Dvostruki pokazivac na trenutni direktorijum, da bi mogao da se menja ukoliko je neophodno.
* @param[in] dir_entries Pokazivac na broj pojmova u direktorijumu, da bi mogao da se menja ukoliko je neophodno.
* @param[in] n_lines Broj linija koje su vidljive u fajl eksploreru.
* @param[in] curr_selection Pokazivac na trenutno izabrani pojam direktorijuma, da bi mogao da se menja.
* @param[in] old_selection Pokazivac na prethodno izabrani pojam direktorijuma, da bi mogao da se menja.
* @param[in] rewrite_old_flag Pokazivac na flag koji govori da li treba ponovo iscrtavati prethodno izabrani pojam direktorijuma.
* @param[out] curr_path String koji sadrzi trenutnu putanju direktorijuma. Ukoliko se izabere neki fajl, u ovom stringu
* ce se nalaziti njegova putanja.
*/
void file_explorer(WINDOW *win, DIR **dir, int *dir_entries, int n_lines, int *curr_selection, int *old_selection, int *rewrite_old_flag, char *curr_path);
/**
* @brief Funkcija koja rukovodi odabirom putanje fajla.
* @param[out] path String u koji ce biti upisana odabrana putanja. Ako treba prikazati neku pocetnu vrednost, onda
* onda treba da bude upisana u ovaj string, u suprotnom string treba da bude prazan.
* @return int vrednost poslednjeg pritisnutog tastera. Ukoliko je to KEY_ESC znaci da je korisnik prekinuo unos putanje,
* u suprotnom je to '\n' i znaci da je putanja uneta.
*/
int get_filepath(char *path);

/**
* @brief Makro koji pokusava da pomeri kursor u okviru prozora w. Ukoliko
* je pomeranje uspesno, u okviru prozora w bice napravljen potrprozor za
* unos i funkcija weditpath ce rukovati unosom, u suprotnom bice vracena greska.
*/
#define mvweditpath(w,y,x,s,f) (wmove(w,y,x)==ERR?ERR:weditpath(w,s,f))

/**
* @brief Funkcija koju treba pozivati u toku enkripcije/dekripcije da se postavi poruka o
* kolicini trenutno odradjenog posla.
* @param[in] progress Kolicina trenutno odradjenog posla u intervalu [0, 1].
*/
void set_progress(double progress);

#endif // _GUI_H
