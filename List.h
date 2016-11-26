#ifndef LIST_H
#define LIST_H

typedef struct ListHead* List;

List list_create();
/*returns a pointer to a list or NULL on failure*/

void list_destroy(List l);
/*Destroy a list and set l to NULL.
  Data contained in the list on this call will NOT be freed.
  It is the callers responsibility to have access to those data
  after the call in order to free them*/

void list_pushEnd(List l,void* data);
/*push an element to the end of the list*/

void* list_pop(List l);
/*get the first element of the list and destroy the first node.
  if the list is empty, NULL is returned instead.*/

int list_isEmpty(List l);
/*check if list is empty*/
#endif