#include "list.h"

List ListCreate()
{
    List new  = (List)malloc(sizeof(struct list));
    new->head = NULL;
    new->size = 0;
    return new;
}
    
void ListAddHead(List lst , void* item)
{
    assert(lst!=NULL);
    assert(item!=NULL);
    
    if ( lst->head == NULL )
    {
	NodeL new = (NodeL)malloc(sizeof(struct nodeL));
	new->item = item;
	new->next = NULL;
	lst->head = new;
	lst->size++;
    }
    else
    {
	NodeL new = (NodeL)malloc(sizeof(struct nodeL));
	new->item = item;
	new->next = lst->head;
	lst->head = new;
	lst->size++;
    }
}
    
void* ListRemoveHead( List lst )
{
    assert( lst!=NULL );
    
    if ( lst->head == NULL )
	return NULL;
    
    void* retval = lst->head->item;
    NodeL tmp = lst->head;
    lst->head = lst->head->next;
    free(tmp);
    lst->size--;
    return retval;
}

void* ListRemoveItem( List lst, void *item)
{
    assert( lst!=NULL );
    
    if ( lst->head == NULL )
	return NULL;
    
    NodeL trav = lst->head;
    NodeL tmp;
    void* retval;
    
    while( trav!=NULL )
    {
	if ( trav->item == item )
	{
	    retval = trav->item;
	    tmp = trav;
	    trav = trav->next;
	    free(tmp);
	    lst->size--;
	    return retval;
	}
	trav = trav->next;
    }
    return NULL;
}

int ListSize( List lst )
{
    assert( lst!=NULL );
    return lst->size;
}

int ListisEmpty( List lst )
{
    assert( lst!=NULL );
    if ( lst->size==0 )
	return 1;
    return 0;
}

List ListKitKat( List lst1, List lst2 )
{
    assert( lst1!=NULL );
    assert( lst2!=NULL );
    
    NodeL tmp = lst1->head;
    if ( tmp==NULL )
    {
	lst1->head = lst2->head;
	lst1->size = lst2->size;
	return lst1;
    }
    else
    {
	while (tmp->next!=NULL)
	    { tmp = tmp->next; }
	tmp->next = lst2->head;
	lst1->size += lst2->size;
	return lst1;
    }
}

void ListPrintStr( List lst )
{
    NodeL tmp = lst->head;
    while ( tmp!=NULL )
    {
	printf("-%s\n",(char*)tmp->item);
	tmp=tmp->next;
    }
    printf("Size:%d\n",lst->size);
}
