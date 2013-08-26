#include "stack.h"

Stack StackCreate()
{
    Stack new  = (Stack)malloc(sizeof(struct stack));
    new->head = NULL;
    new->size = 0;
    return new;
}

void  StackPush( Stack stk , void* _item )
{
    assert( stk!=NULL);
    assert( _item!=NULL);
    
    if ( stk->head == NULL )
    {
	Node new = (Node)malloc(sizeof(struct node));
	new->item = _item;
	new->next = NULL;
	stk->head = new;
	stk->size++;
    }
    else 
    {
	Node new = (Node)malloc(sizeof(struct node));
	new->item = _item;
	new->next = stk->head;
	stk->head = new;
	stk->size++;
    }
}

void* StackPop( Stack stk )
{
    assert(stk!=NULL);
    void* retval = stk->head->item;
    Node tmp = stk->head;
    stk->head = stk->head->next;
    free(tmp);
    stk->size--;
    return retval;
}

int StackSize( Stack stk )
{
    assert(stk!=NULL);
    return stk->size;
}

int StackIsEmpty( Stack stk )
{
    assert(stk!=NULL);
    if ( stk->size==0 )
	return 1;
    return 0;
}

void StackPrintStr( Stack stk )
{
    Node tmp = stk->head;
    while ( tmp!=NULL )
    {
	//printf("--%s\n",(char*)tmp->item);
	tmp = tmp->next;
    }
}

void * StackTop(Stack stk)
{
 return StackIsEmpty(stk)  ? (void *)NULL : (void *)stk->head->item;  
}

Stack StackKitKat( Stack stk1, Stack stk2 )
{
  if(stk1==NULL && stk2==NULL)
    return NULL;
  
  if (stk1==NULL || StackIsEmpty(stk1)==1 )
  {
    return stk2;
  }
  else if ( stk2==NULL || StackIsEmpty(stk2)==1 )
  {
    return stk1;
  }
  else
  {
    while ( StackIsEmpty(stk2)==0 )
    {
      StackPush( stk1, StackPop(stk2) );
    }
    return stk1;
  }
    
}

/*
int main()
{
    Stack stk = StackCreate();
    
    char* str1= "example1";
    char* str2= "example2";
    char* str3= "example3";
    char* str4= "example4";
    
    StackPush( stk , str1 );
    StackPush( stk , str2 );
    StackPush( stk , str3 );
    StackPush( stk , str4 );
    
    StackPrintStr( stk );
    printf("->>%d\n",StackSize(stk));
    StackPop( stk);
    printf("->>%d\n",StackSize(stk));
    StackPrintStr( stk );
   
    return 1;
}*/