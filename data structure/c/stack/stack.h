#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct node* Node;
typedef struct stack* Stack;

struct stack
{
    Node head;
    int size;
};

struct node
{
    void* item;
    struct node* next;
};

Stack StackCreate();
void  StackPush( Stack stk , void* item );
Stack StackKitKat( Stack stk1, Stack stk2 );
void* StackPop( Stack stk );

int StackSize( Stack stk );
int StackIsEmpty( Stack stk );
void * StackTop(Stack stk);

#endif