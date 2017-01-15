#ifndef CLUSTERING_H
#define CLUSTERING_H
#include "Ratings.h"

/*NN-LSH*/
void lsh_init(Ratings ratings,char metric);
/*Creates hash tables for lsh and inserts ratings in them*/

unsigned int* lsh_getRecommendedItems(User user);

void lsh_terminate();

/*Clustering*/
void clustering_init(Ratings ratings,char metric);

Neighbors clustering_getNeighbors(User user);

void clustering_terminate();

#endif