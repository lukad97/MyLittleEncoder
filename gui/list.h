#ifndef _LIST_H
#define _LIST_H

typedef struct ListElement {
    struct ListElement *next, *prev;
    void *info;
} ListElement;

typedef struct List {
    int length;
    ListElement *head, *tail;
} List;


List* init_list();
void tail_add(List*, void *info);
void remove_element(List*, ListElement*);
void remove_list(List*);
ListElement* find_kth_prev(ListElement*, int);
// DEBUG
// void print_list(List*);

#endif // _LIST_H
