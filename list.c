/**
* @file
* @author David Milicevic (davidmilicevic97@gmail.com)
* @brief Funkcije za rad sa dvostruko povezanom ulancanom listom.
*/

#include "list.h"
#include <stdlib.h>

List* init_list() {
    List *list = (List*) malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->length = 0;
    return list;
}

void tail_add(List *list, void *info) {
    if (!list->head) {
        list->head = list->tail = (ListElement*) malloc(sizeof(ListElement));
        list->head->info = info;
        list->head->next = list->head->prev = NULL;
    }
    else {
        list->tail->next = (ListElement*) malloc(sizeof(ListElement));
        list->tail->next->prev = list->tail;
        list->tail = list->tail->next;
        list->tail->info = info;
        list->tail->next = NULL;
    }
    list->length++;
}

void remove_list(List *list) {
    ListElement *curr;
    for (curr = list->head; list->head; curr = list->head) {
        list->head = list->head->next;
        free(curr->info);
        free(curr);
    }
    list->head = list->tail = NULL;
    list->length = 0;
}

void remove_element(List *list, ListElement *curr) {
    if (!list->head || !curr)
        return;

    if (curr == list->head && list->length == 1) {
        list->head = list->tail = NULL;
    }
    else if (curr == list->head) {
        list->head = curr->next;
        list->head->prev = NULL;
    }
    else if (curr == list->tail) {
        list->tail = curr->prev;
        list->tail->next = NULL;
    }
    else {
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
    }

    free(curr->info);
    free(curr);

    list->length--;
}

ListElement* find_kth_prev(ListElement *elem, int k) {
    int i;
    ListElement *curr = elem;

    for (i = 0; i < k && curr; i++, curr = curr->prev);

    return curr;
}
