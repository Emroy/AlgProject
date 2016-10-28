#include <stdio.h>
#include <stdlib.h>
#include "../HashTable.h"
#define SIZE 50

unsigned int hashFunc(void* x){
	int* x2=x;
	return x2[0]%SIZE;
}

int main(void){
	HashTable ht = hash_create(SIZE,hashFunc);
	int* data = malloc(10*SIZE*sizeof(int));
	if(data == NULL){
		perror("Could not allocate memory for test elements");
		return -1;
	}

	int i;
	for(i=0;i<10*SIZE;i++){
		data[i] = i;
		hash_insert(ht,&data[i]);
	}

	int* data2 = malloc(sizeof(int));
	if(data2 == NULL){
		perror("Failed to allocate memory for query test element");
		return -2;
	}
	*data2 = 15;

	int* next;
	while((next=hash_getNext(ht,data2)) != NULL){
		printf("%d\n",*next);
	}

	hash_destroy(ht);
	free(data);
	free(data2);

	return 0;
}