#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#define LSH_L 5
#define LSH_K 4

struct NearestNeighbors{

};

struct Cluster{

};

/*NN-LSH*/

void lsh_init(Ratings ratings,char metric)
{
	User currentUser;

	switch(metric)
	{
		case 'h':
			while(currentUser = getNextUser(ratings)){
				int8_t* vector = getUserVector(currentUser);
				int8_t* hammingData;
				unsigned int numOfItems = getNumberOfItems(ratings);
				unsigned int hammingDataSize = numOfItems/8;
				if(numOfItems % 8) hammingDataSize++;

				hammingData = malloc(sizeof(int8_t)*hammingDataSize);
				if(hammingData == NULL){
					perror("Failed to allocate memory for hammingData");
					exit(-2);
				}
			}
}

