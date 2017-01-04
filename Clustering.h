#ifndef CLUSTERING_H
#define CLUSTERING_H
#include "HashGen.h"
#include "HashTable.h"

/*NN-LSH*/
typedef struct TwentyNeighbors* Neighbors;

void lsh_init(Ratings ratings);

Neighbors getNeighbors();

void lsh_terminate();

/*Clustering*/


#endif