#include <stdio.h>
#include <stdlib.h>
#include "HashGen.h"
#include "RNG.h"

typedef struct HammingDesc{
	uint8_t* g;
	unsigned short size;	
} HammingDescriptor;

struct HashDesc{
	HammingDescriptor hamming;
};

HashDescriptor hamming_hash_create(int k,int d){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}

	retVal->hamming.g = malloc(sizeof(uint8_t)*k);
	if(retVal->hamming.g == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}
	retVal->hamming.size=k;
	/*make other metric descriptors NULL*/

	int i;
	for(i=0;i<k;i++) retVal->hamming.g[i] = (uint8_t)integerUniform(d);

	return retVal;
}

int hamming_is_equal(HashDescriptor a,HashDescriptor b){
	/*check if a and b are Hamming HashDescriptors*/
	if(a->hamming.size != b->hamming.size) return 0;

	int i;
	for(i=0;i<a->hamming.size;i++)
		if(a->hamming.g[i] != b->hamming.g[i]) return 0;

	return 1;
}

void hamming_destroy(HashDescriptor hd){
	free(hd->hamming.g);
	free(hd);
	hd = NULL;
}

unsigned int hash_apply(HashDescriptor hd,void* x){
	unsigned int retVal = 0;
	int i;
	/*check what metric is used*/

	/*Hamming case*/
	long long int* data = x;
	long long int temp=1;
	for(i=0;i<hd->hamming.size;i++){
		temp = temp << hd->hamming.g[i];
		retVal += (temp & *data) != 0;
		retVal << 1;
		temp = 1;
	}

	return retVal >> 1;
}