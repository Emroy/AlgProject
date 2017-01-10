#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"

typedef struct ChainNode{
	void* data;
	struct ChainNode* next;
} ChainNode;

typedef struct ChainHead{
	ChainNode* start;
	ChainNode* end;
	int size;
} ChainHead;

struct HashTable_type{
	ChainHead* table;
	int size;
	HashDescriptor hash;
};

HashTable hashTable_create(int size,HashDescriptor hd){
	HashTable ht = malloc(sizeof(struct HashTable_type));
	if(ht == NULL){
		perror("Failed to allocate memory for new Hash Table");
		return NULL;
	}

	ht->size = size;
	ht->table = malloc(size*sizeof(ChainHead));
	if(ht->table == NULL){
		perror("Failed to allocate memory for buckets while creating new Hash Table");
		free(ht);
		return NULL;
	}

	int i;
	for(i=0;i<size;i++){
		ht->table[i].start = NULL;
		ht->table[i].end = NULL;
		ht->table[i].size = 0;
	}

	ht->hash = hd;

	return ht;
}

void hashTable_destroy(HashTable ht){
	int i;
	for(i=0;i<ht->size;i++){
		ChainNode* temp = ht->table[i].start;
		while(temp != NULL){
			ht->table[i].start = temp->next;
			free(temp);
			temp = ht->table[i].start;
		}
	}

	free(ht->table);
	free(ht);
}

int hashTable_insert(HashTable ht,void* data){
	int i = hash_apply(ht->hash,data);
	if(i<0) return -1;

	if(ht->table[i].size == 0){
		ht->table[i].start = malloc(sizeof(ChainNode));
		if(ht->table[i].start == NULL){
			perror("Failed to create node for new element on Hash Table");
			return -2;
		}

		ht->table[i].end = ht->table[i].start;
	}
	else{
		ht->table[i].end->next = malloc(sizeof(ChainNode));
		if(ht->table[i].end->next == NULL){
			perror("Failed to create node for new element on Hash Table");
			return -3;
		}

		ht->table[i].end = ht->table[i].end->next;
	}


	ht->table[i].end->next = NULL;
	ht->table[i].end->data = data;
	ht->table[i].size++;
	return 0;
}

void* hashTable_getNext(HashTable ht,void* q){
	static ChainNode* current = NULL;

	if(current == NULL){
		int pot = hash_apply(ht->hash,q);
		if(pot < 0) return NULL;
		current = ht->table[pot].start;
	}
	else current = current->next;

	if(current ==  NULL) return NULL;
	else return current->data;
}
