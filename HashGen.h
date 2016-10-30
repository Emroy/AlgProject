#ifndef HASH_GEN_H
#define HASH_GEN_H
#include <stdint.h>

typedef struct HashDesc *HashDescriptor;

HashDescriptor hamming_hash_create(int d,int k);
/*Creates a function g as described by theory, return NULL on failure*/

int hamming_is_equal(HashDescriptor a,HashDescriptor b);
/*if HashDescritor function a is the same as HashDescriptor function b returns 1 else returns 0*/

unsigned int hash_apply(HashDescriptor hd,void* x);

void hamming_hash_destroy(HashDescriptor hd);
/*Destory a HashDescriptor and make hd NULL*/
#endif