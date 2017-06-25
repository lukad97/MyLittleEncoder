/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Funkcije za obradu argumenata ukoliko se program pokrece iz komandne linije i izvrsavanje
* zadatog algoritma enkripcije/dekripcije.
*/

#include "cmd_line.h"
#include "process.h"
#include "list.h"
#include "keys.h"
#include <stdio.h>
#include <string.h>

/*********************** INTERNAL FUNCTIONS ***********************/
/**
* @brief Funkcija za ispisivanje kljuceva na standardnom izlazu.
* @param[in] key_list Pokazivac na listu trenutno ucitanih kljuceva
*/
static void print_keys(List *key_list) {
    ListElement *curr;
    Key *key;

    for (curr = key_list->head; curr; curr = curr->next) {
        key = (Key*)curr->info;
        if (!strcmp(key->type, TDES_STR))
            printf(KEY_PRINT_FORMAT_TDES, key->type, key->mode, key->key_name, (key->key)[0], (key->key)[1], (key->key)[2]);
        else
            printf(KEY_PRINT_FORMAT_OTHERS, key->type, key->mode, key->key_name, (key->key)[0]);
        printf("\n");
    }
}

/**
* @brief Funkcija za ispis pomocne poruke o argumentima komandne linije.
* Argumenti se ispisuju na standardnom izlazu.
*/
static void print_help() {
    printf("encrypt(.exe) -[e/d[m/r]] key_name file_path\n");
    printf("encrypt(.exe) -b file_path\n");
    printf("encrypt(.exe) -l file_path\n");
}

/**
* @brief Funkcija za generisanje argumenata iz zadatog stringa.
* @param[in] line String koji sadrzi komandu koja odgovara formatu za poziv programa iz komandne linije
* @param[out] argc Pokazivac na broj u koji treba upisati broj argumenata komandne linije
* @param[out] argv Niz stringova u koji treba upisati argumente komandne linije (mora imati najmanje 4 stringa)
* @return 0 ukoliko je broj argumenata manji od 5, 1 u suprotnom (zato sto sve ispravne komande nemaju vise od 4 argumenta)
*/
static int parse_line(char *line, int *argc, char *argv[]) {
    char *pntr = strtok(line, ARGS_SEPARATORS);

    *argc = 0;
    while (pntr) {
        if (*argc == 4)
            return 1;
        strcpy(argv[(*argc)++], pntr);
        pntr = strtok(NULL, ARGS_SEPARATORS);
    }

    return 0;
}

/**
* @brief Funkcija za obradu komande za enkripciju/dekripciju fajlova.
* @param[in] argc Broj argumenata komandne linije bez naziva programa
* @param[in] argv Argumenti komandne linije bez naziva programa
* @param[in] key_list Pokazivac na listu trenutno ucitanih kljuceva
* @param[in] log_file Pokazivac na fajl u koji treba ispisivati poruke
* @param[in] print_to_stdout Ukoliko je 0 sve poruke ce biti ispisivane u zadati fajl,
* u suprotnom ce poruke biti ispisivane na standardnom izlazu, a fajl ce biti koriscen
* samo u slucaju da se komanda odnosi na enkripciju/dekripciju vise fajlova
*/
static void process_ed_command(int argc, char *argv[], List *key_list, FILE *log_file, int print_to_stdout) {
    int encr_flag, more_files_flag = 0, regex_flag = 0;
    char error_msg[MAX_STR_LEN];
    Key *key;
    FILE *log_file_tmp = print_to_stdout ? stdout : log_file;

    if (argc != 3 || strlen(argv[0]) != 2 && strlen(argv[0]) != 3) {
        print_log(log_file_tmp, INVALID_COMMAND_STR);
        return;
    }

    encr_flag = argv[0][1] == 'e';
    more_files_flag = argv[0][2] == 'm';
    regex_flag = argv[0][2] == 'r';

    if (strlen(argv[0]) == 3 && !more_files_flag && !regex_flag) {
        print_log(log_file_tmp, INVALID_COMMAND_STR);
        return;
    }

    if (!(key = find_key_with_name(key_list, argv[1]))) {
        print_log(log_file_tmp, "Key %s does not exist!\n", argv[1]);
        return;
    }

    if (encr_flag) {
        if (!more_files_flag && !regex_flag) {
            if (encrypt_file(argv[2], key, error_msg)) {
                print_log(log_file_tmp, "Error with file %s:%s\n", argv[2], error_msg);
            }
            else {
                print_log(log_file_tmp, "File %s encrypted\n", argv[2]);
            }
        }
        else if (more_files_flag) {
            if (encrypt_more_files(argv[2], key, log_file)) {
                print_log(log_file_tmp, "Unable to open file %s\n", argv[2]);
            }
            else if (print_to_stdout)
                printf("See log.txt for info about encryption...\n");
        }
        else {
            if (argv[2][0] != '\'' || argv[2][strlen(argv[2]) - 1] != '\'') {
                print_log(log_file_tmp, "File path not enclosed in ''");
            }
            else {
                /// filepath is enclosed in ''
                strcpy(argv[2], argv[2] + 1);
                argv[2][strlen(argv[2]) - 1] = '\0';

                if (encrypt_regex_files(argv[2], key, error_msg, log_file)) {
                    print_log(log_file_tmp, "%s\n", error_msg);
                }
                else if (print_to_stdout)
                    printf("See log.txt for info about encryption...\n");
            }
        }
    }
    else {
        if (!more_files_flag && !regex_flag) {
            if (decrypt_file(argv[2], key, error_msg)) {
                print_log(log_file_tmp, "Error with file %s:%s\n", argv[2], error_msg);
            }
            else {
                print_log(log_file_tmp, "File %s decrypted\n", argv[2]);
            }
        }
        else if (more_files_flag) {
            if (decrypt_more_files(argv[2], key, log_file)) {
                print_log(log_file_tmp, "Unable to open file %s\n", argv[2]);
            }
            else if (print_to_stdout)
                printf("See log.txt for info about decryption...\n");
        }
        else {
            if (argv[2][0] != '\'' || argv[2][strlen(argv[2]) - 1] != '\'') {
                print_log(log_file_tmp, "File path not enclosed in ''");
            }
            else {
                /// filepath is enclosed in ''
                strcpy(argv[2], argv[2] + 1);
                argv[2][strlen(argv[2]) - 1] = '\0';

                if (decrypt_regex_files(argv[2], key, error_msg, log_file)) {
                    print_log(log_file_tmp, "%s\n", error_msg);
                }
                else if (print_to_stdout)
                    printf("See log.txt for info about decryption...\n");
            }
        }
    }
}

/**
* @brief Funkcija za obradu batch komande.
* @param[in] argc Broj argumenata komandne linije bez naziva programa
* @param[in] argv Argumenti komandne linije bez naziva programa
* @param[in] key_list Pokazivac na listu trenutno ucitanih kljuceva
* @param[in] log_file Pokazivac na fajl u koji treba ispisivati poruke
*/
static void process_b_command(int argc, char *argv[], List *key_list, FILE *log_file) {
    FILE *f;
    if (strlen(argv[0]) != 2 || argc != 2) {
        printf(INVALID_COMMAND_STR);
        return;
    }

    f = fopen(argv[1], "r");

    if (f) {
        int new_argc;
        char *new_argv[4];
        char str[MAX_STR_LEN];
        int i;
        for (i = 0; i < 4; i++)
            new_argv[i] = malloc(sizeof(char) *MAX_STR_LEN);

        while (fgets(str, MAX_KEY_ROW_LEN, f)) {
            if (parse_line(str, &new_argc, new_argv) || (new_argc > 1 && new_argv[1][0] != '-')) {
                print_log(log_file, "Invalid command: %s\n", str);
            }
            else if (new_argv[1][1] == 'b' || new_argv[1][1] == 'l') {
                print_log(log_file, "-[b/l] command not allowed: %s\n", str);
            }
            else
                process_ed_command(new_argc - 1, new_argv + 1, key_list, log_file, 0);
        }

        printf("See log.txt for info about encryption...\n");

        for (i = 0; i < 4; i++)
            free(new_argv[i]);
        fclose(f);
    }
    else
        printf("Unable to open given file!\n");
}

/**
* @brief Funkcija za obradu komande za izlistavanje kljuceva. Kljucevi se izlistavaju samo na standardnom izlazu.
* @param[in] argc Broj argumenata komandne linije bez naziva programa
* @param[in] argv Argumenti komandne linije bez naziva programa
* @param[in] key_list Pokazivac na listu trenutno ucitanih kljuceva
*/
static void process_l_command(int argc, char *argv[], List *key_list) {
    if (strlen(argv[0]) != 2 || argc != 1) {
        printf(INVALID_COMMAND_STR);
        return;
    }
    print_keys(key_list);
}

/*********************** EXTERNAL FUNCTIONS ***********************/
void process_command(int argc, char *argv[], List *key_list) {
    FILE *log_file = fopen("log.txt", "w");
    argc--;
    argv++;

    if (argv[0][0] != '-'/* && argv[0][0] != '/'*/) {
        printf(INVALID_COMMAND_STR);
        return;
    }

    switch (argv[0][1]) {
    case 'e':
    case 'd':
        process_ed_command(argc, argv, key_list, log_file, 1);
        break;
    case 'b':
        process_b_command(argc, argv, key_list, log_file);
        break;
    case 'l':
        process_l_command(argc, argv, key_list);
        break;
    case 'h':
        print_help();
        break;
    default:
        printf(INVALID_COMMAND_STR);
    }

    if (log_file)
        fclose(log_file);
}
