#ifndef SYMTABLE_H
#define SYMTABLE_H



typedef void * SymTable_T;

/* This function will return SymTable_new a new
  SymTable_T is empty (contains no bindings) */

extern SymTable_T SymTable_new (void);

/* This function SymTable_free should release all memory used by oSymTable.
If oSymTable is NULL, then the function should not do anything .*/

extern void SymTable_free (SymTable_T oSymTable);

/* § SymTable_getLength The function should return the number of bindings in oSymTable.
 * It is a controlled runtime error program (checked runtime error) the oSymTable be NULL .*/

extern unsigned int SymTable_getLength (SymTable_T oSymTable);

/* § SymTable_put The function should, if there is no binding with the key pcKey oSymTable,
add a new binding to oSymTable consisting of the key and value pcKey pvValue,
and returns 1 (TRUE). Otherwise, the function should not change the oSymTable, and must return 0 (FALSE).
It is controlled runtime error (checked runtime error) whether or oSymTable pcKey is NULL .*/

extern int SymTable_put (SymTable_T oSymTable, const char * pcKey, const void * pvValue);

/* § SymTable_remove The function should, if there is a key binding to the pcKey oSymTable,
remove the binding from oSymTable and returns 1 (TRUE).
Otherwise, the function should not change the oSymTable, and must return 0 (FALSE).
It is controlled runtime error (checked runtime error) whether or oSymTable pcKey is NULL .*/

extern int SymTable_remove (SymTable_T oSymTable, const char * pcKey);

/* § SymTable_contains The function should return 1 (TRUE) if oSymTable
 * Contains a binding whose key is pcKey, and 0 (FALSE) otherwise.
 * It is a controlled runtime error (checked runtime error) whether or oSymTable pcKey is NULL .*/

extern int SymTable_contains (SymTable_T oSymTable, const char * pcKey);

/* § SymTable_get The function should return the value of the binding
 * OSymTable whose key is pcKey, or NULL, if no such binding.
 * It is a controlled runtime error (checked runtime error) whether or oSymTable pcKey is NULL .*/

extern void * SymTable_get (SymTable_T oSymTable, const char * pcKey);

/* § SymTable_map The function must implement the function * pfApply each
binding in oSymTable, passing the pvExtra as an additional parameter.
It is controlled runtime error (checked runtime error) whether or oSymTable pfApply is NULL .*/

extern void SymTable_map (SymTable_T oSymTable,
      void (* pfApply) (const char * pcKey, void * pvValue, void * pvExtra),
      const void * pvExtra);

#endif