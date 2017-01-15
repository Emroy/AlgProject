#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "HashGen.h"

typedef struct HashTable_type *HashTable;

HashTable hashTable_create(int size,HashDescriptor hd);
/*Returns a new Hash Table with empty buckets or NULL on failure.
  CAUTION: It is the caller's responsibility to make the hash function
  able to accept the type of data that will be stored inside the table.*/

void hashTable_destroy(HashTable ht);
/*Frees all allocated memory for given Hash Table*/

int hashTable_insert(HashTable ht,void* data);
/*Inserts whatever data points to on Hash Table ht and returns 0.
  A negative number is returned on failure*/

void* hashTable_getNext(HashTable ht,void* q);
/*Returns next element from bucket corresponding to element q.
  Returns null when there is no next element.*/

void* hashTable_getAll(HashTable ht);
/*Returns one by one all the elements inside the hash table.
  First call returns the first item, second call the second item etc.
  Returns null when there are no more items left.*/
#endif