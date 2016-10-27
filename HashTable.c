#include <stdio.h>
#include <stdlib.h>
#include <HashTable.h>

typedef struct ChainList{
	void* data;
	Chain* next;
} Chain;

struct HashTable_type{
	Chain* table;
	int size;
	unsigned int (*hash)(void* x);
};

HashTable createHashTable(int size,unsigned int(*hashFunction)(void* x)){
	HashTable ht = malloc(sizeof(struct HashTable_type));
	if(ht == NULL){
		perror("Failed to allocate memory for new Hash Table");
		return NULL;
	}

	ht->size = size;
	ht->table = malloc(size*sizeof(Chain));
	if(ht->table == NULL){
		perror("Failed to allocate memory for buckets while creating new Hash Table");
		free(ht);
		return NULL;
	}

	int i;
	for(i=0;i<size;i++){
		ht->table[i].data = NULL;
		ht->table[i].next = NULL;
	}

	ht->hash = hashFunction;

	return ht;
}

void destroyHashTable(HashTable ht){
	int i;
	for(i=0;i<ht->size;i++){
		Chain* current=&(ht->table[i]);
		Chain* temp = current;
		while(current != NULL){
			current = current->next;
			free(temp);
			temp = current;
		}
	}

	free(table);
}

int insert(HashTable ht,void* data){
	unsigned int pot = ht->hash(data);

	if(ht->table[pot].data == NULL){
		ht->table[pot].data = data;
		return 0;
	}

	Chain* next = ht->table[pot].next;
	while(next != NULL) next = next->next;

	next = malloc(sizeof(Chain));
	if(next == NULL){
		perror("Failed to allocate memory for new Hash Table element");
		return -1;
	}

	next->data = data;
	next->next = NULL;

	return 0;
}

void* getNext(HashTable ht,void* q){
	static Chain* current = NULL;

	if(current == NULL) current = &(ht->table[ht->hash(q)]);
	else current = current->next;

	return current->data;
}