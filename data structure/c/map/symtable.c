/*
 * John Apostolidis (log in: japostol)
 * AM:2576
 * symtablehash.c
 * 
 * Implementation with hash table
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "symtable.h"

#define HASH_MULTIPLIER 59999
#define STARTING_BUCKETS 509

struct list
{
 char *pckey;
 void *pcvalue;  
 struct list *next;
};


typedef struct 
{
  unsigned int length;
  struct list **hush;  
  unsigned int hushSize;
}symtable;

/*
 * initialize a array with prime numbers.
 * prime number is usefull for hust table expanding.
 */
static unsigned int primeNumbers[]={0, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521, 0};

/*
 * if you want to analyse how many elements are at every bucket.
 * run the following code.
 */

/*
void Display(SymTable_T oSymTable)
{
  struct list *curr;
  int i;
  
  assert(oSymTable);
  
  for(i=0;i<oSymTable->hushSize;)
  {
    curr = oSymTable->hush[i];
    printf("bucket %d:\n", i);
    while (curr) 
    {
	printf("%s\n", curr->pckey);
	curr = curr->next;
    } 
    i++;
  }  
}*/

/*
 * look at string.h library in c.
 * 
 * duplicate the string from argument
 * allocating different memmory.
 */

static char * mystrdup(const char *str)
{
    char *new;
    
    assert(str);
    
    if ((new=(char *)malloc(sizeof(char)*strlen(str)))==NULL)
      return NULL;
    
    return strcpy(new, str);
}

/*
 * returns a hash code for pcKey with unsigned int size
 */
static unsigned int SymTable_hash(const char *pcKey) {
  int i;
  unsigned int uiHash = 0;
  
  assert(pcKey);
 
  for (i = 0; pcKey[i] != '\0'; i++)
    uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];
  
  return (uiHash);
}

/*Takes hash code and trasmit it to the current hush table size [0, n-1]*/
static unsigned int hush_compression(symtable * oSymTable, unsigned int hashCode)
{
  assert(oSymTable);
  
  return (hashCode%oSymTable->hushSize);
}


/*
 * Check if the length of elements are bigger than the hush table size.
 * if it is, it checks the array with prime numbers and returns the bigger
 * prime number after the current. if the raising number has reach the n-1 element of
 * array it returns 0.
 * 
 * In other case returns 0.
 */
static unsigned int can_Raise(symtable * oSymTable)
{
  unsigned int i;
  
  assert(oSymTable);
  
  if(oSymTable->length<oSymTable->hushSize)
  {
    return 0;	
  }
  
  for(i=1;primeNumbers[i]!=0;i++)
  {
    if(primeNumbers[i]==oSymTable->hushSize)
    {
      return primeNumbers[i+1];
    }
  }
  
  return 0;
}

/*
 * check if the element before the current of prime numbers is 
 * bigger than the current, if it is it returns it. if the decreasing
 * number has reach the n-1 element of
 * array it returns 0.
 * 
 * In other case returns 0.
 */

static unsigned int can_Digrees(symtable * oSymTable)
{
  unsigned int i;
  
  assert(oSymTable);

  for(i=1;primeNumbers[i]!=0;i++)
  {
    if(primeNumbers[i]==oSymTable->hushSize 
	&& primeNumbers[i-1]>oSymTable->length 
        && primeNumbers[i]!=0)
      
      return primeNumbers[i-1];
  }
  
  return 0;
}

/*
 * Takes the hash table and the new size.
 * change the hash table size to the new size,
 * trasmiting all the element to the new size.
 * 
 * return NULL in other case.
 */

static SymTable_T SymTable_Change_hushSize(symtable * oSymTable, unsigned int new_HushSize)
{
  struct list ** new, *temp, *curr, *prev;
  unsigned int old_HushSize;
  int i;
  
  assert(oSymTable);

    if ((new=(struct list **)malloc(sizeof(struct list *)*new_HushSize))==NULL)
	return NULL;
    
    for(i=0;i<new_HushSize;i++)
    {
      new[i]=NULL;
    }
    
    old_HushSize=oSymTable->hushSize;
    oSymTable->hushSize=new_HushSize;
    
    for(i=0;i<old_HushSize;i++)
    {
      curr=oSymTable->hush[i];
      if (curr!=NULL)
      {
	  prev=curr;
	  curr=curr->next;
	  while(prev)
	  {
	    temp=new[hush_compression( oSymTable , SymTable_hash(prev->pckey))];
	    new[hush_compression( oSymTable , SymTable_hash(prev->pckey))]=prev;
	    prev->next=temp;
	  
	    if(curr!=NULL)
	    {
	      prev=curr;
	      curr=curr->next;
	    }
	    else 
	      break;
	  } 
      }      
    }
    free(oSymTable->hush);
    oSymTable->hushSize=new_HushSize;
    oSymTable->hush=new;
      
  return oSymTable;
}

/*Η συνάρτηση SymTable_new θα επιστρέφει ένα καινούργιο 
  SymTable_T που είναι άδειο (δεν περιέχει καθόλου bindings*/

SymTable_T SymTable_new(void)
{
  symtable *new;
  int i;
  
  if ((new=(symtable *)malloc(sizeof(symtable)))==NULL)
    return NULL;
    
  new->length=0;
  new->hushSize=STARTING_BUCKETS;
  
  if ((new->hush=(struct list **)malloc(sizeof(struct list *)*STARTING_BUCKETS))==NULL)
    return NULL;

  for(i=0;i<STARTING_BUCKETS;i++)
  {
    new->hush[i]=NULL;
  }
  assert(new!=NULL);
  return (SymTable_T)new;
}

/*Η συνάρτηση SymTable_free πρέπει να ελευθερώνει όλη τη μνήμη που χρησιμοποιείται από το oSymTable. 
Αν το oSymTable είναι NULL, τότε η συνάρτηση δεν πρέπει να κάνει τίποτε.*/

void SymTable_free(SymTable_T oSymTable)
{
  int i;
  struct list *curr, *prev;
  
  assert(oSymTable);
  
  symtable *SymTable = (symtable *) oSymTable; 
  
  SymTable->length=0;
  for(i=0;i<SymTable->hushSize;i++)
  {      
      curr=SymTable->hush[i];
      if (curr!=NULL)
	  {
	  prev=curr;
	  curr=curr->next;
	  while(prev)
	  {
	    free(prev->pckey);
	    free(prev);
	    
	    if(curr!=NULL)
	    {
	      prev=curr;
	      curr=curr->next;
	    }
	    else 
	      break;
	  }
	  SymTable->hush[i]=NULL; 
	 }
  }
}

/*§ Η συνάρτηση SymTable_getLength πρέπει να επιστρέφει τον αριθμό των bindings στο oSymTable. 
 * Είναι ελεγχόμενο λάθος χρόνου εκτέλεσης του προγράμματος (checked runtime error) το oSymTable να είναι NULL.*/

unsigned int SymTable_getLength(SymTable_T oSymTable)
{
  
  assert(oSymTable);
  
  return ((symtable *)oSymTable)->length;
}

/*§ Η συνάρτηση SymTable_put πρέπει, αν δεν υπάρχει binding με κλειδί pcKey στο oSymTable, 
να προσθέτει ένα καινούργιο binding στο oSymTable που θα αποτελείται από το κλειδί pcKey και την τιμή pvValue, 
και θα επιστρέφει 1 (TRUE). Διαφορετικά, η συνάρτηση δεν πρέπει να αλλάζει το oSymTable, και πρέπει να επιστρέφει 0 (FALSE). 
Είναι ελεγχόμενο λάθος χρόνου εκτέλεσης (checked runtime error) αν το oSymTable ή το pcKey είναι NULL.*/

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue)
{
  struct list *new, *temp;
  unsigned int new_HushSize;
  
  assert(oSymTable);
  assert(pcKey);
  
  symtable *SymTable = (symtable *) oSymTable; 
    
  if (SymTable_contains(SymTable, pcKey))
    return 0;
  
  if ((new=(struct list *)malloc(sizeof(struct list *)))==NULL)
    return 0;

  
  if((new_HushSize=can_Raise(SymTable))!=0)
  {
      SymTable_Change_hushSize(SymTable, new_HushSize);
  }
  
  new->pckey=mystrdup(pcKey);
  new->pcvalue=(void *)pvValue;
  
  temp=SymTable->hush[hush_compression( SymTable , SymTable_hash(pcKey ))];
  SymTable->hush[hush_compression( SymTable , SymTable_hash(pcKey ))]=new;
  new->next=temp;
  
  SymTable->length++;
  return 1;
}

/*§ Η συνάρτηση SymTable_remove πρέπει, αν υπάρχει ένα binding με κλειδί pcKey στο oSymTable, 
να αφαιρεί το binding από το oSymTable και να επιστρέφει 1 (TRUE). 
Διαφορετικά, η συνάρτηση δεν πρέπει να αλλάζει το oSymTable, και πρέπει να επιστρέφει 0 (FALSE). 
Είναι ελεγχόμενο λάθος χρόνου εκτέλεσης (checked runtime error) αν το oSymTable ή το pcKey είναι NULL.*/

int SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
  struct list *curr, *prev, *temp;
  unsigned int new_HushSize;
  
  assert(pcKey); 
  assert(oSymTable);
  
  symtable *SymTable = (symtable *) oSymTable; 
  
  if (!SymTable_contains(SymTable, pcKey))
    return 0;
      
  curr=SymTable->hush[hush_compression( SymTable , SymTable_hash(pcKey ))];
  if(!strcmp(curr->pckey, pcKey))
  {
    temp=curr->next; 
    free(curr->pckey);
    free(curr);
    SymTable->hush[hush_compression( SymTable , SymTable_hash(pcKey ))]=temp;
  }
  else
  {
    prev=curr;
    curr=curr->next;
    while(curr)
    {
      if (!strcmp(curr->pckey, pcKey))
      {
	temp=curr->next;
	free(curr->pckey);
	free(curr);
	prev->next=temp;
      }	
      prev=curr;
      curr=curr->next;
    }
  }
  
  SymTable->length--;
  
    if((new_HushSize=can_Digrees(SymTable))!=0)
      SymTable_Change_hushSize(SymTable, new_HushSize);
    
  return 1; 
}

/*§ Η συνάρτηση SymTable_contains πρέπει να επιστρέφει 1 (TRUE) αν το oSymTable 
 * περιέχει ένα binding του οποίου το κλειδί είναι pcKey, και 0 (FALSE) διαφορετικά. 
 * Είναι ελεγχόμενο λάθος χρόνου εκτέλεσης (checked runtime error) αν το oSymTable ή το pcKey είναι NULL.*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
  struct list *curr;
  
  assert(pcKey); 
  assert(oSymTable);
  
  symtable *SymTable = (symtable *) oSymTable; 
  
  curr=SymTable->hush[hush_compression( SymTable , SymTable_hash(pcKey ))];
  
  while(curr!=NULL)
  {
    if (!strcmp(curr->pckey, pcKey))
    {
      return 1;
    }
    curr=curr->next;    
  }
   
  return 0;   
}

/*§ Η συνάρτηση SymTable_get πρέπει να επιστρέφει την τιμή του binding στο 
 * oSymTable του οποίου το κλειδί είναι pcKey, ή NULL, αν δεν υπάρχει τέτοιο binding. 
 * Είναι ελεγχόμενο λάθος χρόνου εκτέλεσης (checked runtime error) αν το oSymTable ή το pcKey είναι NULL.*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
  struct list * curr;
  
  assert(pcKey); 
  assert(oSymTable);
  
  symtable *SymTable = (symtable *) oSymTable;
  
  curr=SymTable->hush[hush_compression( SymTable , SymTable_hash(pcKey ))];
  
  while(curr!=NULL)
  {
    if (!strcmp(curr->pckey, pcKey))
    {
      return curr->pcvalue;
    }
    curr=curr->next;    
  }
   
  return NULL; 
}


/*§ Η συνάρτηση SymTable_map πρέπει να εφαρμόζει τη συνάρτηση *pfApply σε κάθε 
binding στο oSymTable, περνώντας το pvExtra ως επιπλέον παράμετρο. 
Είναι ελεγχόμενο λάθος χρόνου εκτέλεσης (checked runtime error) αν το oSymTable ή το pfApply είναι NULL.*/
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra)
{
  struct list *curr;
  int i;
  
  assert(oSymTable);
  assert(pfApply);
  
  symtable *SymTable = (symtable *) oSymTable;
  
  for(i=0;i<SymTable->hushSize;i++)
  {
    curr = SymTable->hush[i];
    while (curr) 
    {
	pfApply(curr->pckey, curr->pcvalue, (void *)pvExtra);
	
	curr = curr->next;
    }  
  }
}