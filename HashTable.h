#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "HashGen.h"

typedef struct HashTable_type *HashTable;

HashTable hash_create(int size,HashDescriptor hd);
/*Returns a new Hash Table with empty buckets or NULL on failure.
  CAUTION: It is the caller's responsibility to make the hash function
  able to accept the type of data that will be stored inside the table.*/

void hash_destroy(HashTable ht);
/*Frees all allocated memory for given Hash Table*/

int hash_insert(HashTable ht,void* data);
/*Inserts whatever data points to on Hash Table ht and returns 0.
  A negative number is returned on failure*/

void* hash_getNext(HashTable ht,void* q);
/*Returns next element from bucket corresponding to element q.
  Returns null when there is no next element.*/
#endif