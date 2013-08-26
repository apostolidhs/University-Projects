#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct nodeL* NodeL;
typedef struct list* List;

struct list
{
    NodeL head;
    int size;
};

struct nodeL
{
    void* item;
    struct nodeL* next;
};

List ListCreate();
List ListKitKat( List lst1, List lst2 );
void ListAddHead(List lst , void* item);
void* ListRemoveHead( List lst );
void* ListRemoveItem( List lst, void *item);

int ListSize( List lst );
int ListisEmpty( List lst );



void ListPrintStr( List lst );

#endif