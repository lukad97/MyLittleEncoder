#include "cmd_line.h"
#include "process.h"
#include "list.h"
#include "keys.h"
#include <stdio.h>
#include <string.h>

/*********************** INTERNAL FUNCTIONS ***********************/
void print_keys(List *key_list) {
    ListElement *curr;
    Key *key;

    for (curr = key_list->head; curr; curr = curr->next) {
        key = (Key*)curr->info;
        printf(KEY_PRINT_FORMAT, key->type, key->mode, key->key_name, (key->key)[0], (key->key)[1], (key->key)[2]);
        printf("\n");
    }
}

/*** TODO ***/
void print_help() {
    printf("HELP HELP HELP\n");
}

int parse_line(char *line, int *argc, char *argv[]) {
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

void process_ed_command(int argc, char *argv[], List *key_list) {
    int encr_flag, more_files_flag = 0, regex_flag = 0;
    char error_msg[MAX_STR_LEN];
    Key *key;

    if (argc != 3 || strlen(argv[0]) != 2 && strlen(argv[0]) != 3) {
        printf(INVALID_COMMAND_STR);
        return;
    }

    encr_flag = argv[0][1] == 'e';
    more_files_flag = argv[0][2] == 'm';
    regex_flag = argv[0][2] == 'r';

    if (strlen(argv[0]) == 3 && !more_files_flag && !regex_flag) {
        printf(INVALID_COMMAND_STR);
        return;
    }

    if (!(key = find_key_with_name(key_list, argv[1]))) {
        printf("Key does not exist!\n");
        return;
    }

    if (encr_flag) {
        if (!more_files_flag && !regex_flag) {
            if (encrypt_file(argv[2], key, error_msg))
                printf("Error with file %s:%s\n", argv[2], error_msg);
            else
                printf("File %s encrypted\n", argv[2]);
        }
        else if (more_files_flag) {
            if (encrypt_more_files(argv[2], key))
                printf("Unable to open file!");
            else
                printf("See log.txt for info about encryption.");
        }
        else
            encrypt_regex_files(argv[2], key);
    }
    else {
        if (!more_files_flag && !regex_flag) {
            if (decrypt_file(argv[2], key, error_msg))
                printf("Error with file %s:%s\n", argv[2], error_msg);
            else
                printf("File %s decrypted\n", argv[2]);
        }
        else if (more_files_flag) {
            if (decrypt_more_files(argv[2], key))
                printf("Unable to open file!");
            else
                printf("See log.txt for info about decryption.");
        }
        else
            decrypt_regex_files(argv[2], key);
    }
}

void process_b_command(int argc, char *argv[], List *key_list) {
    FILE *f, *log;
    if (strlen(argv[0]) != 2 || argc != 2) {
        printf(INVALID_COMMAND_STR);
        return;
    }

    f = fopen(argv[1], "r");
    //log = fopen("log.txt", "w");

    if (f) {
        int new_argc;
        char *new_argv[4];
        char str[MAX_STR_LEN];
        int i;
        for (i = 0; i < 4; i++)
            new_argv[i] = malloc(sizeof(char) *MAX_STR_LEN);

        while (fgets(str, MAX_KEY_ROW_LEN, f)) {
            if (parse_line(str, &new_argc, new_argv))
                printf("Invalid command!\n");
            else
                process_command(new_argc, new_argv, key_list);
        }

        for (i = 0; i < 4; i++)
            free(new_argv[i]);
        fclose(f);
    }
    else
        printf("Unable to open given file!");
}

void process_l_command(int argc, char *argv[], List *key_list) {
    if (strlen(argv[0]) != 2 || argc != 1) {
        printf(INVALID_COMMAND_STR);
        return;
    }
    print_keys(key_list);
}

/*********************** EXTERNAL FUNCTIONS ***********************/
void process_command(int argc, char *argv[], List *key_list) {
    argc--;
    argv++;

    if (argv[0][0] != '-' && argv[0][0] != '/') {
        printf(INVALID_COMMAND_STR);
        return;
    }

    switch (argv[0][1]) {
    case 'e':
    case 'd':
        process_ed_command(argc, argv, key_list);
        break;
    case 'b':
        process_b_command(argc, argv, key_list);
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
}
