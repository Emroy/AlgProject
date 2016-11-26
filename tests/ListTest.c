#include <stdio.h>
#include "../List.h"

int main(void){
	List l = list_create();
	if(l == NULL) return -1;

	int a,b,c;
	a=0;
	b=1;
	c=2;

	list_pushEnd(l,&a);
	list_pushEnd(l,&b);
	list_pushEnd(l,&c);

	while(!list_isEmpty(l)){
		int* temp = (int *)list_pop(l);
		printf("%d\n",*temp);
	}

	list_destroy(l);

	return 0;
}