#ifndef _CMD_LINE_H
#define _CMD_LINE_H

#include "list.h"

#define ARGS_SEPARATORS " \n"
#define INVALID_COMMAND_STR "Invalid command! Type command with [- or /]h for help!\n"
#define MAX_STR_LEN 256

void process_command(int argc, char *argv[], List *key_list);

#endif // _BATCH_H
