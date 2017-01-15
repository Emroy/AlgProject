#ifndef HASH_GEN_H
#define HASH_GEN_H
#include <stdint.h>
#include "data.h"

typedef struct HashDesc *HashDescriptor;

unsigned int hash_apply(HashDescriptor hd,Data x);
/*Aplly function described by HashDescriptor on element x and return the ouput*/

HashDescriptor hamming_hash_create(int d,int k);
/*Creates a hamming function g as described by theory, return NULL on failure*/

HashDescriptor euclidean_hash_create(int d,int k,int n);
/*Creates an euclidean function g as described by theory, NULL on failure
  n: number of total elements in query. Used to determine TableSize. See theory for more info*/

HashDescriptor cosine_hash_create(int d,int k);
/*Creates a cosine function g as described by theory, NULL on failure*/

int hash_isEqual(HashDescriptor a,HashDescriptor b);

void hash_destroy(HashDescriptor hd);
#endif