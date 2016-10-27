#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashTable_type *HashTable;

HashTable createHashTable(int size,unsigned int (*hashFunction)(void* x));
/*Returns a new Hash Table with empty buckets or NULL on failure.
  CAUTION: It is the caller's responsibility to make the hash function
  able to accept the type of data that will be stored inside the table.*/

void destroyHashTable(HashTable ht);
/*Frees all allocated memory for given Hash Table*/

int insert(HashTable ht,void* data);
/*Inserts whatever data points to on Hash Table ht and returns 0.
  A negative number is returned on failure*/

void* getNext(HashTable ht,void* q);
/*Returns next element from bucket corresponding to element q.
  Returns null when there is no next element.*/
#endif