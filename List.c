#include "List.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode{
	struct ListNode* next;
	void* data;
}ListNode;

struct ListHead{
	ListNode* start;
	ListNode* end;
	unsigned int size;
};

List list_create(){
	List l = malloc(sizeof(struct ListHead));
	if(l == NULL){
		perror("Failed to allocate memory for new list head");
		return NULL;
	}

	l->start=NULL;
	l->end=NULL;
	l->size=0;

	return l;
}

void list_pushEnd(List l,void* data){
	/*create new node and initialize it*/
	ListNode* newNode = malloc(sizeof(ListNode));
	if(newNode == NULL){
		perror("Failed to allocate memory for new list node");
		return;
	}
	newNode->data = data;
	newNode->next = NULL;

	/*attach new node to the list*/
	if(l->size != 0) l->end->next = newNode;
	l->end = newNode;
	if(l->start == NULL) l->start = newNode;
	l->size++;
}

int list_isEmpty(List l){
	return (l->size == 0);
}

void* list_pop(List l){
	if(list_isEmpty(l)) return NULL;

	ListNode* temp = l->start;
	void* retVal = temp->data;

	l->start = temp->next;
	l->size--;
	if(l->size == 0) l->end = NULL;

	free(temp);
	return retVal;
}

void list_destroy(List l){
	ListNode* iter = l->start;
	while(iter){
		iter = iter->next;
		free(l->start);
		l->start = iter;
	}

	free(l);
}

unsigned int list_length(List l){
	return l->size;
}