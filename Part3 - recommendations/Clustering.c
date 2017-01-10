#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#define LSH_L 5
#define LSH_K 4

/*calculates 2^num*/
unsigned int two_pow(unsigned int num){
	unsigned int i;
	unsigned int retVal = 1;
	for(i=0;i<num;i++) retVal*=2;
	return retVal;
}

static unsigned short range;

struct NearestNeighbors{
	User* users;
};

/*NN-LSH*/

void lsh_init(Ratings ratings,char metric)
{
	int i,j;
	int8_t mask;
	User currentUser;

	switch(metric)
	{
		case 'h':
			while(currentUser = getNextUser(ratings))
			{
				int8_t* vector = getUserVector(currentUser);
				int8_t* hammingData;
				unsigned int numOfItems = getNumberOfItems(ratings);
				unsigned int hammingDataSize = numOfItems/8;
				if(numOfItems % 8) hammingDataSize++;
				hammingData = malloc(sizeof(int8_t)*hammingDataSize);
				if(hammingData == NULL)
				{
					perror("Failed to allocate memory for hammingData");
					exit(-2);
				}
				for(i=0;i<=hammingDataSize-1;i++)
				{
					hammingData[i]=0;
				}
				j=0;
				mask=1;
				for(i=0;i<=numOfItems-1;i++)
				{
				    if(vector[i])
				    {
				    	hammingData[j]|=mask;
				    }
				    if(!(mask<<=1))
				    {
				    	mask=1;
				    	j++;
				    }
				}
			}
			
}

Neighbors lsh_getNeighbors(User user){

}

void lsh_terminate(){

}

void clustering_init(Ratings ratings,char metric){

}

Neighbors clustering_getNeighbors(User user){

}

void clustering_terminate(){

}